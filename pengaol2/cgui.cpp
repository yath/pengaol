/***************************************************************************
                          cgui.cpp  -  description
                             -------------------
    begin                : Fri Aug 31 2001
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
#ifndef WIN32

#include "cgui.h"

CGui::CGui()
{
bConnected=false;
}
CGui::~CGui(){
}
/** envoie une commande a PengGui */
bool CGui::SendCommand(char *sCommand)
{
bool bRet;

if (bConnected)
	{
	if (write(nServeurFd,sCommand,strlen(sCommand))>0)
		bRet=true;

	bConnected=bRet;
	}

return bRet;
}
/** Connection a PengGui */
bool CGui::Connect()
{
int sockfd_serveur, sockfd_client;
int long_serveur,long_client;
bool bGood=false;

struct sockaddr_in adresse_serveur;
struct sockaddr_in adresse_client;

sockfd_serveur=socket(AF_INET,SOCK_STREAM,0);

adresse_serveur.sin_family=AF_INET;
adresse_serveur.sin_addr.s_addr=inet_addr("127.0.0.1");
adresse_serveur.sin_port=8128;
long_serveur=sizeof(adresse_serveur);
bind(sockfd_serveur,(struct sockaddr *) &adresse_serveur,long_serveur);
listen(sockfd_serveur,5);

while (bGood==false)
	{
	long_client=sizeof(adresse_client);
	sockfd_client=accept(sockfd_serveur,(struct sockaddr *) &adresse_client,(socklen_t *) &long_client);
	bGood=true;

	if ((adresse_client.sin_addr.s_addr)!=(inet_addr("127.0.0.1")))
		{
		printf("Connection refused !\n");
		bGood=false;
		close(sockfd_client);
		}
	 }

nServeurFd=sockfd_serveur;
nClientFd=sockfd_client;
bConnected=true;

DonneUser();
if (nWindow!=0)
	SetWindow(nWindow);

return true;
}

#endif
/** Lecture des demande du GUI */
void CGui::Receiver()
{
char sOrdre[200];
int nLong;
char sLogin[50];
char sPass[50];

while (1)
{
	while (!bConnected)
		Connect();

	while ((nLong=read(nClientFd,&sOrdre,200))<1);

	switch (sOrdre[0])
		{
	  	case 'L' :	strcpy((char *) &sLogin,(const char *) &sOrdre[1]);
							break;
		case 'P' :	strcpy((char *) &sPass,(const char *) &sOrdre[1]);
							break;
       case 'S' : 	SaveUser((char *) &sLogin,(char *) &sPass);
							break;
		case 'K' : 	KillUser((char *) &sLogin,(char *) &sPass);
							break;
		case 'D' : 	Start((char *) &sLogin);
							break;
		case 'X' : 	Exit(0);
							break;
		}

	}
}
/** definie la fenetre à afficher */
void CGui::SetWindow(int nWin)
{
switch (nWin)
	{
	case 1: 	SendCommand("F1");
					nWindow=1;
					break;
	case 2: 	SendCommand("F2");
					nWindow=2;
					break;
	case 3: 	SendCommand("F3");
					nWindow=3;
					break;
	}
}
