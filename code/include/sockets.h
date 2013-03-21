/**********************************************************************************************//**
 * @file	librtp\code\include\sockets.h
 *
 * @brief	Classes to encapsulate basic socket functionality used by the RTP library for network
 * 			transmission
 * 			
 * @author	Kenny Baxter
 **************************************************************************************************/

#ifndef LIBRTP_SOCKETS
#define LIBRTP_SOCKETS

#include <string>
#include "buffers.h"

namespace sockets
{
	const std::string DEFAULT_MULTICAST_GROUP = "225.0.0.1";			// The IP Address of the multicast group we're broadcasting on...
	const int ERR_INVALID_SOCKET = -1;
	const int ERR_MSG_TOO_LARGE = -2;

	/**********************************************************************************************//**
	 * @enum	SocketResultType
	 *
	 * @brief	Values that represent the result of a socket operation.
	 * 			
	 * @author	Kenny Baxter
	 * @date	19/03/2013
	 **************************************************************************************************/
	enum SocketResultType 
	{ 
		///< The operation compelted succesfully.
		SUCCESS = 0,
		///< There was a platform or operating system level error.
		PLATFORM_ERROR = 10, 
		///< There was a physical network or network stack error.
		NETWORK_ERROR = 20, 
		///< An unknown error occured.
		UNKNOWN_ERROR = 30 
	};

	/**********************************************************************************************//**
	 * @class	SocketResult
	 *
	 * @brief	Represents the result of a socket operation. 
	 *
	 * @author	Kenny Baxter
	 * @date	19/03/2013
	 **************************************************************************************************/
	class SocketResult
	{
	public:

		/**********************************************************************************************//**
		 * @fn	SocketResult::SocketResult();
		 *
		 * @brief	Creates a new instance of the SocketResult class with a result type of
		 * 			SocketResultType::SUCCESS and an error code of 0.
		 *
		 * @author	Kenny Baxter
		 * @date	19/03/2013
		 **************************************************************************************************/
		SocketResult();

		/**********************************************************************************************//**
		 * @fn	SocketResult::SocketResult(int code);
		 *
		 * @brief	Creates a new instance of the SocketResult class with a result type of
		 * 			SocketResultType::UNKNOWN_ERROR and the specified error code.
		 *
		 * @author	Kenny Baxter
		 * @date	19/03/2013
		 *
		 * @param	code	The error code retrieved from teh underlying socket call that failed.
		 **************************************************************************************************/
		SocketResult(int code);

		/**********************************************************************************************//**
		 * @fn	SocketResult::SocketResult(SocketResultType, int code);
		 *
		 * @brief	Creates a new instance of the SocketResult class with the specified SocketResultType
		 * 			and the specified error code.
		 *
		 * @author	Kenny Baxter
		 * @date	19/03/2013
		 *
		 * @param	parameter1	The type of error that ocurred.
		 * @param	code	  	The error code retrieved from teh underlying socket call that failed.
		 **************************************************************************************************/
		SocketResult(SocketResultType, int code);

		/**********************************************************************************************//**
		 * @fn	SocketResultType SocketResult::GetResultType();
		 *
		 * @brief	Get the type of the result.
		 *
		 * @author	Kenny Baxter
		 * @date	19/03/2013
		 *
		 * @return	A SocketResulType value detailing the nature of the result.
		 **************************************************************************************************/
		SocketResultType GetResultType();

		/**********************************************************************************************//**
		 * @fn	int SocketResult::GetErrorCode();
		 *
		 * @brief	Get the underlying result or error code.
		 *
		 * @author	Kenny Baxter
		 * @date	19/03/2013
		 *
		 * @return	The success code from teh underlying socket call.
		 **************************************************************************************************/
		int GetErrorCode();
	private:

		SocketResultType m_last_error;
		int m_error_code;
	};

	/**********************************************************************************************//**
	 * @class	DatagramSocket
	 *
	 * @brief	Encapsulates a Datagram socket wrapping familiar behaviour and functionality found
	 * 			in the berkely sockets API. The Datagram socket utilises the UDP transport protocol
	 * 			and sends and receives data as UDP packets.
	 *
	 * @author	Kenny Baxter
	 * @date	19/03/2013
	 **************************************************************************************************/
	class DatagramSocket
	{
	public:

		/**********************************************************************************************//**
		 * @fn	DatagramSocket::DatagramSocket();
		 *
		 * @brief	Creates a new datagram socket using UPD as teh underlying protocol (protocol = IPPROTO_UDP)
		 *
		 * @author	Kenny Baxter
		 * @date	19/03/2013
		 **************************************************************************************************/
		DatagramSocket();

		/**********************************************************************************************//**
		 * @fn	DatagramSocket::~DatagramSocket();
		 *
		 * @brief	Safely de-allocates any resources used by this socket.
	     *
		 * @author	Kenny Baxter
		 * @date	19/03/2013
		 **************************************************************************************************/
		~DatagramSocket();

		/**********************************************************************************************//**
		 * @fn	SocketResult DatagramSocket::Bind(unsigned short port);
		 *
		 * @brief	Binds this datagram socket to the multicast address (INADDR_ANY) on the specified port
		 * 			number;
		 *
		 * @author	Kenny Baxter
		 * @date	19/03/2013
		 *
		 * @param	port	The IP posrt number to bind the socket to.
		 *
		 * @return	A SocketResult describing the status of the bind call.
		 **************************************************************************************************/
		SocketResult Bind(unsigned short port);

		/**********************************************************************************************//**
		 * @fn	SocketResult DatagramSocket::Bind(std::wstring host, unsigned short port);
		 *
		 * @brief	Binds this datagram socket to the specified host on the specified port
		 *
		 * @author	Kenny Baxter
		 * @date	19/03/2013
		 *
		 * @param	host	The hostname to be resolved by DNS (or the IP4 address of the host)
		 * @param	port	The IP port number to bind the socket to.
		 *
		 * @return	A SocketResult describing the status of the bind call.
		 **************************************************************************************************/
		SocketResult Bind(std::wstring host, unsigned short port);

		/**********************************************************************************************//**
		 * @fn	SocketResult DatagramSocket::SendTo(unsigned short port, buffers::Buffer data);
		 *
		 * @brief	Sends the data in the specified buffer to the multicast group (LIBRTP_MULTICAST_GROUP)
		 * 			on the specified port.
		 *
		 * @author	Kenny Baxter
		 * @date	19/03/2013
		 *
		 * @param	port	The port to multicast data to
		 * @param	data	The data to be sent to the multicast group
		 *
		 * @return	A SocketResult containing the result of this operation
		 **************************************************************************************************/
		SocketResult SendTo(unsigned short port, buffers::Buffer data);

		/**********************************************************************************************//**
		 * @fn	SocketResult DatagramSocket::JoinMulticastGroup(std::string group_address);
		 *
		 * @brief	Join this socket to the specified multicast group for receiving datagrams
		 *
		 * @author	Kenny Baxter
		 * @date	20/03/2013
		 *
		 * @param	group_address	an ANSI string representation of the class D IP address of the
		 * 							desired multicast group.(See DEFAULT_MULTICAST_GROUP)
		 *
		 * @return	A SocketResult containing the result of this operation
		 **************************************************************************************************/
		SocketResult JoinMulticastGroup(std::string group_address);

	private:

		bool m_socket_initialised;
		unsigned long m_max_msg_size;
		int m_socket_handle;
	};

}

#endif