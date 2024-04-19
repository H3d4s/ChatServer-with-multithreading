
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions
std::map<std::string, Room*> rooms; 
pthread_mutex_t rooms_mutex;        


////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace 
{

void *worker(void *arg) 
{
  pthread_detach(pthread_self());

  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
  int clientfd = *(static_cast<int*>(arg));
  delete static_cast<int*>(arg);

 Connection conn(clientfd);

  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
 Message login_msg;
 if (!conn.receive(login_msg)) {
     std::cerr << "Failed to receive login message.\n";
     conn.close();
     return nullptr;
 }
  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
 if (login_msg.tag == "slogin") {
     // Xử lý đăng nhập sender
     handle_sender(conn, login_msg.data);
 }
 else if (login_msg.tag == "rlogin") {
     // Xử lý đăng nhập receiver
     handle_receiver(conn, login_msg.data);
 }
 else {
     std::cerr << "Invalid login type.\n";
     conn.close();
 }

  return nullptr;
}

void handle_sender(Connection& conn, const std::string& username) {
  
    conn.send(Message("ok", "Logged in as sender"));

    
    Message msg;
    while (conn.receive(msg)) {
        
    }

    
    conn.close();
}
void handle_receiver(Connection& conn, const std::string& username) {
    
    conn.send(Message("ok", "Logged in as receiver"));

    
    conn.close();
}


}


////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port), m_ssock(-1) 
{
  // TODO: initialize mutex
	 
	pthread_mutex_init(&rooms_mutex, NULL);
}

Server::~Server() {
  // TODO: destroy mutex
	pthread_mutex_destroy(&rooms_mutex);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
	m_ssock = open_listenfd(std::to_string(m_port).c_str());
	return m_ssock >= 0;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
    while (true) {
        int* clientfd = new int(Accept(m_ssock, NULL, NULL)); // Hoặc dùng accept
        if (*clientfd < 0) {
            std::cerr << "Server accept failed.\n";
            continue;
        }
        pthread_t tid;
        pthread_create(&tid, NULL, worker, clientfd);
    }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary

    Guard guard(rooms_mutex);
    auto it = rooms.find(room_name);
    if (it != rooms.end()) {
        return it->second;
    }

    Room* new_room = new Room(room_name);
    rooms[room_name] = new_room;
    return new_room;
}
