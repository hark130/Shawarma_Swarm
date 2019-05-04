#include "Harklecurse.h"
#include "Harklemath.h"         // dble_greater_than()
#include "Harklerror.h"         // HARKLE_ERROR
#include "Harkleswarm.h"
#include "Randoroad.h"          // rando_me()
#include <stdlib.h>             // abs()

#ifndef HARKLESWARM_MAX_TRIES
// MACRO to limit repeated search attempts
#define HARKLESWARM_MAX_TRIES 30
#endif  // HARKLESWARM_MAX_TRIES

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// LOCAL FUNCTIONS //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


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
                // printf("%d", (int)(*(tmp_ptr + i)));  // DEBUGGING
                if (0x0 != (*(tmp_ptr + i)))
                {
                    foundNull = false;
                }
            }
            // printf("\n");  // DEBUGGING
            if (false == foundNull)
            {
                numEntries++;
                tmp_ptr += ptrSize;
            }
        }
    }
    
    // DONE
    return numEntries;
}


/*
    PURPOSE - Replace the oldDist entry in coord_arr with the data from newEntry_ptr
    INPUT
        oldEntry_ptr - The old entry in coord_arr to replace
        newEntry_ptr - New data to replace the old data (see: oldEntry_ptr) with
        coord_arr - NULL-terminated array of hsLineLen struct pointers to search through
    OUTPUT
        On success, true
        On failure, false
    NOTES
        The newEntry_ptr will not overwrite any existing pointers in the coord_arr.  Rather, the data
            from newEntry_ptr will overwrite the data in array entry that matches oldEntry_ptr.
        Only the xCoord and yCoord member will be used to match oldEntry_ptr in coord_arr since
            double comparisons can be imprecise.
        This is a highly specialized function and, as flexible as it is, was never intended to be
            used outside of Harkleswarm.  Maybe, one day...
 */
bool replace_hsLineLen_array_entry(hsLineLen_ptr oldEntry_ptr, hsLineLen_ptr newEntry_ptr, hsLineLen_ptr* coord_arr)
{
    // LOCAL VARIABLES
    bool success = false;                // Set this to true if the entry is found and replaced
    hsLineLen_ptr* tmp_arr = coord_arr;  // Iterating variable

    // INPUT VALIDATION
    if (!oldEntry_ptr)
    {
        HARKLE_ERROR(Harkleswarm, replace_entry, Invalid oldEntry_ptr);
    }
    else if (!newEntry_ptr)
    {
        HARKLE_ERROR(Harkleswarm, replace_entry, Invalid newEntry_ptr);
    }
    else if (!coord_arr)
    {
        HARKLE_ERROR(Harkleswarm, replace_entry, Invalid coord_arr);
    }
    else if (NULL == *coord_arr)
    {
        HARKLE_ERROR(Harkleswarm, replace_entry, Empty coord_arr);
    }
    else
    {
        // FIND AND REPLACE
        while (*tmp_arr && false == success)
        {
            // 1. Find
            if ((*tmp_arr)->xCoord == oldEntry_ptr->xCoord && (*tmp_arr)->yCoord == oldEntry_ptr->yCoord)
            {
                // 2. Replace
                (*tmp_arr)->xCoord = newEntry_ptr->xCoord;
                (*tmp_arr)->yCoord = newEntry_ptr->yCoord;
                (*tmp_arr)->dist = newEntry_ptr->dist;
                success = true;
                break;
            }

            // Next entry
            tmp_arr++;
        }
    }

    // DONE
    return success;
}


/*
    PURPOSE - Find the longest entry in coord_arr and copy the data into outNode_ptr
    INPUT
        coord_arr - NULL-terminated array of hsLineLen struct pointers to search through
        outNode_ptr - The new storage location of the longest length hsLineLen struct
        maxPrec - Maximum floating point precision when checking "dist" value against "dist" value
    OUTPUT
        On success, true
        On failure, false
    NOTES
        Maximum precision will be used to compare "dist" member values
        This is a highly specialized function and, as flexible as it is, was never intended to be
            used outside of Harkleswarm.  Maybe, one day...
 */
