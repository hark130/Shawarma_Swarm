#include "Harklecurse.h"        // winDetails, winDetails_ptr
#include "Harklerror.h"         // HARKLE_ERROR
#include "Harkleswarm.h"
#include <ncurses.h>            // WINDOW
#include <stdio.h>              // puts()
#include <stdbool.h>            // bool, true, false


int main(void)
{
    // LOCAL VARIABLES
    int retVal = 0;                  // Program's return value
    bool success = true;             // Set this to false if anything fails
    winDetails_ptr stdWin = NULL;    // hCurseWinDetails struct pointer for the stdscr window
    winDetails_ptr fieldWin = NULL;  // hCurseWinDetails struct pointer for the field window
    
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
        fieldWin = populate_a_winDetails_ptr(stdWin->nRows - HS_INNER_BORDER_WIDTH_V - HS_OUTER_BORDER_WIDTH_V, \
                                             stdWin->nCols - HS_INNER_BORDER_WIDTH_H - (2 * HS_OUTER_BORDER_WIDTH_H), \
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
            HARKLE_ERROR(Grand_Prix, main, wrefresh failed on fieldWin);
            success = false;
        }
    }
    // getchar();  // DEBUGGING

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
