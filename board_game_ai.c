#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 7

typedef struct {
  char** board; 
  int board_size;
  int p1_count;
  int p2_count;
} GameState;

typedef struct {
  GameState state;
  int depth; 
  int score;
  char curPosition[3];
  char newPosition[3];  
} Move;



void print_state(GameState state);
char** get_empty_board(int size);
GameState create_random_state(int size, int pieces);
void ai_make_move(GameState state, int player);
Move* get_possible_moves(GameState state, int player,int heuristicPlayer);
GameState apply_move(GameState state, char* newPosition, char* curPosition, int player);
Move minimax(GameState state, int depth, int player, int maximizingPlayer,int heuristicPlayer);
void place_random_pieces(char** board, int size,int pieces);
int is_valid_move(char** board,char* curPosition, char* newPosition, int player);
void make_move(char** board, char* newPosition,char* curPosition, int player);
int has_valid_move(GameState state, int player);
int count_player_moves(GameState state, int player);
int evaluate_board_positions(GameState state, int player);
int heuristic(GameState state, int player);

int main(){
    
    int turnLimit, pieceCount, playerChoice, aiChoice;
    GameState state;

    printf("Enter the turn limit: ");
    scanf("%d", &turnLimit);
    printf("Enter the initial number of piece(If you write 10, then 5 X , 5 O): ");
    scanf("%d", &pieceCount);
 
    while (getchar() != '\n');

    printf("Choose your player: 1 for Player 1, 2 for Player 2: ");
    scanf("%d", &playerChoice);

    aiChoice = (playerChoice == 1) ? 2 : 1;

    while (getchar() != '\n');

    state = create_random_state(7, pieceCount);
    
    for (int turn = 0; turn < 2*turnLimit; turn++) {
        printf("\n");
        print_state(state);
        if (((playerChoice == 1) && (turn % 2 == 0)) || ((playerChoice == 2) && (turn % 2 == 1))) 
        {

          if(!has_valid_move(state,playerChoice))
          {
            printf("Ai won!");
            break;
          }

          int validMove = 0;
          
          while (!validMove) 
          {
            char curPosition[10], newPosition[10];
            int cur_x, cur_y;
            char piece = (playerChoice == 1) ? 'X' : 'O';

            while (1){
                printf("Choose piece to move: ");
                scanf("%s", curPosition);

                cur_x = curPosition[0] - 'a';
                cur_y = curPosition[1] - '1';

                if (cur_x < 0 || cur_x >= SIZE || cur_y < 0 || cur_y >= SIZE || state.board[cur_x][cur_y] != piece) 
                {
                  printf("Invalid position, no '%c' at %s. Try again.\n", piece, curPosition);
                } 
                else 
                {
                  break; 
                }
            }

            printf("Choose the new position for %s: ", curPosition);
            scanf("%s", newPosition);

            if (is_valid_move(state.board, curPosition, newPosition, playerChoice))
            {
                make_move(state.board, newPosition,curPosition, playerChoice);
                printf("Player moves the piece at %s to %s\n", curPosition, newPosition);
                validMove = 1;
            } 
            else 
            {
              printf("Invalid move. Try again.\n");
            }
          }
          
        }  
        else 
        {
          if(!has_valid_move(state,aiChoice))
          {
            printf("Player Won!");
            break;
          }
          ai_make_move(state,aiChoice);
        }


    }
    int p1_moves = count_player_moves(state, 1);
    int p2_moves = count_player_moves(state, 2);
    //printf("p1 : %d ",p1_moves);
    //printf("p2 : %d ",p2_moves);
    if (p1_moves == p2_moves) 
    {
      printf("Game is a draw.\n");
    } 
    else 
    {
      int winner = (p1_moves > p2_moves) ? 1 : 2;
      printf("Player %d wins!\n", winner);
    }
    exit(0);
    
    return 0;
}



void print_state(GameState state){
    int size = 7 ;
    printf("  "); 
    for (int col = 1; col <= size; col++) {
        printf("%d ", col);
    }
    printf("\n");

    for (int row = 0; row < size; row++) {
        printf("%c ", 'a' + row);
        for (int col = 0; col < size; col++) {
            printf("%c ", state.board[row][col]);
        }
        printf("\n");
    }

}

