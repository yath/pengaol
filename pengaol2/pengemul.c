#include "config.h"

#ifdef WITH_GUI


/*********************
**                  **
**   PengEmul.c&h   **
**  Zeon@Paris.com  **
**                  **
*********************/

#include <stdio.h>
#include <malloc.h>
#include "pengemul.h"
#include "window1.h"
#include "window2.h"

char *logins[] = {"Batman",
		  "HackZeon",
		  "Tarzan",
		  "Superman",
		  "SuperZeon",
		  NULL };

char *contacts[] = {"Batman",
		    "HackZeon",
		    "Tarzan",
		    "Superman",
		    "SuperZeon",
		    NULL };

char *drivers[] = {"CModemDriver",
		    "CPppDriver",
		    "CTunTapDriver",
		    "CCableDriver",
		    NULL };

char *driverargnames[] = {"FullDuplex",
			  "Device",
			  "InitStr1",
			  "InitStr2",
			  "PhoneNbr",
			  "ModemSpeed",
			  "DialCommand",
			  "CheckString",
			  "ForceDetect",
			  "AutoDetected",
			  "DialAt9600baud",
			  NULL };

char *driverargvalues[] = {"No",
			   "Auto",
			   "AT&K3",
			   "ATM1L1",
			   "0860880310",
			   "Auto",
			   "ATDT",
			   "AT",
			   "No",
			   "/dev/ttyS0",
			   "No",
			   NULL };

char **DriverArgNames()
{
  return(driverargnames);
}

char **DriverArgValue()
{
  return(driverargvalues);
}

char **DriverList()
{
  return(drivers);
}

char **ContactList()
{
  return(logins);
}


/*void connection()
{
   Il faut que cette fonction rende la main tout de suite.
     Sinon, elle bloque l'interface et on risque le plantage.
     Je pense que le mieux serait que cette fonction place
     un flag a 1 quand elle s'execute et rende la main.
     Une autre fonction en interruption ou fork  pourrait 
     executer les commandes ci-dessous.

  printf("Connection de l'utilisateur %s\n", selected_user());

  Window1_hide();
  connect_window_set_text("Connection au 0860000000");
  connect_window_show();  
  // Numerotation
  connect_window_set_text("Recherche du fournisseur");
  // ...
  connect_window_set_text("Verification du mot de passe");
  // ...
  connect_window_set_text("Connection");
  // ...
  connect_window_hide();
  active_window_show();
} */

int ConnectionTime()
{
  // 3x60x60 + 60 + 7 = 03h 01mn 07s = 10867s
  return(10867);
}

int DownloadRate()
{
  return(5205);
}

int UploadRate()
{
  return(4342);
}

#endif

