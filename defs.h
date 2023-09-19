#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_HUNTERS 4
#define MAX_FEAR 100
#define MAX_BOREDOM 100
#define MAX_STR 64
#define MAX_ROOMS 16
#define MAX_DOORS 16
#define MIN_WAIT 1000
#define MAX_WAIT 5000
#define GHOST_WAIT 10000
#define GHOST_EV_PROB 30
#define LOGGING_ENABLED 1

#define TRUE 2
#define FALSE 0

/*  By putting DIRECTION_COUNT at the end, it defaults to
    the number of things in the enum! It's a helpful trick for
    defining the size.
*/
enum Direction
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    DIRECTION_COUNT
};

/* By using bits like this, we can actually construct a GhostCategory from its evidence! */
enum EvidenceCategory
{
    EMF = 1 << 0,          // 0b0001
    FINGERPRINTS = 1 << 1, // 0b0010
    TEMPERATURE = 1 << 2,  // 0b0100
    SOUND = 1 << 3,        // 0b1000
    UNKNOWN_EVIDENCE = 0xF // 0b1111
};

enum GhostCategory
{
    BANSHEE = EMF | TEMPERATURE | SOUND,            // 0b1101
    BULLIES = EMF | FINGERPRINTS | SOUND,           // 0b1011
    PHANTOM = FINGERPRINTS | TEMPERATURE | SOUND,   // 0b1110
    POLTERGEIST = EMF | FINGERPRINTS | TEMPERATURE, // 0b0111
    UNKNOWN_GHOST = 0xFF                            // 0b11111111
};

/*  When working with lots of connected structures, forward declaring is often
    best to avoid circular dependencies. This just does the typedefs for us
    so that we can refer to the types before they've been defined!

    Also, the Ghost uses the full struct/enum names, so you can change the typedef
    to use any naming convention you want, including camel_case. When working in VS Code,
    "F2" while selecting the Type name will let you rename it everywhere it's used.
*/
typedef struct Room RoomType;
typedef struct Building BuildingType;
typedef struct Door DoorType;
typedef struct Hunter HunterType;
typedef struct Ghost GhostType;
typedef struct Journal JournalType;
typedef struct Evidence EvidenceType;
typedef struct EvidenceNode EvidenceNodeType;
typedef enum Direction DirectionType;
typedef enum EvidenceCategory EvidenceCategoryType;
typedef enum GhostCategory GhostCategoryType;

struct Ghost
{
    RoomType *room;
    GhostCategoryType type;
    int boredom;
    int fear;
    pthread_t thread;
};

struct Hunter
{
    char name[MAX_STR];
    RoomType *room;
    JournalType *journal;
    int fear;
    int boredom;
    pthread_t thread;
};

struct Evidence
{
    EvidenceCategoryType type;
    HunterType *founder;
    RoomType *origin;
}; // EvidenceType

struct EvidenceNode
{
    EvidenceType *evidence;
    EvidenceNodeType *next;
}; // EvidenceNodeType

struct Journal
{
    EvidenceNodeType *head;
    int size;
    GhostCategoryType guess;
    char banished;
    sem_t mutex;
};

struct Room
{
    char name[MAX_STR];
    GhostType *ghost;
    EvidenceType *evidence;
    HunterType *hunters[MAX_HUNTERS];
    DoorType *doors[DIRECTION_COUNT];
    int hunterCount;
    sem_t mutex;
};

struct Door
{
    RoomType *roomA;
    RoomType *roomB;
    sem_t mutex;
};

struct Building
{
    JournalType *journal;
    GhostType *ghost;
    HunterType *hunters[MAX_HUNTERS];
    RoomType *rooms[MAX_ROOMS];
    DoorType *doors[MAX_DOORS];
    int doorCount;
    int roomCount;
    int hunterCount;
};

void initRoom(RoomType **room, char *name);
void initDoor(DoorType **door, RoomType *roomA, RoomType *roomB);
void initHunter(HunterType **hunter, char *name, RoomType *room, JournalType *journal);
void initBuilding(BuildingType **building);
void initEvidence(EvidenceType **evidence, EvidenceCategoryType type, RoomType *origin);
void initJournal(JournalType **journal);

DoorType *connectRooms(RoomType *, DirectionType, RoomType *);
void populateBuilding(BuildingType *building);

/* Utility Functions: Feel free to modify */
GhostCategoryType typeFromEvidence(JournalType *evidence);
void ghostToString(GhostCategoryType type, char *str);
void evidenceToString(EvidenceCategoryType evidence, char *buffer);
DirectionType getOppositeDirection(DirectionType dir);
void slowThread();

/* Library Declarations: Do not make any modifications to the below definitions */
/* Ghost Functions Implemented in ghost.o */
void initGhost(GhostType **ghost, RoomType *room);
int sayGhostName(GhostType *ghost, GhostCategoryType category);
void cleanupGhost(GhostType *ghost);

/* Logging Functions & Struct*/
typedef enum LogAction
{
    HUNTER = 1 << 15,
    GHOST = 1 << 14,
    MOVE = 1 << 13,
    EXIT = 1 << 12,
    EVIDENCE = 1 << 11,
    HAUNTING = 1 << 10,
    ROOM = 1 << 9,

    HUNTER_MOVE_TO = HUNTER | MOVE | ROOM,
    HUNTER_EXIT_AFRAID = HUNTER | EXIT | 1,
    HUNTER_EXIT_BORED = HUNTER | EXIT | 2,
    HUNTER_EXIT_BANISHED = HUNTER | EXIT | 3,
    HUNTER_EVIDENCE_COLLECT = HUNTER | EVIDENCE | ROOM | 1,
    HUNTER_EVIDENCE_DISCARD = HUNTER | EVIDENCE | ROOM | 2,
    GHOST_MOVE_TO = GHOST | MOVE | ROOM,
    GHOST_EXIT_AFRAID = GHOST | EXIT | 1,
    GHOST_EXIT_BORED = GHOST | EXIT | 2,
    GHOST_HAUNTING = GHOST | HAUNTING | ROOM,
} LogActionType;

void logAction(enum LogAction action, ...);

// added functions
void *hunterThreadFunc(void *arg);
void hunterMove(HunterType *h);
void removeHunter(HunterType *h, RoomType *curRoom);
void addHunter(HunterType *h, RoomType *enterRoom);
RoomType *findEnterRoom(RoomType *curRoom, int dir);
void cleanupBuilding(BuildingType *b);
void collectEvidence(HunterType *h, EvidenceType *ev);
int containsEvidence(JournalType *journal, EvidenceType *ev);
void addEvidence(JournalType *journal, EvidenceType *ev, HunterType *h);
void printEvidence(JournalType *j);
void printResults(BuildingType *b);