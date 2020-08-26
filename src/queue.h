/*
*******************************************************************************
 * @file           : queue.h
 * @brief         Header of fifo.c
 ******************************************************************************
 *  Created on: 26.08.2020
 *      Author: kvb
 *      mail: mat3x@mail.ru
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "stdint.h"
#include "stddef.h"

/** @brief Structure of mem QUEUE*/
typedef struct{
	uint16_t priority;
	uint8_t *data;
}queuemem_t;

/** @brief Structure of QUEUE*/
typedef struct{
	uint16_t wr;
	uint16_t rd;
	uint16_t bsize;
	uint16_t depth;
	uint16_t full_bsize;
	void** mempool;

}queue_t;

_Bool queue_isEmpty();
_Bool queue_isFull(queue_t * q);
void queue_init(queue_t *q, size_t nblocks, size_t block_size);
void queue_push(queue_t * q, void* data, uint16_t priority);
_Bool queue_add(queue_t * q, void* data, uint16_t priority);
uint16_t queue_pop(queue_t * q, void* data);
void clr_queue(queue_t *q);

#endif /* QUEUE_H_ */
