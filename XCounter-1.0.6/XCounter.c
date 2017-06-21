/*

XCounter.c - main module.

XCounter is an X11-based traffic monitoring program for Linux 
that displays information about IP traffic on selected interface.

This software is open-source and is distributed under the terms of the GNU
General Public License.

Version 1.0.6
Copyright (c) 1999-2002  Sergey I. Rotar

If you find any errors please mail to m_rsi@usa.net

*/

#include "XCounter.h"

void init_dpy() 
{        
	if ((dpy = XOpenDisplay(NULL)) == NULL) exit(1);
}

void init_font(char *program) 
{         
	if ((fontName = XGetDefault(dpy, program, "font")) == NULL) 
		fontName="fixed";
	
	if ((fontstruct = XLoadQueryFont(dpy, fontName)) == NULL) {
		fprintf(stderr, "%s: display %s doesn't know font %s\n",
		program, DisplayString(dpy), fontName);
		exit(1);
	}
}

void init_colors(char *program) 
{
	char       *tempstr;		/* Temporary string */
	XColor      color;		/* Temporary color */
	Colormap    cmap;		/* Color map to use */
    
	cmap = DefaultColormap(dpy, DefaultScreen(dpy));
    
	if ((tempstr = XGetDefault(dpy, program, "bordercolor")) == NULL
		|| XParseColor(dpy, cmap, tempstr, &color) == 0
		|| XAllocColor(dpy, cmap, &color) == 0)
		bd = WhitePixel(dpy, DefaultScreen(dpy));
	else
		bd = color.pixel;
    
	if (!XParseColor(dpy, cmap, bgcolor, &color)
		|| !XAllocColor(dpy, cmap, &color)) 
		bg = BlackPixel(dpy, DefaultScreen(dpy));
	else
		bg = color.pixel;
    
	if (!XParseColor(dpy, cmap, fgcolor1, &color)
		|| !XAllocColor(dpy, cmap, &color))
		fg1 = WhitePixel(dpy, DefaultScreen(dpy));
	else
		fg1 = color.pixel;
    
	if (!XParseColor(dpy, cmap, fgcolor2, &color)
		|| !XAllocColor(dpy, cmap, &color))
		fg2 = WhitePixel(dpy, DefaultScreen(dpy));
	else
		fg2 = color.pixel;
    
	if (!XParseColor(dpy, cmap, txcolor, &color)
		|| !XAllocColor(dpy, cmap, &color))
		tx = WhitePixel(dpy, DefaultScreen(dpy));
	else
		tx = color.pixel;
}

void init_geom(char *iface) 
{
	int i;
	char *c;
	FILE *conf;
	char var[16];
	char value[8]; 
	char buf[BUF_SIZE];
	char conf_file[BUF_SIZE];
	char *conf_var[8] = { "interface", "update", "ontop", "cpuload", 
				"x", "y", "height", "width" };
	char conf_value[8][8] = { "ppp0", "3", "1", "1",
				"0", "0", "60", "140" };
    
	snprintf(conf_file, sizeof(conf_file), "%s/.XCounter", getenv("HOME"));
	conf_file[sizeof(conf_file)-1] = 0x00;
    
	if ((conf = fopen(conf_file, "r")) != NULL) {
		while (!feof(conf)) {
			fgets(buf, BUF_SIZE, conf); 
			if (ferror(conf)) {
				fclose(conf);
				break;
			}

	    		if ((c = strchr(buf, '#')) != NULL) 
			*c = 0x00;

			if (sscanf(buf, "%s %s", var, value) == 2) {
				for (i=0; i < 8; i++) {
					if (strcmp(var, conf_var[i]) == 0) {
						strncpy(conf_value[i], value, 8);
						conf_value[i][7] = 0x00;
					}
				}
			}
        	}
        
		fclose(conf);
	}

	strcpy(iface, conf_value[0]);
	speed_update_time = strtol(conf_value[1], (char **)NULL, 10);
	speed_update_time = ( speed_update_time < 1 || speed_update_time > 60 ) ? 3 : speed_update_time;
	ontop = strtol(conf_value[2], (char **)NULL, 10);
	cpuload = strtol(conf_value[3], (char **)NULL, 10);
	xsh.x = strtol(conf_value[4], (char **)NULL, 10);
	xsh.y = strtol(conf_value[5], (char **)NULL, 10);
	xsh.height = strtol(conf_value[6], (char **)NULL, 10);
	xsh.width = strtol(conf_value[7], (char **)NULL, 10);
}

