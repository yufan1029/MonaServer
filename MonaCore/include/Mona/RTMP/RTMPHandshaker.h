/* 
	Copyright 2013 Mona - mathieu.poux[a]gmail.com
 
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License received along this program for more
	details (or else see http://www.gnu.org/licenses/).

	This file is a part of Mona.
*/

#pragma once

#include "Mona/Mona.h"
#include "Mona/TCPSender.h"
#include "Mona/MemoryWriter.h"
#include "Mona/Buffer.h"
#include "Poco/SharedPtr.h"
#include <openssl/rc4.h>

namespace Mona {

class RTMPHandshaker : public TCPSender, virtual Object {
public:
	RTMPHandshaker(const UInt8* data, UInt32 size);
	RTMPHandshaker(const UInt8* farPubKey,const UInt8* challengeKey,bool middle,const Poco::SharedPtr<RC4_KEY>& pDecryptKey,const Poco::SharedPtr<RC4_KEY>& pEncryptKey);

private:
	
	bool run(Exception& ex);
	void runComplex();

	const UInt8*	begin(bool displaying = false) { return _writer.begin(); }
	UInt32			size(bool displaying = false) { return _writer.length(); }

	UInt8					_buffer[3073];
	MemoryWriter			_writer;
	Buffer<UInt8>		_farPubKey;
	UInt8					_challengeKey[HMAC_KEY_SIZE];	
	bool						_middle;
	Poco::SharedPtr<RC4_KEY>	_pEncryptKey;
	Poco::SharedPtr<RC4_KEY>	_pDecryptKey;
};


} // namespace Mona
