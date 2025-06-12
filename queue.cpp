#include <iostream>
#include "queue.h"


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
        delete temp;
    }
    delete queue;
}

Node* nalloc(Item item) {
    Node* node = new Node;
    node->item = item;
    node->next = nullptr;
    return node;
}


void nfree(Node* node) {
    delete node;
}

Node* nclone(Node* node) {
    return nalloc(node->item);
}


Reply enqueue(Queue* queue, Item item) {
	Reply reply = { false, NULL };
	return reply;
}

Reply dequeue(Queue* queue) {
	Reply reply = { false, NULL };
	return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
	return NULL;
}
