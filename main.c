#include "defs.h"

int main()
{
    // Seed the random number generator
    srand(time(NULL));

    // Your code below here!
    // init building
    BuildingType *b;
    initBuilding(&b);

    // load data to building
    populateBuilding(b);
    // init a journal and add it to the building
    JournalType *j; // shared
    initJournal(&j);
    b->journal = j;

    GhostType *g;
    b->ghost = g;
    int rand_room = rand() % b->roomCount;
    initGhost(&g, b->rooms[0]);
    j->guess = g->type;

    // init hunter and create thread
    HunterType *h1;
    HunterType *h2;
    HunterType *h3;
    HunterType *h4;

    rand_room = rand() % b->roomCount;
    initHunter(&h1, "Hunter 1", b->rooms[rand_room], j);
    b->hunters[b->hunterCount] = h1;
    b->hunterCount++;

    rand_room = rand() % b->roomCount;
    initHunter(&h2, "Hunter 2", b->rooms[rand_room], j);
    b->hunters[b->hunterCount] = h2;
    b->hunterCount++;

    rand_room = rand() % b->roomCount;
    initHunter(&h3, "Hunter 3", b->rooms[rand_room], j);
    b->hunters[b->hunterCount] = h3;
    b->hunterCount++;

    rand_room = rand() % b->roomCount;
    initHunter(&h4, "Hunter 4", b->rooms[rand_room], j);
    b->hunters[b->hunterCount] = h4;
    b->hunterCount++;

    // join all threads
    pthread_join(g->thread, NULL);
    pthread_join(h1->thread, NULL);
    pthread_join(h2->thread, NULL);
    pthread_join(h3->thread, NULL);
    pthread_join(h4->thread, NULL);

    printResults(b);

    // clean all memory
    cleanupGhost(g);
    cleanupBuilding(b);

    return 0;
}