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

#ifndef __LATCH_H__
#define __LATCH_H__

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
char* lock(const char*);
char* operationLock(const char*, const char*);
char* unlock(const char*);
char* operationUnlock(const char*, const char*);
char* history(const char*);
char* timePeriodHistory(const char*, time_t, time_t);
char* operationCreate(const char*, const char*, const char*, const char*);
char* operationUpdate(const char*, const char*, const char*, const char*);
char* operationRemove(const char*);
char* operationGet(const char*);
char* operationsGet();

#endif /* __LATCH_H__ */
