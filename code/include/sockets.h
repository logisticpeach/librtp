namespace sockets
{
	class SocketBase
	{
	public:
		SocketBase();
	protected:
		bool IsConnected();
		void ConnectSocket();
		void CloseSocket();
	private:
		int m_socket_handle;
		bool m_socket_open;
		bool m_socket_connected;
	};

	class StreamSocket : public SocketBase
	{
	public:
		StreamSocket();
		~StreamSocket();
	};

	class DatagramSocket : public SocketBase
	{
	public:
		DatagramSocket();
		~DatagramSocket();
	};
}