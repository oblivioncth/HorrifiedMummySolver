# HorrifiedMummySolver

HorrifiedMummySolver is a command line utility that determines the least number of moves required to solve the Mummy monster's tablet puzzle from the board game "Horrified"

See https://boardgamegeek.com/boardgame/282524/horrified

Additionally, it will display the first set of moves/move path found that completes the puzzle in that number of steps. While the use of all this tool's features would somewhat constitute cheating, the intention is for it to be used as a double check to ensure the solution one determined on their own takes the optimal number of moves and to make them aware they should keep trying other moves if not. If truly stuck, the moves list can be used directly.

## Features

 - Determine the efficient ways to solve the puzzle based on any starting state
 - See how many permutations of the puzzle state had to be checked in order to determine the minimum number of moves
 - Complete input validation

## Usage

Use of HorrifiedMummySolver is straightforward, other than some minor terminology.

Start by running the compiled application (See [Releases](https://github.com/oblivioncth/HorrifiedMummySolver/releases)) via the command line or your operating systems UI (i.e. Windows Explorer)

You will be prompted to enter the initial state of the puzzle as a series of comma-separated chiplet (the little tiles that you move around on the puzzle board) values. The first value is the number of the chiplet in slot one, the second value is the number of the chiplet in slot two, and so on. The initial chiplet in the center slot cannot and needs not to be entered since it is always assumed to be empty in accordance with the game's rules.

If your input is invalid the program will alert you and produce another prompt. Simply try to input the starting state of the puzzle again.

The calculation will begin after pressing enter once a valid starting sequence has been provided.

After a solution has been found, the following details are reported:
- **Iteration Count** - The number of simulated moves that were made before a solution was found
- **Move Path (Slot Nubmer)** - The exact moves to solve the puzzle from its starting state, based on slot (i.e. a value of 5 means move the chiplet that is in position 5 to the position/slot that is currently empty). A value of 'C' represents the center slot
- **Move Path (Slot Nubmer)** - The exact moves to solve the puzzle from its starting state, based on slot (i.e. a value of 5 means move the chiplet labeled '5' to the position/slot that is currently empty, regardless of its current position)

Both move lists are providing the same exact moves, just from different perspectives.

After this information is reported a divider will be printed and the program will display another prompt for calculating the solution of another initial state. This will continue ad infinitum until the console window is closed externally.

Example usage:

![Puzzle starting state](https://i.imgur.com/KPmBj2s.png "Example mummy puzzle initial state")

![Program usage](https://i.imgur.com/Swbfwu2.png "Solving the above puzzle using the tool")

## Limitations
- The program does ensure that the starting positions of each chiplet are restricted to the same degree that the game's rules enforce (i.e. you can place chiplets 1-3 on the 1-3 side of the tablet and vice-versa)
- The calculation does not take into account flipped chiplets, though this doesn't actually change the moves required and only increases the number of actions needed to complete the puzzle
- There is currently no way to see all possible solutions of the same move length
- The main calculation loop uses a lightly optimized, near-brute-force approach. Worst-case starting position calculations are still very fast but can take upwards of ~180MB of system RAM

## To-do

 - [ ] Add switch to enable increased verbosity with additional metrics
 - [ ] Add ability to exit the input loop via a command
 - [ ] Add switch that causes the tool to search for and report all possible solution paths that are the minimum length

## Source
This tool was written in C++ 17 along within MSVC2019 and currently only targets Windows Vista and above; however, this tool should be fully portable as it relies on no OS specific libraries. The source includes an easy-to-use .proj file if you wish to build the application in Visual Studio. Other than a C++ 17 capable compiler all files required to compile this software are included, with the exception of a standard make file.
