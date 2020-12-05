#include <stdlib.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <cpcdos/logger.h>

static Display *_display = NULL;
static Window _root;
static bool _a_wm_already_exist = false;

static int
wm_init(void)
{
	_display = XOpenDisplay(NULL);
	if (_display == NULL)
	{
		LOG(LOG_ERR, "Failed to open X dispaly '%s'", XDisplayName(NULL));
		return (-1);
	}

	_root = DefaultRootWindow(_display);
	return (0);
}

static int
wm_detected(Display *display, XErrorEvent *e)
{
	(void)display;

	if (e->error_code == BadAccess)
		_a_wm_already_exist = true;

	return (0);
}

static int
wm_on_error(Display *display, XErrorEvent *e)
{
	char buffer[1024];

	XGetErrorText(display, e->error_code, buffer, 1024);
	LOG(LOG_ERR, "X error: %s", buffer);

	return (0);
}

int
wm_run(void)
{
	XEvent event;

	if (wm_init() != 0)
		return (-1);
	
	XSetErrorHandler(&wm_detected);
	XSelectInput(_display, _root,
		SubstructureNotifyMask | SubstructureRedirectMask);
	XSync(_display, false);
	if (_a_wm_already_exist)
	{
		LOG(LOG_ERR, "Another window manager is already running on display '%s'",
			XDisplayString(_display));
		return (-1);
	}
	XSetErrorHandler(&wm_on_error);

	for (;;)
	{
		XNextEvent(_display, &event);
    }

	return (0);
}

void
wm_stop(void)
{
	if (_display != NULL)
		XCloseDisplay(_display);
}