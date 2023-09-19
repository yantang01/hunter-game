#include "defs.h"

/*
    Function: populateBuilding
    Purpose: Populates a building with rooms and doors
    Parameters:
        in-out: BuildingType* building: The building to populate
    Returns: void
*/
void populateBuilding(BuildingType *building)
{
    /*
     * Requires that the building has been initialized with initBuilding()
     * Requires that initRoom and connectRooms have been implemented
     * You are allowed to modify this function, but the room connections
     * must remain the same. The overall map MUST be the same, with the same names
     * and the same connections. Failure to do so will result in major
     * deductions.
     */

    // Room names array
    char *room_names[] = {
        "Van", "Hallway N", "Hallway S", "Master Bedroom", "Boy's Bedroom", "Bathroom",
        "Kitchen", "Living Room", "Garage", "Utility Room"};
    int num_rooms = sizeof(room_names) / sizeof(room_names[0]);

    // Initialize rooms: It should be strcpy/strlcpy/strncpy the name
    for (int i = 0; i < num_rooms; i++)
    {
        initRoom(&(building->rooms[i]), room_names[i]);
    }

    // Connect rooms. The first room, in the direction, you will find the second room.
    // connectRooms() returns a pointer to the door that connects the two rooms.
    building->doors[0] = connectRooms(building->rooms[0], SOUTH, building->rooms[1]);
    building->doors[1] = connectRooms(building->rooms[1], EAST, building->rooms[4]);
    building->doors[2] = connectRooms(building->rooms[1], WEST, building->rooms[3]);
    building->doors[3] = connectRooms(building->rooms[1], SOUTH, building->rooms[2]);
    building->doors[4] = connectRooms(building->rooms[2], EAST, building->rooms[5]);
    building->doors[5] = connectRooms(building->rooms[2], SOUTH, building->rooms[6]);
    building->doors[6] = connectRooms(building->rooms[6], SOUTH, building->rooms[7]);
    building->doors[7] = connectRooms(building->rooms[6], WEST, building->rooms[8]);
    building->doors[8] = connectRooms(building->rooms[8], NORTH, building->rooms[9]);

    building->roomCount = num_rooms;
    building->doorCount = 9;
}

/*
    Function: initBuilding
    Purpose: initialize the building; Dynamically allocate memory for the building and set the default values for its fields
    Parameters:
           out: BuildingType **building: The building to be initialized
    Returns: void
*/
void initBuilding(BuildingType **building)
{
    BuildingType *b = malloc(sizeof(BuildingType)); // return pointer's address

    // set 3 count values to 0
    b->doorCount = 0;
    b->roomCount = 0;
    b->hunterCount = 0;

    // set 3 pointers
    b->journal = NULL;
    b->ghost = NULL;

    // array of pointers
    // init to NULL
    for (int i = 0; i < MAX_HUNTERS; i++)
    {
        b->hunters[i] = NULL;
    }
    for (int i = 0; i < MAX_ROOMS; i++)
    {
        b->rooms[i] = NULL;
    }
    for (int i = 0; i < MAX_DOORS; i++)
    {
        b->doors[i] = NULL;
    }

    *building = b;
}

/*
    Function: cleanupBuilding
    Purpose: cleanup all allocated memory at the end of the program
    Parameters:
            in/out: BuildingType *b: it is used to get access to all data and to be cleared after this function is completed
    Returns: void
*/
void cleanupBuilding(BuildingType *b)
{
    // clean all evidence
    EvidenceNodeType *currNode;
    EvidenceNodeType *nextNode;
    currNode = b->journal->head;
    while (currNode != NULL)
    {
        nextNode = currNode->next;
        free(currNode->evidence);
        currNode = nextNode;
    }

    currNode = b->journal->head;
    while (currNode != NULL)
    {
        nextNode = currNode->next;
        free(currNode);
        currNode = nextNode;
    }

    free(b->journal);

    // clean hunters
    for (int i = 0; i < b->hunterCount; i++)
    {
        free(b->hunters[i]);
    }

    // clean rooms
    for (int i = 0; i < b->roomCount; i++)
    {
        free(b->rooms[i]->evidence);
        free(b->rooms[i]);
    }

    // clean doors
    for (int i = 0; i < b->doorCount; i++)
    {
        free(b->doors[i]);
    }

    // clean building
    free(b);
}

/*
    Function: printResults
    Purpose: display required results to the terminal after all threads finish running
    Parameters:
            in: BuildingType *b: it is used to get access to specific data in order to print results to the screen
    Returns: void
*/
void printResults(BuildingType *b)
{
    printf("\n\nResults: \n");
    for (int i = 0; i < b->hunterCount; i++)
    {
        if (b->hunters[i]->boredom == MAX_BOREDOM)
        {
            printf("%s was too bored and ran away!\n", b->hunters[i]->name);
        }
        else if (b->hunters[i]->fear == MAX_FEAR)
        {
            printf("%s was too afraid and ran away!\n", b->hunters[i]->name);
        }
        else
        {
            printf("%s escaped the ghost!\n", b->hunters[i]->name);
        }
    }
    printf("\n");

    printEvidence(b->journal);

    char ghostType[MAX_STR];

    ghostToString(b->journal->guess, ghostType);

    if (b->journal->banished == TRUE)
    {
        printf("The hunters guessed correctly! The ghost was a %s.\n", ghostType);
        printf("The hunters banished the ghost!\n");
    }
    else
    {
        printf("The hunters did not have a guess! They didn't know it was a %s.\n", ghostType);
        printf("The hunters did not banish the ghost!\n");
    }
}
