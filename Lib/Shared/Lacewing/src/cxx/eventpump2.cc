
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

eventpump lacewing::eventpump_new ()
{
	return (eventpump) lw_eventpump_new ();
}

error _eventpump::start_eventloop ()
{
	return (error) lw_eventpump_start_eventloop ((lw_eventpump) this);
}

error _eventpump::tick ()
{
	return (error) lw_eventpump_tick ((lw_eventpump) this);
}

error _eventpump::start_sleepy_ticking
	  (void (lw_callback * on_tick_needed) (eventpump))
{
	return (error) lw_eventpump_start_sleepy_ticking
	  ((lw_eventpump) this, (void (*) (lw_eventpump)) on_tick_needed);
}

void _eventpump::post_eventloop_exit ()
{
	lw_eventpump_post_eventloop_exit ((lw_eventpump) this);
}


