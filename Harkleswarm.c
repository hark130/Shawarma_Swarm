#include "Harklecurse.h"
#include "Harkleswarm.h"

#ifndef HARKLESWARM_MAX_TRIES
// MACRO to limit repeated allocation attempts
#define HARKLESWARM_MAX_TRIES 3
#endif  // HARKLESWARM_MAX_TRIES

typedeft struct lineLengthCalculation
{
    int xCoord;   // Absolute x coordinate
    int yCoord;   // Absolute y coordinate
    double dist;  // Distance from the current point
} hsLineLen, *hsLineLen_ptr;

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
    double retVal = 0;       // Store the largest value of lineLengthCalculation.dist here
    tmpLineLens_ptr = NULL;  // Temporary lineLengthCalculation struct pointer
    
    // INPUT VALIDATION
    if (storedLineLens_arr && *storeLineLens_arr)
    {
        // CALCULATE
        tmpLineLens_ptr = *storeLineLens_arr;
        
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
    localShChar = shChar;             // Level of indirection for the struct graphic member
    int i = 0;                        // Iterating variable
    cartPnt newCoord = {0, 0};         // Temp struct to use as 'out' parameter for rando_unique_coordinates() 
    
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
                localShChar = i;
            }
            
            // First node
            if (!retVal)
            {
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
                // 1. Randomize some coordinates
                // TO DO: DON'T DO NOW
                // Use the struct cartesianCoordinate from Harklemath as an 'out' parameter for a new function
                // that randomizes coordinates, walks the linked list, and verifies those coordinates are unique.
                // if maxSearch is 0, searches forevert
                // bool rando_unique_coordinates(shawarma_ptr headNode, cartPnt_ptr cartCoord_ptr, int maxSearch);
                
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
                              cartPnt_ptr cartCoord_ptr, int maxSearch)
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
        else if (listLen >= ((xMax - xMin + 1)(yMax - yMin + 1)))
        {
            HARKLE_ERROR(Harkleswarm, rando_unique_coordinates, No room for more coordinates in the list);
            success = false;
        }
    }
    
    // RANDOMIZE COORDINATES
    while (!success)
    {
        // 1. Randomize coordinates
        randoX = rando_me(xMin, xMax);
        randoY = rando_me(yMin, yMax);
        
        // 2. Verify they're unique
        success = verify_unique_coordinates(randoX, randoY, headNode_ptr);
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
        }
    }
    
    // DONE
    return unique;
}


bool free_shawarma_struct(shawarma_ptr* oldStruct_ptr)
{
    return free_cartCoord_struct(oldStruct_ptr);
}


bool free_shawarma_linked_list(shawarma_ptr* oldHeadNode_ptr)
{
    return free_cardCoord_linked_list(oldHeadNode_ptr);
}
