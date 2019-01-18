# Bacteria
A game based on Cellular Automata

Bacteria is a game based on the Cellular Automata concept, more specifically on the Conway's Game of Life.
The name comes from the Bacterial Cell Growth process, which is similar to how the Cellular Automata works.
 
The player should kill all the bacterial cells in the environment, to do it he might shoot cells against the enemies. But these cells reproduce in a very efficient way, so we should shot with a logic, following the cell reproduction's rules.
 
The rules of the bacterial cells are very simple:
  - Each alive cell with one or no neighbors dies, as if by solitude.
  - Each alive cell with four or more neighbors dies, as if by overpopulation.
  - Each alive cell with two or three neighbors survives.
  - Each dead cell with three neighbors becomes populated.
  
  The Game is written in C++ (openFrameworks).

![Alt Text](https://media.giphy.com/media/mPIB3dIbY1MUMI3mXX/giphy.gif)
