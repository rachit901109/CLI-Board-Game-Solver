# 1. Eight-puzzle-problem
Commands to run on the terminal:
1. To solve puzzle:
```
cd eight_puzzle
g++ -g eight_puzzle.cpp -fsanitize=address -o solve.exe
./solve.exe
```

Solution path:

![Screenshot 2023-08-11 011824](https://github.com/rachit901109/Eight-puzzle-problem/assets/110279690/be220709-0e6c-483c-819d-1950cd89734d)

2. To generate game tree:
```
g++ -g game_tree.cpp -fsanitize=address -o game_tree.exe
./game_tree.exe
```
Game Tree for Depth 5:

![Screenshot 2023-08-11 185529](https://github.com/rachit901109/Eight-puzzle-problem/assets/110279690/8d12c5f1-4fa8-4a93-8790-e7d8326a026b)


# 2. Lights Out Puzzle
Commands to run on the terminal:
1. To solve puzzle:
```
cd lightsout
g++ -g lightsout.cpp -o solve.exe
./solve.exe
```
Solution Sequence:

![Screenshot 2024-02-16 034652](https://github.com/rachit901109/CLI-Board-Game-Solver/assets/110279690/37c7e88f-e132-4401-be33-6e0f5ac395f5)

2. To generate game tree:
```
g++ -g game_tree.cpp -o game_tree.exe
./game_tree.exe
```
Game Tree (All possible moves for a 3x3 grid):

![Screenshot 2024-02-16 034729](https://github.com/rachit901109/CLI-Board-Game-Solver/assets/110279690/1fc2dd5f-c30b-4cd5-9b25-f4defe163464)
