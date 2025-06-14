#include <mutex>
#include <cstdlib>
#include <cstring>
#include "queue.h"

std::mutex mtx;

Queue* init(void) {
    Queue* queue = new Queue;
    queue->head = nullptr;
    queue->tail = nullptr;
    return queue;
}

void release(Queue* queue) {
    Node* current = queue->head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        if (temp->item.value) {
            free(temp->item.value);
        }
        delete temp;
    }
    delete queue;
}

Reply enqueue(Queue* queue, Item item) {
    std::lock_guard<std::mutex> lock(mtx);
    Reply reply = { false, item };

    int raw_value = (int)(intptr_t)(item.value);

    void* new_value = malloc(sizeof(int));
    if (!new_value) return reply;
    memcpy(new_value, &raw_value, sizeof(int));

    Node* current = queue->head;
    while (current != nullptr) {
        if (current->item.key == item.key) {
            free(current->item.value);
            current->item.value = new_value;
            current->item.value_size = sizeof(int);
            reply.success = true;
            reply.item = current->item;
            return reply;
        }
        current = current->next;
    }

    Node* new_node = new Node;
    new_node->item.key = item.key;
    new_node->item.value = new_value;
    new_node->item.value_size = sizeof(int);
    new_node->next = nullptr;

    if (queue->tail == nullptr) {
        queue->head = queue->tail = new_node;
    }
    else {
        queue->tail->next = new_node;
        queue->tail = new_node;
    }

    reply.success = true;
    reply.item = new_node->item;
    return reply;
}

Reply dequeue(Queue* queue) {
    std::lock_guard<std::mutex> lock(mtx);
    Reply reply = { false, {0, nullptr, 0} };

    if (queue->head == nullptr)
        return reply;

    Node* temp = queue->head;
    reply.item.key = temp->item.key;
    reply.item.value = temp->item.value;
    reply.item.value_size = temp->item.value_size;

    queue->head = queue->head->next;
    if (queue->head == nullptr)
        queue->tail = nullptr;

    delete temp;
    reply.success = true;
    return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
    std::lock_guard<std::mutex> lock(mtx);
    Queue* new_queue = init();

    Node* current = queue->head;
    while (current != nullptr) {
        if (current->item.key >= start && current->item.key <= end) {
            void* copied_value = malloc(current->item.value_size);
            if (!copied_value) {
                current = current->next;
                continue;
            }
            memcpy(copied_value, current->item.value, current->item.value_size);

            Item new_item;
            new_item.key = current->item.key;
            new_item.value = copied_value;
            new_item.value_size = current->item.value_size;

            enqueue(new_queue, new_item);
        }
        current = current->next;
    }

    return new_queue;
}
