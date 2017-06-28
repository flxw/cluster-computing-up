#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_WORK_ITEMS 200

#define CONSUME_DELAY_MS (20 * 1000)

/* list data type definitions */
typedef struct queue_item
{
    int data;
    struct queue_item *next;
} queue_item_t;

typedef struct
{
    queue_item_t *head;
    queue_item_t *tail;
    int num_produced, num_consumed;
} queue_t;

/* list functions */
void queue_append(queue_t *q, queue_item_t *item)
{
    item->next = NULL;

#pragma omp critical
    {
        if (q->head != NULL)
        {
            /* add to non-empty list */
            q->tail->next = item;
            q->tail = item;
        }
        else
        {
            /* add to an empty list */
            q->tail = item;
            q->head = item;
        }
    }
}

queue_item_t *queue_remove(queue_t *q)
{
    queue_item_t *retval;
#pragma omp critical
    {
        retval = q->head;

        if (q->head != NULL)
        {
            if (q->head->next != NULL)
            {
                q->head = q->head->next;
            }
            else
            {
                /* removing last element */
                q->head = NULL;
                q->tail = NULL;
            }
        }
    }
    return retval;
}

/* producer consumer stuff */
void produce(queue_t *queue)
{
    while (queue->num_produced < NUM_WORK_ITEMS)
    {
        int np = queue->num_produced;
#pragma omp task firstprivate(np)
        {
            queue_item_t *item = NULL;
            do
            {
                item = calloc(1, sizeof(*item));
                if (item != NULL)
                {
                    /* give item some id */
                    item->data = np;
                    queue_append(queue, item);
                }
                else
                {
                    perror("allocation of queue item failed");
                }
            } while (item == NULL);
        }
        queue->num_produced++;
    }

    printf("%d producer tasks created\n", NUM_WORK_ITEMS);
    #pragma omp taskwait
    printf("%d items produced\n", NUM_WORK_ITEMS);
}

void consume(queue_t *queue)
{
    while (queue->num_consumed < NUM_WORK_ITEMS)
    {
#pragma omp task
        {
            queue_item_t *item = queue_remove(queue);
            if (item != NULL)
            {
                printf("item %d consumed\n", item->data);
                free(item);

                /* simulate consuming/processing an element takes some time */
                usleep(CONSUME_DELAY_MS);
            }
        }
        queue->num_consumed++;
    }

    printf("%d consumer tasks created\n", NUM_WORK_ITEMS);
    #pragma omp taskwait
    printf("%d items consumed\n", NUM_WORK_ITEMS);
}

int main(int argc, char **argv)
{
    queue_t items = {NULL, NULL, 0, 0};

#pragma omp parallel
    {
#pragma omp sections
        {
#pragma omp section
            {
                produce(&items);
            }

#pragma omp section
            {
                consume(&items);
            }
        }
    }

    return EXIT_SUCCESS;
}
