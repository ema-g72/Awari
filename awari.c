/*
   Awari. 
   
   A simple two players game implemented with the minimax algorihm.
   Set the macros ALPHA and BETA to enable the alpha and/or beta pruning of search tree.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Number of houses per side. */
#define N_HOUSES 6 
/* Initial number of seeds per house. */
#define N_SEEDS 3
/* Maximum search depth. */
#define N_MAX_DEPTH 25

/* Total number of houses including the 'score' house. */
#define N_TOT_HOUSES (N_HOUSES + 1)

/* Players: computer or human. */
#define CPU    0
#define PLAYER 1
/* Game end flags. */
#define END_TIE  2   
#define END_NONE 3   

/* Global variables. */

/* Board game descriptor */
int g_boards[N_MAX_DEPTH+1][2][N_TOT_HOUSES];
int g_current_depth;
int g_max_depth;
/* Store the possible moves. */
int g_moves[N_MAX_DEPTH+1][N_HOUSES];
/* Store the player and evaluation of the previous moves, used for Aplha/Beta pruning. */
int g_previous[N_MAX_DEPTH+1][2];
/* Counter of visited nodes during minimax search. */
int g_visited_nodes;

/* 
 * Print program command line parameters. 
 */
void print_usage()
{
   printf("Usage: awari <level> <initial player>\n");
   printf("                |            |\n");
   printf("               1-%d    0-cpu, 1-player\n", N_MAX_DEPTH);
}

/* 
 * Parse command line parameters, returns the game difficult level (the search depth)
 * and the initial player.. 
 */
int parse_options(int num, char **arg, int *level, int *ini_player)
{
   if(num != 3) 
   {
      print_usage();
      return -1;
   }

   *level = atoi(arg[1]);
   *ini_player = atoi(arg[2]);

   if( (*level < 1) || (*level >= N_MAX_DEPTH) )
   {
      printf("Invalid game level, select a number between 1 and %d\n", N_MAX_DEPTH);
      return -1;
   }

   if((*ini_player != CPU) && (*ini_player != PLAYER))
   {
      printf("Invalid initial player, select 0=CPU or 1=PLAYER\n");
      return -1;      
   }

   return 0;
}

/* 
 * Initialize the data structures for a new game.
 */
void init_board()
{
   g_current_depth = 0;
   g_visited_nodes = 0;

   /* Reset the score house. */
   g_boards[g_current_depth][CPU][0] = 0;
   g_boards[g_current_depth][PLAYER][0] = 0;

   /* Fill the houses with initial seeds. */
   for(int i=1 ; i<N_TOT_HOUSES ; i++)
   {
      g_boards[g_current_depth][CPU][i] = N_SEEDS;
      g_boards[g_current_depth][PLAYER][i] = N_SEEDS;
   }
}

/* 
 * Print an horizontal line to separate game moves.
 */
void print_line()
{
   for(int i=0 ; i<5*N_HOUSES+18 ; i++)
   {
      putchar('_');      
   }
   printf("\n\n        ");
}

/* 
 * Print the current board.
 */
void print_board()
{ 
   print_line();  

   /* CPU (top side). */
   for(int i=N_HOUSES ; i>0 ; i--)
   {
      printf("| %2d ", g_boards[g_current_depth][CPU][i]);
   }
   printf("|\n");
   
   /* CPU score house. */
   printf("    %2d  ", g_boards[g_current_depth][CPU][0]);
   /* Middle space. */
   for(int i=0; i<N_HOUSES ; i++)
   {
      printf("|====");
   }
   printf("|");
   /* PLAYER score house */
   printf("  %2d\n        ",g_boards[g_current_depth][PLAYER][0]);
  
   /* PLAYER (bottom side). */
   for(int i=0 ; i<N_HOUSES ; i++)
   {
      printf("| %2d ", g_boards[g_current_depth][PLAYER][i+1]);
   }
   printf("|\n\n         ");

   /* House indexes. */
   for(int i=0 ; i<N_HOUSES ; i++)
   {
      printf(" (%1d) ",i+1);
   }
   printf("\n\n");  
}

/* 
 * Checks if the current board is in a final state or not, returns:
 * CPU/PLAYER if there is a winner, END_TIE if the game ended with a tie
 * or END_NONE if the game is not ended.
 */
