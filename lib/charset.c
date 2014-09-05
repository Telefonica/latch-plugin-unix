/*
 * This library handles allowed charset.
 * Copyright (C) 2013 Eleven Paths

 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "charset.h"


#define EXCEPTION_LENGTH 6


static const char exceptions[] = {'S','5','l','1','0','O'};



static int inExceptions(char c){

    unsigned int i;
    for(i = 0 ; i < EXCEPTION_LENGTH ; i++){
        if(c == exceptions[i])
            return 1;
    }
    return 0;
}


int validCode(char *code){

    if(strlen(code) != PAIRING_CODE_LENGTH)
        return 0;

    unsigned int i;
    for(i = 0 ; i < PAIRING_CODE_LENGTH ; i++){
        if(inExceptions(code[i]))
            return 0;
        if(! isalnum(code[i]))
            return 0;
    }   
    return 1;
}
