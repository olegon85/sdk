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
#include <ps5/kernel_extra.h>
#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>

typedef uint64_t (*ZeroArgFn)(void);

static uint64_t call(void* lib, const char* sym) {
  ZeroArgFn f = (ZeroArgFn)dlsym(lib, sym);
  return f ? f() : ~0ULL;
}

int main(void) {
  void* lib = dlopen("libkernel_sys.sprx", RTLD_LAZY);
  if (lib == NULL) {
    printf("dlopen failed: %s\n", dlerror());
    return 1;
  }
  printf("hello_kernel_sys fw=%08x\n", (unsigned)kernel_get_fw_version());
  printf("zeros=%llu,%llu,%llu,%llu,%llu err=0x%08llx const=0x%llx\n",
         (unsigned long long)call(lib, "pCbBddNl66Y"),
         (unsigned long long)call(lib, "zAloppPJxy0"),
         (unsigned long long)call(lib, "vq2+HyP5U5I"),
         (unsigned long long)call(lib, "z+e0vV-tnH4"),
         (unsigned long long)call(lib, "gJ5gpW45mU0"),
         (unsigned long long)call(lib, "N94a0T4F4+U"),
         (unsigned long long)call(lib, "C49jelxiaVE"));
  printf("byte=%llu set=%llu byte_after=%llu\n",
         (unsigned long long)call(lib, "FR0odl35yQY"),
         (unsigned long long)call(lib, "1WXQlxMpnNA"),
         (unsigned long long)call(lib, "FR0odl35yQY"));
  return 0;
}
