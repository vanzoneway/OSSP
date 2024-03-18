#include "message.h"


void initialize_queue(MessageQueue* messageQueue) {
    messageQueue->head = 0;
    messageQueue->tail = 0;
    messageQueue->added_count = 0;
    messageQueue->extracted_count = 0;
}

void add_message(MessageQueue* queue, Message message) {
    if ((queue->tail + 1) % BUFFER_SIZE == queue->head) {
        printf("Ошибка: Очередь сообщений заполнена.\n");
        return;
    }

    queue->messages[queue->tail] = message;
    queue->tail = (queue->tail + 1) % BUFFER_SIZE;
    queue->added_count++;
}
Message get_message(MessageQueue* queue) {
    if (queue->head == queue->tail) {
        printf("Ошибка: Очередь сообщений пуста.\n");
        Message empty_message = { 0 };
        return empty_message;
    }

    Message message = queue->messages[queue->head];
    queue->head = (queue->head + 1) % BUFFER_SIZE;
    queue->extracted_count++;

    return message;
}

Message generate_message() {
    Message message;
    message.hash = 0;
    message.size = rand() % 257;

    if (message.size == 0 || message.size == 256) {
        message.size = 0;
    }

    for (int i = 0; i < message.size; i++) {
        message.data[i] = rand() % 256;
        message.hash ^= message.data[i];
    }

    return message;
}

void print_message(Message message) {
    printf("_______________________________________\n");
    printf("|                                     |\n");
    printf("|   Size: %u\n", message.size);
    printf("|   Data:                             |\n");
    printf("|   ");
    for (int i = 0; i < 12; i++) {
        printf("_________");
    }
    printf("   |\n");
    printf("|  |               |               |  |\n");
    printf("|  |    Data:      |      Data:    |  |\n");

    int charsPerLine = 12;

    int numLines = (message.size + charsPerLine - 1) / charsPerLine;
    int start = 0;

    for (int line = 0; line < numLines; line++) {
        printf("|  |");
        int end = start + charsPerLine;
        if (end > message.size) {
            end = message.size;
        }

        for (int i = start; i < end; i++) {
            printf(" %8u ", message.data[i]);
        }

        for (int i = end - start; i < charsPerLine; i++) {
            printf("         ");
        }

        printf("|  |\n");
        start = end;
    }

    printf("|  |_______________|_______________|  |\n");
    printf("|                                     |\n");
    printf("|_____________________________________|\n");
}

void print_message_queue(MessageQueue* queue) {
    printf("____________________________________________________________\n");
    printf("|                                                          |\n");
    printf("|   Head: %d\n", queue->head);
    printf("|   Tail: %d\n", queue->tail);
    printf("|   Added Count: %d\n", queue->added_count);
    printf("|   Extracted Count: %d\n", queue->extracted_count);
    printf("|                                                          |\n");
    printf("|__________________________________________________________|\n");

}
