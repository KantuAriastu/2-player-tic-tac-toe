/* Client side Tic Tac Toe Player 2       */
/* Ngakan Putu Ariastu , 6 February 2018  */
/* Description : to be run on client side of the game, client will be playing
                the X token as the player 2*/

/* Library declaration */
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080 //game using port 8080

/* function declaration */
void display_board(int board[3][3]);
int check_win (int board[3][3]);
void init_board (int board[3][3]);
int check_pos (int board[3][3],int pos[2]);
int check_draw (int board[3][3]);

/* main program */
int main()
{
    /* variable for connection setting */
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    // creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    /* Start Game */
    printf("connection establihed\n\n");
    printf("Starting New Game\n\n");
    printf("you are player 2\n\n");

    /* Variable for the game */
    int play = 0;
    int board[3][3];
    int pos[2];
    int done = 0;
    int win,draw;
    char input[20];

    init_board(board);//board initialization

    /* start game until one of the player win */
    while (done == 0)
    {
      display_board(board);


      if (play == 1) //play = 1 indicate this player turn
      {
        printf("Your turn...\n");
        printf("give X position : "); //give coordinate of token (0,0 -> 2,2)
        scanf("%s",input);
        pos[0] = atoi(input);
        pos[1] = atoi(input+2);

        if (check_pos(board,pos) ==  0) //check whether the position is available
        {
          printf("invalid input\n");
        }
        else if(check_pos(board,pos) == 1) //if available
        {
          send(sock , input , strlen(input) , 0 );//send position to other player
          board[pos[0]][pos[1]] = -1;  //set that position as X
          play = 0;   //change player turn
        }
    }
    else  //this is when the opponent turn
    {
      printf("opponent's turn...\n");
      valread = read( sock , input, 20);  //read ooponent position of O
      pos[0] = atoi(input);
      pos[1] = atoi(input+2);
      board[pos[0]][pos[1]] = 1; //set that position as O
      play = 1;
    }
    win = check_win(board);       //check win
    if ((win==3)||(win==-3))
      done = 1;
    draw = check_draw(board); //check draw
    if (draw == 1)
      done = 1;
  }
  display_board(board);
  if (win==3)
    printf("You lose\n");
  else if (win == -3)
    printf("You Win\n");
  if (draw==1)
    printf("Draw\n");

  return 0;
}

/* Function Implementation */
int check_pos(int board[3][3], int pos[2])
{
  int a = pos[0];
  int b = pos[1];
  if (board[a][b] == 0)
    return 1;
  else
    return 0;
}

void display_board(int board[3][3])
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      if ((board[i][j] == 1) && (j<2))
        printf(" O |");
      else if ((board[i][j] == -1) && (j<2))
        printf(" X |");
      else if ((board[i][j] == 1) && (j==2))
        printf(" O\n");
      else if ((board[i][j] == -1) && (j==2))
        printf(" X\n");
      else if ((board[i][j] == 0) && (j<2))
        printf("   |");
      else if ((board[i][j] == 0) && (j==2))
        printf("   \n");
    }
    printf (" --  --  --\n");
  }
}

void init_board (int board[3][3])
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      board[i][j] = 0;
    }
  }
}

int check_win (int board[3][3])
{
  int res;
  int i;
  for (i = 0; i < 3; i++)
  {
    res = board[i][0] + board[i][1] + board[i][2];
    if ((res == 3)||(res == -3))
      return res;
  }

  for (i = 0; i< 3; i++)
  {
    res = board[0][i] + board[1][i] + board[2][i];
    if ((res == 3)||(res == -3))
      return res;
  }

  res = board[0][0] + board[1][1] + board[2][2];
  if ((res == 3)||(res == -3))
    return res;

  res = board[0][2] + board[1][1] + board[2][0];
  if ((res == 3)||(res == -3))
    return res;

  return 0;
}

int check_draw (int board[3][3])
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      if (board[i][j] == 0)
        return 0;
    }
  }
  return 1;
}
