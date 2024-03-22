
#ifndef LAB5_2_MESSAGE_H
#define LAB5_2_MESSAGE_H

#define BUFFER_SIZE 100
#include <cstdio>
#include <cstdlib>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <cstdint>
#include <iostream>
#include <limits>
#include <csignal>
#include <semaphore.h>

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

#endif //LAB5_2_MESSAGE_H
