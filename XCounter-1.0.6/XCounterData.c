/*

XCounterData.c

XCounter is an X11-based traffic monitoring program for Linux 
that displays information about IP traffic on selected interface.

This software is open-source and is distributed under the terms of the GNU
General Public License.

Version 1.0.6
Copyright (c) 1999-2002  Sergey I. Rotar

If you find any errors please mail to m_rsi@usa.net

*/

#include "XCounterData.h"

int get_data(FILE *dev_file, char iface[8])
{                      
	char buf[BUF_SIZE];
	unsigned long values[16];
	char *strpnt;
	int num, result = SCAN_ERROR;
    
	rewind(dev_file); // fseek(dev_file, 0, SEEK_SET);
	
	while (!feof(dev_file)) {

		fgets(buf, BUF_SIZE, dev_file);
		
		if (ferror(dev_file))
			return FILE_READ_ERROR;
			
		if ( (strpnt = strstr(buf, iface)) != 0  ) {
			strpnt += strlen(iface);
			strpnt++;
			num = sscanf(strpnt, "%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
				&values[0], &values[1], &values[2], &values[3], &values[4], &values[5],
				&values[6], &values[7], &values[8], &values[9], &values[10], &values[11],
				&values[12], &values[13], &values[14], &values[15]);
               
			switch (num) {
				case 11:
					bytes.in = (u_int64_t) values[0];
					bytes.out = (u_int64_t) values[5];
					result = READ_PACKETS;
					break;
				case 14:
					bytes.in = (u_int64_t) values[0];
					bytes.out = (u_int64_t) values[6];
					result = READ_BYTES;
					break;
				case 16:
					bytes.in = (u_int64_t) values[0];
					bytes.out = (u_int64_t) values[8];
					result = READ_BYTES;
					break;
				default:
					bytes.in = 0;
					bytes.out = 0;
					result = SCAN_ERROR;
			}
		}
	}

	return result;
}

int prev_user = 0, prev_nice = 0, prev_sys = 0, prev_idle = 0;

int get_cpu_load()
{   
	FILE *stat_file;
	char buf[BUF_SIZE];
	int user, nice, sys, idle, n, c;
	
	if ((stat_file = fopen("/proc/stat", "r")) == NULL) 
		return 0;
	
	fgets(buf, BUF_SIZE, stat_file);

	if (strncmp(buf, "cpu", 3)) {
		fclose(stat_file);
		return 0;
	} 
    
	if (sscanf(&buf[3], "%u %u %u %u", &user, &nice, &sys, &idle) != 4) {
		fclose(stat_file);   
		return 0;
	}
    
	fclose(stat_file);

	n = idle - prev_idle;
	c = (user - prev_user) + (nice - prev_nice) + (sys - prev_sys) + n;
	n = (c != 0) ? (100 * n / c) : 100;
	prev_user = user; 
	prev_nice = nice; 
	prev_sys = sys; 
	prev_idle = idle;
	
	return (100 - n);
}
