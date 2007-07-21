#ifndef FREEDESKTOPTRAY_H
#define FREEDESKTOPTRAY_H

#include <X11/Xlib.h>

class FreeDesktopTray
{
public:
	enum IconName {
		Icon_NoMessages,
		Icon_Messages,
		Icon_Error
	};

	static FreeDesktopTray* instance();
	void start();
	void setBackgroundColor(const std::string& color);
	void setIcon(IconName icon);
	void setPopupTimeout(unsigned int timeout);
	void showPopup(const std::string& message);
	void setDock(bool);

protected:
	struct Icon {
		Pixmap pixmap;
		Pixmap mask;
		int width;
		int height;
	};

	void drawIcon();
	const Icon& getIcon() const;
	void createIconWindow();
	void sendTrayOpcode(int opcode, long data1 = 0, long data2 = 0, long data3 = 0);
	void sendTrayData(const std::string&);
	void dock();

private:
	FreeDesktopTray();
	~FreeDesktopTray();

	static FreeDesktopTray* instance_;
	IconName icon_;
	bool running_;
	Display* display_;
	std::string background_color_name_;
	unsigned long background_color_;
	int screen_;
	Window icon_window_;
	Window tray_window_;
	int icon_window_width_, icon_window_height_;
	Icon messages_icon_, nomessages_icon_, error_icon_;
	unsigned int manager_atom_;
	int selection_atom_;
	int message_id_;
	int popup_timeout_;
	bool dock_;
};

#endif
