/*
 * This library offers an API to use Latch in some systems, like UNIX.
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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// libcurl
#include <curl/curl.h>
#include <curl/easy.h>

// openssl
#include <openssl/hmac.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#define AUTHORIZATION_HEADER_NAME "Authorization"
#define DATE_HEADER_NAME "X-11Paths-Date"
#define AUTHORIZATION_METHOD "11PATHS"
#define AUTHORIZATION_HEADER_FIELD_SEPARATOR " "
#define UTC_STRING_FORMAT "%Y-%m-%d %H:%M:%S"
#define LATCH_BUFFER_SIZE (256 * 1024) /* 256kB */

#define API_CHECK_STATUS_URL "/api/0.6/status"
#define API_PAIR_URL "/api/0.6/pair"
#define API_PAIR_WITH_ID_URL "/api/0.6/pairWithId"
#define API_UNPAIR_URL "/api/0.6/unpair"

void init(const char*, const char*);
void setHost(const char*);
void setProxy(const char*);
void setTimeout(const int);
void setNoSignal(const int);
void setTLSCAFile(const char*);
void setTLSCAPath(const char*);
void setTLSCRLFile(const char*);
char* pairWithId(const char*);
char* pair(const char*);
char* status(const char*);
char* operationStatus(const char*, const char*);
char* unpair(const char*);
