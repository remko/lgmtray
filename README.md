# [LGMTray: Lightweight GMail Notifier](http://el-tramo.be/software/lgmtray)

## About

LGMTray is a lightweight application that docks in your system tray, and
notifies you when new mail has arrived on your GMail account. 

It is written in
C++, is designed to have very few dependencies, and can therefore be easily
built on many systems. Currently, only an X11 implementation is available
(which uses the [FreeDesktop.org System Tray Protocol](http://standards.freedesktop.org/systemtray-spec/systemtray-spec-latest.html)), and the
features are very limited.

## Usage

Copy the sample configuration file `lgmtray.sample_config`, rename
it to `~/.lgmtray`, and edit it to reflect your username/password and 
preferences. Then, simply run the application by calling `lgmtray`.

## Building

First, make sure you have the necessary libraries installed. LGMTray currently needs the following libraries:

- [libcurl](http://curl.haxx.se) (with OpenSSL support)
- [libxml2](http://xmlsoft.org/)
- libxpm
- Xlib (ships with your X installation)

To build the project, run

		make

If you have a special system setup, you should edit `Makefile.config`
to reflect this in the build parameters.

## Known Issues

On one of my machines, Curl crashes very regularly with a SIGPIPE when opening
a connection. I have reported this to the Curl developers, but no cause has
been found so far. As a workaround, LGMTray currently ignores SIGPIPE signals,
and continues as normal. This should have no visual impact, except log
messages saying that there was a problem, and that the application continues
anyway.

## TODO
- Allow the background of the tray to be transparent, taking the system 
  tray's background color.
- Make the check interval dynamic, where mail is checked more frequently 
  after new mail has arrived, less frequently when there is no mouse 
	movement, ...
- Open GMail when the icon is clicked
- Add a tooltip and a context menu

## Links

- [Official GMail notifier](http://toolbar.google.com/toolbar/gmail-helper/): Works only on Windows and Mac OS X.
- [GMail Notifier](http://gmail-notify.sourceforge.net/): Written in 
	Python, but has dependencies on several add-on Python modules (e.g. Gtk).
