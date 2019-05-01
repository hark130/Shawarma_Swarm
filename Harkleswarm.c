#include "Harklecurse.h"
#include "Harklemath.h"         // dble_greater_than()
#include "Harklerror.h"         // HARKLE_ERROR
#include "Harkleswarm.h"
#include "Randoroad.h"          // rando_me()

#ifndef HARKLESWARM_MAX_TRIES
// MACRO to limit repeated search attempts
#define HARKLESWARM_MAX_TRIES 30
#endif  // HARKLESWARM_MAX_TRIES

/* LOCAL FUNCTIONS */


/*
    PURPOSE - Find the longest distance from an array of lineLengthCalculation structs
    INPUT
        storedLineLens_arr - Pointer to a NULL terminated array of lineLengthCalculation struct pointers
    OUTPUT
        On success, the largest distance member found in storedLineLens_arr
        On failure, 0
    NOTES
        This is a highly specialized function and, as flexible as it is, was never intended to be
            used outside of Harkleswarm
 */
double find_max_len_from_array(hsLineLen_ptr* storedLineLens_arr)
{
    // LOCAL VARIABLES
    double retVal = 0;                     // Store the largest value of lineLengthCalculation.dist here
    hsLineLen_ptr tmpLineLens_ptr = NULL;  // Temporary lineLengthCalculation struct pointer
    
    // INPUT VALIDATION
    if (storedLineLens_arr && *storedLineLens_arr)
    {
        // CALCULATE
        tmpLineLens_ptr = *storedLineLens_arr;
        
        while (tmpLineLens_ptr)
        {
            if (true == dble_greater_than(tmpLineLens_ptr->dist, retVal, DBL_PRECISION))
            {
                retVal = tmpLineLens_ptr->dist;
            }
            
            tmpLineLens_ptr++;
        }
    }

    // DONE
    return retVal;
}


/*
    PURPOSE - Count the number of entries in a NULL-terminated array
    INPUT
        some_arr - Pointer to a NULL terminated array of pointers, each pointer of size ptrSize
    OUTPUT
        On success, the number of non-NULL entries in the array
        On failure, 0
        On error, -1
    NOTES
        This is a highly specialized function and, as flexible as it is, was never intended to be
            used outside of Harkleswarm.  Maybe, one day...
 */
int count_entries(void* some_arr, size_t ptrSize)
{
    // LOCAL VARIABLES
    int numEntries = -1;     // -1 on error, 0 on failure, number of entries on success
    char *tmp_ptr = NULL;    // Use this to operate on individual bytes of the void*
    bool foundNull = false;  // Set this to true if a NULL pointer is found
    size_t i = 0;            // Iterating variable
    
    // INPUT VALIDATION
    if (some_arr)
    {
        numEntries = 0;
        tmp_ptr = (char *)some_arr;

        // COUNT ENTRIES
        while (false == foundNull)
        {
            foundNull = true;  // Prove this wrong
            for (i = 0; i < ptrSize; i++)
            {
                if (0x0 != (*(tmp_ptr + i)))
                {
                    foundNull = false;
                }
            }
            if (false == foundNull)
            {
                numEntries++;
            }
        }
    }
    
    // DONE
    return numEntries;
}


/* GLOBAL FUNCTIONS */


shawarma_ptr alloc_shawarma_struct(void)
{
    return allocate_cartCoord_struct();
}


shawarma_ptr build_new_shawarma_struct(int xVal, int yVal, int shNum, char shChar, unsigned long shStatus)
{
    // LOCAL VARIABLES
    shawarma_ptr retVal = NULL;
    
    // ALLOCATE & BUILD
    retVal = build_new_cartCoord_struct(xVal, yVal, shChar, shStatus);
    
    // UPDATE
    if (retVal)
    {
        retVal->posNum = shNum;
    }
    
    // DONE
    return retVal;
}


shawarma_ptr add_shawarma_node(shawarma_ptr headShawarma, shawarma_ptr newShawarma, int newPos)
{
    return add_cartCoord_node(headShawarma, newShawarma, newPos);
}


