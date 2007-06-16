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
	void setIcon(IconName icon);
	void showMessage(const std::string& message);

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

private:
	FreeDesktopTray();
	~FreeDesktopTray();

	static FreeDesktopTray* instance_;
	IconName icon_;
	bool running_;
	Display* display_;
	int screen_;
	Window icon_window_;
	Window tray_window_;
	int icon_window_width_, icon_window_height_;
	Icon messages_icon_, nomessages_icon_, error_icon_;
	int manager_atom_, selection_atom_;
	int message_id_;
	
};

#endif
