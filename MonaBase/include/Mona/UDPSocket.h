/*
Copyright 2014 Mona
mathieu.poux[a]gmail.com
jammetthomas[a]gmail.com

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
#include "Mona/Socket.h"

namespace Mona {

class UDPSocket : private SocketEvents, virtual Object {
public:
	UDPSocket(const SocketManager& manager, bool allowBroadcast=false);
	virtual ~UDPSocket();

	// unsafe-threading
	const SocketAddress&	address() const { std::lock_guard<std::mutex> lock(_mutex); return updateAddress(); }
	const SocketAddress&	peerAddress() const {  std::lock_guard<std::mutex> lock(_mutex); return updatePeerAddress(); }
	// safe-threading
	SocketAddress&			address(SocketAddress& address) const { std::lock_guard<std::mutex> lock(_mutex); return address.set(updateAddress()); }
	SocketAddress&			peerAddress(SocketAddress& address) const { std::lock_guard<std::mutex> lock(_mutex); return address.set(updatePeerAddress()); }

	bool					bind(Exception& ex, const SocketAddress& address) { bool result = _socket.bind(ex, address); resetAddresses(); return result; }
	void					close() { _socket.close(); resetAddresses(); }
	bool					connect(Exception& ex, const SocketAddress& address) { bool result = _socket.connect(ex, address, _allowBroadcast);  resetAddresses(); return result; }
	void					disconnect() { Exception ex; _socket.connect(ex, SocketAddress::Wildcard()); resetAddresses(); }

	bool					send(Exception& ex, const UInt8* data, UInt32 size);
	bool					send(Exception& ex, const UInt8* data, UInt32 size, const SocketAddress& address);

	template<typename SenderType>
	bool send(Exception& ex, std::shared_ptr<SenderType>& pSender) {
		return _socket.send<SenderType>(ex, pSender);
	}
	template<typename SenderType>
	PoolThread* send(Exception& ex,const std::shared_ptr<SenderType>& pSender, PoolThread* pThread) {
		return _socket.send<SenderType>(ex, pSender,pThread);
	}

	const SocketManager&	manager() const { return _socket.manager(); }
private:
	virtual void			onReception(PoolBuffer& pBuffer, const SocketAddress& address) = 0;

	const SocketAddress&	updateAddress() const { if (_address) return _address;  Exception ex; _socket.address(ex, _address); return _address; }
	const SocketAddress&	updatePeerAddress() const { if (_peerAddress) return _peerAddress; Exception ex; _socket.peerAddress(ex, _peerAddress); return _peerAddress; }
	void					resetAddresses() {std::lock_guard<std::mutex> lock(_mutex);_address.reset();_peerAddress.reset();}

	void					onReadable(Exception& ex,UInt32 available);
	
	const bool				_allowBroadcast;
	Socket					_socket;
	mutable std::mutex		_mutex;
	mutable SocketAddress	_address;
	mutable SocketAddress	_peerAddress;
};



} // namespace Mona