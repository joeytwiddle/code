#include "xvvis.h"

int get_xvisinfo_class(XVisualInfo xvi) {
	// return 1;
	return xvi.class;
}

int get_visual_class(Visual *xv) {
	// return 1;
	return xv->class;
}