shawarma_ptr create_shawarma_list(int xMin, int xMax, int yMin, int yMax, int listLen, char shChar, unsigned long shStatus)
{
    // LOCAL VARIABLES
    shawarma_ptr retVal = NULL;       // Store the head node to the new linked list here
    shawarma_ptr tmp_ptr = NULL;      // Temp storage for newly built child nodes
    shawarma_ptr tmpHead_ptr = NULL;  // Temp storage for newly built child nodes
    bool success = true;              // Set this to false if anything fails
    char localShChar = shChar;        // Level of indirection for the struct graphic member
    int i = 0;                        // Iterating variable
    hsLineLen newCoord = {0, 0, 0};   // Temp struct to use as 'out' parameter for rando_unique_coordinates() 
    
    // INPUT VALIDATION
    if (xMin > xMax)
    {
        HARKLE_ERROR(Harkleswarm, create_shawarma_list, Invalid x coordinate min and max);
        success = false;
    }
    else if (yMin > yMax)
    {
        HARKLE_ERROR(Harkleswarm, create_shawarma_list, Invalid y coordinate min and max);
        success = false;
    }
    else if (listLen < 1)
    {
        HARKLE_ERROR(Harkleswarm, create_shawarma_list, Invalid list length);
        success = false;
    }
    
    // ALLOCATE NODES
    if (true == success)
    {
        for (i = 1; i <= listLen; i++)
        {
            // Determine character value
            if (0 == shChar)
            {
                localShChar = i + 48;
            }
            
            // First node
            if (!retVal)
            {
                // puts("Making head node");  // DEBUGGING
                newCoord.xCoord = rando_me(xMin, xMax);
                newCoord.yCoord = rando_me(yMin, yMax);
                retVal = build_new_shawarma_struct(newCoord.xCoord, newCoord.yCoord, i, localShChar, shStatus);
                if (!retVal)
                {
                    HARKLE_ERROR(Harkleswarm, create_shawarma_list, build_new_shawarma_struct failed);
                    success = false;
                }
            }
            // Additional node
            else
            {
                // puts("Making child node");  // DEBUGGING
                // 1. Randomize some coordinates
                success = rando_unique_coordinates(xMin, xMax, yMin, yMax, retVal, &newCoord, HARKLESWARM_MAX_TRIES);
                // puts("Got unique coordinates");  // DEBUGGING
                if (false == success)
                {
                    HARKLE_ERROR(Harkleswarm, create_shawarma_list, rando_unique_coordinates failed);
                }
                else
                {
                    // 2. Create a node
                    tmp_ptr = build_new_shawarma_struct(newCoord.xCoord, newCoord.yCoord, i, localShChar, shStatus);
                    if (!tmp_ptr)
                    {
                        HARKLE_ERROR(Harkleswarm, create_shawarma_list, build_new_shawarma_struct failed);
                        success = false;
                    }
                    else
                    {
                        // 3. Add the node to the list
                        tmpHead_ptr = add_shawarma_node(retVal, tmp_ptr, listLen + 1);

                        if (tmpHead_ptr != retVal && retVal)
                        {
                            HARKLE_ERROR(Harkleswarm, create_shawarma_list, add_shawarma_node failed);
                            success = false;
                        }
                    }
                }
            }
            
            if (true == success)
            {
                // Reset temp variables because everything succeeded
                localShChar = shChar;
                newCoord.xCoord = 0;
                newCoord.yCoord = 0;
                tmp_ptr = NULL;  // Not a memory leak because it's in the linked list
                tmpHead_ptr = NULL;  // Not a memory leak because the head node pointer is also in retVal
            }
        }
    }
    
    // CLEAN UP
    if (false == success)
    {
        if (retVal)
        {
            // tmpHead_ptr
            if (tmpHead_ptr == retVal)
            {
                tmpHead_ptr = NULL;
            }
            // retVal
            if (false == free_shawarma_linked_list(&retVal))
            {
                HARKLE_ERROR(Harkleswarm, create_shawarma_list, free_shawarma_linked_list failed);
            }
            // tmp_ptr
            if (tmp_ptr)
            {
                if (false == free_shawarma_struct(&tmp_ptr))
                {
                    HARKLE_ERROR(Harkleswarm, create_shawarma_list, free_shawarma_struct failed);
                }
            }
        }
    }
    
    // DONE
    return retVal;
}


