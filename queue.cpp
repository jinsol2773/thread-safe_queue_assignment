#include <iostream>
#include "queue.h"
#include <mutex> 

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
	std::lock_guard<std::mutex> lock(mtx);
	Reply reply = { false, item };

	Node* new_node = nalloc(item);
	if (queue->tail == nullptr) {
		queue->head = queue->tail = new_node;
	}
	else {
		queue->tail->next = new_node;
		queue->tail = new_node;
	}

	reply.success = true;
	return reply;
}


Reply dequeue(Queue* queue) {
	std::lock_guard<std::mutex> lock(mtx);
	Reply reply = { false, {0, nullptr} };

	if (queue->head == nullptr) {
		return reply;
	}

	Node* temp = queue->head;
	reply.item = queue->head->item;
	queue->head = queue->head->next;

	if (queue->head == nullptr) {
		queue->tail = nullptr;
	}

	nfree(temp);
	reply.success = true;
	return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
	std::lock_guard<std::mutex> lock(mtx);
	Queue* new_queue = init();

	Node* current = queue->head;
	while (current != nullptr) {
		if (current->item.key >= start && current->item.key <= end) {
			enqueue(new_queue, current->item);
		}
		current = current->next;
	}

	return new_queue;
}
