
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef WIN32
	#define System 2
#else
	#define System 1
#endif

#define LKernel 1

#include <iostream.h>
#include <stdlib.h>
#include <unistd.h>
#include "nulldriver.h"
#include "modemdriver.h"
#include "CableDriver.h"
#include "ctuntapdriver.h"
#include "cloader.h"
#include "cmsgerror.h"
#include "ctextlinux.h"
#include "ccommand.h"
#include "ckernel.h"
#include "kernel30.h"
#include "crouteur.h"
#include "crouteurlinux.h"
#include "cusermanager.h"
#include "cppp.h"
#include <signal.h>
#include <arpa/inet.h>

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

int main(int argc, char *argv[])
{
int i;
char *Login;
char *PassWord;

Loader=new CLoader;

UserManager=new CUserManager;

if (!bReady)
{
	if (System==1)
		{
		Msg=new CTextLinux;
		MsgI=new CTextLinux;
		MsgO=new CTextLinux;
		Routeur=new CRouteurLinux;
		(void) signal(SIGINT,Exit);
		}
		else
		{
		Msg=new CMsgError;
		MsgI=new CMsgError;
		MsgO=new CMsgError;
		Routeur=new CRouteur;
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

// Traitement des options

if (argc==1) // Pas d'options alors liste
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
	if (!(UserManager->AddUser(Command->GetOpt(),Command->GetOpt2())))
	   	Msg->Printf("%M%t\n",UserManager->GetErrorNbr());
       else
		Msg->Printf("%M%t\n",143);
   	
// Efface un utilisateur

if (Command->Check((char *) "-ListUser"))
	if (!(UserManager->ListUser()))
	   	Msg->Printf("%M%t\n",UserManager->GetErrorNbr());

// Liste les utilisateur

if (Command->CheckOpt((char *) "-DelUser"))
	if (!(UserManager->DeleteUser(Command->GetOpt())))
	   	Msg->Printf("%M%t\n",UserManager->GetErrorNbr());
       else
		Msg->Printf("%M%t\n",144);

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
exit(0);

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
