# The game

Awari is a strategy game for two players belonging to the mancala family of board games. It has many variants known with different names: oware, wari, awele, etc.
The version implemented here consists in the typical board with two opposite rows of six pits, called *houses*, and a special *score house* placed to the right of each player.
The game starts with three seeds in each house, while the score houses are empty. The objective of the game is to capture more seeds that the other player.

## Rules
 Each player makes a move in turn by choosing a not empty house in his side of the board and distributing them one per house in counter-clockwise direction, including the score houses of both players, if a sufficient number of seeds are available.
If the last seed lands in an empty house of the player side, all the seeds in the opposite house are captured and placed in the score house.
Normally each player makes a move in turn, except when the last seed lands in its own score house, in this case the player continue moving.

## Winning
The game is over when a player captures more than half seeds, because the opponents cannot reach a greater score or if a player cannot move because all its houses are empty. Since the total number seeds is even, it is possible to end the game in a draw.