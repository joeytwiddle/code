#include <sys/ipc.h>
#include <sys/shm.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>    /* For the visual matching */
#include <X11/extensions/XShm.h>

#include <stdlib.h>
#include <stdio.h>

/* These are for the CPU time measurement. */
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

int get_xvisinfo_class(XVisualInfo xvi);

int get_visual_class(Visual *v);
