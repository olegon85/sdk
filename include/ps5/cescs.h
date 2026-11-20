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

#ifndef _PS5_CESCS_H_
#define _PS5_CESCS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * libSceCesCs single-byte character encoding support.
 *
 * The module is loaded with sceSysmoduleLoadModule(0x010c) or resolves as a
 * dependency; there is no import library, so resolve the exports through
 * dlopen("libSceCesCs.sprx", RTLD_LAZY) and dlsym. Function pointer types:
 */
typedef const uint8_t* (*SceCesRefersProfileFn)(void);
typedef int (*SceCesSbcToUcs2Fn)(const uint8_t* profile, uint8_t sbc, uint16_t* ucs2);
typedef int (*SceCesSbcToUtf16Fn)(const uint8_t* profile, uint8_t sbc, uint16_t* utf16,
                                  uint32_t utf16max, uint32_t* utf16_len);
typedef int (*SceCesSbcToUtf32Fn)(const uint8_t* profile, uint8_t sbc, uint32_t* utf32);
typedef int (*SceCesSbcToUtf8Fn)(const uint8_t* profile, uint8_t sbc, uint8_t* utf8,
                                 uint32_t utf8max, uint32_t* utf8_len);
typedef int (*SceCesUcs2ToSbcFn)(uint16_t ucs2, const uint8_t* profile, uint8_t* sbc);

/* sceCesRefersUcsProfileIso8859_1 and the other 30 profiles follow the
 * sceCesRefersProfileFn shape; the full export list is in AnyPS5's
 * libSceCesCs/include/Api.hpp. */

#define SCE_CES_ERROR_NULL_PROFILE 0x805C0004 /* all converters, null profile */
#define SCE_CES_ERROR_SRC_BUFFER_END 0x805C0011 /* sceCesUtf16ToSbc lone high surrogate */
#define SCE_CES_ERROR_ILLEGAL_CODE 0x805C0015   /* surrogates into *ToSbc on ISO/Cp125x profiles */
#define SCE_CES_ERROR_INVALID_ENCODE 0x805C0014 /* byte not in profile (ASCII, BS4730, GbT1988, JIS, KSX) */
#define SCE_CES_ERROR_UNASSIGNED_CODE 0x805C0020 /* mapped to nothing (Cp1252 gaps, U+1F600, ...) */

/*
 * Payload-process measurements on firmware 13.60 (PS5 Pro, elfldr):
 * - the ISO 8859-1 table is identity for all 256 bytes through SbcToUcs2,
 *   SbcToUtf16 and SbcToUtf32; 46 further single-byte tables match their
 *   public sources byte for byte (see samples/hello_cescs);
 * - on an error the wide encoders set the output to 0 and return the code
 *   above; sceCesSbcToUtf8 leaves its output untouched and writes length 0;
 * - sceCesSbcToUtf16 with utf16max 0 returns 0x805C0031 and sets length 1.
 */

#ifdef __cplusplus
}
#endif

#endif /* _PS5_CESCS_H_ */
