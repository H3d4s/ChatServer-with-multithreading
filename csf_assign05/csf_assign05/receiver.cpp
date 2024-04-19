#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // TODO: connect to server
  conn.connect(server_hostname, server_port);
  if (!conn.is_open()) {
      std::cerr << "Failed to connect to server.\n";
      return 1;
  }

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)

 
  Message rlogin_msg("rlogin", username);
  if (!conn.send(rlogin_msg)) {
      std::cerr << "Failed to send rlogin message.\n";
      return 1;
  }


  Message response;
  if (!conn.receive(response) || response.tag != "ok") {
      std::cerr << "Failed to log in or received error from server: " << response.data << "\n";
      return 1;
  }

 
  Message join_msg("join", room_name);
  if (!conn.send(join_msg)) {
      std::cerr << "Failed to send join message.\n";
      return 1;
  }

  
  if (!conn.receive(response) || response.tag != "ok") {
      std::cerr << "Failed to join room or received error from server: " << response.data << "\n";
      return 1;
  }

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)

  while (true) {
      if (!conn.receive(response)) {
          std::cerr << "Error receiving message from server.\n";
          break;
      }

      if (response.tag == "delivery") {
          std::cout << response.data << std::endl; 
      }
      else if (response.tag == "err") {
          std::cerr << "Server error: " << response.data << std::endl;
      }
  }



  return 0;
}
