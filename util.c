#include "defs.h"

/*
    Function: slowThread
    Purpose: Sleeps the thread for a random amount of time, as specified
             in the assignment specification. Used at the top of the thread function.
    Returns: void
*/
void slowThread()
{
    usleep(MIN_WAIT + rand() % MAX_WAIT);
}

/*
    Function: typeFromEvidence
    Purpose: Returns the GhostCategoryType of a journal
    Parameters:
        in:     JournalType* evidence: The journal containing all evidence collected
    Returns: GhostCategoryType - the type of ghost,

*/
GhostCategoryType typeFromEvidence(JournalType *evidence)
{

    GhostCategoryType type = 0;
    int count = 0;
    EvidenceNodeType *current = evidence->head;

    // We're using bitwise OR to combine the evidence types
    // And our GhostCategoryType is setup to align with the
    // bits correctly
    while (current != NULL)
    {
        type |= current->evidence->type;
        current = current->next;
        count++;
    }

    if (count != 3)
        return UNKNOWN_GHOST;

    return type;
}

/*
    Function: evidenceToString
    Purpose: Converts an EvidenceCategoryType to a string
    Parameters:
        in:     EvidenceCategoryType evidence: The evidence to convert
        out:    char* buffer: The buffer to write the string to
    Returns: void
*/
void evidenceToString(EvidenceCategoryType evidence, char *buffer)
{

    /*  strncpy, and setting the last char to \0 OR strlcpy
        would be safer than strcpy, but because we know the
        buffer size is going to be set to MAX_STR and we know
        the literal strings are less, we can use strcpy
        for this simple example.
    */
    switch (evidence)
    {
    case EMF:
        strcpy(buffer, "EMF");
        break;
    case FINGERPRINTS:
        strcpy(buffer, "Fingerprints");
        break;
    case TEMPERATURE:
        strcpy(buffer, "Temperature");
        break;
    case SOUND:
        strcpy(buffer, "Sound");
        break;
    default:
        strcpy(buffer, "Unknown");
        break;
    }
}

/*
 * Function: getOppositeDirection
 * Returns the opposite direction of the given direction from the Direction enum
 * Parameters:
 *   Input:  DirectionType direction - the direction to get the opposite of
 *   Output: none
 * Returns: DirectionType - the opposite direction, or DIRECTION_COUNT if the direction is invalid
 */
DirectionType getOppositeDirection(DirectionType direction)
{
    switch (direction)
    {
    case NORTH:
        return SOUTH;
    case SOUTH:
        return NORTH;
    case EAST:
        return WEST;
    case WEST:
        return EAST;
    default:
        return DIRECTION_COUNT;
    }
}

/* * Function: ghostToString
 * Converts a ghost to a string
 * Parameters:
 *   Input:  GhostEnumType ghost - the ghost to convert
 *   Output: char* str - the string to convert to
 * Returns: void
 */
void ghostToString(GhostCategoryType ghost, char *str)
{
    switch (ghost)
    {
    case POLTERGEIST:
        strcpy(str, "Poltergeist");
        break;
    case BANSHEE:
        strcpy(str, "Banshee");
        break;
    case PHANTOM:
        strcpy(str, "Phantom");
        break;
    case BULLIES:
        strcpy(str, "Bullies");
        break;
    default:
        strcpy(str, "Unknown");
        break;
    }
}
