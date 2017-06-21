/***************************************************************************
                          ccommand.cpp  -  description
                             -------------------
    begin                : Fri May 25 2001
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

#include "ccommand.h"

CCommand::CCommand()
{
m_sBuffer=new char[200];
m_sBuffer2=new char[200];
}
CCommand::~CCommand()
{
delete(m_sBuffer);
delete(m_sBuffer2);
}
/** initialise la class */
void CCommand::Set(int argc,char **argv)
{
m_nArgc=argc;
m_pArgv=argv;
}
/** regarde si le parametre est donne */
bool CCommand::Check(char *Param)
{
int Pos;
bool bRet=false;
for (Pos=0;Pos<m_nArgc;Pos++)
	if (strcmp(Param,m_pArgv[Pos])==0)
		{
		bRet=true;
		break;
		}

return bRet;
}
/** teste avec un parametre */
bool CCommand::CheckOpt(char *Param)
{
int Pos;
bool bRet=false;
for (Pos=0;Pos<m_nArgc;Pos++)
	if (strcmp(Param,m_pArgv[Pos])==0)
		{
		Pos++;
		if (Pos<m_nArgc)
			{
			strcpy(m_sBuffer,m_pArgv[Pos]);
			bRet=true;
			break;
			}
		break;
		}

return bRet;

}
/** retourne l'option */
char* CCommand::GetOpt()
{
return m_sBuffer;
}
/** fournit le 2 eme argument */
char* CCommand::GetOpt2()
{
return m_sBuffer2;
}
/** avec 2 options */
bool CCommand::CheckOpt2(char *Param)
{
int Pos;
bool bRet=false;
for (Pos=0;Pos<m_nArgc;Pos++)
	if (strcmp(Param,m_pArgv[Pos])==0)
		{
		Pos++;
		if (Pos<m_nArgc)
			{
			strcpy(m_sBuffer,m_pArgv[Pos]);
			Pos++;
			if (Pos<m_nArgc)
				{
				strcpy(m_sBuffer2,m_pArgv[Pos]);
				bRet=true;
				break;
				}
			}
		break;
		}

return bRet;}
