#include <X11/xpm.h>
#include <cassert>

#include <iostream>
#include <sstream>

#include "FreeDesktopTray.h"
#include "icons/22x22/messages.xpm"
#include "icons/22x22/nomessages.xpm"
#include "icons/22x22/error.xpm"

// TODO: Set an icon

#define WINDOW_NAME "GNotify"
#define RESOURCE_NAME "GNotify"
#define RESOURCE_CLASS "GNotify"

#define TRAY_DATA_CHUNK_SIZE 20

#define OPCODE_TRAY_DOCK 0
#define OPCODE_TRAY_BEGIN_MESSAGE 1
#define OPCODE_TRAY_CANCEL_MESSAGE 2

char* eventNames[] = { "XEvent0", "XEvent1",
"KeyPress", "KeyRelease", "ButtonPress", "ButtonRelease", "MotionNotify",
"EnterNotify", "LeaveNotify", "FocusIn", "FocusOut", "KeymapNotify",
"Expose", "GraphicsExpose", "NoExpose", "VisibilityNotify", "CreateNotify",
"DestroyNotify", "UnmapNotify", "MapNotify", "MapRequest", "ReparentNotify",
"ConfigureNotify", "ConfigureRequest", "GravityNotify", "ResizeRequest",
"CirculateNotify", "CirculateRequest", "PropertyNotify", "SelectionClear",
"SelectionRequest", "SelectionNotify", "ColormapNotify", "ClientMessage",
"MappingNotify", "LASTEvent",
};

FreeDesktopTray::FreeDesktopTray() : icon_(Icon_NoMessages), running_(false), message_id_(0), popup_timeout_(3), dock_(true)
{
}

FreeDesktopTray::~FreeDesktopTray()
{
}

FreeDesktopTray* FreeDesktopTray::instance()
{
	if (!instance_) {
		instance_ = new FreeDesktopTray();
	}
	return instance_;
}

void FreeDesktopTray::setPopupTimeout(unsigned int timeout) 
{
	popup_timeout_ = timeout;
}

void FreeDesktopTray::setDock(bool d)
{
	bool old_dock_ = dock_;
	dock_ = d;
	if (running_ && (old_dock_ != dock_)) {
		if (dock_) {
			dock();
		}
		else {
			XDestroyWindow(display_,icon_window_);
			createIconWindow();
		}
	}
}

void FreeDesktopTray::setIcon(IconName icon) 
{
	icon_ = icon;
	if (running_) {
		drawIcon();
	}
}

void FreeDesktopTray::showPopup(const std::string& message)
{
	if (!running_ || !tray_window_ || popup_timeout_ == 0) {
		return;
	}

	sendTrayOpcode(OPCODE_TRAY_BEGIN_MESSAGE, popup_timeout_*1000, message.size(), message_id_++);
	for (unsigned int i = 0; i < message.size(); i += TRAY_DATA_CHUNK_SIZE) {
		sendTrayData(message.substr(i, TRAY_DATA_CHUNK_SIZE));
	}
}

void FreeDesktopTray::sendTrayOpcode(int opcode, long data1, long data2, long data3)
{
	if (!tray_window_) {
		return;
	}

	XEvent ev;
	memset(&ev, 0, sizeof(ev));
	ev.xclient.type = ClientMessage;
	ev.xclient.window = tray_window_;
	ev.xclient.message_type = XInternAtom(display_, "_NET_SYSTEM_TRAY_OPCODE", False);
	ev.xclient.format = 32;
	ev.xclient.data.l[0] = CurrentTime;
	ev.xclient.data.l[1] = opcode;
	ev.xclient.data.l[2] = data1;
	ev.xclient.data.l[3] = data2;
	ev.xclient.data.l[4] = data3;
	XSendEvent(display_, tray_window_, False, NoEventMask, &ev);
}

void FreeDesktopTray::sendTrayData(const std::string& data)
{
	if (!tray_window_) {
		return;
	}

	XEvent ev;
	memset(&ev, 0, sizeof(ev));
	ev.xclient.type = ClientMessage;
	ev.xclient.window = tray_window_;
	ev.xclient.message_type = XInternAtom(display_, "_NET_SYSTEM_TRAY_MESSAGE_DATA", False);
	ev.xclient.format = 8;
	memcpy(&ev.xclient.data, data.c_str(), data.size());
	XSendEvent(display_, tray_window_, False, NoEventMask, &ev);
}


const FreeDesktopTray::Icon& FreeDesktopTray::getIcon() const
{
	switch(icon_) {
		case Icon_NoMessages:
			return nomessages_icon_;
		case Icon_Messages:
			return messages_icon_;
		default:
			return error_icon_;
	}
	return error_icon_;
}

void FreeDesktopTray::drawIcon() 
{
	XGCValues values;
	values.clip_mask = getIcon().mask;
	unsigned long valuemask = GCClipMask;
	GC gc = XCreateGC(display_, icon_window_, valuemask, &values);
	int x = (icon_window_width_ - getIcon().width) / 2;
	int y = (icon_window_height_ - getIcon().height) / 2;
	XCopyArea(display_, getIcon().pixmap, icon_window_, gc, 0, 0, getIcon().width, getIcon().height, x, y);
	XSync(display_,False); // Is this really necessary ?
	XFreeGC(display_, gc);
}
	
