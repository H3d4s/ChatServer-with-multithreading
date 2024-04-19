#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) 
{
  // TODO: call rio_readinitb to initialize the rio_t object
	rio_readinitb(&rio, m_fd);
}



void Connection::connect(const std::string &hostname, int port) 
{
  // TODO: call open_clientfd to connect to the server
  // TODO: call rio_readinitb to initialize the rio_t object

	m_fd = open_clientfd(hostname.c_str(), std::to_string(port).c_str());
	if (m_fd < 0) {
		m_last_result = EOF_OR_ERROR; 
		return;
	}
	rio_readinitb(&rio, m_fd);
}

Connection::~Connection() 
{
  // TODO: close the socket if it is open
	if (m_fd >= 0) {
		Close(m_fd);
	}
}

bool Connection::is_open() const 
{
  // TODO: return true if the connection is open
	return m_fd >= 0;
}

void Connection::close() 
{
  // TODO: close the connection if it is open
	if (m_fd >= 0) {
		Close(m_fd);
		m_fd = -1;
	}
}

bool Connection::send(const Message &msg) 
{
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

	std::string full_msg = msg.tag + ":" + msg.data + "\n";
	if (rio_writen(m_fd, full_msg.c_str(), full_msg.length()) < 0) {
		m_last_result = EOF_OR_ERROR;
		return false;
	}
	m_last_result = SUCCESS;
	return true;
}

bool Connection::receive(Message &msg) 
{
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

	char buf[MAXLINE];
	if (rio_readlineb(&rio, buf, MAXLINE) < 0) {
		m_last_result = EOF_OR_ERROR;
		return false;
	}

	std::string full_msg(buf);
	size_t colon_pos = full_msg.find(':');
	if (colon_pos == std::string::npos) {
		m_last_result = INVALID_MSG;
		return false;
	}

	msg.tag = full_msg.substr(0, colon_pos);
	msg.data = full_msg.substr(colon_pos + 1);
	m_last_result = SUCCESS;
	return true;
}
