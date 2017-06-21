/*

XCounterData.h - definitions and structure definition
                 for XCounter.c and XCounterData.c

*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

typedef int boolean; 
#define TRUE		1
#define FALSE		0

#define KB 		1024
#define MB 		1048576
#define UL 		4294967295.0	/* unsigned long */
#define BUF_SIZE	512
#define KB_BUF_LEN	2

#define FILE_OPEN_ERROR	-1		/* Can't open file */
#define FILE_READ_ERROR	-2		/* Can't read file */
#define SCAN_ERROR	-3		/* File scan error */
#define READ_BYTES	1		/* Reads byte counts */
#define READ_PACKETS	2		/* Reads packet counts */

struct bc {
    u_int64_t in;
    u_int64_t out;
} bytes;

int get_data(FILE *dev_file, char iface[8]);
int get_cpu_load();
