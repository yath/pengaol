/***************************************************************************
                          threadELV3.cpp  -  description
                             -------------------
    begin                : Mon Jul 30 2001
    copyright            : (C) 2001 by stephane (birdy57)
    email                : birdy57@pengaol.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "threadELV3.h"
CAolCmd30 *GCmd;
CAolToClient30 *GAolClient;
CClientToAol30 *GClientAol;
pthread_t Thread1;
pthread_t Thread2;
pthread_t Thread3;

void *StartCmd(void *)
{
GCmd->Main();
return 0;
}

void *StartATC(void *)
{
pthread_setcanceltype(PTHREAD_CANCEL_ENABLE,NULL);
GAolClient->Main();
return 0;
}

void *StartCTA(void *)
{
pthread_setcanceltype(PTHREAD_CANCEL_ENABLE,NULL);
GClientAol->Main();
return 0;
}


void StartELV3Sub(CAolCmd30 *Cmd,CAolToClient30 *AolClient,CClientToAol30  *ClientAol)
{
int res;

GCmd=Cmd;
GAolClient=AolClient;
GClientAol=ClientAol;

res=pthread_create(&Thread1,NULL,StartCmd,NULL);
res=pthread_create(&Thread2,NULL,StartATC,NULL);
res=pthread_create(&Thread3,NULL,StartCTA,NULL);

res=pthread_join(Thread1,NULL);
}

void KillELV3Sub()
{
pthread_cancel(Thread2);
pthread_cancel(Thread3);
}