void init_window(int argc, char **argv) 
{
	win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
		xsh.x, xsh.y, xsh.width, xsh.height, bw, bd, bg);

	XSetStandardProperties(dpy, win, PROGRAM, PROGRAM, None, argv, argc, &xsh);
	XSetWMHints(dpy, win, &xwmh);
    
	xswa.colormap = DefaultColormap(dpy, DefaultScreen(dpy));
	XChangeWindowAttributes(dpy, win, CWColormap, &xswa);
    
	gcv.background = bg;
	gcv.foreground = fg1;
	gc = XCreateGC(dpy, win, (GCForeground | GCBackground), &gcv);
    
	XSelectInput(dpy, win, ExposureMask | KeyPressMask | EnterWindowMask |
			LeaveWindowMask | VisibilityChangeMask);
    
	XMapWindow(dpy, win);
	XMoveWindow(dpy, win, xsh.x, xsh.y);
}

void out_time(char *str_iface, char *str_time)
{   
	int x1, x2, y;
	int cpu_load;
	XWindowAttributes xwa; // Temp Get Window Attribute struct 
    
	while (XCheckTypedEvent( dpy, Expose, &ev));
			XGetWindowAttributes(dpy, win, &xwa);
    
	x1 = (xwa.width - XTextWidth(fontstruct, str_iface, strlen(str_iface))) / 2;
	x2 = (xwa.width - XTextWidth(fontstruct, str_time, strlen(str_time))) / 2;
	y = ((xwa.height - xwa.height / 2) + fontstruct->max_bounds.ascent - 
		fontstruct->max_bounds.descent) / 3;
    
	XClearArea(dpy, win, 0, xwa.height / 2, xwa.width, xwa.height, 0);
	XSetForeground(dpy, gc, tx);
	XDrawString(dpy, win, gc, x1, y + xwa.height / 2, str_iface, strlen(str_iface));
	XDrawString(dpy, win, gc, x2, y * 2 + xwa.height / 2, str_time, strlen(str_time));
    
	if (cpuload) { 
		cpu_load = get_cpu_load();
		XSetLineAttributes(dpy, gc, 3, LineSolid, 0, 0);
		x1 = xwa.width;
		y = xwa.height - 3;
        
		if (cpu_load <= 50) {
			XDrawLine(dpy, win, gc, 0, y, (int)(x1 * cpu_load / 100), y);
		} else if (cpu_load <= 75) {
			XDrawLine(dpy, win, gc, 0, y, (int)(x1 * 0.5), y);
			XSetForeground(dpy, gc, fg1);
			XDrawLine(dpy, win, gc, (int)(x1 * 0.5), y, (int)(x1 * cpu_load / 100), y);
		} else {
			XDrawLine(dpy, win, gc, 0, y, (int)(x1 * 0.5), y);
			XSetForeground(dpy, gc, fg1);
			XDrawLine(dpy, win, gc, (int)(x1 * 0.5), y, (int)(x1 * 0.75), y);
			XSetForeground(dpy, gc, fg2);
			XDrawLine(dpy, win, gc, (int)(x1 * 0.75), y, (int)(x1 * cpu_load / 100), y);
		}
	} 
}

