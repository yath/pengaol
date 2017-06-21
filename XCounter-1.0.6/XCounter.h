/*

XCounter.h - the header file for XCounter.c

*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "XCounterData.h"

#define PROGRAM		"X Counter"
#define VERSION		"1.0.6"

XWMHints xwmh = {
	(InputHint|StateHint),		/* flags */
	True,				/* input */
	NormalState,			/* initial state */
	0,				/* icon pixmap */
	0,				/* icon window */
	0, 0,				/* icon location */
	0,				/* icon mask */
	0,				/* Window group */
};
    
Display    *dpy;			/* display */
Window      win;			/* Window ID */
GC          gc;				/* graphics context */
char       *fontName;			/* font name */
XFontStruct *fontstruct;		/* font structure */
unsigned int fg1, fg2, bg, bd, tx;	/* color descriptors*/
unsigned int bw = 0;			/* border width */
unsigned int dw;			/* depth */
XGCValues   gcv;			/* graphics context structure */
XSizeHints  xsh;			/* window properties structure */
XSetWindowAttributes xswa;		/* window attributies structure */
XEvent ev;				/* event structure */
Pixmap pixmap;				/* pixmap */

char* bgcolor = "black";		/* backgroud color */
char* fgcolor1 = "yellow";		/* medium cpu load color */ 
char* fgcolor2 = "red";			/* high cpu load color*/
char* txcolor = "green";		/* text and low cpu load color */

unsigned int res = 0;
unsigned int speed_update_time = 3;
boolean update = TRUE;
boolean ontop = TRUE;
boolean cpuload = TRUE;