bool find_longest_entry(hsLineLen_ptr* coord_arr, hsLineLen_ptr outNode_ptr, int maxPrec)
{
    // LOCAL VARIABLES
    bool success = false;                // Set this to true when outNode_ptr is written to
    hsLineLen_ptr* tmp_arr = coord_arr;  // Iterating variable

    // INPUT VALIDATION
    if (!coord_arr)
    {
        HARKLE_ERROR(Harkleswarm, find_longest_entry, Invalid coord_arr);
    }
    else if (NULL == *coord_arr)
    {
        HARKLE_ERROR(Harkleswarm, find_longest_entry, Empty coord_arr);
    }
    else if (!outNode_ptr)
    {
        HARKLE_ERROR(Harkleswarm, find_longest_entry, Invalid newEntry_ptr);
    }
    else
    {
        // FIND LONGEST DISTANCE ENTRY
        success = true;  // We're good now
        // Start with the first
        outNode_ptr->xCoord = (*tmp_arr)->xCoord;
        outNode_ptr->yCoord = (*tmp_arr)->yCoord;
        outNode_ptr->dist = (*tmp_arr)->dist;
        tmp_arr++;  // Next

        while (*tmp_arr)
        {
            if (true == dble_greater_than((*tmp_arr)->dist, outNode_ptr->dist, maxPrec))
            {
                outNode_ptr->xCoord = (*tmp_arr)->xCoord;
                outNode_ptr->yCoord = (*tmp_arr)->yCoord;
                outNode_ptr->dist = (*tmp_arr)->dist;
            }

            tmp_arr++;
        }
    }

    // DONE
    return success;
}


/*
    PURPOSE - Update the coordiantes of shawarma node 'srcNum' by moving it 'maxMoves' toward equilibrium in
        one (1) dimension
    INPUT
        curWindow - Pointer to a winDetails struct (used to determine window border points)
        headNode_ptr - Pointer to the head node of a linked list of shawarma pointers containing available points
        sourceNode_ptr - shawarma struct pointer to use as the 'origin' point to calculate distances
        maxMoves - Number of one-dimensional moves, regardless of dimension, the node may move to pursue equilibrium
        intercepts - If true, dimensional intercepts will be treated as points for the purposes of equilibrium
    OUTPUT
        On success, number of moves made (not to exceed maxMoves).  0 indicates success (and also equilibrium).
        On failure, -1
    NOTES
        This function does not perform input validation.  It assumes the calling function has already validated
            the parameter being passed in.
 */