void out_counter(float speed_in, 
		float speed_out, 
		u_int64_t bytes_in_since_up, 
		u_int64_t bytes_out_since_up, 
		boolean since_up) 
{   
	int x, y;
	XWindowAttributes xwa;
	char str1[32];
	char str2[32];
	char *str_bytes[3] =   { "%llu  %.2fKB/s", 
                               	     "%.1fK  %.2fKB/s", 
                               	     "%.1fM  %.2fKB/s" };
	char *str_packets[3] = { "%llu  %.2fP/s", 
                                     "%.1fK  %.2fP/s", 
                                     "%.1fM  %.2fP/s" };
	since_up = !since_up;
	 
	switch (res) {
		case FILE_OPEN_ERROR: 
			strcpy(str1, "Can't open file :");
			strcpy(str2, "  /proc/net/dev  ");
			break;
		case FILE_READ_ERROR:
			strcpy(str1, "Can't read file :");
			strcpy(str2, "  /proc/net/dev  ");
			break;
		case READ_BYTES:
			if ((bytes.in - bytes_in_since_up * since_up) >= MB * 1024)  
				snprintf(str1, sizeof(str1), str_bytes[2],
                   		(float)(bytes.in - bytes_in_since_up * since_up) / MB, speed_in); 
			else if ((bytes.in - bytes_in_since_up * since_up) >= MB)  
            			snprintf(str1, sizeof(str1), str_bytes[1], 
				(float)(bytes.in - bytes_in_since_up * since_up) / KB, speed_in);
			else snprintf(str1, sizeof(str1), str_bytes[0], 
				(bytes.in - bytes_in_since_up * since_up), speed_in);

			if ((bytes.out - bytes_out_since_up * since_up) >= MB * 1024)
				snprintf(str2, sizeof(str2), str_bytes[2], 
				(float)(bytes.out - bytes_out_since_up * since_up) / MB, speed_out);
			else if ((bytes.out - bytes_out_since_up * since_up) >= MB)
				snprintf(str2, sizeof(str2), str_bytes[1],
				(float)(bytes.out - bytes_out_since_up * since_up) / KB, speed_out);
			else snprintf(str2, sizeof(str2), str_bytes[0], 
				bytes.out - bytes_out_since_up * since_up, speed_out); 
		     
			str1[sizeof(str1)-1] = 0x00;
			str2[sizeof(str2)-1] = 0x00;
			break;
		case READ_PACKETS:
			speed_in = speed_in * KB;
			speed_out = speed_out * KB;
        
			if ((bytes.in - bytes_in_since_up * since_up) >= MB*1024)  
				snprintf(str1, sizeof(str1), str_packets[2], 
				(float)(bytes.in - bytes_in_since_up * since_up) / MB, speed_in); 
			else if ((bytes.in - bytes_in_since_up * since_up) >= MB)
				snprintf(str1, sizeof(str1), str_packets[1], 
				(float)(bytes.in - bytes_in_since_up * since_up) / KB, speed_in);
			else snprintf(str1, sizeof(str1), str_packets[0],
				bytes.in - bytes_in_since_up * since_up, speed_in);

			if ((bytes.out - bytes_out_since_up * since_up) >= MB*1024)
				snprintf(str2, sizeof(str2), str_packets[2],
				(float)(bytes.out - bytes_out_since_up * since_up) / MB, speed_out);
			else if ((bytes.out - bytes_out_since_up * since_up) >= MB)
				snprintf(str2, sizeof(str2), str_packets[1], 
				(float)(bytes.out - bytes_out_since_up * since_up) / KB, speed_out);
			else snprintf(str2, sizeof(str2), str_packets[0],
				bytes.out - bytes_out_since_up * since_up, speed_out); 

			str1[sizeof(str1)-1] = 0x00;
			str2[sizeof(str2)-1] = 0x00;
			break;
		default:
			strcpy(str1, "0  0.0KB/s");
			strcpy(str2, "0  0.0KB/s");
	}
    
	while(XCheckTypedEvent(dpy, Expose, &ev));
		XGetWindowAttributes(dpy, win, &xwa);
    
	x = (xwa.width - XTextWidth(fontstruct, str1, strlen(str1))) / 2 + 5;
	y = ((xwa.height - xwa.height / 2) + fontstruct->max_bounds.ascent - 
		fontstruct->max_bounds.descent) / 3;
    
	XClearArea(dpy, win, 0, 0, xwa.width, xwa.height-xwa.height/2, 0);
	XSetForeground(dpy, gc, tx);
	XDrawString(dpy, win, gc, x, y, str1, strlen(str1));
	XDrawString(dpy, win, gc, x, y * 2, str2, strlen(str2));
}

