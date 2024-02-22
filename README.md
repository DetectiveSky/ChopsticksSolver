# ----- Chopsticks Solver -----
High-Performance Computing Final Assignment
Last updated: February 7, 2024

This program was written by myself and a friend in late 2022, with the goal of solving the children's game of Chopsticks (Description below). To achieve this in a reasonable amount of time, we parallelized the program, drawing upon knowledge gained through the course, as well as organizing the data using tree structures for easy identification and pruning of game states.

The program makes use of OpenMP for its parallelization, and so requires the -f openmp compiler flag.

If I were to redo this program, I would provide more documentation and do some different workarounds for stalemates.

## Chopsticks

Chopsticks is a game for two players played with the hands. Each player extends two hands, with one finger on each extended likewise. The players take turns tapping each other's hands together, and each tap adds the number of fingers extended on the tapping hand to the number of fingers extended on the hand tapped. A hand with the maximum amount of fingers extended is eliminated from the game. Once both of a player's hands are eliminated, they lose.

Split moves, which split finger extensions from one hand to half the extensions on both hands, thus recovering a hand into the game, have been disallowed due to time and complexity constraints.

See more: https://en.wikipedia.org/wiki/Chopsticks_(hand_game)