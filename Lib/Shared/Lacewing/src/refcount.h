
/* vim: set noet ts=4 sw=4 sts=4 ft=c:
 *
 * Copyright (C) 2013, 2014 James McLaughlin.  All rights reserved.
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
#include <atomic>

#ifndef _lw_refcount_h
#define _lw_refcount_h

struct lwp_refcount
{
	std::atomic<unsigned short> refcount;
	void (* on_dealloc) (void *);
};

static inline lw_bool _lwp_retain (struct lwp_refcount * refcount)
{
	++ refcount->refcount;

	return lw_false;
}

static inline lw_bool _lwp_release (struct lwp_refcount * refcount)
{
	if ((-- refcount->refcount) == 0)
	{
	  if (refcount->on_dealloc)
		 refcount->on_dealloc ((void *) refcount);
	  else
		 free (refcount);

	  return lw_true;
	}

	return lw_false;
}

#define lwp_refcounted														\
	struct lwp_refcount refcount;											  \

#define lwp_retain(x, name)													\
	_lwp_retain ((struct lwp_refcount *) (x))								 \

#define lwp_release(x, name)												  \
	_lwp_release ((struct lwp_refcount *) (x))								 \

#define lwp_set_dealloc_proc(x, proc) do {									\
  *(void **) &(((struct lwp_refcount *) (x))->on_dealloc) = (void *) (proc);  \
} while (0);																  \

#define lwp_set_retain_proc(x, proc)
#define lwp_set_release_proc(x, proc)
#define lwp_set_refcount_name(x, name)
#define lwp_enable_refcount_logging(x, name)

#endif