int is_ended()
{
   int tot_player = 0;
   int tot_cpu = 0;

   /* Count the total seeds in the houses. */
   for(int i=0; i<N_HOUSES ; i++)
   {
      tot_cpu += g_boards[g_current_depth][CPU][i+1]; 
      tot_player += g_boards[g_current_depth][PLAYER][i+1];
   } 
   
   /* The game ends when one side has no seeds and the winner is the player with more seeds in the score house. */
   if( !tot_cpu || !tot_player)
   {
      if( g_boards[g_current_depth][CPU][0] < g_boards[g_current_depth][PLAYER][0] )
      {
         return PLAYER;
      }
      
      if( g_boards[g_current_depth][CPU][0] > g_boards[g_current_depth][PLAYER][0] )
      {
         return CPU;
      }
      
      return END_TIE;
   }
  
   return END_NONE;  
}

/* 
 * Checks if the game is over and prints the result. Return 0 if
 * the game is not terminated.
 */
int check_winner()
{
   /* Check if the game is terminated and print the winner. */
   switch( is_ended() )
   {
      case CPU:
         print_line();
         printf("* CPU WINS! *\n\n");
         return 1;

      case PLAYER:
         print_line();
         printf("* CPU WINS! *\n\n");
         return 1;

      case END_TIE:
         print_line();
         printf("* IT'S A TIE! *\n\n");
         return 1;
      
      default:
         break;
   }   
   return 0;
}

/* 
 * Get the move of human player.
 */
int prompt_player_move()
{
   int move;
   const int MAX_LEN = 16;   
   char str[MAX_LEN];

   while(1)
   {
      printf("PLAYER> ");
      fgets(str, MAX_LEN, stdin);
      move = atoi(str);
      /* Accept the selection if is in the valid range and if the house is not empty. */
      if(move>0 && move <=N_HOUSES && g_boards[g_current_depth][PLAYER][move])
      {
         return move;
      }
   }
   return 0;
}

/* 
 * Increment the seeds in the houses from 'ini' to 'end' in the 'player' side.
 */
void increment_seed(int ini, int end, int player)
{
   for( ; ini<=end ; ini++)
   {
      g_boards[g_current_depth][player][ini]++;
   }
}

/* 
 * Update the game board according to the 'move' for 'player'.
 * Return the next player.
 */
int do_move(int move, int player)
{
   int pos, n_seeds;
   
   int other_player = (player == CPU ? PLAYER : CPU);

   /* Get the seeds in selected house. */
   n_seeds = g_boards[g_current_depth][player][move];

   /* Empty the house. */
   g_boards[g_current_depth][player][move] = 0;
   
   pos = N_TOT_HOUSES - (move + n_seeds);
   
   if( pos > 0 ) /* Here the last seed lands in the current player's side. */
   { 
      /* Distribute the seeds. */
      increment_seed(move+1, N_TOT_HOUSES-pos, player);
      /* If the last seed lands in an empty house on the player side, get all the seeds of the other player in the opposite house. */
      if( (g_boards[g_current_depth][player][N_TOT_HOUSES-pos] == 1) &&  (g_boards[g_current_depth][other_player][pos] != 0) )
      {
         g_boards[g_current_depth][player][0] += g_boards[g_current_depth][other_player][pos] + 1;
         g_boards[g_current_depth][player][N_TOT_HOUSES-pos] = 0;
         g_boards[g_current_depth][other_player][pos] = 0;
      }
      return other_player;
   } 
   else if( pos == 0 ) /* Here the last seed lands in the current player's score house. */
   {
      /* Distribute the seeds. */
      increment_seed(move+1, N_HOUSES-pos, player);
      g_boards[g_current_depth][player][0]++;
      /* Continue with current player. */
      return player;    
   } 
   else /* Here the last seed  passes the current player's score house */
   {         
      int turns, r_seeds, ini_house, ini_player;

      ini_player = player;
      /* Number of side changes. */
      turns = -pos/N_TOT_HOUSES+1;
      /* Number of remaining seeds. after side changes. */
      r_seeds = -pos%N_TOT_HOUSES;

      ini_house = move+1;

      /* Distribute seeds on both sides for the number of turns. */
      for(int i=0; i<turns ; i++)
      {
         increment_seed(ini_house, N_HOUSES, player);
         player = (player == CPU ? PLAYER:CPU);
         ini_house = 1;
      }
      /* DIstribute the remaining seeds. */
      increment_seed(1, r_seeds, player);
      
      /* Increment the score houses. */
      g_boards[g_current_depth][ini_player][0] += turns/2+turns%2;
      g_boards[g_current_depth][other_player][0] += turns/2;

      /* If the last seed lands on the initial player's score house, keep playing. */
      if( (turns%2) && !r_seeds )
      {         
         return ini_player;
      }
      
      return other_player;
   }
}

/* 
 * Evaluation function: return a score for the current node of the search tree.
 * It is the difference between CPU and PLAYER seeds in the score houses.
 */
