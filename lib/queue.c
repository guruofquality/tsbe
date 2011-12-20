//
// Copyright 2011 Josh Blum
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <tsbe/buffer.h>
#include <tsbe/queue.h>
#include <pthread.h>
#include <stdlib.h> //alloc*
#include <string.h> //memcpy
#include <errno.h>
#include "types_impl.h"

typedef struct{
    tsbe_impl__buffer_t *circb;
    size_t circb_size;
    size_t circb_count;
    size_t rd_ptr, wr_ptr;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} tsbe_guts__queue_t;

typedef struct{
    tsbe_guts__queue_t *guts;
} tsbe_impl__queue_t;

int tsbe__queue__init(tsbe__queue_t *queue_){
    tsbe_impl__queue_t *queue = (tsbe_impl__queue_t *)queue_;

    //allocate guts
    queue->guts = (tsbe_guts__queue_t *)malloc(sizeof(tsbe_guts__queue_t));
    tsbe_guts__queue_t *guts = queue->guts;
    if (guts == NULL) return -1;

    //init circular buffer stuff
    guts->circb_size = 0;
    guts->circb_count = 0;
    guts->circb = NULL; //gets realloc'd on push
    guts->rd_ptr = 0;
    guts->wr_ptr = 0;

    //init pthread stuff
    if (pthread_mutex_init(&guts->mutex, NULL) != 0) return -1;
    if (pthread_cond_init(&guts->cond, NULL) != 0) return -1;

    return 0;
}

void tsbe__queue__done(tsbe__queue_t *queue_){
    tsbe_impl__queue_t *queue = (tsbe_impl__queue_t *)queue_;
    tsbe_guts__queue_t *guts = queue->guts;

    //destroy pthread stuff
    pthread_mutex_destroy(&guts->mutex);
    pthread_cond_destroy(&guts->cond);

    //free guts
    free(queue->guts);
}

int tsbe__queue__push(
    tsbe__queue_t *queue_,
    const tsbe__buffer_t *buff
){
    tsbe_impl__queue_t *queue = (tsbe_impl__queue_t *)queue_;
    tsbe_guts__queue_t *guts = queue->guts;
    int ret = 0;

    //locking mutex - here comes the thread safe section
    pthread_mutex_lock(&guts->mutex);

    //increase the queue space available
    if (guts->circb_count == guts->circb_size){
        if (guts->circb_size == 0) guts->circb_size = 1;
        else guts->circb_size <<= 1; //double it
        guts->circb = (tsbe_impl__buffer_t *)realloc(guts->circb, sizeof(tsbe_impl__buffer_t)*guts->circb_size);
        if (guts->circb == NULL) ret = -1;
    }

    //append this buffer to the end of the queue
    if (guts->circb != NULL){
        if (guts->wr_ptr == guts->circb_size) guts->wr_ptr = 0;
        memcpy(guts->circb + guts->wr_ptr, buff, sizeof(tsbe_impl__buffer_t));
        guts->wr_ptr++;
        guts->circb_count++;
    }

    //unlock and notify the condition variable
    pthread_mutex_unlock(&guts->mutex);
    pthread_cond_signal(&guts->cond);
    return ret;
}

static inline void pop_impl(
    tsbe_guts__queue_t *guts,
    tsbe__buffer_t *buff
){
    if (guts->rd_ptr == guts->circb_size) guts->rd_ptr = 0;
    memcpy(buff, guts->circb + guts->rd_ptr, sizeof(tsbe_impl__buffer_t));
    guts->rd_ptr++;
    guts->circb_count--;
}

int tsbe__queue__pop(
    tsbe__queue_t *queue_,
    tsbe__buffer_t *buff,
    const long long timeout_ns
){
    tsbe_impl__queue_t *queue = (tsbe_impl__queue_t *)queue_;
    tsbe_guts__queue_t *guts = queue->guts;
    int ret = 0;

    //locking mutex - here comes the thread safe section
    pthread_mutex_lock(&guts->mutex);

    if (guts->circb_count != 0) pop_impl(guts, buff);

    else if (timeout_ns == 0) ret = -2;

    else if (timeout_ns > 0){
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        const long long total_ns = ts.tv_nsec + timeout_ns;
        ts.tv_nsec = total_ns % 1000000000ul;
        ts.tv_sec += total_ns / 1000000000ul;
        while (guts->circb_count == 0){
            ret = pthread_cond_timedwait(&guts->cond, &guts->mutex, &ts);
            if (ret == ETIMEDOUT){
                ret = -2;
                break;
            }
        }
        if (guts->circb_count != 0){
            ret = 0;
            pop_impl(guts, buff);
        }
    }

    else if (timeout_ns < 0){
        pthread_cond_wait(&guts->cond, &guts->mutex);
        if (guts->circb_count != 0){
            ret = 0;
            pop_impl(guts, buff);
        }
        else ret = -1;
    }

    pthread_mutex_unlock(&guts->mutex);

    return ret;
}
