
/* vim: set noet ts=4 sw=4 ft=c:
 *
 * Copyright (C) 2011, 2012 James McLaughlin et al.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "../common.h"

static void * MSVCRT (const char * fn)
{
	static HINSTANCE DLL = 0;

	if (!DLL)
		DLL = LoadLibraryA ("msvcrt.dll");

	return DLL ? (void *) GetProcAddress (DLL, fn) : 0;
}

static void * WS2_32 (const char * fn)
{
	static HINSTANCE DLL = 0;

	if (!DLL)
		DLL = LoadLibraryA ("ws2_32.dll");

	return DLL ? (void *) GetProcAddress (DLL, fn) : 0;
}

static void * KERNEL32 (const char * fn)
{
	static HINSTANCE DLL = 0;

	if (!DLL)
		DLL = LoadLibraryA ("kernel32.dll");

	return DLL ? (void *) GetProcAddress (DLL, fn) : 0;
}

fn_getaddrinfo compat_getaddrinfo ()
{
	static fn_getaddrinfo fn = 0;

	return fn ? fn : (fn = (fn_getaddrinfo) WS2_32 ("getaddrinfo"));
}

fn_freeaddrinfo compat_freeaddrinfo ()
{
	static fn_freeaddrinfo fn = 0;

	return fn ? fn : (fn = (fn_freeaddrinfo) WS2_32 ("freeaddrinfo"));
}

fn_mkgmtime64 compat_mkgmtime64 ()
{
	static fn_mkgmtime64 fn = 0;

	return fn ? fn : (fn = (fn_mkgmtime64) MSVCRT ("_mkgmtime64"));
}

fn_GetFileSizeEx compat_GetFileSizeEx ()
{
	static fn_GetFileSizeEx fn = 0;

	return fn ? fn : (fn = (fn_GetFileSizeEx) KERNEL32 ("GetFileSizeEx"));
}

#if defined(_WIN32) && defined(_UNICODE)

// Returns null or a wide-converted version of the U8 string passed. Free it with free(). Pass size -1 for null-terminated strings.
extern "C" lw_import wchar_t * lw_char_to_wchar(const char * u8str, int size)
{
	int length = MultiByteToWideChar(CP_UTF8, 0, u8str, size, NULL, 0);
	if (length > 0)
	{
		length += 10;
		wchar_t * u8Wide = (wchar_t *)malloc(length * sizeof(wchar_t));
		if (u8Wide)
		{
			length = MultiByteToWideChar(CP_UTF8, 0, u8str, size, u8Wide, length);
			if (length > 0)
			{
				// If size does not include the null byte, the converted result won't either.
				u8Wide[length] = L'\0';
				return u8Wide;
			}
			free(u8Wide);
		}
	}
	return NULL;
}
#endif
