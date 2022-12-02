#include <stdlib.h>
#include "queue.h"

void enqueue(struct PointQueue *q, int x, int y)
{
    struct PointQueue *elt = malloc(sizeof(struct PointQueue));
    elt->x = x;
    elt->y = y;

    if (q->next == NULL)
        elt->next = elt;
    else
    {
        elt->next = q->next->next;
        q->next->next = elt;
    }
    q->next = elt;
}

struct PointQueue *pop(struct PointQueue *q)
{
    struct PointQueue* oldest = q->next->next;
    if (q->next == oldest)
        q->next = NULL;
    else
        q->next->next = oldest->next;

    return oldest;
}

int isempty(struct PointQueue *q)
    return q->next == NULL;
