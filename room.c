#include "defs.h"

/*
    Function: initRoom
    Purpose: initialize an empty room with a name. Dynamically allocate memory for the rooms , and set appropriate default values for their fields
    Parameters:
            in: char *name: room's name field
           out: RoomType **room: The room to be initialized
    Returns: void
*/
void initRoom(RoomType **room, char *name)
{
    RoomType *r = malloc(sizeof(RoomType));
    strcpy(r->name, name);
    r->ghost = NULL;
    r->evidence = NULL;

    for (int i = 0; i < MAX_HUNTERS; i++)
    {
        r->hunters[i] = NULL;
    }
    for (int i = 0; i < DIRECTION_COUNT; i++)
    {
        r->doors[i] = NULL;
    }

    r->hunterCount = 0;

    sem_init(&r->mutex, 0, 1); // init to 1;

    *room = r;
}

/*
    Function: connectRooms
    Purpose: connect two rooms
    Parameters:
           out: RoomType *r1: modify r1's door at the dir direction
           out: RoomType *r2: modify r2's door at the opposite direction of dir direction
            in: DirectionType dir: it is used to get the correct door
    Returns: DoorType
*/
DoorType *connectRooms(RoomType *r1, DirectionType dir, RoomType *r2)
{
    DoorType *d; // pointer to the door
    initDoor(&d, r1, r2);

    r1->doors[dir] = d;
    r2->doors[getOppositeDirection(dir)] = d;

    return d;
}

/*
    Function: findEnterRoom
    Purpose: find the room the hunter is entering into
    Parameters:
            in: RoomType *curRoom: it is used to find the door
            in: int dir: it is used to find the door
    Returns: RoomType
*/
RoomType *findEnterRoom(RoomType *curRoom, int dir)
{
    if (curRoom->doors[dir]->roomA == curRoom)
    {
        return curRoom->doors[dir]->roomB;
    }
    else
    {
        return curRoom->doors[dir]->roomA;
    }
}