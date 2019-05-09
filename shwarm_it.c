#include "Harklecurse.h"        // winDetails, winDetails_ptr
#include "Harklerror.h"         // HARKLE_ERROR
#include "Harkleswarm.h"
#include <ncurses.h>            // WINDOW
#include <stdio.h>              // puts()
#include <stdbool.h>            // bool, true, false
#include <unistd.h>             // sleep()

#define NUM_STARTING_POINTS 3  // Number of initial shawarma
#define SLEEPY_SHAWARMA 1      // Number of seconds to sleep between shwarm iterations

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
    int tmpNumMoves = 0;               // Capture error codes before incrementing numMoves
    int numMoves = 0;                  // Number of total moves made each 'cycle'
    int i = 0;                         // Iterating variable
    // Current number of points
    int curNumPoints = NUM_STARTING_POINTS;
    
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
                                            curNumPoints, 0, 0);

        if (!headNode_ptr)
        {
            HARKLE_ERROR(Shwarm_It, main, create_shawarma_list failed);
            success = false;
        }
    }

    // 2. Print swarm
    if (true == success)
    {
/* DEBUGGING BEGIN */
// headNode_ptr->absX = fieldWin->leftC + 1 - HS_OUTER_BORDER_WIDTH_H;
// headNode_ptr->absY = fieldWin->upperR + 1 - HS_OUTER_BORDER_WIDTH_V;
shawarma_ptr tmpNode_ptr = headNode_ptr->nextPnt;
// int offset = 10;
while (tmpNode_ptr)
{
    tmpNode_ptr->absX = headNode_ptr->absX;  // Vertical line
    // tmpNode_ptr->absY = headNode_ptr->absY;  // Horizontal line
    // Slanted line
    // tmpNode_ptr->absX = headNode_ptr->absY + offset;  // Slanted line
    // tmpNode_ptr->absY = headNode_ptr->absY + offset;  // Slanted line
    // offset += 20;  // Slanted line
    tmpNode_ptr = tmpNode_ptr->nextPnt;
}
/* DEBUGGING END */
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
    // getchar();  // DEBUGGING

    // START SWARMING
    if (true == success)
    {
        do
        {
            numMoves = 0;  // Reset counting variable

            for (i = 1; i <= curNumPoints; i++)
            {
                // tmpNumMoves = shwarm_it(fieldWin, headNode_ptr, HS_MAX_SWARM_MOVES, i, 1, false);
                tmpNumMoves = shwarm_it(fieldWin, headNode_ptr, HS_MAX_SWARM_MOVES, i, 1, true);
                // printf("Node %d was moved %d times.\n", i, tmpNumMoves);  // DEBUGGING

                if (0 > tmpNumMoves)
                {
                    HARKLE_ERROR(Shwarm_It, main, shwarm_it failed);
                    success = false;
                    getchar();  // DEBUGGING
                    break;
                }
                else
                {
                    numMoves += tmpNumMoves;
                }
            }

            // Update field window
            // clear();  // Clear the screen
            // if (OK != wrefresh(stdWin->win_ptr))  // Print it on the real screen
            // {
            //     HARKLE_ERROR(Shwarm_It, main, wrefresh failed on stdWin);
            //     success = false;
            // }
            // else if (OK != wrefresh(fieldWin->win_ptr))  // Print it on the real screen
            // {
            //     HARKLE_ERROR(Shwarm_It, main, wrefresh failed on fieldWin);
            //     success = false;
            // }
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
            else
            {
                // 𝄞 Why are you sleepy? ♬
                // ♩ Sleepy thread ♪
                // ♭ Thread is sleepy ♫
                // 𝄫 Sleepy thread ♫
                sleep(SLEEPY_SHAWARMA);
            }
        }
        while (numMoves && true == success);  // Keep swarming until equilibrium is reached
    }

    // END THE SWARM
    if (true == success)
    {
        // Print "Press any key to end race"
        if (OK != mvwaddstr(stdWin->win_ptr, 1, 1, "Press any key to end the swarm"))
        {
            HARKLE_ERROR(Shwarm_It, main, mvwaddstr failed);
            success = false;
        }
        getch();  // Wait for the user to press a key
        clear();  // Clear the screen
    }

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
    clear();  // Clear the screen
    // Restore tty modes, reset cursor location, and resets the terminal into the proper non-visual mode
    endwin();  // End curses mode
    
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