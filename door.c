#include "defs.h"

/*
    Function: initDoor
    Purpose: initialize a door between two rooms. Dynamically allocate memory for the door
    Parameters:
            in: RoomType *roomA: roomA to be connected with roomB
            in: RoomType *roomB: roomB to be connected with roomA
           out: DoorType **door: The door to be initialized
    Returns: void
*/
void initDoor(DoorType **door, RoomType *roomA, RoomType *roomB)
{
    DoorType *d = malloc(sizeof(DoorType));
    sem_init(&d->mutex, 0, 1); // init to 1;
    d->roomA = roomA;
    d->roomB = roomB;

    *door = d;
}