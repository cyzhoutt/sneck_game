#ifndef _SNK_STATE_C
#define _SNK_STATE_C

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int x, unsigned int y, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_x(unsigned int cur_x, char c);
static unsigned int get_next_y(unsigned int cur_y, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);


/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.
//  game_state_t state;
  game_state_t* state = malloc(sizeof(game_state_t));
  char** board = (char**) calloc(18, sizeof(char*));
  for (int i = 0; i < 18; ++i) {
    board[i] = (char*) calloc(21, sizeof(char));
  }
  for (int i= 0; i<20;++i){
    board[0][i] = '#';
  }
  for (int i= 0; i<20;++i){
    board[17][i] = '#';
  }
  for(int i=1;i<17;++i){
    for(int j=1;j<19;++j){
      board[i][j]=' ';
    }
  }
  for (int i= 0; i<18;++i){
    board[i][0] = '#';
  }
  for (int i= 0; i<18;++i){
    board[i][19] = '#';
  }
  board[2][2] = 'd';
  board[2][9] = '*';
  board[2][3] = '>';
  board[2][4] = 'D';
  state->board = board;

  state->num_rows = 18;
//  state->num_cols = 20;

  snake_t snake;
  snake.head_x = 4;
  snake.head_y = 2;
  snake.live = true;
  snake.tail_x = 2;
  snake.tail_y = 2;

  state->snakes = calloc(1, sizeof(snake_t));
  state->snakes[0] = snake;

  state->num_snakes =1;
  return state;
}


/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
  for (int i=0; i< state->num_rows;++i){
    free(state->board[i]);
  }
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
//  fp = fopen("fp","w");
  for (int i=0;i < state->num_rows;++i){
    for(int j=0; j< strlen(state->board[i]);++j){
      fprintf(fp,"%c", state->board[i][j]);
    }
    fprintf(fp, "%c",'\n');
  }
//  fprintf(fp, "%c",'\n');
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}


/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int x, unsigned int y) {
  return state->board[y][x];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int x, unsigned int y, char ch) {
  state->board[y][x] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
  return c =='w'||c=='a'||c=='s'||c=='d';
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
  return c =='W'||c=='A'||c=='S'||c=='D'||c=='x';
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<>vWASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  return c =='W'||c=='A'||c=='S'||c=='D'||c=='x'||c =='w'||c=='a'||c=='s'||c=='d'||c=='^'||c=='>'||c=='<'||c=='v';
}

/*
  Converts a character in the snake's body ("^<>v")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function.
  if (c=='^'){
    c = 'w';
  }if(c=='<'){
    c = 'a';
  }if(c=='>'){
    c ='d';
  }if(c=='v'){
    c='s';
  }
  return c;
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<>v").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  if (c=='W'){
    c = '^';
  }if(c=='A'){
    c = '<';
  }if(c=='S'){
    c ='v';
  }if(c=='D'){
    c='>';
  }
  return c;
}

/*
  Returns cur_x + 1 if c is '>' or 'd' or 'D'.
  Returns cur_x - 1 if c is '<' or 'a' or 'A'.
  Returns cur_x otherwise.
*/
static unsigned int get_next_x(unsigned int cur_x, char c) {
  // TODO: Implement this function.
  if (c=='>'||c=='d'||c=='D'){
    cur_x = cur_x+1;
  }if (c=='<'||c=='a'||c=='A'){
    cur_x = cur_x -1;
  }
//  }else{cur_x = cur_x;}
  return cur_x;
}

