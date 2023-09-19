#include "defs.h"

/*
    Function: initEvidence
    Purpose: Dynamically allocate memory for the evidence, and set the default values for its fields.
    Parameters:
            in: EvidenceCategoryType type: evidence's type field
            in: RoomType *origin: room to be connected with the evidence
           out: EvidenceType **evidence: The evidence to be initialized
    Returns: void
*/
void initEvidence(EvidenceType **evidence, EvidenceCategoryType type, RoomType *origin)
{

    EvidenceType *ev = malloc(sizeof(EvidenceType));

    ev->type = type;
    ev->founder = NULL;
    ev->origin = origin;

    *evidence = ev;
}

/*
    Function: printEvidence
    Purpose: helper function to the printResults function. This is to print all evidence hunters collected on the screen
    Parameters:
            in: JournalType *j: it is used to get access to evidence data in order to print them to the screen
    Returns: void
*/
void printEvidence(JournalType *j)
{
    printf("The hunters collected the following evidence: \n");
    EvidenceNodeType *cur = j->head;
    while (cur != NULL)
    {
        printf("---\n");

        char ev[MAX_STR];
        evidenceToString(cur->evidence->type, ev);

        printf("Evidence Type: %s\n", ev);
        printf("Founder: %s\n", cur->evidence->founder->name);
        printf("Origin: %s\n", cur->evidence->origin->name);
        cur = cur->next;
    }
    printf("---\n");
}

/*
    Function: collectEvidence
    Purpose: hunter collects evidence
    Parameters:
        in/out: HunterType *h: get the shared journal/room from hunter variable; we will also edit hunter fields once collecting the evidence
            in: EvidenceType *ev: it is used to check if this evidence type is already in the journal
    Returns: void
*/
void collectEvidence(HunterType *h, EvidenceType *ev)
{
    JournalType *journal = h->journal; // shared
    sem_wait(&(journal->mutex));       // lock journal

    if (containsEvidence(journal, ev) == TRUE)
    {
        logAction(HUNTER_EVIDENCE_DISCARD, h, ev->type, h->room);
    }
    else
    {
        addEvidence(journal, ev, h);
        logAction(HUNTER_EVIDENCE_COLLECT, h, ev->type, h->room);
    }

    h->room->evidence = NULL;

    sem_post(&(journal->mutex)); // unlock journal
}

/*
    Function: containsEvidence
    Purpose: check if journal contains evidence type
    Parameters:
            in: JournalType *journal: loop through the journal and check evidence one by one
            in: EvidenceType *ev: it is used to check if this evidence type is already in the journal
    Returns: "boolean" indicating whether evidence is in the journal
*/
int containsEvidence(JournalType *journal, EvidenceType *ev)
{

    if (journal->size == 0)
    {
        return FALSE;
    }

    EvidenceNodeType *curNode = journal->head;

    while (curNode != NULL)
    {
        if (curNode->evidence->type == ev->type)
        {
            return TRUE; // contain
        }
        curNode = curNode->next;
    }
    return FALSE;
}

/*
    Function: addEvidence
    Purpose: add evidence to the journal
    Parameters:
           out: JournalType *journal: to be added one more evidence
           out: HunterType *h: change the evidence founder to the hunter
            in: EvidenceType *ev: to be added to the journal
    Returns: void
*/
void addEvidence(JournalType *journal, EvidenceType *ev, HunterType *h)
{
    ev->founder = h;

    EvidenceNodeType *newNode;
    newNode = malloc(sizeof(EvidenceNodeType));
    newNode->evidence = ev;
    newNode->next = NULL;

    if (journal->size == 0)
    {
        journal->head = newNode;
    }
    else
    {
        EvidenceNodeType *cur = journal->head;
        EvidenceNodeType *pre;

        while (cur != NULL)
        {
            pre = cur;
            cur = cur->next;
        }
        pre->next = newNode;
    }

    h->boredom = 0;
    journal->size++;
}
