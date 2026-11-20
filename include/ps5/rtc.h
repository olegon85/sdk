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

#ifndef _PS5_RTC_H_
#define _PS5_RTC_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * libSceRtc real-time clock. There is no import library; resolve through
 * dlopen("libSceRtc.sprx", RTLD_LAZY) and dlsym:
 */
typedef uint64_t SceRtcTick;
typedef int (*SceRtcGetDayOfWeekFn)(int year, int month, int day);
typedef int (*SceRtcFormatRFC3339Fn)(char* dateTime, const SceRtcTick* utc, int timeZoneMinutes);
typedef int (*SceRtcFormatRFC2822Fn)(char* dateTime, const SceRtcTick* utc, int timeZoneMinutes);

/*
 * Payload-process measurements on firmware 13.60 (PS5 Pro, elfldr):
 * - a tick is microseconds since 0001-01-01; tick 0 formats as 0001-01-01;
 * - time zone offsets are accepted in [-1439, 1439] minutes, outside returns
 *   0x80B50003 (INVALID_VALUE);
 * - GetDayOfWeek: month is validated before the year; an invalid month
 *   returns 0x80B50009, an invalid day 0x80B5000A, a year past 9999 or below
 *   1 0x80B50008 (INVALID_YEAR); a valid date returns 0..6 with 0 = Sunday;
 * - the SceRtcDateTime year field is 16-bit: ticks in year 65536 return
 *   0x80B50008, year 65537 wraps and formats as year 0001 with the RFC 2822
 *   weekday of the wrapped date;
 * - a negative tick: FormatRFC2822 returns 0x80B50003, FormatRFC3339
 *   returns 0x80B50008.
 */

#ifdef __cplusplus
}
#endif

#endif /* _PS5_RTC_H_ */
