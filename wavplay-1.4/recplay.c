/* $Header: /cvsroot/pengaol/pengaol/wavplay-1.4/recplay.c,v 1.1.1.1 2001/08/22 12:58:27 birdy57 Exp $
 * Warren W. Gay VE3WWG		Sun Feb 16 20:14:02 1997
 *
 * RECORD/PLAY MODULE FOR WAVPLAY :
 *
 * 	X LessTif WAV Play :
 * 
 * 	Copyright (C) 1997  Warren W. Gay VE3WWG
 * 
 * This  program is free software; you can redistribute it and/or modify it
 * under the  terms  of  the GNU General Public License as published by the
 * Free Software Foundation version 2 of the License.
 * 
 * This  program  is  distributed  in  the hope that it will be useful, but
 * WITHOUT   ANY   WARRANTY;   without   even  the   implied   warranty  of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details (see enclosed file COPYING).
 * 
 * You  should have received a copy of the GNU General Public License along
 * with this  program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 * Send correspondance to:
 * 
 * 	Warren W. Gay VE3WWG
 * 
 * Email:
 *	ve3wwg@yahoo.com
 *	wgay@mackenziefinancial.com
 *
 * $Log: recplay.c,v $
 * Revision 1.1.1.1  2001/08/22 12:58:27  birdy57
 *
 *
 * Revision 1.2  1999/12/04 00:01:20  wwg
 * Implement wavplay-1.4 release changes
 *
 * Revision 1.1.1.1  1999/11/21 19:50:56  wwg
 * Import wavplay-1.3 into CVS
 *
 * Revision 1.3  1997/04/19 01:24:22  wwg
 * 1.0pl2 : Removed the extraneous ' after Hz in the wav info
 * display. Also removed some extraneous ';' from the same
 * series of prints.
 *
 * Revision 1.2  1997/04/17 23:42:02  wwg
 * Added #include <errno.h> for 1.0pl2 fix.
 *
 * Revision 1.1  1997/04/14 00:18:31  wwg
 * Initial revision
 *
 */
static const char rcsid[] = "@(#)recplay.c $Revision: 1.1.1.1 $";

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <linux/soundcard.h>
#include "wavplay.h"
#include "server.h"

static int um = 0666;				/* Current umask() value */
static ErrFunc v_erf;				/* Error function for reporting */

/*
 * Play a series of WAV files:
 */
int
wavplay(WavPlayOpts *wavopts,char **argv,ErrFunc erf) {
	WAVFILE *wfile;				/* Opened wav file */
	DSPFILE *dfile = NULL;			/* Opened /dev/dsp device */
	const char *Pathname;			/* Pathname of the open WAV file */
	int e;					/* Saved error code */

	if ( erf != NULL )			/* If called from external module.. */
		v_erf = erf;			/* ..set error reporting function */

	if ( (Pathname = *argv) == NULL )
		Pathname = "-";			/* Standard input */
	else	Pathname = *argv++;		/* Point to first pathname on command line */

	/*
	 * Play each Pathname:
	 */
	do	{
		if ( cmdopt_x )
			fprintf(stderr,"Playing WAV file %s :\n",Pathname);

		/*
		 * Open the wav file for read, unless its stdin:
		 */
		if ( (wfile = WavOpenForRead(Pathname,v_erf)) == NULL )
			goto errxit;

		/*
		 * Merge in command line option overrides:
		 */
		WavReadOverrides(wfile,wavopts);

		/*
		 * Send current settings to client when in server mode:
		 */
		if ( clntIPC >= 0 )
			toclnt_settings(0,wfile,wavopts);

		/*
		 * Report the file details, unless in quiet mode:
		 */
		if ( !wavopts->bQuietMode && clntIPC < 0 ) {
			printf("Pathname:\t%s\n",wfile->Pathname);
			printf("Device:\t\t%s\n",env_AUDIODEV);
			printf("Sampling Rate:\t%lu Hz\n",(unsigned long)wfile->wavinfo.SamplingRate);
			printf("Mode:\t\t%s\n",wfile->wavinfo.Channels == Mono ? "Mono" : "Stereo");
			printf("Samples:\t%lu\n",(unsigned long)wfile->wavinfo.Samples);
			printf("Bits:\t\t%u\n\n",(unsigned)wfile->wavinfo.DataBits);
		}

		if ( !wavopts->bInfoMode ) {
			/*
			 * If not -i mode, play the file:
			 */
			if ( (dfile = OpenDSP(wfile,O_WRONLY,v_erf)) == NULL )
				goto errxit;

			if ( PlayDSP(dfile,wfile,svr_work_proc,v_erf) )
				goto errxit;

			if ( CloseDSP(dfile,v_erf) ) {	/* Close /dev/dsp */
				dfile = NULL;		/* Mark it as closed */
				goto errxit;
			}
		}
		
		dfile = NULL;				/* Mark it as closed */
		if ( WavClose(wfile,v_erf) )		/* Close the wav file */
			wfile = NULL;			/* Mark the file as closed */
		wfile = NULL;				/* Mark the file as closed */
	} while ( (Pathname = *argv++) != NULL );

	return 0;

	/*
	 * Error exit:
	 */
errxit:	e = errno;					/* Save errno value */
	if ( wfile != NULL )
		WavClose(wfile,NULL);			/* Don't report errors here */
	if ( dfile != NULL )
		CloseDSP(dfile,NULL);			/* Don't report errors here */
	errno = e;					/* Restore error code */
	return -1;
}

