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

#include <ps5/cescs.h>
#include <ps5/kernel.h>
#include <dlfcn.h>
#include <stdio.h>

int main(void) {
  SceCesRefersProfileFn refers;
  SceCesSbcToUcs2Fn to_ucs2;
  const uint8_t* profile;
  int bad = 0;

  printf("hello_cescs fw=%08x\n", (unsigned)kernel_get_fw_version());
  void* lib = dlopen("libSceCesCs.sprx", RTLD_LAZY);
  if (lib == NULL) {
    printf("dlopen failed: %s\n", dlerror());
    return 1;
  }
  refers = (SceCesRefersProfileFn)dlsym(lib, "sceCesRefersUcsProfileIso8859_1");
  to_ucs2 = (SceCesSbcToUcs2Fn)dlsym(lib, "sceCesSbcToUcs2");
  if (refers == NULL || to_ucs2 == NULL) {
    printf("dlsym failed\n");
    return 1;
  }
  profile = refers();
  for (unsigned byte = 0; byte < 256; ++byte) {
    uint16_t ucs2 = 0xA5A5;
    int rc = to_ucs2(profile, (uint8_t)byte, &ucs2);
    if (rc != 0 || ucs2 != byte) {
      bad++;
    }
  }
  printf("iso8859_1 identity mismatches=%d\n", bad);
  return bad != 0;
}
