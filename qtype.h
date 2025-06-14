#ifndef _QTYPE_H
#define _QTYPE_H

#include <atomic>

typedef unsigned int Key;
typedef void* Value;

typedef struct {
    Key key;
    Value value;
    int value_size;
} Item;

typedef struct {
    bool success;
    Item item;
} Reply;

typedef struct node_t {
    Item item;
    struct node_t* next;
} Node;

typedef struct {
    Node* head, * tail;
    // 필드 추가 가능
} Queue;

#endif