int evaluate_move()
{
   /* The board configuration is evaluated as the differences of seeds in score houses. */ 
   int val = g_boards[g_current_depth][CPU][0] - g_boards[g_current_depth][PLAYER][0];

   return val;
}

/* 
 * Get the possible moves for the 'player' for the current board configuration.
 * It stores the moves in the global stack "g_moves" and return the number of elements.
 */
int get_player_moves(int player)
{
   int j = 0;

   for(int i=1 ; i<N_TOT_HOUSES ; i++)
   {
      /* A move is possible if the house is not empty. */
      if(g_boards[g_current_depth][player][i])
      {
         g_moves[g_current_depth][j++] = i;
      }
   }

   return j;
}

/* 
 * Minimax algorithm function.
 */
int minimax_search(int depth, int player, int *best_move)
{
   int n_moves, next_player, tmp_val, tmp_move;
  
   int val = (player == PLAYER ? 999 : -999);

   g_visited_nodes ++;
   
   /* If we reached the max search depth or there is a winner stop the recursion and return the evaluation. */
   if( depth == 0 || is_ended() != END_NONE ) 
   {
      int score = evaluate_move();
      g_current_depth--;
      return score;
   }
   
   /* Get the possible moves for the player in the current board configuration, it updates the g_moves array. */
   n_moves = get_player_moves(player);
                                  
   /* Store the player (for alpha/beta pruning). */
   g_previous[g_current_depth][0] = player;     /* Registra il giocatore per il taglio alfa-beta */

   for( int i=n_moves-1; i>=0 ; i-- )
   {
      /* Store the evaluation (for alpha/beta pruning). */
      g_previous[g_current_depth][1] = val;    
      /* Prepare for recursion. */
      g_current_depth++;                             
      /* Duplicate the current board configuration. */
      memcpy(g_boards[g_current_depth], g_boards[g_current_depth-1], sizeof(g_boards[0]));

      /* Do the move on the new board and get the next player. */
      next_player = do_move(g_moves[g_current_depth-1][i], player);
    
      /* Explore the search tree from the new node. */
      tmp_val = minimax_search(depth-1, next_player, &tmp_move); 

      /* CPU player generates OR nodes, select the max value. */
      if(player == CPU)
      {         
         if(tmp_val > val)
         {
            val = tmp_val;
            *best_move = g_moves[g_current_depth][i];
         }
#ifdef BETA        
         /* Stop searching under AND nodes (nodes where one of the previous player was PLAYER) and if */
         /* val is greater than the previous evaluation. */
         {
	         int cut = 0;
	
            for(int k=1 ; k<=g_current_depth-1 && !cut ; k++)
            {
               if( g_previous[k][0] == PLAYER && val >= g_previous[k][1] )
               {
                  cut = 1;
                  break;
               }
            }
	         
            if(cut)
            {
	            break;
            }
         }
#endif	
      } 
      else /* PLAYER generates AND nodes, select the min value. */
      { 
         if(tmp_val < val)
         {
	         val = tmp_val;
         }
#ifdef ALPHA    
         /* Stop searching under OR nodes (nodes where one of the previous player was CPU) and if */
         /* val is less than the previous evaluation. */
         {
	         int cut = 0;
	
            for(int k=0 ; k<=g_current_depth-1 && !cut ; k++)
            {
               if( g_previous[k][0] == CPU && val <= g_previous[k][1] )
               {
                  cut = 1;
                  break;
               }
            }
            if(cut)
            {
               break;
            }
         }
#endif
      }   
  }
  
  /* Return to previous level. */
  g_current_depth--;

  return val;
}

/* 
 * Call the minimax recursive search and returns the best move.
 * Prints the move and the number of evaluated nodes. 
 */
int get_cpu_move()
{
   int move;

   minimax_search(g_max_depth, CPU, &move);

   printf("CPU[%d]> %d\n", g_visited_nodes, N_TOT_HOUSES-move);

   g_visited_nodes = 0;
   g_current_depth ++;

   return move;
}

/* 
 * main
 */
int main(int argc, char *argv[])
{
   int move;
   int player;

   /* Read command line. */
   if( parse_options(argc, argv, &g_max_depth, &player) < 0 )
   {
      return -1;
   }
   
   /* Initialize the game. */
   init_board();

   /* Game loop. */
   while(1)
   {
      /* Print the current board. */
      print_board();
    
      /* Stop the game if there is a winner. */
      if( check_winner() )
      {
         return 0;
      }
  
      /* Select the move. */
      switch( player )
      {                   
         case CPU:
            move = get_cpu_move();
            break;
      
         case PLAYER:
            move = prompt_player_move();
            break;

         default:
            return -1;
      }
      
      /* Execute the move and update the current board. */
      player = do_move(move, player);  
  }
}
