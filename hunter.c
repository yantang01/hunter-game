#include "defs.h"

/*
    Function: initHunter
    Purpose: initializes a hunter, sets its attributes to reasonable values and assigns its name, and assigns its room to and journal to the provided parameters
    Parameters:
            in: char *name: hunter's name field
            in: RoomType *room: which room is the hunter in
            in: JournalType *journal: to connect hunter with the shared journal
           out: HunterType **hunter: The hunter to be initialized
    Returns: void
*/
void initHunter(HunterType **hunter, char *name, RoomType *room, JournalType *journal)
{
    HunterType *h = malloc(sizeof(HunterType));
    stpcpy(h->name, name);
    h->room = room;
    h->journal = journal;
    h->fear = 0;
    h->boredom = 0;

    pthread_create(&h->thread, NULL, hunterThreadFunc, (void *)h);
    *hunter = h;
}

/*
    Function: hunterThreadFunc
    Purpose: calling when hunter threads are created. It contains all the logic for each hunter when moving around
    Parameters:
            in: void *arg: this will be typecasted to HunterType pointer. Used to perform all hunter operations
    Returns: void
*/
void *hunterThreadFunc(void *arg)
{
    slowThread();

    HunterType *hunter = (HunterType *)arg;

    while (TRUE)
    {

        slowThread(); // for threads to run at the same time

        // check banished
        if (hunter->journal->banished == TRUE)
        {
            removeHunter(hunter, hunter->room);
            logAction(HUNTER_EXIT_BANISHED, hunter, NULL);
            break;
        }
        // check fear and boredom levels
        if (hunter->fear >= MAX_FEAR)
        {
            removeHunter(hunter, hunter->room);
            logAction(HUNTER_EXIT_AFRAID, hunter, NULL);
            break;
        }
        if (hunter->boredom >= MAX_BOREDOM)
        {
            removeHunter(hunter, hunter->room);
            logAction(HUNTER_EXIT_BORED, hunter, NULL);
            break;
        }

        // adjust fear and boredom levels
        hunter->boredom++;
        if (hunter->room->ghost != NULL)
        {
            hunter->fear++;
            hunter->boredom = 0;
        }

        if (hunter->journal->size < 3)
        {
            if (hunter->room->evidence != NULL)
            {
                // collect evidence
                collectEvidence(hunter, hunter->room->evidence);
            }
            else
            {
                // start moving around
                hunterMove(hunter);
            }
        }
        else
        {
            if (hunter->room->ghost != NULL)
            {
                if (hunter->room->hunterCount >= 2)
                {
                    // banish
                    hunter->journal->guess = typeFromEvidence(hunter->journal);
                    if (sayGhostName(hunter->room->ghost, hunter->journal->guess) == 1)
                    {
                        hunter->journal->banished = TRUE;
                    };
                }
            }
            else
            {
                // start moving around
                hunterMove(hunter);
            }
        }
    }
}

/*
    Function: hunterMove
    Purpose: randomly move hunters from one room to another room
    Parameters:
            in: HunterType *h: this hunter will move to a random possible room
    Returns: void
*/
void hunterMove(HunterType *h)
{

    RoomType *curRoom = h->room; // init a pointer to the room

    int dir = rand() % DIRECTION_COUNT;
    while (curRoom->doors[dir] == NULL)
    {
        // pick another door
        dir = rand() % DIRECTION_COUNT;
    }

    RoomType *enterRoom = findEnterRoom(curRoom, dir);

    // lock the door between two rooms
    sem_wait(&(curRoom->doors[dir]->mutex));

    // lock the room the hunter is exiting
    sem_wait(&(curRoom->mutex));
    // remove
    removeHunter(h, curRoom);
    // unlock the room
    sem_post(&curRoom->mutex);

    // lock the entering room
    sem_wait(&(enterRoom->mutex));
    // add
    addHunter(h, enterRoom);

    logAction(HUNTER_MOVE_TO, h, enterRoom, NULL);

    // unlock the room
    sem_post(&(enterRoom->mutex));

    // unlock the door between two rooms
    sem_post(&(curRoom->doors[dir]->mutex));
}

/*
    Function: removeHunter
    Purpose: remove the hunter from curRoom
    Parameters:
        in/out: RoomType *curRoom: we will loop through the room and find the hunter, and remove the hunter from the room
           out: HunterType *h: we will set hunter's room to NULL once we remove the hunter from the room
    Returns: void
*/
void removeHunter(HunterType *h, RoomType *curRoom)
{

    for (int i = 0; i < curRoom->hunterCount; i++)
    {
        if (curRoom->hunters[i] == h)
        {
            for (int j = i; j < curRoom->hunterCount - 1; j++)
            {
                curRoom->hunters[j] = curRoom->hunters[j + 1];
            }
            curRoom->hunterCount = curRoom->hunterCount - 1;
            h->room = NULL;
            return;
        }
    }
}

/*
    Function: addHunter
    Purpose: add the hunter to enterRoom
    Parameters:
        in/out: RoomType *enterRoom: we will directly add the hunter to this new room
           out: HunterType *h: we will set hunter's room to the new room once we add the hunter to the room
    Returns: void
*/
void addHunter(HunterType *h, RoomType *enterRoom)
{
    enterRoom->hunters[enterRoom->hunterCount] = h;
    enterRoom->hunterCount++;

    h->room = enterRoom;
}