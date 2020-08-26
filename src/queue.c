/*
 **********************************************************************************************************
 * @file   : queue.c
 * @brief  : Library containing functions for working with priority queue.
 * This library has been successfully tested on x86 PC and MCU boards such as STM32 (Cortex M3 and M7 core)
 * and Xilinx Spartan6 (Microblaze core).
 **********************************************************************************************************
 *  Created on: 26.08.2020
 *      Author: kvb
 *      mail: mat3x@mail.ru
 */


#include "queue.h"
#include <stdio.h>
//#include <conio.h>
#include <stdlib.h>
#include "string.h"


//void** mempool1 = NULL;

/**
  * @brief Prepare pool of memory for queue
  * @param queue_t * q - Pointer to the QUEUE structure.
  * @param uint16_t num_blocks - number of data blocks
  * @param uint16_t block_size - size of one data block
  * @retval None.
  */
static void prepare_pool(queue_t *q, size_t nblocks, size_t block_size) {

	//mempool = NULL;
    q->mempool = (void**) malloc(nblocks*sizeof(void*));
    for (int i = 0; i < nblocks; i++) {
    	q->mempool[i] = malloc(block_size);
    }
}

/**
  * @brief Free memory pool
  * @param queue_t * q - Pointer to the QUEUE structure.
  * @param void** mempool - Pointer to memory array
  * @retval None.
  */
static void deepfreemempool(queue_t *q, void** mempool) {
    for (int i = 0; i < q->depth; i++) {
        free(mempool[i]);
    }
    free(mempool);
}

/**
  * @brief Getting memory for queue
  * @param int id - identifier of memory array.
  * @param void** mempool - Pointer to memory array
  * @retval None.
  */
static void* get_mempool(int id,void** mempool)
{

    return mempool[id];

}

/**
  * @brief QUEUE structure initialization
  * @param queue_t * q - Pointer to the QUEUE structure.
  * @param uint16_t num_blocks - number of data blocks
  * @param uint16_t block_size - size of one data block
  * @retval None.
  */
void queue_init(queue_t *q, size_t nblocks, size_t block_size)
{
	size_t full_bsize=(block_size+sizeof(queuemem_t));
	//printf("qsize: %u bsize: %u full: %u\n",sizeof(queue_t),block_size,full_bsize);
	prepare_pool(q,nblocks+1,full_bsize);
	q->bsize=block_size;
	q->wr=0;
	q->rd=0;
	q->depth=nblocks+1;
	q->full_bsize=full_bsize;

}

static inline uint16_t ret_delta(queue_t * q)
{
	return (((int)q->wr<(int)q->rd)?(q->depth-q->rd+q->wr):\
			(q->wr-q->rd));
}

/**
  * @brief Checking if QUEUE is empty or not
  * @param queue_t * q - Pointer to the QUEUE structure.
  * @retval _Bool - True/False (1,0). True if QUEUE is empty.
  */
_Bool queue_isEmpty(queue_t *q)
{
	return (q->wr==q->rd)?1:0;
}


/**
  * @brief Checking if QUEUE is full or not
  * @param queue_t * q - Pointer to the QUEUE structure.
  * @retval _Bool - True/False (1,0). True if QUEUE is full.
  */
_Bool queue_isFull(queue_t * q)
{
	uint16_t delta = ret_delta(q);


	if(delta == q->depth-1)
		return 1;

	return 0;
}

/**
  * @brief Clear QUEUE memory
  * @param queue_t * q - Pointer to the QUEUE structure.
  * @retval None.
  */
void clr_queue(queue_t *q)
{
	q->wr=0;
	q->rd=0;
	deepfreemempool(q,q->mempool);
}

//static int cmp_prior(const void *a, const void *b) {
//    return ((queuemem_t*)a)->priority - ((queuemem_t*)b)->priority;
//}

/**
  * @brief Push one block of data into queue
  * @param queue_t * q - Pointer to the QUEUE structure.
  * @param void* data - Pointer to input data
  * @retval None.
  */
void queue_push(queue_t * q, void* data, uint16_t priority)
{
	queuemem_t *qmem;
	if(q->wr==q->depth)
		if(!queue_isFull(q))
			q->wr=0;
	qmem=(queuemem_t *)get_mempool(q->wr,q->mempool);
	qmem->priority=priority;
	memcpy(&qmem->data,data,q->bsize);
	q->wr++;

}

/**
  * @brief Adding one block to QUEUE
  * @param queue_t * q - Pointer to the QUEUE structure.
  * @param void* data - Pointer to input data
  * @param uint16_t priority - priority in range from 0 to 65535. 0 is a maximum priority. 65535 is a minimum priority
  * @retval True/False. True if successfully adding block of data to queue
  */
_Bool queue_add(queue_t * q, void* data, uint16_t priority)
{
	if(!queue_isFull(q))
		queue_push(q,data,priority);
	else
		return 0;

	uint16_t qdepth = ret_delta(q);

	if(qdepth>1){

		//sort
		for(int j=0;j<qdepth-1;j++){
			uint16_t sp=q->rd;
			uint16_t spp=q->rd+1;
			for(int i=0;i<qdepth-1;i++){
				if(sp>=q->depth)
					sp=0;
				if(spp>=q->depth)
					spp=0;
				if(((queuemem_t*)q->mempool[sp])->priority>((queuemem_t*)q->mempool[spp])->priority){

					queuemem_t *tmp=(queuemem_t*)q->mempool[spp];
					q->mempool[spp]=q->mempool[sp];
					q->mempool[sp]=tmp;
				}
				sp++;
				spp++;
			}
		}


	}

	return 1;
}

/**
  * @brief Pop one block of data from QUEUE
  * @param queue_t * q - Pointer to the QUEUE structure.
  * @param void* data - Pointer to output data
  * @retval Current priority.
  */
uint16_t queue_pop(queue_t * q, void* data)
{
	queuemem_t *qmem;
	if(q->rd==q->depth)
		if(!queue_isEmpty(q))
			q->rd=0;
	qmem=(queuemem_t *)get_mempool(q->rd,q->mempool);
	memcpy(data,&qmem->data,q->bsize);
	q->rd++;

	return qmem->priority;
}
