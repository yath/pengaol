/***************************************************************************
                          cvjcompress.cpp  -  description
                             -------------------
    begin                : Wed Jun 6 2001
    copyright            : (C) 2001 by stephane (birdy57)
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

#include "cvjcompress.h"

CVjcompress::CVjcompress()
{
vjstruct=(struct slcompress *) new char[10500];
// testvjcompress
sl_compress_init (vjstruct);
m_cTcpIpHeader=new CTcpIpHeader;
}

CVjcompress::~CVjcompress()
{
}


/** Decompresse une trame */
unsigned short CVjcompress::Deflate(char *sBuffer,int nLen)
{
unsigned int  nType;
char ssTmpBuffer[2000];
char *sTmpBuffer=&ssTmpBuffer[0];
char *pResult;
unsigned short nRet;

nRet=nLen;

// trame non compresse
if (sBuffer[0]!=0x45)
	{

	if (sBuffer[0]!=0x75)
		nType=TYPE_COMPRESSED_TCP;
		else
		{
		nType=TYPE_UNCOMPRESSED_TCP;
		sBuffer[0]=0x45;
		}

	memcpy((sTmpBuffer+200),sBuffer,nLen);

	// demande de decompression
	pResult=(char *) sl_uncompress_tcp((u_char *) (sTmpBuffer+200),nLen,nType,vjstruct);
	
	// trame non compresser ou erreur
	if (pResult!=NULL)
		{
		nRet+=((sTmpBuffer+200)-pResult);
		memcpy(sBuffer,pResult,nRet);
		}
	}
return nRet;
}
/** Compress une trame TCP/IP */
int CVjcompress::Compress(char *sBuffer,int nLon)
{
int nType;
int nRet=nLon;
mbuf mBuf;
return nRet;
nLon=TcpNormal(sBuffer,nLon);
nRet=nLon;
return nRet;
// la compression
mBuf.m_off=(unsigned char *) sBuffer;
mBuf.m_len=nLon;

nType=sl_compress_tcp(&mBuf,vjstruct,1);

 if ((nType!=TYPE_IP) && (nType!=TYPE_ERROR)) {
      nRet=mBuf.m_len;
      // Type non compresser ? 0x75 sinon +0x80
      (nType==TYPE_UNCOMPRESSED_TCP) ? (sBuffer[0]=0x75) : /* ici c du compresser*/ (sBuffer[0]+=0x80);
   }


return nRet;	
}

/** Reduit l'entete à 40 octets */
int CVjcompress::TcpNormal(char *sBuffer,int nLen)
{
int nRet=nLen;
int nSize=0;
int nDif=0;

m_cTcpIpHeader->SetHeader(sBuffer);
printf("trame type : %x iplen : %d tcplen : %d \n",m_cTcpIpHeader->GetIpProtocol(),m_cTcpIpHeader->GetIpLen(),m_cTcpIpHeader->GetTcpLen());
if ((m_cTcpIpHeader->GetIpProtocol()==0x06) && (m_cTcpIpHeader->GetIpLen()>40))
	{
	// Verifie la taille du header TCP
	if (m_cTcpIpHeader->GetTcpLen()>20)
		{
		// Regenere la trame
		nSize=(m_cTcpIpHeader->GetTcpLen()+20);
		nDif=nSize-40;
		memcpy(sBuffer+40,sBuffer+nSize,(nLen-nDif));	
		m_cTcpIpHeader->SetIpLen(m_cTcpIpHeader->GetIpLen()-nDif);
		m_cTcpIpHeader->SetTcpLen(20);
		m_cTcpIpHeader->CalculateIpChkSum();
		m_cTcpIpHeader->CalculateTcpChkSum();
		nRet-=nDif;
		}
	}

return nRet;
}
