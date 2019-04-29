#ifndef __HARKLESWARM__
#define __HARKLESWARM__

// Maximum moves made by one point in one iteration
#define HS_MAX_SWARM_MOVES 2
// NCURSES Windows Border Specifications
#define HS_OUTER_BORDER_WIDTH_H 4
#define HS_OUTER_BORDER_WIDTH_V 2
#define HS_INNER_BORDER_WIDTH_H 4
#define HS_INNER_BORDER_WIDTH_V 2

// Defines the struct that holds a link list of shawarma nodes
typedef struct hcCartesianCoordinate shawarma, *shawarma_ptr;
// STRUCT MEMBERS
// int absX;								// X coordinate starting at window's top left
// int absY;								// Y coordinate starting at window's top left
// int posNum;								// Uniquely number the points with this member
// char graphic;							// Character to print at this coordinate
// unsigned long hcFlags;					// Implementation-defined coordinate details
// pCoord_ptr nextPnt;                      // Next node in the linked list


/*
	PURPOSE - Allocate heap memory for one shawarma struct
	INPUT - None
	OUTPUT
		On success, pointer to a shawarma struct on the heap
		On failure, NULL
	NOTES
		It is the caller's responsibility to free the memory allocated by this function call
 */
shawarma_ptr alloc_shawarma_struct(void);


/*
	PURPOSE - Initialize a newly heap-allocated shawarma struct
	INPUT
		xVal - Absolute, from the ncurse window's top left, x coordinate of this point
		yVal - Absolute, from the ncurse window's top left, y coordinate of this point
        shNum - Identifying number of this struct
		shChar - The character to print at coordinate (xVal, yVal)
		initStatus - Initial value of the implementation-defined flags available for this point
	OUTPUT
		On success, pointer to a shawarma struct on the heap that has been populated with the given arguments
		On failure, NULL
	NOTES
		This function calls alloc_shawarma_struct() to allocate memory for the struct
		It is the caller's responsibility to free the memory allocated by this function call
 */
shawarma_ptr build_new_shawarma_struct(int xVal, int yVal, int shNum, char shChar, unsigned long shStatus);


/*
	PURPOSE - This function will add a new shawarma node (shawarma struct) to a linked list of shawarma nodes,
        if one already exists.
	INPUT
		headShawarma - shawarma struct pointer to an existing head node, if any.  If headPnt is NULL, newPnt
            will become the head node of a linked list
		newShawarma - shawarma struct pointer to a new node to add to the existing link list, if any
		newPos - Position to add newShawarma:
			0 - Add newShawarma to the end
			X - Insert newShawarma in front of node #X
	OUTPUT
		On success, pointer to the head node of a linked list of shawarma structs.
			If headShawarma is NULL, newShawarma will be returned.  Otherwise, headShawarma will be returned.
		On failure, NULL
	NOTES
		If newPos exceeds the length of the linked list, this function will just add newShawarma to the end.
        If newShawarma->posNum already exists in headShawarma's, function will fail.  This implementation
            requires that the posNum values be unique.
		Take care not to lose the pointer to your head node in case this function experiences
			an error.  PRO TIP: Use a temp variable to store this function's return value.
 */
shawarma_ptr add_shawarma_node(shawarma_ptr headShawarma, shawarma_ptr newShawarma, int newPos);


/*
    PURPOSE - Create a linked list of uniquely numbered nodes, each with a uniquely randomized x and y coordinate
    INPUT
        xMin - Lowest appropriate value for the node's x coordinate
        xMax - Largest appropriate value for the node's x coordinate
        yMin - Lowest appropriate value for the node's y coordinate
        yMax - Largest appropriate value for the node's y coordinate
        listLen - Number of nodes to add to the linked list
        shChar - The character to print for each node (If 0, will use the node's posNum member value)
        shStatus - shStatus member value for each node
    OUTPUT
        On success, pointer to the head node of a linked list containing 'listLen' number of shawarma nodes
        On failure, NULL
	NOTES
		This function calls build_new_shawarma_struct() to allocate and define the struct
		It is the caller's responsibility to free the memory allocated by this function call
 */
shawarma_ptr create_shawarma_list(int xMin, int xMax, int yMin, int yMax, int listLen, char shChar, unsigned long shStatus);


/*
    PURPOSE - Randomize a unique set of coordinates compared to the linked list provided
    INPUT
        xMin - Lowest appropriate value for the x coordinate
        xMax - Largest appropriate value for the x coordinate
        yMin - Lowest appropriate value for the y coordinate
        yMax - Largest appropriate value for the y coordinate
        headNode_ptr - Pointer to a linked list of nodes
        cartCoord_ptr - Out parameter in which to store the randomized coordinates
        maxSearch - Maximum number of time to randomize coordinates (If 0, will search forever)
    OUTPUT
        On success, true
        On failure, false
    NOTES
        Do not trust the contents of cartCoord_ptr if this function returns false
        If maxSearch is 0, this function will attempt to randomize forever until it finds a unique coordinate set!
 */
bool rando_unique_coordinates(int xMin, int xMax, int yMin, int yMax, shawarma_ptr headNode_ptr, \
                              cartPnt_ptr cartCoord_ptr, int maxSearch);


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
bool verify_unique_coordinates(int xCoord, int yCoord, shawarma_ptr headNode_ptr);


/*
	PURPOSE - Free the heap-allocated memory associated with a shawarma struct
	INPUT
		oldStruct_ptr - A pointer to a heap-allocated shawarma struct pointer
	OUTPUT
		On success, true
		On failure, false
	NOTES
		This function will attempt to zeroize and free the memory at *oldStruct_ptr.  It
			will also set the original pointer to NULL.  Call this function as
			free_shawarma_struct(&myShawarma_ptr);
		This function will recursively attempt to free any nextPnt pointers it finds.
		Also, this function will recursively attempt to free any nextPnt pointers it finds.
 */
bool free_shawarma_struct(shawarma_ptr* oldStruct_ptr);


/*
	PURPOSE - Free all of the shawarma struct nodes contained in the linked list that starts at the given head node
	INPUT
		oldHeadNode_ptr - A pointer to the head node pointer of a shawarma struct linked list
	OUTPUT
		On success, true
		On failure, false
	NOTES
		This function will attempt to zeroize, free, and NULL all of the memory of
			all the nodes in the linked list starting at *oldHeadNode_ptr.  Call 
			this function as free_shawarma_linked_list(&myHeadNode_ptr);
 */
bool free_shawarma_linked_list(shawarma_ptr* oldHeadNode_ptr);


#endif  // __HARKLESWARM__
