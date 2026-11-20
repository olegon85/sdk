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

#ifndef _PS5_KERNEL_EXTRA_H_
#define _PS5_KERNEL_EXTRA_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * libkernel_sys exports whose names are only NIDs. Resolve them with
 * dlsym against a handle from dlopen("libkernel_sys.sprx", RTLD_LAZY) (or
 * RTLD_DEFAULT); the strings are not valid C identifiers, so they cannot be
 * declared here, only listed:
 *
 *   "pCbBddNl66Y" "zAloppPJxy0" "vq2+HyP5U5I" "z+e0vV-tnH4" "gJ5gpW45mU0"
 *     -> (uint64_t (*)(void)) returning 0
 *   "N94a0T4F4+U" -> 0x8002004E, "C49jelxiaVE" -> 0x20018
 *   "FR0odl35yQY" -> a module byte zero-extended: 0 before the first call to
 *                    "1WXQlxMpnNA", 1 after it; "1WXQlxMpnNA" returns 0.
 *
 * Measured in a payload process on firmware 13.60 (PS5 Pro, elfldr).
 */
#define PS5_KS_NID_FIVE_ZEROS \
  "pCbBddNl66Y", "zAloppPJxy0", "vq2+HyP5U5I", "z+e0vV-tnH4", "gJ5gpW45mU0"

/*
 * scePthreadMutexInitForInternalLibc is one-shot: libkernel_sys calls it
 * while the module loads, and a payload calling it again aborts the process
 * with "Can't call MutexInitForInternalLibc twice" (errno 2) on firmware
 * 13.60. Never call it from a payload; no prototype is declared here on
 * purpose.
 *
 * scePthreadCondattrGetclock(NULL, NULL) returns SCE_KERNEL_ERROR_EINVAL
 * (0x80020016) without faulting on firmware 13.60.
 */
#define PS5_KERNEL_ERROR_EINVAL 0x80020016

#ifdef __cplusplus
}
#endif

#endif /* _PS5_KERNEL_EXTRA_H_ */
