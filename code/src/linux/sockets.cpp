#include <sys/socket.h>
#include "../../include/sockets.h"

namespace sockets
{

SocketBase::SocketBase()
{
	m_socket_handle = -1;
	m_socket_open = false;
	m_socket_connected = false;
}

void SocketBase::ConnectSocket()
{
	m_socket_handle = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket_handle >= 0)
		m_socket_open = true;
}

void SocketBase::CloseSocket()
{
	shutdown(m_socket_handle, SHUT_RDWR);
	m_socket_open = false;
	m_socket_connected = false;
}



}

int main()
{
	return 0;
}
