//
// Created by ivan on 18.3.24.
//

#ifndef LAB4_MESSAGE_H
#define LAB4_MESSAGE_H
#include <cstdio>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstdint>
#include <iostream>
#include <limits>
#include <csignal>
#include <semaphore.h>
#define BUFFER_SIZE 5
typedef struct {
    uint8_t type;
    uint8_t hash;
    uint8_t size;
    uint8_t data[256];
} Message;

typedef struct {

    int head;
    int tail;
    int added_count;
    int extracted_count;
    Message messages[BUFFER_SIZE];

}MessageQueue;

void initialize_queue(MessageQueue* messageQueue);
void add_message(MessageQueue* queue, Message message);
Message get_message(MessageQueue* queue);
Message generate_message();
void print_message(Message message);
void print_message_queue(MessageQueue* queue);



#endif //LAB4_MESSAGE_H