/*
 * Record a WAV file:
 */
int
wavrecd(WavPlayOpts *wavopts,char *Pathname,void (*erf)(const char *format,va_list ap)) {
	WAVFILE *wfile;				/* Opened wav file */
	DSPFILE *dfile = NULL;			/* Opened /dev/dsp device */
	int e;					/* Saved error code */
	UInt32 samples;				/* The number of samples to record */
	int bServerMode = 0;			/* True if in server mode */

	if ( erf != NULL )			/* If called from external module.. */
		v_erf = erf;			/* .. then set error reporting function */

	samples = (UInt32) wavopts->Seconds * (UInt32) wavopts->SamplingRate.optValue;

	/*
	 * Open the wav file for read, unless its stdin:
	 */
	if ( Pathname != NULL ) {
		if ( (wfile = WavOpenForWrite(Pathname,wavopts->Channels.optValue,
			wavopts->SamplingRate.optValue,wavopts->DataBits.optValue,samples,
			v_erf)) == NULL ) {
			goto errxit;
		}
	} else	{
		bServerMode = 1;		/* We're in server mode... */
		Pathname = svr.path;		/* so this is the pathname.. */
		wfile = svr.wfile;		/* And the file is already opened */
	}

	if ( (dfile = OpenDSP(wfile,O_RDWR,v_erf)) == NULL )
		goto errxit;

	if ( RecordDSP(dfile,wfile,samples,svr_work_proc,v_erf) )
		goto errxit;

	if ( CloseDSP(dfile,v_erf) ) {		/* Close /dev/dsp */
		dfile = NULL;			/* Mark it as closed */
		goto errxit;
	}
	dfile = NULL;				/* Mark it as closed */

	WavClose(wfile,v_erf);			/* Close the wav file */
	wfile = NULL;				/* Mark the file as closed */

	if ( bServerMode )
		svr.wfile = NULL;		/* Indicate no open file now */

	return 0;

	/*
	 * Error exit:
	 */
errxit:	e = errno;				/* Save errno value */
	if ( wfile != NULL )
		WavClose(wfile,NULL);
	if ( dfile != NULL )
		CloseDSP(dfile,NULL);
	errno = e;				/* Restore error code */
	return -1;
}

/*
 * Record or Play WAV files:
 */
int
recplay(WavPlayOpts *wavopts,char **argv,ErrFunc erf) {
	int rc;					/* Return code */
	int e;					/* Saved errno value */

	v_erf = erf;				/* Error function to use */

	/*
	 * Find out what are umask setting is :
	 */
	um = umask(0666);			/* Discover umask setting by changing it.. */
	umask(um);				/* Now Restore umask setting */

	/*
	 * Lock the device :
	 */
	if ( wavopts->ipc >= 0 && LockDSP(wavopts->ipc,wavopts->Mode==OprPlay?0:1,v_erf,0) )
		return -1;			/* Failed */

	if ( wavopts->Mode == OprPlay )
		rc = wavplay(wavopts,argv,NULL); /* Play samples */
	else 	rc = wavrecd(wavopts,*argv,NULL); /* Record samples */

	e = errno;				/* Save errno value */

	/*
	 * Unlock the device:
	 */
	if ( wavopts->ipc >= 0 && UnlockDSP(wavopts->ipc,wavopts->Mode==OprPlay?0:1,v_erf) )
		if ( !rc ) {
			e = errno;
			rc = -1;
		}

	errno = e;
	return rc;
}

/* $Source: /cvsroot/pengaol/pengaol/wavplay-1.4/recplay.c,v $ */
