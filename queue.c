#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO: How about freeing the list elements and the strings? */
    if (!q) {
        return;
    }

    while (q->head) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;
        free(tmp->value);
        free(tmp);
        tmp = q->head;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }

    memset(newh->value, '\0', strlen(s) + 1);
    strncpy(newh->value, s, strlen(s));

    newh->next = q->head;
    q->head = newh;

    // first time
    if (!q->tail) {
        q->tail = newh;
    } else {
        while (q->tail->next) {
            q->tail = q->tail->next;
        }
    }

    q->size += 1;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (!q) {
        return false;
    }
    list_ele_t *newt;  // newt means new element in tail
    newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        return false;
    }

    newt->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newt->value) {
        free(newt);
        return false;
    }

    memset(newt->value, '\0', strlen(s) + 1);
    strncpy(newt->value, s, strlen(s));
    newt->next = NULL;
    if (!q->tail) {
        q->head = newt;
        q->tail = newt;
    } else {
        q->tail->next = newt;
        q->tail = newt;
    }

    q->size += 1;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO: You need to fix up this code. */
    if (!q || !q->head) {
        return false;
    }

    if (sp) {
        // Insure copy string is right
        size_t realbufsize;
        if (bufsize > strlen(q->head->value)) {
            realbufsize = strlen(q->head->value);
        } else {
            realbufsize = bufsize - 1;
        }

        memset(sp, '\0', realbufsize + 1);
        strncpy(sp, q->head->value, realbufsize);
    }

    list_ele_t *tmp;

    tmp = q->head;
    q->head = q->head->next;
    tmp->next = NULL;
    free(tmp->value);
    free(tmp);

    q->size -= 1;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if (!q || !q->head) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    if (!q)
        return;

    if (q->size <= 1)
        return;

    list_ele_t *prev = NULL;
    list_ele_t *cur = q->head;
    list_ele_t *next = NULL;
    while (cur) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }

    q->tail = q->head;
    q->head = prev;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void merge_sort(list_ele_t **head)
{
    if (!(*head))
        return;

    if (!(*head)->next)
        return;

    list_ele_t *l1 = (*head)->next;
    list_ele_t *l2 = (*head);

    while (l1 && l1->next) {
        l2 = l2->next;
        l1 = l1->next->next;
    }

    l1 = l2->next;
    l2->next = NULL;
    l2 = *head;

    merge_sort(&l1);
    merge_sort(&l2);

    *head = NULL;
    list_ele_t **tmp = head;

    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) < 0) {
            *tmp = l1;
            l1 = l1->next;
        } else {
            *tmp = l2;
            l2 = l2->next;
        }
        tmp = &((*tmp)->next);
    }

    if (l1)
        *tmp = l1;
    else
        *tmp = l2;
}

void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    if (!q)
        return;

    if (q->size <= 1)
        return;

    merge_sort(&q->head);

    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}
