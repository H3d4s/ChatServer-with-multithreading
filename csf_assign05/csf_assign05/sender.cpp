#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // TODO: connect to server
  Connection conn;
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
      std::cerr << "Failed to connect to server.\n";
      return 1;
  }


  // TODO: send slogin message

  Message slogin_msg("slogin", username);
  if (!conn.send(slogin_msg)) {
      std::cerr << "Failed to send slogin message.\n";
      return 1;
  }

 
  Message response;
  if (!conn.receive(response) || response.tag != "ok") {
      std::cerr << "Login failed or received error from server: " << response.data << "\n";
      return 1;
  }


  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  std::string line;
  while (getline(std::cin, line)) {
      std::istringstream iss(line);
      std::string command;
      iss >> command;

      if (command == "/join") {
          std::string room_name;
          iss >> room_name;
          Message join_msg("join", room_name);
          conn.send(join_msg);
      }
      else if (command == "/leave") {
          conn.send(Message("leave", ""));
      }
      else if (command == "/quit") {
          conn.send(Message("quit", ""));
          break;
      }
      else {
          
      }
  }


  return 0;
}
