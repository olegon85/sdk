/*   Copyright (C) 2026 Oleg
    This file is part of ps5-payload-sdk.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _PS5_PTHREAD_H_
#define _PS5_PTHREAD_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Opaque libkernel synchronization objects.
 *
 * A payload process on firmware 13.60 showed that the initializers below
 * write a single-word handle into the caller slot and keep the object in
 * libkernel memory; 128 bytes are reserved so a future firmware that
 * stores objects inline still fits. The attribute types are passed as
 * NULL by every case measured so far, so their layout is intentionally
 * incomplete here.
 */
typedef struct {
  uint64_t opaque[16];
} ScePthreadCond;

typedef struct {
  uint64_t opaque[16];
} ScePthreadMutex;

typedef struct {
  uint64_t opaque[16];
} ScePthreadRwlock;

typedef struct {
  uint64_t opaque[16];
} ScePthreadCondattr;

typedef struct {
  uint64_t opaque[16];
} ScePthreadMutexattr;

typedef struct {
  uint64_t opaque[16];
} ScePthreadRwlockattr;

int scePthreadCondInit(ScePthreadCond* cond, const ScePthreadCondattr* attr,
                       const char* name);
int scePthreadCondDestroy(ScePthreadCond* cond);

int scePthreadMutexInit(ScePthreadMutex* mutex, const ScePthreadMutexattr* attr,
                        const char* name);
int scePthreadMutexDestroy(ScePthreadMutex* mutex);

int scePthreadRwlockInit(ScePthreadRwlock* rwlock,
                         const ScePthreadRwlockattr* attr, const char* name);
int scePthreadRwlockDestroy(ScePthreadRwlock* rwlock);

/*
 * Names the condition variable. On firmware 13.60 (payload process) this
 * returns 0 for a cond created with a NULL name and EAGAIN (35) for one
 * that was named at creation, so it names once and cannot rename.
 * A NULL cond faults; no null check is performed.
 */
int pthread_cond_setname_np(ScePthreadCond* cond, const char* name);

#ifdef __cplusplus
}
#endif

#endif /* _PS5_PTHREAD_H_ */
