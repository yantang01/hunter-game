#include "defs.h"

/*
    Function: initJournal
    Purpose: initialize the journal. Dynamically allocate memory for the journal and set the default values for its fields
    Parameters:
           out: JournalType **journal: The journal to be initialized
    Returns: void
*/
void initJournal(JournalType **journal)
{
    JournalType *j = malloc(sizeof(JournalType));
    j->head = NULL;
    j->size = 0;
    j->guess = UNKNOWN_GHOST;
    j->banished = FALSE;
    sem_init(&j->mutex, 0, 1); // init to 1;
    *journal = j;
}