bool rando_unique_coordinates(int xMin, int xMax, int yMin, int yMax, shawarma_ptr headNode_ptr, \
                              hsLineLen_ptr cartCoord_ptr, int maxSearch)
{
    // LOCAL VARIABLES
    bool success = false;  // Set this to true if unique coordinates are found
    int listLen = 0;       // Store the list length here
    int searchNum = 0;     // Current search number
    int randoX = 0;        // Store randomized x coordinates here
    int randoY = 0;        // Store randomized y coordinates here
    
    // INPUT VALIDATION
    if (xMin > xMax)
    {
        HARKLE_ERROR(Harkleswarm, rando_unique_coordinates, Invalid x coordinate min and max);
        success = false;
    }
    else if (yMin > yMax)
    {
        HARKLE_ERROR(Harkleswarm, rando_unique_coordinates, Invalid y coordinate min and max);
        success = false;
    }
    else if (!headNode_ptr)
    {
        HARKLE_ERROR(Harkleswarm, rando_unique_coordinates, Invalid headNode_ptr);
        success = false;
    }
    else if (!cartCoord_ptr)
    {
        HARKLE_ERROR(Harkleswarm, rando_unique_coordinates, Invalid cartCoord_ptr);
        success = false;
    }
    else if (0 > maxSearch)
    {
        HARKLE_ERROR(Harkleswarm, rando_unique_coordinates, Invalid maxSearch value);
        success = false;
    }
    else
    {
        listLen = get_num_cartCoord_nodes(headNode_ptr);
        
        if (0 >= listLen)
        {
            HARKLE_ERROR(Harkleswarm, rando_unique_coordinates, get_num_cartCoord_nodes failed);
            success = false;
        }
        else if (listLen >= ((xMax - xMin + 1) * (yMax - yMin + 1)))
        {
            HARKLE_ERROR(Harkleswarm, rando_unique_coordinates, No room for more coordinates in the list);
            success = false;
        }
    }
    
    // RANDOMIZE COORDINATES
    while (1)
    {
        // 1. Randomize coordinates
        // puts("Randomizing...");  // DEBUGGING
        randoX = rando_me(xMin, xMax);
        randoY = rando_me(yMin, yMax);
        // puts("Done.");  // DEBUGGING
        
        // 2. Verify they're unique
        // puts("Verify coordinates are unique...");  // DEBUGGING
        success = verify_unique_coordinates(randoX, randoY, headNode_ptr);
        // puts("Done.");  // DEBUGGING
        if (true == success)
        {
            cartCoord_ptr->xCoord = randoX;
            cartCoord_ptr->yCoord = randoY;
            break;  // Found one.  Stop looking.
        }
        else
        {
            searchNum++;
        }

        // 3. Check search count
        if (maxSearch > 0 && searchNum >= maxSearch)
        {
            break;  // Didn't find one but we've hit the maximum search iterations
        }
    }
    
    // DONE
    return success;
}


/*
    PURPOSE - Verify a set of coordinates are unique within a linked list of shawarma nodes
    INPUT
        xCoord - Compare this to each absX
        yCoord - Compare this to each absY
        headNode_ptr - Pointer to a linked list of nodes
    OUTPUT
        On success, true if the coordinates are unique and false if the coordinates already exist
        On failure, false
 */
bool verify_unique_coordinates(int xCoord, int yCoord, shawarma_ptr headNode_ptr)
{
    // LOCAL VARIABLES
    bool unique = true;                   // Set this to false if a coordinate match is found
    shawarma_ptr tmp_ptr = headNode_ptr;  // Iterating node pointer variable
    
    // INPUT VALIDATION
    if (!headNode_ptr)
    {
        HARKLE_ERROR(Harkleswarm, verify_unique_coordinates, Invalid headNode_ptr);
        unique = false;
    }
    else
    {
        while(tmp_ptr && true == unique)
        {
            if (xCoord == tmp_ptr->absX && yCoord == tmp_ptr->absY)
            {
                unique = false;
            }

            tmp_ptr = tmp_ptr->nextPnt;
        }
    }
    
    // DONE
    return unique;
}


int find_closest_points(winDetails_ptr curWindow, shawarma_ptr headNode_ptr, int srcNum, hsLineLen_ptr* coord_arr)
{
    // LOCAL VARIABLES
    int numPoints = -1;               // -1 on error, number of points calculated on success
    shawarma_ptr srcShawarma = NULL;  // Source point from which to base all calculations
    
    // INPUT VALIDATION
    if (!curWindow)
    {
        HARKLE_ERROR(Harkleswarm, find_closest_points, Invalid curWindow);
    }
    else if (!headNode_ptr)
    {
        HARKLE_ERROR(Harkleswarm, find_closest_points, Invalid headNode_ptr);
    }
    else if (!coord_arr || NULL == *coord_arr)
    {
        HARKLE_ERROR(Harkleswarm, find_closest_points, Invalid coord_arr);
    }
    else
    {
        numPoints = 0;  // No errors
    }
    
    // FIND POINTS
    if (-1 < numPoints)
    {
        // 1. Find the pointer to the source coordinate
        srcShawarma = get_pos_num(headNode_ptr, srcNum);

        // 2. Determine intended dimensions
        numPoints = count_entries(coord_arr, sizeof(hsLineLen_ptr));
        // printf("Found %d entries!", numPoints);  // DEBUGGING

        // 3. Find nearest points given dimensions
        // TO DO: DON'T DO NOW
    }
    
    // DONE
    return numPoints;
}


bool free_shawarma_struct(shawarma_ptr* oldStruct_ptr)
{
    return free_cartCoord_struct(oldStruct_ptr);
}


bool free_shawarma_linked_list(shawarma_ptr* oldHeadNode_ptr)
{
    return free_cardCoord_linked_list(oldHeadNode_ptr);
}
