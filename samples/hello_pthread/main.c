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

#include <ps5/kernel.h>
#include <ps5/pthread.h>
#include <stdio.h>

int main(void) {
  ScePthreadCond cond;
  ScePthreadMutex mutex;
  ScePthreadRwlock rwlock;
  int rc;

  printf("hello_pthread fw=%08x\n", (unsigned)kernel_get_fw_version());
  rc = scePthreadCondInit(&cond, NULL, "cond");
  printf("cond init=%d\n", rc);
  if (rc == 0) {
    printf("cond setname_named=%d\n", pthread_cond_setname_np(&cond, "renamed"));
    printf("cond destroy=%d\n", scePthreadCondDestroy(&cond));
  }
  rc = scePthreadCondInit(&cond, NULL, NULL);
  printf("cond anon init=%d\n", rc);
  if (rc == 0) {
    printf("cond setname_anon=%d\n", pthread_cond_setname_np(&cond, "anon_named"));
    printf("cond destroy=%d\n", scePthreadCondDestroy(&cond));
  }
  rc = scePthreadMutexInit(&mutex, NULL, "mutex");
  printf("mutex init=%d destroy=%d\n", rc, rc == 0 ? scePthreadMutexDestroy(&mutex) : -1);
  rc = scePthreadRwlockInit(&rwlock, NULL, "rwlock");
  printf("rwlock init=%d destroy=%d\n", rc, rc == 0 ? scePthreadRwlockDestroy(&rwlock) : -1);
  return 0;
}
