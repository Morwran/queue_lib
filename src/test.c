/*
 * test.c
 *
 *  Created on: 26.08.2020
 *      Author: kvb
 */


#include "queue.h"
#include <stdlib.h>
#include "stdio.h"

typedef struct {
	float ch11;
	float ch12;
	float ch21;
	float ch22;

}adc_smpl_t;

/** @brief Testing structure*/
typedef struct{
	adc_smpl_t d;
	int priority;
}data_priority_t;

/** @brief Structure of QUEUE*/
queue_t q;
queue_t q1;

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
	data_priority_t TestData[4]={\
			{{1,2,3,4},3},\
			{{5,6,7,8},5},\
			{{9,10,11,12},0},\
			{{13,14,15,16},2}\
	};

	data_priority_t TestData2[4]={\
			{{17,18,19,20},3},\
			{{21,22,23,24},5},\
			{{25,26,27,28},0},\
			{{29,30,31,32},2}\
	};

	for(uint8_t i=0;i<sizeof(TestData)/sizeof(data_priority_t);i++){
		printf("Din1: %f %f %f %f prio: %d\n",TestData[i].d.ch11,TestData[i].d.ch12,\
				TestData[i].d.ch21,TestData[i].d.ch22,TestData[i].priority);
		printf("Din2: %f %f %f %f prio: %d\n",TestData2[i].d.ch11,TestData2[i].d.ch12,\
			TestData2[i].d.ch21,TestData2[i].d.ch22,TestData2[i].priority);
	}

	queue_init(&q,4,sizeof(adc_smpl_t));
	queue_init(&q1,4,sizeof(adc_smpl_t));
	printf("ini queue st: cnt_w: %u cnt_rd: %u empty: %u full: %u\n",\
			q.wr,q.rd,queue_isEmpty(&q),queue_isFull(&q));
	for(int c=0;c<3;c++){
		for(int i=0;i<4;i++){
			if(!queue_isFull(&q))
				queue_add(&q,(void*)&TestData[i].d,TestData[i].priority);

			if(!queue_isFull(&q1))
				queue_add(&q1,(void*)&TestData2[i].d,TestData2[i].priority);

		}

		adc_smpl_t d[2];
		uint16_t prio[2];

		while(!queue_isEmpty(&q)){

			prio[0]=queue_pop(&q,&d[0]);
			printf("Dout1: %f %f %f %f prio: %d\n",d[0].ch11,d[0].ch12,d[0].ch21,d[0].ch22,prio[0]);
			//printf("queue st: cnt_w: %u cnt_rd: %u empty: %u full: %u\n",\
					q.wr,q.rd,queue_isEmpty(&q),queue_isFull(&q));
		}

		while(!queue_isEmpty(&q1)){

			prio[1]=queue_pop(&q1,&d[1]);
			printf("Dout2: %f %f %f %f prio: %d\n",d[1].ch11,d[1].ch12,d[1].ch21,d[1].ch22,prio[1]);
			//printf("queue st: cnt_w: %u cnt_rd: %u empty: %u full: %u\n",\
					q.wr,q.rd,queue_isEmpty(&q),queue_isFull(&q));
		}

		printf("cycle %u =================================================\n",c);
	}
	clr_queue(&q);
	clr_queue(&q1);
	return 0;
}