/*
  Returns cur_y - 1 if c is '^' or 'w' or 'W'.
  Returns cur_y + 1 if c is 'v' or 's' or 'S'.
  Returns cur_y otherwise.
*/
static unsigned int get_next_y(unsigned int cur_y, char c) {
  // TODO: Implement this function.
  if (c=='v'||c=='s'||c=='S'){
    cur_y = cur_y+1;
  }if (c=='^'||c=='W'||c=='w') {
    cur_y = cur_y - 1;
  }
//  }else{cur_y = cur_y;}
  return cur_y;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  if(get_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y)=='W'){
    return get_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y-1);
  }else if(get_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y)=='S'){
    return get_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y+1);
  }else if(get_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y)=='D'){
    return get_board_at(state, state->snakes[snum].head_x+1, state->snakes[snum].head_y);
  }else if(get_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y)=='A'){
    return get_board_at(state, state->snakes[snum].head_x-1, state->snakes[snum].head_y);
  }
  return '?';
}


/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the x and y coordinates of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int headX= state->snakes[snum].head_x;
  unsigned int headY= state->snakes[snum].head_y;
  char current_c = get_board_at(state, headX, headY);
  char c = head_to_body( current_c);
  set_board_at(state, headX, headY, c);
  set_board_at(state, get_next_x(headX,current_c), get_next_y(headY,current_c),current_c);
  if (current_c=='W'){
    state->snakes[snum].head_y =state->snakes[snum].head_y -1;
  }if(current_c=='A') {
    state->snakes[snum].head_x =state->snakes[snum].head_x -1;
  }if (current_c=='S') {
    state->snakes[snum].head_y =state->snakes[snum].head_y +1;
  }if(current_c=='D'){
    state->snakes[snum].head_x =state->snakes[snum].head_x +1;
  }

//  if(next_square(state, snum)=='#'|| is_snake(next_square(state, snum))) {
//    set_board_at(state,
//                 get_next_x(headX, get_board_at(state, headX, headY)),
//                 get_next_y(headY, get_board_at(state, headX, headY)), 'x');
//  }else {
//    if (get_board_at(state, headX, headY) == 'W') {
//      set_board_at(state, headX, headY - 1, 'W');
//      set_board_at(state, headX, headY, '^');
//      headY = headY-1;
////      update int headX, headY?
//    }
//    if (get_board_at(state, headX, headY) == 'S') {
//      set_board_at(state, headX, headY + 1, 'S');
//      set_board_at(state, headX, headY, 'v');
//    }
//    if (get_board_at(state, headX, headY) == 'D') {
//      set_board_at(state, headX + 1, headY, 'D');
//      set_board_at(state, headX, headY, '>');
//    }
//    if (get_board_at(state, headX, headY) == 'A') {
//      set_board_at(state, headX - 1, headY, 'A');
//      set_board_at(state, headX, headY, '<');
//    }
//  }
  return;
}


/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^v<>) into a tail character (wasd)

  ...in the snake struct: update the x and y coordinates of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  unsigned int tailX= state->snakes[snum].tail_x;
  unsigned int tailY= state->snakes[snum].tail_y;
  char current_c = get_board_at(state, tailX, tailY);
  char direction_c = get_board_at(state, get_next_x(tailX,current_c), get_next_y(tailY,current_c));
  char c = body_to_tail( direction_c);
  set_board_at(state, tailX, tailY, ' ');
  set_board_at(state, get_next_x(tailX,current_c), get_next_y(tailY,current_c),c);
  if (current_c=='w'){
    state->snakes[snum].tail_y =state->snakes[snum].tail_y -1;
  }if(current_c=='a') {
    state->snakes[snum].tail_x =state->snakes[snum].tail_x -1;
  }if (current_c=='s') {
    state->snakes[snum].tail_y =state->snakes[snum].tail_y +1;
  }if(current_c=='d'){
    state->snakes[snum].tail_x =state->snakes[snum].tail_x +1;
  }
  return;
}


/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  unsigned int total_snake = state->num_snakes;
//  while(state->num_snakes !=0) {
//    int snum = 0;
    for (unsigned int snum = 0; snum < total_snake; ++snum) {
      if (state->snakes[snum].live == true) {
        if (next_square(state, snum) == '#' || is_snake(next_square(state, snum))) {
          set_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y, 'x');
          state->snakes[snum].live = false;
//          state->num_snakes = state->num_snakes - 1;
//          continue;
        }
        else if (next_square(state, snum) == '*') {
          add_food(state);
          update_head(state, snum);
//        update_head(state,snum);
//        update_tail(state,snum);
        } else {
          update_head(state, snum);
          update_tail(state, snum);
        }
      }
    }

    return;
  }