GameState create_random_state(int size, int pieces) {
    GameState state;
    state.board = get_empty_board(size);
    state.p1_count = pieces / 2;
    state.p2_count = pieces / 2;
    place_random_pieces(state.board, size, pieces);
    return state;
}


char** get_empty_board(int size) {
    char** board = (char**)malloc(size * sizeof(char*));
    for(int i = 0; i < size; i++) {
        board[i] = (char*)calloc(size, sizeof(char));
        for(int j = 0; j < size; j++) {
            board[i][j] = '.';
        }
    }
    return board;
}

void place_random_pieces(char** board, int size, int count) {
   srand(time(NULL));
    for (int i = 0; i < count; i++) {
        int x, y;
        do {
            x = rand() % size;
            y = rand() % size;
        } while (board[x][y] != '.');

        board[x][y] = (i < count / 2) ? 'X' : 'O';
    }
    
}

int is_valid_move(char** board, char* curPosition, char* newPosition, int player) {
    int from_x = curPosition[0] - 'a';
    int from_y = curPosition[1] - '1';
    int to_x = newPosition[0] - 'a';
    int to_y = newPosition[1] - '1';

    if (from_x < 0 || from_x >= SIZE || from_y < 0 || from_y >= SIZE ||
        to_x < 0 || to_x >= SIZE || to_y < 0 || to_y >= SIZE) {
        return 0;
    }

    if (board[to_x][to_y] != '.') {
        return 0;
    }

    if (abs(to_x - from_x) > 0 && abs(to_y - from_y) > 0) {
        return 0;
    }

    if (abs(to_x - from_x) > 1 || abs(to_y - from_y) > 1) {
        return 0;
    }

    char piece = (player == 1) ? 'X' : 'O';
    if (board[from_x][from_y] != piece) {
        return 0;
    }

    return 1;
}

void make_move(char** board, char*newPosition,char* curPosition, int player)
{
  int from_x = curPosition[0] - 'a';
  int from_y = curPosition[1] - '1';
  int to_x = newPosition[0] - 'a';
  int to_y = newPosition[1] - '1';

  char piece = (player == 1) ? 'X' : 'O';
  board[to_x][to_y] = piece;
  board[from_x][from_y] = '.';

}

int has_valid_move(GameState state, int player) {

    char piece = (player == 1) ? 'X' : 'O';

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (state.board[i][j] == piece) {
                if ((i > 0 && state.board[i-1][j] == '.') ||
                    (i < state.board_size - 1 && state.board[i+1][j] == '.') ||
                    (j > 0 && state.board[i][j-1] == '.') ||
                    (j < state.board_size - 1 && state.board[i][j+1] == '.')) {
                    return 1; 
                }
            }
        }
    }
    return 0; 
}

int count_player_moves(GameState state, int player) {
    int moveCount = 0;
    char piece = (player == 1) ? 'X' : 'O';

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (state.board[i][j] == piece) {
                
                if (i > 0 && state.board[i-1][j] == '.') moveCount++; 
                if (i < 6 && state.board[i+1][j] == '.') moveCount++; 
                if (j > 0 && state.board[i][j-1] == '.') moveCount++; 
                if (j < 6 && state.board[i][j+1] == '.') moveCount++; 
            }
        }
    }
    return moveCount;
}

int evaluate_board_positions(GameState state, int player) {
    int score = 0;
    char piece = (player == 1) ? 'X' : 'O';

    int centerValue = 3; // Kenarlarda olmayan taslar icin ekstra puan

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (state.board[i][j] == piece) {
                score += 1;
                if ((i != 0) && (j != 0) && (i != 6) && (j!=6)) {
                    score += centerValue;
                }
            }
        }
    }

    return score;
}

int heuristic(GameState state, int player) {
    int score = 0;
    
    score += count_player_moves(state, player);

    score += evaluate_board_positions(state, player);

    score -= count_player_moves(state, 3 - player) ;

    return score;
}

