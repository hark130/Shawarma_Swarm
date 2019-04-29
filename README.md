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

[ ] Harkle_Library

    [ ] New repo
    [ ] Move up Harklecurse
    [ ] Move up Harklerror
    [ ] Move up Harklemath
    [ ] Move up and rename Rando
[ ] Harkleswarm library

    [X] Document reuse of struct hcCartesianCoordinate from Harklecurse.h (Harkleswarm.h)
    [X] Macro for a point's speed (number of moves it makes at once) (HS_MAX_SWARM_MOVES in Harkleswarm.h)
    [ ] Add facility to determine swarm equilibrium
    [ ] Add facility to interject a "shawarma" (before and after equilibrium)
[X] Main binary

    [X] Start shwarm_it.c
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
