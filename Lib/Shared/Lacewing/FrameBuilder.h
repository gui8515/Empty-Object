
/* vim: set noet ts=4 sw=4 ft=cpp:
 *
 * Copyright (C) 2011 James McLaughlin.  All rights reserved.
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

#include "MessageBuilder.h"

#ifndef lacewingframebuilder
#define lacewingframebuilder

class framebuilder : public messagebuilder
{
protected:

	void preparefortransmission()
	{
		if (tosend)
			return;

		lw_ui32 type = *(lw_ui32 *) buffer;
		lw_i32 messagesize = size - 8;

		lw_ui32 headersize;

		// Message size < 254; store as type byte + size byte
		if (messagesize < 0xfe)
		{
			(*(lw_ui8*)(buffer + 6)) = (lw_ui8)type;
			(*(lw_ui8*)(buffer + 7)) = (lw_ui8)messagesize;

			headersize = 2;
		}
		// Message size >= 0xFF and <= 0xFFFF; store as type byte, plus size indicator byte of 254, plus size uint16
		else if (messagesize < 0xffff)
		{
			(*(lw_ui8*)(buffer + 4)) = (lw_ui8)type;

			(*(lw_ui8 *) (buffer + 5))	= 254;
			(*(lw_ui16 *) (buffer + 6)) = (lw_ui16)messagesize;

			headersize = 4;
		}
		// Message size > 0xFFFF and <= 0xFFFFFFFF; store as type byte, plus size indicator byte of 255, plus size uint32
		else if ((lw_ui32)messagesize < 0xffffffff)
		{
			(*(lw_ui8*)(buffer + 2)) = (lw_ui8)type;

			(*(lw_ui8 *) (buffer + 3))	= 255;
			(*(lw_ui32 *) (buffer + 4)) = messagesize;

			headersize = 6;
		}
		else
			return;

		tosend	 = (buffer + 8) - headersize;
		tosendsize =  messagesize + headersize;
	}

	bool isudpclient;

	char * tosend;
	int tosendsize;

public:

	framebuilder(bool isudpclient)
	{
		this->isudpclient = isudpclient;
		tosend = nullptr;
		tosendsize = 0;
	}

	inline void addheader(lw_ui8 type, lw_ui8 variant, bool forudp = false, int udpclientid = -1)
	{
		assert(size == 0 && "lacewing framebuilder.addheader() error: adding header to message that already has one.");

		if (!forudp)
		{
			add <lw_ui32> ((type << 4) | variant);
			add <lw_ui32> (0);

			return;
		}

		add <lw_ui8>  ((lw_ui8)((type << 4) | variant));

		if (isudpclient)
			add <lw_ui16> ((lw_ui16)udpclientid);
	}

	inline void send(lacewing::server_client client, bool clear = true)
	{
		preparefortransmission();
		client->write(tosend, tosendsize);

		if (clear)
			framereset();
	}

	inline void send(lacewing::client client, bool clear = true)
	{
		preparefortransmission();
		client->write(tosend, tosendsize);

		if (clear)
			framereset();
	}

	inline void send(lacewing::udp udp, lacewing::address address, bool clear = true)
	{
		udp->send(address, buffer, size);

		if (clear)
			framereset();
	}

	inline void framereset()
	{
		reset();
		tosend = 0;
		tosendsize = 0;
	}

};

#endif

