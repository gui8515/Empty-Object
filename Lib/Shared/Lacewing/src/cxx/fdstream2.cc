
/* vim: set noet ts=4 sw=4 ft=cpp:
 *
 * Copyright (C) 2012, 2013 James McLaughlin et al.  All rights reserved.
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

fdstream lacewing::fdstream_new (lacewing::pump pump)
{
	return (fdstream) lw_fdstream_new ((lw_pump) pump);
}

void _fdstream::set_fd (lw_fd fd, lw_pump_watch watch, bool auto_close, bool is_socket)
{
	lw_fdstream_set_fd ((lw_fdstream) this, fd, watch, auto_close, is_socket);
}

bool _fdstream::valid ()
{
	return lw_fdstream_valid ((lw_fdstream) this);
}

void _fdstream::cork ()
{
	lw_fdstream_cork ((lw_fdstream) this);
}

void _fdstream::uncork ()
{
	lw_fdstream_uncork ((lw_fdstream) this);
}

void _fdstream::nagle (bool enabled)
{
	lw_fdstream_nagle ((lw_fdstream) this, enabled);
}


