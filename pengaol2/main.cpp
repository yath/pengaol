#include "globals.h"

/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : dim avr 29 18:14:09 CEST 2001
    copyright            : (C) 2001 by Birdy57 (Stephane)
    email                : birdy57@multimania.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef WIN32
#include "..\config.h"
#else
#include "config.h"
#endif


#ifdef WIN32
	#define System 2
#else
	#define System 1
#endif

#define LKernel 1

#include <iostream.h>
#include <stdlib.h>
#include "nulldriver.h"
#include "cloader.h"
#include "cmsgerror.h"
#include "ctextlinux.h"
#include "ccommand.h"
#include "ckernel.h"
#include "kernel30.h"
#include "crouteurWin.h"
#include "crouteurlinux.h"
#include "cusermanager.h"
#include "threadELV3.h"

#ifndef WIN32
#include "cgui.h"
#endif

#ifdef WITH_MODEM
#include "modemdriver.h"
#endif
#ifdef WITH_CABLE
#include "CableDriver.h"
#endif
#ifdef WITH_TUNTAP
#include "ctuntapdriver.h"
#endif
#ifdef WITH_PPP
#include "cppp.h"
#endif
#ifdef WITH_WINTAP
#include "cwintap.h"
#endif
#include <signal.h>

//#include "cnewparamconfig.h"

// Toutes les classes sont globales

CNullDriver *Input= new CNullDriver;
CNullDriver *Output= new CNullDriver;
CLoader *Loader;
CMsgError *Msg;
CMsgError *MsgI;
CMsgError *MsgO;
CMsgError::CStatus *Status=new CMsgError::CStatus;
CCommand *Command=new CCommand;
CKernel *Kernel;
CRouteur *Routeur;
CUserManager *UserManager;
char *sIp=new char[200];
char *sDns=new char[200];
char *sDomain=new char[200];
bool bDriver=false;
bool bKernel=false;
bool bStop=false;
bool bAolMerde=false;
bool bReady=false;
unsigned long i;
char Language[3];
char sTmp[200];
struct in_addr adresse;
char *Lan;
bool bGui=false;
bool bStart=false;
char *Login;
char *PassWord;
CGui *Gui= new CGui;

void Exit(int sig)
{
if (sig==0xff)
	bAolMerde=true;

if (!bStop)
	{
	// connection etablie ?
	if (bKernel)
		Kernel->Stop();
		else
	if (bDriver)
		{
		Input->Disconnect();
		Output->Disconnect();
		Msg->Printf("%M%t\n",78);
	   exit(EXIT_SUCCESS);
		}
	bStop=true;
	}
}

#ifndef WIN32
void SendInfo(char *stexte)
{
char sTmp[200];
if (strlen(stexte)<190)
	{
	sprintf((char *) &sTmp,"I%s",stexte);
	Gui->SendCommand((char *) &sTmp);
	}
}


void DonneUser()
{
Gui->SendCommand("C");
UserManager->ListUserGui();
 }

void AddGuiUser(char *stexte)
{
char sTmp[200];
if (strlen(stexte)<190)
	{
	sprintf((char *) &sTmp,"L%s",stexte);
	Gui->SendCommand((char *) &sTmp);
	}
}


void BuddyClear()
{
Gui->SendCommand("D");
}

void BuddyAdd(char *stexte)
{
char sTmp[200];
if (strlen(stexte)<190)
	{
	sprintf((char *) &sTmp,"E%s",stexte);
	Gui->SendCommand((char *) &sTmp);
	}
}

void GuiMess(char *stexte)
{
char sTmp[200];
if (strlen(stexte)<190)
	{
	sprintf((char *) &sTmp,"M%s",stexte);
	Gui->SendCommand((char *) &sTmp);
	}
}

void Start(char *sLogin)
{
Login=sLogin;
if ((PassWord=UserManager->GetPass(Login))==NULL)
	Msg->Printf("%M%t\n",146);
	else
	bStart=true;
}

void KillUser(char *Login,char *PassWord)
{
UserManager->DeleteUser(Login);
}

void SaveUser(char *Login,char *PassWord)
{
UserManager->AddUser(Login,PassWord);
}

#endif

