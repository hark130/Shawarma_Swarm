#ifndef __HARKLESWARM__
#define __HARKLESWARM__

#include "Harklemath.h"

// Maximum moves made by one point in one iteration
#define HS_MAX_SWARM_MOVES 2
#define HS_MAX_SLOPE_PRECISION 5
// NCURSES Windows Border Specifications
#define HS_OUTER_BORDER_WIDTH_H 4
#define HS_OUTER_BORDER_WIDTH_V 2
#define HS_INNER_BORDER_WIDTH_H 4
#define HS_INNER_BORDER_WIDTH_V 2

// Defines the struct that holds a link list of shawarma nodes
typedef struct hcCartesianCoordinate shawarma, *shawarma_ptr;
// STRUCT MEMBERS
// int absX;                                // X coordinate starting at window's top left
// int absY;                                // Y coordinate starting at window's top left
// int posNum;                                // Uniquely number the points with this member
// char graphic;                            // Character to print at this coordinate
// unsigned long hcFlags;                    // Implementation-defined coordinate details
// pCoord_ptr nextPnt;                      // Next node in the linked list

typedef struct hmLineLengthCalculation hsLineLen, *hsLineLen_ptr;

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
                              hsLineLen_ptr cartCoord_ptr, int maxSearch);


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
    PURPOSE - Fill the given array with coordinates and distances representing the closest points to srcNum
    INPUT
        curWindow - Pointer to a winDetails struct (used to determine window border points)
        headNode_ptr - Pointer to the head node of a linked list of shawarma pointers containing available points
        sourceNode_ptr - shawarma struct pointer to use as the 'origin' point to calculate distances
        coord_arr - NULL-terminated array of hsLineLen struct pointers to use as 'out' parameters
    OUTPUT
        On success, number of hsLineLen struct pointers populate with points and distances
        On failure, 0
        On error, -1
    NOTES
        The return value of this function should equal the number of hsLineLen pointers provided in the array.
        The number of available pointers in the array will be used to determine the desired dimension.  Arranging
            a point in one dimension requires two points, two dimensions requires three points (see: triangulation), etc.            
 */
int find_closest_points(winDetails_ptr curWindow, shawarma_ptr headNode_ptr, shawarma_ptr sourceNode_ptr, hsLineLen_ptr* coord_arr);


/*
    PURPOSE - Update the coordiantes of shawarma node 'srcNum' by moving it 'maxMoves' toward equilibrium in
        'numDim' dimensions
    INPUT
        curWindow - Pointer to a winDetails struct (used to determine window border points)
        headNode_ptr - Pointer to the head node of a linked list of shawarma pointers containing available points
        maxMoves - Number of one-dimensional moves, regardless of dimension, the node may move to pursue equilibrium
        srcNum - shawarma struct posNum value to use as the 'origin' point to calculate distances
        numDim - Number of dimensions in which to organize the swarm
        intercepts - If true, dimensional intercepts will be treated as points for the purposes of equilibrium
    OUTPUT
        On success, number of moves made (not to exceed maxMoves).  0 indicates success (and also equilibrium).
        On failure, -1
    NOTES
        This function is really a 'wrapper' around dimensionally-specific helper functions
 */
int shwarm_it(winDetails_ptr curWindow, shawarma_ptr headNode_ptr, int maxMoves, int srcNum, int numDim, bool intercepts);


/*
    PURPOSE - Verify that all points represented in the given link list conform to the given slope
    INPUT
        headNode_ptr - Pointer to the head node of a linked list of shawarma pointers containing available points
        slope - Verify that all points in headNode_ptr's linked list form this slope
        maxPrec - Maximum floating point precision when checking calculated slope against 'slope'
    OUTPUT
        If all nodes conform to the given slope, true
        Otherwise, false
        On error, false
 */
bool verify_line(shawarma_ptr headNode_ptr, double slope, int maxPrec);


