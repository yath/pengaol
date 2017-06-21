/***************************************************************************
                          main.h  -  description
                             -------------------
    begin                : Wed Aug 8 2001
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

#include "config.h"
#ifndef MAIN_H
#define MAIN_H


#ifdef WITH_GUI
#ifdef __cplusplus
	extern "C" {
#endif
void connection();
void DelUser(char *login);
char* GetPass(char *login);
char **LoginList();
void QuitPeng();
void disconnect();
void AddUser(char *login, char *password);
#ifdef __cplusplus
	}
#endif

void *StartKernel(void *);
void EcritConnect(char *texte);
#endif
void Exit(int sig);
#endif
