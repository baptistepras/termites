# Termites

This project simulate 4 teams of termites who need to go collect objects on the map and bring them back to their nest. After 2000 rounds, the biggest nest wins. Termites have a random behavior dictated by a few rules and variables. Each team has different settings for variables.

The project has a very clear explanation of the termites behavior in `Description Générale du Projet.pdf` (in French).

This project made in groups of 2 was realized during my first year of university.

# Members who contributed to the project

Raphael LEONARDI
Baptiste PRAS

# How to use it ?

The `Makefile` will make it easy for you. Do `make` to compile all code and create `projet` and `tests`. A few options can be added to make:
- `make clean`: Delete all files related to the compilation
- `make tests`: Compile `tests.cpp` and create the executable `tests`. Can be used with `./tests`
- `make projet`: Compile `projet.cpp` and create the executable `projet`. Can be used with `./projet`
- `make check`: Compile `tests.cpp`, create the executable `tests` and execute it
- `make main`: Compile `projet.cpp`, create the executable `projet` and execute it

When you execute the main, you will have to chose between 3 options, type `1`, `2` or `3` in shell then validate with `Enter`. All instructions are recalled in French in the shell.
- `1`: Launch the simulation in a round by round mode. Press `Enter` to pass to the next round. Press `Space` to print the `vector Termites`
- `2`: Launch the simulation in automatic mode. You will not be able to do anything before the end but will see the evolution of each round graphically.
- `3`: Launch 600 simulations in automatic mode. You will not be able to do anything before the end nor will the evolution of each round graphically. You will only see the result of each simulation and the final result of the 600 simulations.