int shwarm_one_dim(winDetails_ptr curWindow, shawarma_ptr headNode_ptr, shawarma_ptr sourceNode_ptr, int maxMoves, bool intercepts)
{
    // LOCAL VARIABLES
    int numMoves = -1;     // Number of moves made
    bool success = true;   // Prove this wrong
    double slope = 0.0;    // Store slope here
    int numClosePnts = 0;  // Number of points that find_closest_points() found
    hsLineLen point1 = { 0, 0, 0.0 };  // Out parameter for find_closest_points()
    hsLineLen point2 = { 0, 0, 0.0 };  // Out parameter for find_closest_points()
    hsLineLen midPnt = { 0, 0, 0.0 };  // Out parameter for determine_mid_point()
    hsLineLen_ptr coord_arr[] = { &point1, &point2, NULL };

    // SWARM
    // 1. Verify minimum number of points (need two to get a slope)
    if (true == success)
    {
        if (2 > get_num_cartCoord_nodes(headNode_ptr))
        {
            HARKLE_ERROR(Harkleswarm, shwarm_one_dim, Too few points to determine slope);
            success = false;
        }
    }

    // 2. Calculate slope
    if (true == success)
    {
        slope = calc_int_point_slope(headNode_ptr->absX, headNode_ptr->absY,
                                     headNode_ptr->nextPnt->absX, headNode_ptr->nextPnt->absY);
    }

    // 3. Verify line
    if (true == success)
    {
        success = verify_line(headNode_ptr, slope, HS_MAX_SLOPE_PRECISION);

        if (false == success)
        {
            HARKLE_ERROR(Harkleswarm, shwarm_one_dim, Provided points are not in a line);
        }
    }

    // 4. Consider intercepts
    if (true == success && true == intercepts)
    {
        // TO DO: DON'T DO NOW
        // Add this as a p1-1 feature
        // If I had to do it now:
        //      A. write a line_intercept() function in Harklemath.*
        //      B. Use that function to populate new shawarma nodes added to the end of the linked list
        //      C. Let find_closest_point() do what it does
        //      D. At the end of shwarm_one_dim(), remove & free those temporary "intercept" shawarma nodes from the linked list
    }

    // 5. Find closest points
    if (true == success)
    {
        numClosePnts = find_closest_points(curWindow, headNode_ptr, sourceNode_ptr, coord_arr);

        // One dimension should find two points (dim + 1)
        if (-1 == numClosePnts)
        {
            HARKLE_ERROR(Harkleswarm, shwarm_one_dim, find_closest_points encountered an error);
            success = false;
        }
        else if (0 == numClosePnts)
        {
            HARKLE_ERROR(Harkleswarm, shwarm_one_dim, find_closest_points failed);
            success = false;
        }
        else if (2 != numClosePnts)
        {
            HARKLE_ERROR(Harkleswarm, shwarm_one_dim, find_closest_points found an invalid number of points);
            success = false;
        }
    }

    // 6. Calculate center
    if (true == success)
    {
        success = determine_mid_point(&point1, &point2, &midPnt, 0);

        if (false == success)
        {
            HARKLE_ERROR(Harkleswarm, shwarm_one_dim, determine_mid_point failed);
        }
    }

    // 7. Move the point closer
    if (true == success)
    {
        numMoves = move_shawarma(sourceNode_ptr, &midPnt, maxMoves);

        if (0 > numMoves)
        {
            HARKLE_ERROR(Harkleswarm, shwarm_one_dim, move_shawarma failed);
            success = false;
        }
    }

    // DONE
    return numMoves;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// GLOBAL FUNCTIONS /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


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
    else
    {
        HARKLE_ERROR(Harkleswarm, build_new_shawarma_struct, build_new_cartCoord_struct failed);
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


int find_closest_points(winDetails_ptr curWindow, shawarma_ptr headNode_ptr, shawarma_ptr sourceNode_ptr, hsLineLen_ptr* coord_arr)
{
    // LOCAL VARIABLES
    int numPoints = -1;                // -1 on error, number of points calculated on success
    bool success = true;               // Set this to false if anything fails
    int i = 0;                         // Iterating variable for loops
    shawarma_ptr tmpNode_ptr = NULL;   // Iterating variable for the 'in' linked list
    hsLineLen_ptr *tmpArr_ptr = NULL;  // Iterating variable for the 'out' array
    int tmpNum = 0;                    // Temporary number variable
    // Use this as a temp 'out' parameter for calc_hsLineLen_contents()
    hsLineLen localLineLen = { 0, 0, 0.0 };
    // Use this to store the current value of the furthest point in coord_arr
    hsLineLen biggestLineLen = { 0, 0, 0.0 };
    
    // INPUT VALIDATION
    if (!curWindow)
    {
        HARKLE_ERROR(Harkleswarm, find_closest_points, Invalid curWindow);
        success = false;
    }
    else if (!headNode_ptr)
    {
        HARKLE_ERROR(Harkleswarm, find_closest_points, Invalid headNode_ptr);
        success = false;
    }
    else if (!sourceNode_ptr)
    {
        HARKLE_ERROR(Harkleswarm, find_closest_points, Invalid sourceNode_ptr);
        success = false;
    }
    else if (!coord_arr || NULL == *coord_arr)
    {
        HARKLE_ERROR(Harkleswarm, find_closest_points, Invalid coord_arr);
        success = false;
    }
    else
    {
        numPoints = 0;  // No errors
    }
    
    // FIND POINTS
    if (-1 < numPoints && true == success)
    {
        // 1. Determine intended dimensions
        numPoints = count_entries(coord_arr, sizeof(hsLineLen_ptr));
        // printf("Found %d entries!", numPoints);  // DEBUGGING

        // 2. Verify minimum number of nodes exist
        if (numPoints > (get_num_cartCoord_nodes(headNode_ptr) - 1))
        {
            HARKLE_ERROR(Harkleswarm, find_closest_points, Not enough coord_arr entries);
            numPoints = -1;
        }
        else
        {
            // 3. Find nearest points given dimensions
            tmpNum = 0;
            tmpNode_ptr = headNode_ptr;
            tmpArr_ptr = coord_arr;

            // A. Fill the array with the first "numPoints" number of node entries
            tmpNum = 0;  // Reset temp variable
            while (tmpNode_ptr && tmpNum < numPoints && true == success)
            {
                if (tmpNode_ptr != sourceNode_ptr)
                {
                    success = calc_hsLineLen_contents(sourceNode_ptr, tmpNode_ptr, *tmpArr_ptr);

                    if (false == success)
                    {
                        HARKLE_ERROR(Harkleswarm, find_closest_points, calc_hsLineLen_contents failed);
                        numPoints = 0;
                    }
                    else
                    {
                        tmpNum++;
                        // New biggest distance?
                        if (true == dble_greater_than((*tmpArr_ptr)->dist, biggestLineLen.dist, DBL_PRECISION))
                        {
                            biggestLineLen.xCoord = (*tmpArr_ptr)->xCoord;
                            biggestLineLen.yCoord = (*tmpArr_ptr)->yCoord;
                            biggestLineLen.dist = (*tmpArr_ptr)->dist;
                        }
                        tmpArr_ptr++;  // NOW increment
                    }
                }

                tmpNode_ptr = tmpNode_ptr->nextPnt;
            }
            tmpArr_ptr = NULL;  // Don't use this anymore

            // B. Check the rest of the linked list for closer entries
            while (tmpNode_ptr && true == success)
            {
                if (tmpNode_ptr != sourceNode_ptr)
                {
                    // Calculate length
                    success = calc_hsLineLen_contents(sourceNode_ptr, tmpNode_ptr, &localLineLen);

                    if (false == success)
                    {
                        HARKLE_ERROR(Harkleswarm, find_closest_points, calc_hsLineLen_contents failed);
                        numPoints = 0;
                    }
                    else
                    {
                        if (true == dble_less_than(localLineLen.dist, biggestLineLen.dist, DBL_PRECISION))
                        {
                            // Replace entry in coord_arr with localLineLen data
                            success = replace_hsLineLen_array_entry(&biggestLineLen, &localLineLen, coord_arr);

                            if (false == success)
                            {
                                HARKLE_ERROR(Harkleswarm, find_closest_points, replace_hsLineLen_array_entry failed);
                                numPoints = 0;
                            }
                            else
                            {
                                // Replace "biggest" line length local variable with the "new biggest" line length
                                success = find_longest_entry(coord_arr, &biggestLineLen, DBL_PRECISION);

                                if (false == success)
                                {
                                    HARKLE_ERROR(Harkleswarm, find_closest_points, find_longest_entry failed);
                                    numPoints = 0;
                                }
                            }
                        }
                    }
                }

                tmpNode_ptr = tmpNode_ptr->nextPnt;
            }
        }
    }
    
    // DONE
    return numPoints;
}


int shwarm_it(winDetails_ptr curWindow, shawarma_ptr headNode_ptr, int maxMoves, int srcNum, int numDim, bool intercepts)
{
    // LOCAL VARIABLES
    int numMoves = -1;                   // Store the return value from helper functions here
    shawarma_ptr sourceNode_ptr = NULL;  // Node pointer for 'srcNum'

    // INPUT VALIDATION
    if (!curWindow)
    {
        HARKLE_ERROR(Harkleswarm, shwarm_it, Invalid curWindow);
    }
    else if (!headNode_ptr)
    {
        HARKLE_ERROR(Harkleswarm, shwarm_it, Invalid headNode_ptr);
    }
    else if (srcNum < 1)
    {
        HARKLE_ERROR(Harkleswarm, shwarm_it, Invalid srcNum);
    }
    else if (numDim < 1)
    {
        HARKLE_ERROR(Harkleswarm, shwarm_it, Invalid numDim);
    }
    else if (maxMoves < 1)
    {
        HARKLE_ERROR(Harkleswarm, shwarm_it, Invalid maxMoves);
    }
    else
    {
        // SWARM
        // 1. Find the source node pointer
        sourceNode_ptr = get_pos_num(headNode_ptr, srcNum);

        if (!sourceNode_ptr)
        {
            HARKLE_ERROR(Harkleswarm, shwarm_it, get_pos_num failed);
        }
        else
        {
            // 2. Call the dimensionally appropriate helper function
            switch (numDim)
            {
                case 1:
                    numMoves = shwarm_one_dim(curWindow, headNode_ptr, sourceNode_ptr, maxMoves, intercepts);
                    break;
                default:
                    HARKLE_ERROR(Harkleswarm, shwarm_it, Unsupported dimension);
                    break;
            }
        }
    }

    // DONE
    return numMoves;
}


bool verify_line(shawarma_ptr headNode_ptr, double slope, int maxPrec)
{
    // LOCAL VARIABLES
    bool straightLine = false;                // Set this to true if everything checks out
    shawarma_ptr tmpNode_ptr = headNode_ptr;  // Iterating node pointer
    double tmpSlope = 0.0;                    // Store calls to slope functions here

    // INPUT VALIDATION
    if (!headNode_ptr)
    {
        HARKLE_ERROR(Harkleswarm, verify_line, Invalid headNode_ptr);
    }
    else if (maxPrec < 1)
    {
        HARKLE_ERROR(Harkleswarm, verify_line, Invalid level of floating point precision);
    }
    else if (2 > get_num_cartCoord_nodes(headNode_ptr))
    {
        HARKLE_ERROR(Harkleswarm, verify_line, Too few points to determine slope);
    }
    else
    {
        // ITERATE POINTS
        straightLine = true;  // Prove this wrong

        while (tmpNode_ptr->nextPnt)
        {
            tmpSlope = calc_int_point_slope(tmpNode_ptr->absX, tmpNode_ptr->absY,
                                            tmpNode_ptr->nextPnt->absX, tmpNode_ptr->nextPnt->absY);

            if (false == dble_equal_to(tmpSlope, slope, maxPrec))
            {
                straightLine = false;
                break;
            }
            else
            {
                tmpNode_ptr = tmpNode_ptr->nextPnt;  // Keep checking
            }
        }
    }

    // DONE
    return straightLine;
}


bool calc_hsLineLen_contents(shawarma_ptr sourceNode_ptr, shawarma_ptr destNode_ptr, hsLineLen_ptr outParam_ptr)
{
    // LOCAL VARIABLES
    bool success = false;  // Prove this wrong

    // INPUT VALIDATION
    if (!sourceNode_ptr)
    {
        HARKLE_ERROR(Harkleswarm, calc_hsLineLen_contents, Invalid sourceNode_ptr);
    }
    else if (!destNode_ptr)
    {
        HARKLE_ERROR(Harkleswarm, calc_hsLineLen_contents, Invalid destNode_ptr);
    }
    else if (!outParam_ptr)
    {
        HARKLE_ERROR(Harkleswarm, calc_hsLineLen_contents, Invalid outParam_ptr);
    }
    else if (sourceNode_ptr == destNode_ptr)
    {
        HARKLE_ERROR(Harkleswarm, calc_hsLineLen_contents, Source and destination nodes may not be the same);
    }
    else if (sourceNode_ptr->absX == destNode_ptr->absX && sourceNode_ptr->absY == destNode_ptr->absY)
    {
        HARKLE_ERROR(Harkleswarm, calc_hsLineLen_contents, Source and destination coordinates may not be the same);
    }
    else
    {
        // POPULATE STRUCT
        // 1. Get distance
        outParam_ptr->dist = calc_int_point_dist(sourceNode_ptr->absX, sourceNode_ptr->absY,
                                                 destNode_ptr->absX, destNode_ptr->absY);

        // 2. Assign struct member values
        outParam_ptr->xCoord = destNode_ptr->absX;
        outParam_ptr->yCoord = destNode_ptr->absY;

        // 3. Success!
        success = true;
    }

    // DONE
    return success;
}


int move_shawarma(shawarma_ptr node_ptr, hsLineLen_ptr dstCoord_ptr, int maxMoves)
{
    // LOCAL VARIABLES
    int numMoves = -1;

    // INPUT VALIDATION
    if (!node_ptr)
    {
        HARKLE_ERROR(Harkleswarm, move_shawarma, Invalid node_ptr);
    }
    else if (!dstCoord_ptr)
    {
        HARKLE_ERROR(Harkleswarm, move_shawarma, Invalid dstCoord_ptr);
    }
    else if (1 > maxMoves)
    {
        HARKLE_ERROR(Harkleswarm, move_shawarma, Invalid number of moves);
    }
    else
    {
        // MOVE IT
        numMoves = 0;

        while (numMoves < maxMoves)
        {
            // Update distance
            dstCoord_ptr->dist = calc_int_point_dist(node_ptr->absX, node_ptr->absY,
                                                     dstCoord_ptr->xCoord, dstCoord_ptr->yCoord);

            // Ensure points don't "consume" each other
            if (1 == dstCoord_ptr->dist)
            {
                // Check for the "diagonally adjacent point" edge case
                if (dstCoord_ptr->xCoord == node_ptr->absX
                    || dstCoord_ptr->yCoord == node_ptr->absY)
                {
                    break;  // Done.
                }
            }
            // Move it
            if (abs(dstCoord_ptr->xCoord - node_ptr->absX) >= (dstCoord_ptr->yCoord - node_ptr->absY))
            {
                // X coordinate
                if (node_ptr->absX > dstCoord_ptr->xCoord)
                {
                    node_ptr->absX--;
                }
                else
                {
                    node_ptr->absX++;
                }
            }
            else
            {
                // Y coordinate
                if (node_ptr->absY > dstCoord_ptr->yCoord)
                {
                    node_ptr->absY--;
                }
                else
                {
                    node_ptr->absY++;
                }
            }
            numMoves++;  // If you've made it here, you moved one coordinate
        }
    }

    // DONE
    return numMoves;
}


bool free_shawarma_struct(shawarma_ptr* oldStruct_ptr)
{
    return free_cartCoord_struct(oldStruct_ptr);
}


bool free_shawarma_linked_list(shawarma_ptr* oldHeadNode_ptr)
{
    return free_cardCoord_linked_list(oldHeadNode_ptr);
}