/*
    PURPOSE - Populate a hsLineLen struct with the points from destNode_ptr along with the distance between
        destNode_ptr and sourceNode_ptr
    INPUT
        sourceNode_ptr - shawarma struct pointer to use as the 'origin' point to calculate distances
        destNode_ptr - Pointer to a node to use as the actual coordinates and caculate the distance from
            sourceNode_ptr
        outParam_ptr - The hsLineLen struct pointer to hold the coordinates from destNode_ptr and the distance
            between sourceNode_ptr and destNode_ptr
    OUTPUT
        On success, true
        On failure or error, false
    NOTES
        If sourceNode_ptr is the same as destNode_ptr, function will error
        If the source coordinates are the same as the destination coordinates, function will error
 */
bool calc_hsLineLen_contents(shawarma_ptr sourceNode_ptr, shawarma_ptr destNode_ptr, hsLineLen_ptr outParam_ptr);


/*
    PURPOSE - Update the coordinates of node_ptr so that it is "maxMoves" closer to dstCoord_ptr's coordiantes
    INPUT
        node_ptr - The coordinates to update
        dstCoord_ptr - The destination coordinates
        maxMoves - Maximum number of moves to make
    OUTPUT
        On success, number of moves made (0 counts as success)
        On failure, -1
    NOTES
        node_ptr will not be moved if distance is 1 unless neither axis of node_ptr and dstCoord_ptr match
 */
int move_shawarma(shawarma_ptr node_ptr, hsLineLen_ptr dstCoord_ptr, int maxMoves);


/*
    PURPOSE - Create a linked list of shawarma nodes holding the graph intercepts for sourceNode_ptr, a node
        in headNode_ptr's linked list, within curWindow given the desired dimensions in numDim.
    INPUT
        curWindow - Pointer to a winDetails struct (used to determine window border points)
        headNode_ptr - Pointer to the head node of a linked list of shawarma pointers containing available points
        sourceNode_ptr - shawarma struct pointer to use as the 'origin' point to calculate intercepts
        outHeadNode_ptr - 'Out' parameter to store the head node of the newly allocate linked list of intercepts
        numDim - Number of dimensions to calculate intercepts for
    OUTPUT
        On success, true (and outHeadNode_ptr holds a pointer to a linked list holding the intercepts)
        On failure, false
    NOTES
        The linked list is necessary to determine intercepts for one dimension because it will have to
            calculate the slope of the line to establish the intercepts.  The linked list is not necessary
            for two dimensions.
        The nodes in outHeadNode_ptr will be 'zeroized' to the greatest degree possible.  Specifically,
            posNum and graphic will both be set to 0x0.  This should aid in free()ing the nodes when
            calculations are complete.
        The caller of this function is responsible for free()ing any nodes allocated to outHeadNode_ptr.
 */
bool calculate_intercepts(winDetails_ptr curWindow, shawarma_ptr headNode_ptr, shawarma_ptr sourceNode_ptr,
                          shawarma_ptr *outHeadNode_ptr, int numDim);


/*
    PURPOSE - Calculate the line intercepts for sourceNode_ptr within curWindow and store the coordinates
        inside the linked list found at outHeadNode_ptr
    INPUT
        curWindow - Pointer to a winDetails struct (used to determine window border points)
        sourceNode_ptr - shawarma struct pointer to use as the 'origin' point to calculate intercepts
        outHeadNode_ptr - Linked list of 'out' parameter nodes in which to store the line intercepts
        slope - Slope used to determine the intercepts from sourceNode_ptr
    OUTPUT
        On success, true
        On failure, false
    NOTES
        On success, outHeadNode_ptr's linked list will be popuplated with the intersections of the
            line defined by sourceNode_ptr (and it's slope) and curWindow

 */
bool calculate_line_intercepts(winDetails_ptr curWindow, shawarma_ptr sourceNode_ptr,
                               shawarma_ptr outHeadNode_ptr, double slope);


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


void print_debug_info(winDetails_ptr stdWin, winDetails_ptr fieldWin, shawarma_ptr headNode_ptr);


void print_node_info(shawarma_ptr node_ptr);


#endif  // __HARKLESWARM__
