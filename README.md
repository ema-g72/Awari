# Awari
<img src="https://github.com/user-attachments/assets/d7aadc79-fbdf-4f35-b844-49e35e1c59c6" width="50%">

## The game

Awari is a strategy game for two players belonging to the mancala family of board games. It has many variants known with different names: oware, wari, awele, etc.
The version implemented here consists in the typical board with two opposite rows of six pits, called *houses*, and a special *score house* placed to the right of each player.
The game starts with three seeds in each house, while the score houses are empty. The objective of the game is to capture more seeds that the other player.

### Rules
Each player makes a move in turn by choosing a not empty house in his side of the board and distributing them one per house in counter-clockwise direction, including the score houses of both players, if a sufficient number of seeds are available.
If the last seed lands in an empty house of the player side, all the seeds in the opposite house are captured and placed in the score house.
Normally each player makes a move in turn, except when the last seed lands in its own score house, in this case the player continue moving.

### Winning
The game is over when a player captures more than half seeds, because the opponents cannot reach a greater score or if a player cannot move because all its houses are empty. Since the total number seeds is even, it is possible to end the game in a draw.

## Instructions
Run the executable with two command line parameters: the difficult level, corresponding to the search depth in the game tree and the initial player.

```
Usage: awari <level> <initial player>
                |            |
               1-25    0-cpu, 1-player
```

The human player owns the lower side of the board and types the move number at the prompt `PLAYER>`, the computer shows its move at the prompt `CPU[n]>` where n is the number of evaluated nodes during the execution of the minimax algorithm. The alpha-beta pruning has a significant effect on this number, especially when the depth search increases.

```
________________________________________________

        |  3 |  3 |  3 |  3 |  3 |  3 |
     0  |====|====|====|====|====|====|   0
        |  3 |  3 |  3 |  3 |  3 |  3 |

          (1)  (2)  (3)  (4)  (5)  (6)

PLAYER> 1
________________________________________________

        |  3 |  3 |  3 |  3 |  3 |  3 |
     0  |====|====|====|====|====|====|   0
        |  0 |  4 |  4 |  4 |  3 |  3 |

          (1)  (2)  (3)  (4)  (5)  (6)

CPU[32306]> 3
________________________________________________

        |  4 |  4 |  0 |  3 |  3 |  3 |
     1  |====|====|====|====|====|====|   0
        |  0 |  4 |  4 |  4 |  3 |  3 |

          (1)  (2)  (3)  (4)  (5)  (6)

CPU[28309]> 6
________________________________________________

        |  4 |  4 |  0 |  4 |  4 |  0 |
     6  |====|====|====|====|====|====|   0
        |  0 |  4 |  0 |  4 |  3 |  3 |

          (1)  (2)  (3)  (4)  (5)  (6)

PLAYER>
```
## The minimax algorithm
The [minimax algorithm](https://en.wikipedia.org/wiki/Minimax) is an artificial intelligence method for solving decision-making problems. In game theory it is used to find the optimal move for a player.
In a two-player game, one player is the maximizer, aiming to maximize their score, while the other is the minimizer, aiming to minimize the maximizer's score.

### The game tree
The algorithm operates by creating a tree structure where each node is a game state and each edge is a possible move. The terminal nodes are the game configurations that represents the outcome of the game (because there is a winner and the game terminates, or because a pre defined maximun depth has been reached in the tree development). The algorithm assigns scores to each terminal node through an *evaluation function*. A game tree can grow very quickly and is typically explored in a depth first manner.

### Backpropagation
Starting from the terminal nodes the values are propagated upwards through the search tree. For each non-terminal node the value is assigned in the following way:
* Select the maximum value from the child nodes, if it's the maximizer turn;
* Select the minimum value from the child nodes, if it's the minimizer turn;
  
At the root node of the game tree the maximizer player selects the move corresponding to the maximum value.

### Alpha-Beta pruning
Alpha-beta pruning is an optimization in the search for the best move that eliminates branches of the game tree that cannot change the current evaluation of a node. The algorithm stores two values
* **Alpha**: the best value that the maximizer player has at the current depth (initialized to negative infinity) 
* **Beta**: the best value that the minimizer player has at the current depth (initialized to positive infinity)

As the algorithm evaluates each node it compares the score of the node with the current alpha and beta values. If the current alpha is greater than the current beta value, it means that the current branch is worse than the one already evaluated and can be ignored (pruned).


