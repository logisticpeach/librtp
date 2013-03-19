/**********************************************************************************************//**
 * @file	librtp\code\src\win32\sockets.cpp
 *
 * @brief	The Windows (winsock) implementation of the socket classes, wih some related 
 * 			useful functionality.
 **************************************************************************************************/

#include <atomic>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include "../../include/sockets.h"

namespace sockets
{
	std::atomic<bool> g_winsock_initialized = false;
	WSADATA g_winsock_data;

	// Initalise the winsock2 subsystem for this process.
	int InitializeWinsock()
	{
		if (g_winsock_initialized)
			return 0;

		// TODO - implement lock to make this code thread-safe
		auto version = MAKEWORD(2,2);
		int retval = WSAStartup(version, &g_winsock_data);

		if (retval == 0)
		{
			g_winsock_initialized = true;

			return 0;
		}
		else
			return retval;
	}

	sockaddr GetSocketAddressByName(std::wstring hostname, std::wstring port)
	{
		sockaddr addr;
		return addr;
	}

	sockaddr_in GetMulticastAddress(unsigned short port)
	{
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_addr.S_un.S_addr = inet_addr(LIBRTP_MULTICAST_GROUP);
		sin.sin_port = htons(port);
		return sin;
	}

	sockaddr_in GetBroadcastAddress(unsigned short port)
	{
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_addr.S_un.S_addr = INADDR_BROADCAST;
		sin.sin_port = htons(port);
		return sin;
	}


	SocketResult::SocketResult()
	{
		m_error_code = 0;
		m_last_error = SocketResultType::SUCCESS;
	}

	SocketResult::SocketResult(int error)
	{
		m_error_code = error;
		m_last_error = SocketResultType::UNKNOWN_ERROR;
	}

	SocketResult::SocketResult(SocketResultType error_type, int code)
	{
		m_error_code = code;
		m_last_error = error_type;
	}

	SocketResultType SocketResult::GetResultType()
	{
		return m_last_error;
	}

	int SocketResult::GetErrorCode()
	{
		return m_error_code;
	}

	int Win32CreateSocket(std::wstring host, std::wstring service, int sock_handle)
	{
		ADDRINFOW hints;
		ZeroMemory(&hints, sizeof(ADDRINFOW));

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		ADDRINFOW* address_results = nullptr;
		ADDRINFOW* current_address = nullptr;

		int retval = GetAddrInfoW(host.c_str(), service.c_str(),
			&hints, &address_results);

		bool connected = false;
		current_address = address_results;

		while (!connected && current_address != nullptr)
		{
			retval = connect(sock_handle, current_address->ai_addr, current_address->ai_addrlen);
			current_address = current_address->ai_next;
			if (retval != SOCKET_ERROR)
			{
				connected = true;
				retval = 0;
			}
		}

		FreeAddrInfoW(address_results);

		return retval;
	}

	DatagramSocket::DatagramSocket()
	{
		m_socket_handle = INVALID_SOCKET;
		m_socket_initialised = false;
		m_max_msg_size = 0;

		int retval = InitializeWinsock();

		if (retval == 0)
		{
			m_socket_handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

			if (m_socket_handle ==  INVALID_SOCKET)
			{
				m_socket_initialised = false;
				return;
			}

			m_socket_initialised = true;
			int longsize = sizeof(m_max_msg_size);
			getsockopt(m_socket_handle, SOL_SOCKET, SO_MAX_MSG_SIZE, (char*)&m_max_msg_size, &longsize);
		}

		return;
	}

	SocketResult DatagramSocket::SendTo(unsigned short port, buffers::Buffer data)
	{
		if (data.Size() > m_max_msg_size)
			return SocketResult(SocketResultType::PLATFORM_ERROR, ERR_MSG_TOO_LARGE);

		char* buffer = new char[data.Size()];
		ZeroMemory(buffer, data.Size());
		data.Read(0, data.Size(), &buffer);

		sockaddr_in multicast_address = GetMulticastAddress(port);

		int status = sendto(m_socket_handle, buffer, data.Size(), 0, (sockaddr*)&multicast_address, sizeof(multicast_address));

		if (status == SOCKET_ERROR)
		{
			status = WSAGetLastError();
			return SocketResult(SocketResultType::NETWORK_ERROR, status);
		}

		return SocketResult();
	}

	DatagramSocket::~DatagramSocket()
	{
		if (m_socket_initialised)
		{
			closesocket(m_socket_handle);
		}
	}

	SocketResult DatagramSocket::Bind(unsigned short port)
	{
		int status = 0;

		if (!m_socket_initialised)
			return SocketResult(SocketResultType::PLATFORM_ERROR, ERR_INVALID_SOCKET);

		sockaddr_in addr = GetMulticastAddress(port);
		status = bind(m_socket_handle, (sockaddr*)&addr, sizeof(addr));

		if (status != 0)
			return SocketResult(SocketResultType::PLATFORM_ERROR, status);

		return SocketResult();
	}
}



// contains useful address resolution code for stream socket implementations - harvest
// this later...

/*

	int Win32CreateSocket(std::wstring host, std::wstring service, int sock_handle)
	{
		ADDRINFOW hints;
		ZeroMemory(&hints, sizeof(ADDRINFOW));

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		ADDRINFOW* address_results = nullptr;
		ADDRINFOW* current_address = nullptr;

		int retval = GetAddrInfoW(host.c_str(), service.c_str(),
			&hints, &address_results);

		bool connected = false;
		current_address = address_results;

		while (!connected && current_address != nullptr)
		{
			retval = connect(sock_handle, current_address->ai_addr, current_address->ai_addrlen);
			current_address = current_address->ai_next;
			if (retval != SOCKET_ERROR)
			{
				connected = true;
				retval = 0;
			}
		}

		FreeAddrInfoW(address_results);

		return retval;
	}

	*/