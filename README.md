# Shawarma_Swarm
Messing around with swarms and swarming algorithms

## Goal
Make 'swarms' self-organize in a variety of dimensions.

## Phases

| Phase | Dimension | Multi-Threaded |
| :---: | :-------: | :------------: |
| 1     | 1         | No             |
| 2     | 2         | No             |
| 3     | 2         | Yes            |
| 4     | 3?        | Yes            |

## Branching Convention

* Name branches based on phase:
    * Phase 1 work branches into `p1`
    * Further phase-based branching off of `p1`: `p1-1`, `p1-2`, etc
* Branch off of, and merge into, development
* When development is verified functional, merge into master

## To Do

[X] Harkle_Library

    [X] New repo [Harkle_Library](https://github.com/hark130/Harkle_Library)
    [X] Move up Harklecurse
    [X] Move up Harklerror
    [X] Move up Harklemath
    [X] Move up and rename Rando
[ ] Harkleswarm library

    [X] Document reuse of struct hcCartesianCoordinate from Harklecurse.h (Harkleswarm.h)
    [X] Macro for a point's speed (number of moves it makes at once) (HS_MAX_SWARM_MOVES in Harkleswarm.h)
    [X] Add facility to determine swarm equilibrium
    [ ] Add facility to inject a "shawarma" (before and after equilibrium)
    [ ] Modify the library to 'hide' functions callers don't need (e.g., shwarm_one_dim())
    [ ] Refactor library functions with duplicate code (e.g., find_closest_one_dim_points())
    [X] Consider moving 'local' library function definitions to the end and adding prototypes at the top
    [ ] Consider refactoring calc_int_point_slope() to better handle vertical lines. That or write a check_vert_line() function.
    [ ] Refactor calculate_line_intercepts() to be less copy/pasty.  Maybe add struct pointers to an array and iterate over it.
    [ ] Add a check (helper function call) that verifies destination coordinates are unique. (Maybe refactor move_shawarma() with a new head node param or make that call prior to clear_this_coord() in all the shwarm_*_dim() functions)
    [ ] Add a check (helper function call) that verifies destination coordinates are inside the window. (Maybe refactor move_shawarma() with a new winDetails_ptr param or make that call prior to clear_this_coord() in all the shwarm_*_dim() functions)
    [ ] Can I avoid duplicate triangle calculations by "checking off" 3-point calculations in a 3-dimensional array?  Is this the ravings of a madman?!  Would this slow down verify_triangle() even worse?!
    [ ] Could I: 1. Calculate all distances while... 2. Simultaneously finding the three closest points then... 3. Determine if those three closest points form an encapsulating traingle?  Would this be a time-saver?
    [ ] Could I store the three points encapsulating a given point to avoid iterating over the "universe" of points?!  Is there ever a situation in which a "rogue planet" (see: point) could swoop in and form a new, closer triangle with a given source point that was following three other triangle points?  How massive a refactor would this be?!  GOOD NEWS: Harkleswarm has a level of indirection between shawarma structs and hcCartesianCoordinate structs.  BAD NEWS: I'd have to reimplement many of the support functions I am utilizing and adjust existing functionality to utilize new struct members.  WORSE NEWS: I'd lost functionality where a point find a *new* closest triangle and shifts.
    [ ] BUG? There seem to be an awful lot of failed calls to calculate_triangle_area() by verify_triangle().  Is there a way to find a common thread and avoid that function call?  Would that speed things up for me?  (see: Error_Output)
    [ ] Should I dial up optimization in my Makefile recipe?  (Should I buy a new desktop?!)
    [ ] Consider adding an option/interface to calculate, move, and print one point at a time.  It may be more inefficient but I think it would be better to watch.
    [ ] Get the cursor out of the way.  Leaving the cursor by the last point is distracting and detracts from the aesthetic I'm looking for.
    [ ] BUG? Two dimensional shwarming without intercepts hangs(?) on 'border' points.
[X] Main binary

    [X] Start shwarm_it.c
    [ ] Add CLI arguments that control the number of dimensions, number of points, method of swarming, intercepts, trails, etc.
[X] Makefile

    [X] Setup macros(?) for external libraries
    [X] Write production code recipe
[ ] Wiki

    [ ] Installation instructions (e.g., ncurses)
    [ ] Build instructions (e.g., make)
    [ ] Execution instructions (e.g., ./shwarm_it.exe)
    [X] Copy/paste git commands
[X] Repo Branches

    [X] Create development
    [X] Configure development
    [X] Create/document branch naming process
