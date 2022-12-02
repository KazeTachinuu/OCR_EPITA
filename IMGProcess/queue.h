#ifndef PQUEUE_H
#define PQUEUE_H

struct PointQueue
{
    int x;
    int y;
    struct PointQueue *next;
}

void enqueue(struct PointQueue *q, int x, int y);
struct PointQueue *pop(struct PointQueue *q);
int isempty(struct PointQueue *q);
#endif
