/*
 * locks.h
 *
 *  Created on: 8 de out de 2018
 *      Author: solid
 */

#ifndef MAIN_INCLUDE_LOCKS_H_
#define MAIN_INCLUDE_LOCKS_H_

#include <stdint.h>

typedef enum DOOR_STATE{
	DOOR_OPEN,
	DOOR_CLOSE
}DOOR_STATE;

typedef enum DOORS{
	DOOR_1 = 13
}DOORS;

int Door_Initialize(void);
int Door_Open(DOORS door, DOOR_STATE state);




#endif /* MAIN_INCLUDE_LOCKS_H_ */
