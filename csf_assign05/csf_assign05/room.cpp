#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
	: room_name(room_name) {
		pthread_mutex_init(&room_mutex, NULL); 
}

Room::~Room() 
{
  // TODO: destroy the mutex
	pthread_mutex_destroy(&room_mutex);
}

void Room::add_member(User *user) 
{
  // TODO: add User to the room
	Guard guard(room_mutex); 
	members.insert(user);    
}

void Room::remove_member(User *user) 
{
  // TODO: remove User from the room
	Guard guard(room_mutex); 
	members.erase(user);     
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) 
{
  // TODO: send a message to every (receiver) User in the room
	void Room::broadcast_message(const std::string & sender_username, const std::string & message_text) {
		Guard guard(room_mutex); // Sử dụng RAII để đồng bộ hóa

		for (User* user : members) {
			if (user->is_receiver()) { // Giả định User có phương thức is_receiver
				Message* msg = new Message("delivery", sender_username + ":" + message_text);
				user->get_message_queue().enqueue(msg); // Giả định User có phương thức get_message_queue
			}
		}
	}

}
