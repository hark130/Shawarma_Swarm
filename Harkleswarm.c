#include "Harklecurse.h"
#include "Harkleswarm.h"

#ifndef HARKLESWARM_MAX_TRIES
// MACRO to limit repeated allocation attempts
#define HARKLESWARM_MAX_TRIES 3
#endif  // HARKLESWARM_MAX_TRIES


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


bool free_shawarma_struct(shawarma_ptr* oldStruct_ptr)
{
    return free_cartCoord_struct(oldStruct_ptr);
}


bool free_shawarma_linked_list(shawarma_ptr* oldHeadNode_ptr)
{
    return free_cardCoord_linked_list(oldHeadNode_ptr);
}
