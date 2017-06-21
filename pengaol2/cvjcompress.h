/***************************************************************************
                          cvjcompress.h  -  description
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

#ifndef CVJCOMPRESS_H
#define CVJCOMPRESS_H

#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if __GLIBC__ >= 2
#include <asm/types.h>
#else		/* glibc 2 conflicts with linux/types.h */
#include <linux/types.h>
#endif
#include <linux/ppp_defs.h>
#include "vjcompress.h"
#include "ctcpipheader.h"

/**Compression et decompression VJ
  *@author stephane (birdy57)
  */

class CVjcompress {
public: 
	CVjcompress();
	~CVjcompress();
  /** Decompresse une trame */
  unsigned short Deflate(char *sBuffer,int nLen);
  /** Compress une trame TCP/IP */
  int Compress(char *sBuffer,int nLon);
  /** Reduit l'entete à 40 octets */
  int TcpNormal(char *sBuffer,int nLen);
protected: // Protected attributes
  /** structure vjcompress */
  struct slcompress *vjstruct;
  /** TCP/IP Header */
  CTcpIpHeader *m_cTcpIpHeader;
};

#endif
