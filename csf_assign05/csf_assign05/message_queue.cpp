#include <cassert>
#include <ctime>
#include "message_queue.h"

MessageQueue::MessageQueue() 
{
  // TODO: initialize the mutex and the semaphore

	pthread_mutex_init(&mutex, NULL);
	sem_init(&msg_sem, 0, 0);

}

MessageQueue::~MessageQueue() 
{
  // TODO: destroy the mutex and the semaphore

	pthread_mutex_destroy(&mutex);
	sem_destroy(&msg_sem);

}

void MessageQueue::enqueue(Message *msg) 
{
  // TODO: put the specified message on the queue

  // be sure to notify any thread waiting for a message to be
  // available by calling sem_post
	assert(msg != nullptr);

	pthread_mutex_lock(&mutex);
	queue.push_back(msg);
	sem_post(&msg_sem); 
	pthread_mutex_unlock(&mutex);


}

Message *MessageQueue::dequeue() 
{
  struct timespec ts;

  // get the current time using clock_gettime:
  // we don't check the return value because the only reason
  // this call would fail is if we specify a clock that doesn't
  // exist
  clock_gettime(CLOCK_REALTIME, &ts);

  // compute a time one second in the future
  ts.tv_sec += 1;

  // TODO: call sem_timedwait to wait up to 1 second for a message
  //       to be available, return nullptr if no message is available

  // TODO: remove the next message from the queue, return it

  if (sem_timedwait(&msg_sem, &ts) == -1) 
  {
	  if (errno == ETIMEDOUT) {
		  return nullptr; 
	  }
	  
  }
  pthread_mutex_lock(&mutex);
  if (queue.empty()) 
  {
	  pthread_mutex_unlock(&mutex);
	  return nullptr; 
  }
  Message *msg = nullptr;
  queue.pop_front();
  pthread_mutex_unlock(&mutex);
  return msg;
}