void FreeDesktopTray::createIconWindow()
{
	// Determine the parent
	Window parent = (tray_window_ == 0 || !dock_ ? DefaultRootWindow(display_) : tray_window_);

	// Create the icon window
	icon_window_width_ = icon_window_height_ = 22;
	XSetWindowAttributes window_attributes;
	unsigned long long window_attributes_mask = 0;
	icon_window_ = XCreateWindow(display_, parent, 0, 0, icon_window_width_, icon_window_height_, 0, DefaultDepth(display_,screen_),InputOutput,CopyFromParent, window_attributes_mask, &window_attributes);
	XSelectInput(display_, icon_window_, ExposureMask | ButtonPressMask | StructureNotifyMask);

	// Set WM_CLASS
	XClassHint* class_hint = XAllocClassHint();
	class_hint->res_name = RESOURCE_NAME;
	class_hint->res_class = RESOURCE_CLASS;
	XSetClassHint(display_, icon_window_, class_hint);
	XFree(class_hint);

	// Set window name
	std::string wm_name(WINDOW_NAME);
	XChangeProperty(display_, icon_window_, 
		XInternAtom(display_, "_NET_WM_NAME", False),
		XInternAtom(display_, "UTF8_STRING", False),
		8, PropModeReplace, 
		(unsigned char*) wm_name.c_str(), wm_name.size());

	// Map the window
	XMapWindow(display_,icon_window_);

	// Dock the window (if possible)
	dock();
}

void FreeDesktopTray::dock()
{
	if (dock_) {
		sendTrayOpcode(OPCODE_TRAY_DOCK, icon_window_);
	}
}

void FreeDesktopTray::start()
{
	// Open the display
	display_ = XOpenDisplay(NULL);
	assert(display_);

	// Find the screen
	screen_ = DefaultScreen(display_);

	// Initialize some atoms
	std::ostringstream os;
	os << "_NET_SYSTEM_TRAY_S" << screen_;
	selection_atom_ = XInternAtom(display_, os.str().c_str(), False);
	manager_atom_ = XInternAtom(display_, "MANAGER", False);

	// Create the pixmaps
	XpmAttributes attributes;
	attributes.valuemask = XpmDepth;
	attributes.depth = DefaultDepth(display_,screen_);
	int result;
	result = XpmCreatePixmapFromData(display_, DefaultRootWindow(display_), messages_icon, &messages_icon_.pixmap, &messages_icon_.mask, &attributes);
	assert(result == 0);
	messages_icon_.width = attributes.width;
	messages_icon_.height = attributes.height;
	result = XpmCreatePixmapFromData(display_, DefaultRootWindow(display_), nomessages_icon, &nomessages_icon_.pixmap, &nomessages_icon_.mask, &attributes);
	assert(result == 0);
	nomessages_icon_.width = attributes.width;
	nomessages_icon_.height = attributes.height;
	result = XpmCreatePixmapFromData(display_, DefaultRootWindow(display_), error_icon, &error_icon_.pixmap, &error_icon_.mask, &attributes);
	assert(result == 0);
	error_icon_.width = attributes.width;
	error_icon_.height = attributes.height;

	// Listen to incoming manager events
	XSelectInput(display_, DefaultRootWindow(display_), StructureNotifyMask);

	// Find the tray
	tray_window_ = XGetSelectionOwner(display_, selection_atom_);

	// Create the icon window
	createIconWindow();

	running_ = true;
	XEvent event;
	while (running_) {
		XNextEvent(display_, &event);
		//std::cout << "Event: " << eventNames[event.type] << std::endl;
		switch (event.type) {
			case Expose:
				drawIcon();
				break;
			case ConfigureNotify:
				icon_window_width_ = event.xconfigure.width;
				icon_window_height_ = event.xconfigure.height;
				drawIcon();
				break;
			case ClientMessage: 
				if (event.type == ClientMessage && event.xclient.message_type == manager_atom_ && event.xclient.data.l[1] == selection_atom_) {
					tray_window_ = XGetSelectionOwner(display_, selection_atom_);
					dock();
				}
			case DestroyNotify:
				if (event.xdestroywindow.window == icon_window_) {
					tray_window_ = XGetSelectionOwner(display_, selection_atom_);
					createIconWindow();
				}
			default:
				break;
		}
	}

	XDestroyWindow(display_,icon_window_);
	XFreePixmap(display_,messages_icon_.pixmap);
	XFreePixmap(display_,messages_icon_.mask);
	XFreePixmap(display_,nomessages_icon_.pixmap);
	XFreePixmap(display_,nomessages_icon_.mask);
	XFreePixmap(display_,error_icon_.pixmap);
	XFreePixmap(display_,error_icon_.mask);
}

FreeDesktopTray* FreeDesktopTray::instance_ = NULL;