void save_conf(char iface[8])
{
	FILE *conf;
	Window root_win;
	char conf_file[BUF_SIZE];
    
	XGetGeometry(dpy, win, &root_win, &xsh.x, &xsh.y, &xsh.width, &xsh.height, &bw, &dw);

	snprintf(conf_file, sizeof(conf_file), "%s/.XCounter", getenv("HOME"));
	conf_file[sizeof(conf_file)-1] = 0x00;

	if ((conf = fopen(conf_file, "w")) != NULL) {
		fprintf(conf, "#XCounter's config file\n\n");
		fprintf(conf, "interface %s\n", iface);
		fprintf(conf, "update %d\n", speed_update_time);
		fprintf(conf, "ontop %d\n", ontop);
		fprintf(conf, "cpuload %d\n", cpuload);
		fprintf(conf, "x %d\n", xsh.x);
		fprintf(conf, "y %d\n", xsh.y);
		fprintf(conf, "height %d\n", xsh.height);
		fprintf(conf, "width %d\n", xsh.width);
		fclose(conf);
	}
}

void usage(char *program)
{
	fprintf(stderr, "usage: %s [-i <interface>] [-t <sec>] [-oc <TRUE|FALSE>] [-hv]\n", program);
	fprintf(stderr, "  -i an interface name (default is ppp0)\n");
	fprintf(stderr, "  -t number of seconds to update speed\n");
	fprintf(stderr, "     (default is 3, min is 1, max is 60)\n");
	fprintf(stderr, "  -o always on top (default is TRUE)\n");
	fprintf(stderr, "  -c show cpu load (default is TRUE)\n");
	fprintf(stderr, "  -v display version of the program\n");
	fprintf(stderr, "  -h this help\n\n");
}