/* Task 5 */
  game_state_t* load_board(char* filename) {
    // TODO: Implement this function.
    char symbol=' ';
    unsigned int numc = 0;
    unsigned int numr = 0;
//    char actSymbol;

    game_state_t* state = (game_state_t*) malloc(sizeof(game_state_t));
    FILE* file = fopen(filename, "r");

    fscanf(file, "%c", &symbol);
    while (!feof(file)) {
      fscanf(file, "%c", &symbol);
      if (symbol == '\n') {
        numr = numr + 1;
      }
    }
      numr = numr - 1;

    state->num_rows = numr;
//    state->num_cols = numc;
  fclose(file);

  FILE* filesnd = fopen(filename, "r");
   char symbolf=' ';
    char** boardf = (char**) calloc(numr, sizeof(char*));
    for (int i = 0; i < numr; ++i) {
//      while(symbolf != '\n'){
//        fscanf(filesnd, "%c", &symbolf);
//        boardf[i] = realloc(boardf[i], (numc+1) * sizeof(char));
//        boardf[i][numc] = symbolf;
//        numc = numc+1;
//      }
      do{fscanf(filesnd, "%c", &symbolf);
        boardf[i] = realloc(boardf[i], (numc+1) * sizeof(char));
        boardf[i][numc] = symbolf;
        numc = numc+1;
      }while(symbolf != '\n');
      boardf[i][numc-1] = '\0';
//      boardf[i] = realloc(boardf[i], (numc-2) * sizeof(char));
      numc = 0;
    }
//    boardf[numr][0] = '\0';
//    boardf = realloc(boardf[i], (numc-1) * sizeof(char*));
//    for (int i = 0; i < numr; ++i) {
//      for (int j = 0; j < numc; ++j) {
//        fscanf(filesnd, "%c", &actSymbol);
//        boardf[i][j] = actSymbol;
//      }
//    }

    state->board = boardf;

    fclose(filesnd);

    return state;
  }


/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail coordinates filled in,
  trace through the board to find the head coordinates, and
  fill in the head coordinates in the struct.
*/
  static void find_head(game_state_t* state, unsigned int snum) {
    // TODO: Implement this function.
    unsigned int tailX = state->snakes[snum].tail_x;
    unsigned int tailY = state->snakes[snum].tail_y;
    char current_c = get_board_at(state, tailX, tailY);
    unsigned int current_x = tailX;
    unsigned int current_y = tailY;
    while (!is_head(current_c)) {
      current_x = get_next_x(current_x, current_c);
      current_y = get_next_y(current_y, current_c);
      current_c = get_board_at(state, current_x, current_y);
    }
    state->snakes[snum].head_x = current_x;
    state->snakes[snum].head_y = current_y;
    return;
  }


/* Task 6.2 */
  game_state_t* initialize_snakes(game_state_t* state) {
    // TODO: Implement this function.
    state->snakes = malloc(sizeof(snake_t));
    state->num_snakes = 0;
    for (unsigned int i = 0; i < state->num_rows; ++i) {
      for (unsigned int j = 0; j < strlen(state->board[i]); ++j) {
        if (is_tail(state->board[i][j])) {
          state->num_snakes = state->num_snakes + 1;
          state->snakes = realloc(state->snakes, state->num_snakes * sizeof(snake_t));
          snake_t tempSnake;
          tempSnake.tail_x = j;
          tempSnake.tail_y = i;
          tempSnake.live = true;
          state->snakes[state->num_snakes-1]= tempSnake;
          find_head(state, state->num_snakes-1);
        }
      }
    }

    return state;
  }

#endif