int main(int argc, char *argv[])
{
int i;
Loader=new CLoader;

UserManager=new CUserManager;

if (!bReady)
{
		Msg=new CTextLinux;
		MsgI=new CTextLinux;
		MsgO=new CTextLinux;
	if (System==1)
		{
#ifndef WIN32
		/*Msg=new CTextLinux;
		MsgI=new CTextLinux;
		MsgO=new CTextLinux;*/
		Routeur=new CRouteurLinux;
		(void) signal(SIGINT,Exit);
		(void) signal(SIGKILL,Exit);
#endif
		}
		else
		{
		/*Msg=new CMsgError;
		MsgI=new CMsgError;
		MsgO=new CMsgError;*/
		Routeur=new CRouteurWin;
	    }

	Lan=getenv("LANG");
	if (Lan!=NULL)
		memcpy(&Language,getenv("LANG"),2);
		else
		{
		memcpy(&Language,"en",2);
		printf("Can not found your language : assuming English\n");
		printf("Make sure that you have set the variable LANG with your language\n");
		printf("ex: export LANG=fr for french\n");
		}

	Language[2]=0;
	printf("Load language : %s \n",Language);
   	if (!Msg->Load("/etc/PengMessages.txt",(char *) &Language))
		{
		printf("Error loading Msg for your lang assuming English\n");
	   	if (!Msg->Load("/etc/PengMessages.txt","en"))
			{
			printf("Error loading Msg FATAL !!\n");
   		 	exit(0);
			}
        }
}

bReady=true;

#ifdef WITH_PPP
Loader->AddDriver(new CPpp);
#endif
#ifdef WITH_TUNTAP
Loader->AddDriver(new CTunTapDriver);
#endif
#ifdef WITH_CABLE
Loader->AddDriver(new CCableDriver);
#endif
#ifdef WITH_MODEM
Loader->AddDriver(new CModemDriver);
#endif
#ifdef WITH_WINTAP
Loader->AddDriver(new CWinTap);
#endif

// Traitement des options

if (Command->Check((char *) "-help"))
	{
	Msg->SetMessage(true);
	for (i=100;i<121;i++)
		Msg->Printf("%M%t\n",i);
    exit(0);
	}

// sinon on les traites

Command->Set(argc,argv);

// Reconnection
if (Command->Check((char *) "-Reconnect"))
	sleep(10);

// Active ou desactive les messages
if (Command->Check((char *) "-Daemon"))
	Msg->SetMessage(false);
	else
	Msg->SetMessage(true);


#ifdef WITH_DEBUG
// Active ou desactive les logs
if (Command->Check((char *) "-Log"))
	Msg->SetLog(true);
	else
	Msg->SetLog(false);

// Active ou desactive les debug
if (Command->Check((char *) "-Debug"))
	Msg->SetDebug(true);
	else
	Msg->SetDebug(false);
#else
if (Command->Check((char *) "-Log"))
	{
	Msg->Printf("%M%t\n",18);
    exit(0);
	}

if (Command->Check((char *) "-Debug"))
	{
	Msg->Printf("%M%t\n",18);
    exit(0);
	}
#endif

// Ici la fonction Msg est configurer alors on clone
// Cela permet d'economiser des ressources

Msg->GetFastLoad(Status);
MsgI->SetFastLoad(Status);
MsgO->SetFastLoad(Status);
UserManager->GetMsg(Msg);

// et on les transmet au loader

Loader->SetMessage(MsgI,MsgO);

// definie le fichier config
if (Command->CheckOpt((char *) "-LoadInit"))
	Loader->SetConfigFile(Command->GetOpt());

// Liste les drivers
if (Command->Check((char *) "-ListDriver"))
	{
	Loader->ListDriver();
	Loader->MakeConfig(false);
   exit(0);
	}

// genere le fichier d'init
if (Command->Check((char *) "-MakeInit"))
	{
	if (!Loader->MakeConfig(true))
		Msg->Printf("%M%t\n",Loader->GetErrorNbr());
   		else
		Msg->Printf("%M%t\n",50);
	exit(0);
	}

// Ajout d'un utilisateur

if (Command->CheckOpt2((char *) "-AddUser"))
	{
	if (!(UserManager->AddUser(Command->GetOpt(),Command->GetOpt2())))
	   	Msg->Printf("%M%t\n",UserManager->GetErrorNbr());
       else
		Msg->Printf("%M%t\n",143);
   	 exit(0);
	}

// Efface un utilisateur

if (Command->Check((char *) "-ListUser"))
	{
	if (!(UserManager->ListUser()))
	   	Msg->Printf("%M%t\n",UserManager->GetErrorNbr());
    exit(0);
	}

// Liste les utilisateur

if (Command->CheckOpt((char *) "-DelUser"))
	{
	if (!(UserManager->DeleteUser(Command->GetOpt())))
	   	Msg->Printf("%M%t\n",UserManager->GetErrorNbr());
       else
		Msg->Printf("%M%t\n",144);
    exit(0);
	}

// restore les param par defauts
if (Command->Check((char *) "-Restore"))
	{
	//charge les drivers
	if (!Loader->LoadDriver())
		{
		Msg->Printf("%M%t\n",51);
		exit(0);	
		}

	// Memorise les drivers recut

	Input=Loader->GetDriverIn();
	Output=Loader->GetDriverOut();

	Routeur->SetDriver(Input,Output);
	Routeur->SetInfo(sIp,sDns,sDomain);
	Routeur->SetMsg(Msg);

	Routeur->Stop();
	exit(0);
	}

// Si on demande pas une connection alors arrete
if (Command->CheckOpt2((char *) "-User"))
	{
	Login=Command->GetOpt();
	PassWord=Command->GetOpt2();
	}
	else
if (Command->CheckOpt((char *) "-Connect"))
	{
	Login=Command->GetOpt();
	if ((PassWord=UserManager->GetPass(Login))==NULL)
		{
		Msg->Printf("%M%t\n",146);
		exit(0);
		}
	}
	else
	#ifndef WIN32
	{
	bGui=true;
	StartGui(Gui);
	}
	#else
	exit(0);
	#endif

#ifndef WIN32
if (bGui)
	{
	while (!bStart)
	sleep(1);
	}
#endif

//charge les drivers


if (!Loader->LoadDriver())
	{
	Msg->Printf("%M%t\n",51);
	exit(0);	
	}

// Memorise les drivers recut

Input=Loader->GetDriverIn();
Output=Loader->GetDriverOut();

bDriver=true;

// Lance la connection
#ifndef WIN32
// Demmande la 2 eme fenetre
Gui->SetWindow(2);
#endif

Msg->Printf("%M%S%t\n",52);


if (!Input->Connect())
	{
	Msg->Printf("%M%S%t\n",5);
    exit(0);
	}


if (!Output->Connect())
	{
	Msg->Printf("%M%S%t\n",21);
	Input->Disconnect();
    exit(0);
	}


// Donne la vitesse de connection si possible
if (Input->GetSpeed())
	Msg->Printf("%M%S%t:%d\n",53,Input->GetSpeed());


// Charge et initialise le noyau

if (LKernel==1)	
	Kernel=new Kernel30;
	else
	Kernel=new CKernel;	

//Kernel->SetSpeed(Input->GetSpeed());
Kernel->SetSpeed(56600);
Kernel->SetDriver(Input,Output);
Kernel->SetMessage(MsgI,MsgO);


// Negociation de la connection  et connection au drivers
Input->Flush();


if (!Kernel->Connect(Login,PassWord))
	{
   	Msg->Printf("%M%S%t\n",Kernel->GetErrorNbr());
	Input->Disconnect();
	Output->Disconnect();	
	}
    else
	{
	Kernel->GetIp(sIp);
	Kernel->GetDns(sDns);
	Kernel->GetDomainName(sDomain);

	Msg->Printf("%M%t%s\n",74,sIp);
	Msg->Printf("%M%t%s\n",75,sDns);
	Msg->Printf("%M%t%s\n",76,sDomain);
	#ifndef WIN32
	sprintf((char *) &sTmp,"Ip:\n%s\n",sIp);
	SendInfo((char *) &sTmp);
	sprintf((char *) &sTmp,"Dns:\n%s\n",sDns);
	SendInfo((char *) &sTmp);
	sprintf((char *) &sTmp,"Domain:\n%s\n",sDomain);
	SendInfo((char *) &sTmp);
	#endif
	// Lance le routage
	if (Loader->GetDns()!=NULL)
		sDns=Loader->GetDns();

	Routeur->SetDriver(Input,Output);
	Routeur->SetInfo(sIp,sDns,sDomain);
	Routeur->SetMsg(Msg);
	if (!Routeur->Start())
		{
		Input->Disconnect();
		Output->Disconnect();	
		Msg->Printf("%M%S%t\n",Routeur->GetErrorNbr());
		}
		else
	    {
		// Demmarer le kernel
	   Msg->Printf("%M%S%t%s\n",57,Kernel->GetName());
		bKernel=true;
		system("wavplay -q /usr/share/sound/welcome.wav &");
       // Demmarrage du prog auto
		if (strstr(Loader->GetStart(),"None")==NULL)
			{
		    sprintf((char *) &sTmp,"%s &\n",Loader->GetStart());
			 system((char *) &sTmp);
			}

		#ifndef WIN32
		// Demmande la 2 eme fenetre
		Gui->SetWindow(3);
		#endif

		Kernel->Start();
		if (bStop)
		   Msg->Printf("%M%S%t\n",79);
			else
		   Msg->Printf("%M%S%t\n",80);
		Kernel->Stop();
		system("wavplay -q /usr/share/sound/goodbye.wav &");
		Input->Disconnect();
		Output->Disconnect();	
		if (!Routeur->Stop())
			Msg->Printf("%M%S%t\n",73);
		}
  }
Loader->MakeConfig(false);
Loader->KillDriver();

if (bGui)
	StopGui();

delete Loader;
delete Routeur;
delete Msg;
delete MsgI;
delete MsgO;
delete UserManager;
delete Kernel;

if (bAolMerde)
	exit(-1);
	
exit(0);

}