int main(int argc, char **argv) 
{
	char opt;
	char iface[8];
	char kb_buf[KB_BUF_LEN];
	char str_iface[20];
	char str_time[20];
    
    	FILE *dev_file;
    
	time_t sec;
	time_t time_since_up;
	time_t time_begin;
	time_t time_before;
	time_t time_now;
    
	unsigned int n_in = 0; 
	unsigned int n_out = 0;
	unsigned int state = 0;
	unsigned int min, hour;

	float speed_in = 0.0;
	float speed_out = 0.0;
    
	u_int64_t check_in = 0;
	u_int64_t check_out = 0;
	u_int64_t bytes_in_before = 0;
	u_int64_t bytes_out_before = 0;
	u_int64_t bytes_in_since_up = 0;
	u_int64_t bytes_out_since_up = 0;

	init_geom(iface);
	
	while ((opt = getopt(argc, argv, "i:t:o:c:vh")) != -1) {
        	switch (opt) {
			case 'i': 
				strncpy(iface, optarg, sizeof(iface));
				iface[sizeof(iface)-1] = 0x00;
				break;
			case 't':
				speed_update_time = strtol(optarg, (char **)NULL, 10);
				if ( speed_update_time < 1 || speed_update_time > 60 ) {
					usage(argv[0]);
					exit (0);
				}
				break;
			case 'o':
				if (!strcmp(optarg, "FALSE"))
					ontop = FALSE;
				else if (!strcmp(optarg, "TRUE"))
					ontop = TRUE;
				else {
					usage(argv[0]);
					exit (0);
				}
				break;
			case 'c':
				if (!strcmp(optarg, "FALSE"))
					cpuload = FALSE;
				else if (!strcmp(optarg, "TRUE"))
					ontop = TRUE;
				else {
					usage(argv[0]);
					exit (0);
				}
				break;
			case 'v':
				fprintf(stdout, "%s %s\n", PROGRAM, VERSION);
				fprintf(stdout, "Copyright (C) 1999-2002\n");
				fprintf(stdout, "Sergey I. Rotar <m_rsi@usa.net>\n\n");
				exit (0);
			case 'h':
			default:
				usage(argv[0]);
				exit (0);
		}
	}

	sprintf(str_iface, "Interface %s", iface);
	
	if ((dev_file = fopen("/proc/net/dev","r")) == NULL) {
		fprintf(stderr, "%s: can't open the file /proc/net/dev\n", PROGRAM);
		exit(1);
	}
	
	init_dpy();
	init_font(argv[0]);
	init_colors(argv[0]);
	init_window(argc, argv);

	XClearWindow(dpy, win);
   
	time_since_up = time_begin = time(&time_begin);
    
	while (state != 3) {
        
		if (!state) {

			res = get_data(dev_file, iface);
          
			if ((check_in > bytes.in) && (speed_in > 0.0)) 
				n_in++;
			if ((check_out > bytes.out) && (speed_out > 0.0)) 
				n_out++;
			check_in =  bytes.in;
			check_out =  bytes.out;
			bytes.in = bytes.in + UL * n_in;
			bytes.out = bytes.out + UL * n_out;
            
			if ((bytes.in == 0) && (bytes.out == 0)) { 
				time_begin = time(&time_begin);
				bytes_in_since_up = 0;
				bytes_out_since_up = 0;
				bytes_in_before = 0;
				bytes_out_before = 0;
			}
            
			if (update) {
				update = FALSE;
				bytes_in_before = bytes.in; 
				bytes_out_before = bytes.out;
				time_before = time(&time_before);
			} 
            
			time_now = time(&time_now);
			if (time_now-time_before >= speed_update_time) {
				if (bytes.in >= check_in)
					speed_in = (float)(bytes.in - bytes_in_before) / (time_now - time_before) / KB;
				if (bytes.out >= check_out)
                    			speed_out = (float)(bytes.out - bytes_out_before) / (time_now - time_before) / KB;
				update = TRUE;
			}

			sec = time_now - time_begin;
			hour = sec / 3600;
			min = sec / 60 - hour * 60;
			sprintf(str_time, "%.3d:%.2d.%.2ld", hour, min, sec%60);
			out_counter(speed_in, speed_out, bytes_in_since_up, bytes_out_since_up, FALSE);
			out_time(str_iface, str_time);
        	}
        
		while (XPending(dpy)) {
            	
			XNextEvent(dpy, &ev);
			if (ev.xany.window == win)
			switch (ev.type) {
                		case VisibilityNotify:
					if (ontop)
						XRaiseWindow(dpy, win);
					break;
				case KeyPress:
					if (XLookupString((XKeyEvent*)&ev, kb_buf, KB_BUF_LEN, NULL, NULL) == 1) {
	                       			switch (kb_buf[0]) {
							case 'Q': // Quit
								state = 3;
								break;
							case 'C': // CPU Load enable/disable
								cpuload = !cpuload;
								break;
							case 'A': // Always On Top 
								ontop = !ontop;
								break;
							case 'R': // Reset counter
								time_begin = time(&time_begin);
								bytes_in_since_up = bytes.in;
								bytes_out_since_up = bytes.out;
								break; 
							case 'S': // Save config
								save_conf(iface);
						}
					}
					break;
				case EnterNotify: 
					state = 1;
					time_now = time(&time_now);
					sec = time_now - time_since_up;
					hour = sec / 3600;
					min = sec / 60 - hour * 60;
					sprintf(str_time, "%.3d:%.2d.%.2ld", hour, min, sec%60);
					out_counter(speed_in, speed_out, bytes_in_since_up, bytes_out_since_up, TRUE);
					out_time(str_iface, str_time);
					break;
				case LeaveNotify: 
					state = 0;
			}
		}
   
		usleep(100000); // 1/10 sec
	} 

	XFreePixmap(dpy, pixmap);
	
	fclose(dev_file);
	
	return 0;
}