Move* get_possible_moves(GameState state, int player , int heuristicPlayer) {
    int moveCount = count_player_moves(state, player);
    Move* moves = (Move*)malloc(moveCount * sizeof(Move));

    int index = 0;
    char piece = (player == 1) ? 'X' : 'O';

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (state.board[i][j] == piece) {
                // Yukari hareket
                if (i > 0 && state.board[i-1][j] == '.') {
                    char curPosition[3] = {i + 'a', j + '1', '\0'};
                    char newPosition[3] = {i - 1 + 'a', j + '1', '\0'};
                    GameState newState = apply_move(state, newPosition, curPosition, player);
                    moves[index].state = newState;
                    moves[index].score = heuristic(newState, heuristicPlayer);
                    strcpy(moves[index].curPosition, curPosition);
                    strcpy(moves[index].newPosition, newPosition);
                    index++;
                }
                // Asagiya hareket
                if (i < 6 && state.board[i+1][j] == '.') {
                    char curPosition[3] = {i + 'a', j + '1', '\0'};
                    char newPosition[3] = {i + 1 + 'a', j + '1', '\0'};
                    GameState newState = apply_move(state, newPosition, curPosition, player);
                    moves[index].state = newState;
                    moves[index].score = heuristic(newState, heuristicPlayer);
                    strcpy(moves[index].curPosition, curPosition);
                    strcpy(moves[index].newPosition, newPosition);
                    index++;

                }
                // Sola hareket
                if (j > 0 && state.board[i][j-1] == '.') {

                    char curPosition[3] = {i + 'a', j + '1', '\0'};
                    char newPosition[3] = {i + 'a', j - 1 + '1', '\0'};
                    GameState newState = apply_move(state, newPosition, curPosition, player);
                    moves[index].state = newState;
                    moves[index].score = heuristic(newState, heuristicPlayer);
                    strcpy(moves[index].curPosition, curPosition);
                    strcpy(moves[index].newPosition, newPosition);
                    index++;

                }
                // Saga hareket
                if (j < 6 && state.board[i][j+1] == '.') {

                    char curPosition[3] = {i + 'a', j + '1', '\0'};
                    char newPosition[3] = {i + 'a', j + 1 + '1', '\0'};
                    GameState newState = apply_move(state, newPosition, curPosition, player);
                    moves[index].state = newState;
                    moves[index].score = heuristic(newState, heuristicPlayer);
                    strcpy(moves[index].curPosition, curPosition);
                    strcpy(moves[index].newPosition, newPosition);
                    index++;
                    
                }
            }
        }
    }

    return moves;
}

GameState apply_move(GameState state, char* newPosition, char* curPosition, int player) {
    
    GameState newState;
    newState.board_size = 7;
    newState.p1_count = state.p1_count;
    newState.p2_count = state.p2_count;
    newState.board = get_empty_board(7);
    

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            newState.board[i][j] = state.board[i][j];
        }
    }

    make_move(newState.board, newPosition, curPosition, player);

    return newState;
}

Move minimax(GameState state, int depth, int player, int maximizingPlayer,int heuristicPlayer) {
    Move move;
    if (depth == 0 || !has_valid_move(state, player)) {
        Move move;
        move.state = state;
        move.score = heuristic(state,heuristicPlayer);
        return move;
    }

    Move bestMove;
    bestMove.score = maximizingPlayer ? -10000 : 10000;

    Move* moves = get_possible_moves(state, player,heuristicPlayer);
    int moveCount = count_player_moves(state, player);

    for (int i = 0; i < moveCount; i++) {
        GameState newState = apply_move(state, moves[i].newPosition, moves[i].curPosition, player);
        move = minimax(newState, depth - 1, 3 - player, !maximizingPlayer,heuristicPlayer);
        moves[i].score = move.score; 

        if (maximizingPlayer) {
            if (move.score > bestMove.score) {
                bestMove = moves[i];
            }
        } else {
            if (move.score < bestMove.score) {
                bestMove = moves[i];
            }
        }
    }

    free(moves); 
    return bestMove;
}

void ai_make_move(GameState state, int player) {
    Move bestMove = minimax(state, 7, player, 1,player); 
    
    //printf("Pos1: %s",bestMove.curPosition);
    //printf("Pos2: %s",bestMove.newPosition);
    make_move(state.board,bestMove.newPosition,bestMove.curPosition,player);
    //printf("selected move = %s - %s - %d", bestMove.curPosition,bestMove.newPosition,bestMove.score);
    printf("\nAI moves the piece at %s to %s\n",bestMove.curPosition,bestMove.newPosition);
    //print_state(state);
}
