#include "config.h"

#ifdef WITH_CABLE

#ifndef __CABLE_DRIVER__
#define __CABLE_DRIVER__

	#include "nulldriver.h"
	#include "cparamconfig.h"

	#ifdef WIN32
		#include <winsock.h>
	#else
		#include <unistd.h>
		#include <stdlib.h>
		#include <stdio.h>
		#include <termios.h>
		#include <sys/fcntl.h>
		#include <sys/ioctl.h>
		#include <sys/types.h>
		#include <sys/socket.h>
		#include <sys/un.h>
		#include <sys/time.h>
		#include <netinet/in.h>
		#include <netdb.h>
		#include <arpa/inet.h>
		#define SOCKET int
		#define closesocket close
	#endif


	class CCableDriver : public CNullDriver
	{
	public:
		
		CCableDriver();
		virtual ~CCableDriver();

		virtual bool Connect();
		virtual bool Disconnect();

		virtual int Read(char *pData,int nSize);
		virtual int Write(char *pData,int nSize);

    CParamConfig* GetRequest();
  /** teste si la connection ip est ok */
  bool IsAvailable();
  /** connecte ? */
  bool IsConnected();

		SOCKET  m_nSocket;
  /** peripherique */
  char* m_sDev;
  /** Ip De Connection */
  char *m_sConnectIp;

	private:

	  /** Parametre */
  	CParamConfig *m_cParam;
  	/** hote de destination */
  	char *m_sHost;
  	/** Port de connection */
  	char *m_sPort;
		/** FullDuplex ? */
	  bool m_bFullDuplex;
		/** Mutex */
	  bool m_MutexR;
	  bool m_MutexW;
protected: // Protected attributes
  /** connecte ? */
  bool m_bIsConnected;
  /** fullduplex ? */
  char *m_sFull;
	};

  #endif

#endif

