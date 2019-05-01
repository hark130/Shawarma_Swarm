#include "Harklecurse.h"        // winDetails, winDetails_ptr
#include "Harklerror.h"         // HARKLE_ERROR
#include "Harkleswarm.h"
#include <ncurses.h>            // WINDOW
#include <stdio.h>              // puts()
#include <stdbool.h>            // bool, true, false

#define NUM_STARTING_POINTS 3  // Number of initial shawarma

void print_debug_info(winDetails_ptr stdWin, winDetails_ptr fieldWin, shawarma_ptr headNode_ptr);

void print_node_info(shawarma_ptr node_ptr);

int main(void)
{
    // LOCAL VARIABLES
    int retVal = 0;                    // Program's return value
    bool success = true;               // Set this to false if anything fails
    winDetails_ptr stdWin = NULL;      // hCurseWinDetails struct pointer for the stdscr window
    winDetails_ptr fieldWin = NULL;    // hCurseWinDetails struct pointer for the field window
    shawarma_ptr headNode_ptr = NULL;  // Head node of the linked list of shawarmas
    
    // SETUP THE WINDOWS
    if (true == success)
    {
        // 1. Setup ncurses
        initscr();  // Start curses mode
        cbreak();  // Disables line buffering and erase/kill character-processing
        // raw();  // Line buffering disabled
        noecho();  // Disable echo

        // 2. Main Window (stdscr)
        stdWin = build_a_winDetails_ptr();

        if (!stdWin)
        {
            HARKLE_ERROR(Shwarm_It, main, build_a_winDetails_ptr failed);
            success = false;
        }
        else
        {
            // Populate Main Window struct
            stdWin->win_ptr = stdscr;
            stdWin->upperR = 0;
            stdWin->leftC = 0;
            getmaxyx(stdscr, stdWin->nRows, stdWin->nCols);  // Determine the maximum dimensions
            if (ERR == stdWin->nRows || ERR == stdWin->nCols)
            {
                HARKLE_ERROR(Shwarm_It, main, getmaxyx failed);
                success = false;
            }
            else
            {
                // printf("Max rows: %d\nMax cols: %d\n", stdWin->nRows, stdWin->nCols);  // DEBUGGING
                retVal = wborder(stdWin->win_ptr, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, \
                                 ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

                if (OK != retVal)
                {
                    HARKLE_ERROR(Shwarm_It, main, wborder failed);
                    success = false;
                }
            }
        }
    }

    // 3. Field Window
    if (true == success)
    {
        fieldWin = populate_a_winDetails_ptr(stdWin->nRows - (2 * HS_OUTER_BORDER_WIDTH_V), \
                                             stdWin->nCols - (2 * HS_OUTER_BORDER_WIDTH_H), \
                                             HS_OUTER_BORDER_WIDTH_V, HS_OUTER_BORDER_WIDTH_H);

        if (!fieldWin)
        {
            HARKLE_ERROR(Shwarm_It, main, populate_a_winDetails_ptr failed);
            success = false;
        }
        else
        {
            retVal = wborder(fieldWin->win_ptr, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, \
                             ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

            if (OK != retVal)
            {
                HARKLE_ERROR(Shwarm_It, main, wborder failed);
                success = false;
            }
        }
    }

    // 4. Print the Window
    if (true == success)
    {
        if (OK != wrefresh(fieldWin->win_ptr))  // Print it on the real screen
        {
            HARKLE_ERROR(Shwarm_It, main, wrefresh failed on fieldWin);
            success = false;
        }
    }
    // getchar();  // DEBUGGING

    // SETUP SWARM
    // 1. Create swarm
    if (true == success)
    {
        headNode_ptr = create_shawarma_list(fieldWin->leftC + 1 - HS_OUTER_BORDER_WIDTH_H,
                                            fieldWin->nCols - 2,
                                            fieldWin->upperR + 1 - HS_OUTER_BORDER_WIDTH_V,
                                            fieldWin->nRows - 2,
                                            NUM_STARTING_POINTS, 0, 0);

        if (!headNode_ptr)
        {
            HARKLE_ERROR(Shwarm_It, main, create_shawarma_list failed);
            success = false;
        }
    }

    // 2. Print swarm
    if (true == success)
    {
        // Update field window
        if (false == print_plot_list(fieldWin->win_ptr, headNode_ptr))
        {
            HARKLE_ERROR(Shwarm_It, main, print_plot_list failed);
            success = false;
            print_debug_info(stdWin, fieldWin, headNode_ptr);
        }
        else if (OK != wrefresh(fieldWin->win_ptr))  // Print it on the real screen
        {
            HARKLE_ERROR(Shwarm_It, main, wrefresh failed on fieldWin);
            success = false;
        }
    }
    getchar();  // DEBUGGING

	// CLEAN UP
	// ncurses Windows
    // 1. fieldWin
    if (fieldWin)
    {
        // Delete the field window
        if (OK != kill_a_window(&(fieldWin->win_ptr)))
        {
            HARKLE_ERROR(Shwarm_It, main, kill_a_window failed);
        }
        // Free the struct
        if (false == kill_a_winDetails_ptr(&fieldWin))
        {
            HARKLE_ERROR(Shwarm_It, main, kill_a_winDetails_ptr failed);
        }
    }
	// 2. stdWin
	if (stdWin)
	{
		// Delete the main window
        if (OK != kill_a_window(&(stdWin->win_ptr)))
		{
			HARKLE_ERROR(Shwarm_It, main, kill_a_window failed);
		}
		// Free the struct
		if (false == kill_a_winDetails_ptr(&stdWin))
		{
			HARKLE_ERROR(Shwarm_It, main, kill_a_winDetails_ptr failed);	
		}
	}
    
    // DONE
    if (false == success && 0 == retVal)
    {
        retVal = -1;   
    }
    
    return retVal;
}


void print_debug_info(winDetails_ptr stdWin, winDetails_ptr fieldWin, shawarma_ptr headNode_ptr)
{
    // LOCAL VARIABLES
    shawarma_ptr tmp_ptr = headNode_ptr;  // Walk the linked list

    if (stdWin)
    {
        fprintf(stderr, "Max rows: %d\nMax cols: %d\n", stdWin->nRows, stdWin->nCols);
    }
    else
    {
        fprintf(stderr, "stdwin is NULL\n");
    }
    fprintf(stderr, "\n");

    if (fieldWin)
    {
        fprintf(stderr, "Field rows: %d\nField cols: %d\n", fieldWin->nRows, fieldWin->nCols);
    }
    else
    {
        fprintf(stderr, "fieldWin is NULL\n");
    }
    fprintf(stderr, "\n");

    if (headNode_ptr)
    {
        while (tmp_ptr)
        {
            print_node_info(tmp_ptr);
            tmp_ptr = tmp_ptr->nextPnt;
        }
    }
    else
    {
        fprintf(stderr, "headNode_ptr is NULL\n");
    }

    // DONE
    return;
}


void print_node_info(shawarma_ptr node_ptr)
{
    if (node_ptr)
    {
        fprintf(stderr, "\tAddress: %p\n", node_ptr);
        fprintf(stderr, "\tX Coord: %d\n", node_ptr->absX);
        fprintf(stderr, "\tY Coord: %d\n", node_ptr->absY);
        fprintf(stderr, "\tPos Num: %d\n", node_ptr->posNum);
        fprintf(stderr, "\tFlags:   %lu\n", node_ptr->hcFlags);
    }
    else
    {
        fprintf(stderr, "node_ptr is NULL\n");
    }
    fprintf(stderr, "\n");

    // DONE
    return;
}


// STRUCT MEMBERS
// int absX;                                // X coordinate starting at window's top left
// int absY;                                // Y coordinate starting at window's top left
// int posNum;                              // Uniquely number the points with this member
// char graphic;                            // Character to print at this coordinate
// unsigned long hcFlags;                   // Implementation-defined coordinate details
// pCoord_ptr nextPnt;                      // Next node in the linked list