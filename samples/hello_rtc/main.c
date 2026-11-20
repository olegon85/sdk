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
#include <ps5/rtc.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  SceRtcGetDayOfWeekFn day_of_week;
  SceRtcFormatRFC3339Fn format_3339;
  char text[40];
  SceRtcTick tick = 0;

  printf("hello_rtc fw=%08x\n", (unsigned)kernel_get_fw_version());
  void* lib = dlopen("libSceRtc.sprx", RTLD_LAZY);
  if (lib == NULL) {
    printf("dlopen failed: %s\n", dlerror());
    return 1;
  }
  day_of_week = (SceRtcGetDayOfWeekFn)dlsym(lib, "sceRtcGetDayOfWeek");
  format_3339 = (SceRtcFormatRFC3339Fn)dlsym(lib, "sceRtcFormatRFC3339");
  if (day_of_week == NULL || format_3339 == NULL) {
    printf("dlsym failed\n");
    return 1;
  }
  printf("2000-02-29 dow=%d (expect 2)\n", day_of_week(2000, 2, 29));
  printf("bad month dow=0x%08x (expect 80B50009)\n", (unsigned)day_of_week(2024, 13, 1));
  memset(text, 0xCC, sizeof(text));
  printf("tick0 fmt=%d text=%s\n", format_3339(text, &tick, 0), text);
  return 0;
}
