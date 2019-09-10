/* Server side Tic Tac Toe Player 1       */
/* Ngakan Putu Ariastu , 6 February 2018  */
/* Description : to be run on server side of the game, server will be playing
                the O token as the player 1*/


/* Library declaration*/
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
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf ("Waiting for connection\n\n");

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    /* start game */
    printf("connection establihed\n\n");
    printf("Starting New Game\n\n");
    printf("you are player 1\n\n");

    /* variable for the game */
    int play = 1;
    int board[3][3];
    int pos[2];
    int done = 0;
    int win, draw;
    char input[20];

    init_board(board); //board initialization

    /* start game until one of the player win */
    while (done == 0)
    {
      display_board(board);     //display current status of the board

      if (play == 1)            //play = 1 indicate this player turn
      {
        printf("Your turn...\n");
        printf("give O position : ");
        scanf("%s",input);     //give coordinate of token (0,0 -> 2,2)
        pos[0] = atoi(input);
        pos[1] = atoi(input+2);

        if (check_pos(board,pos) ==  0) //check whether the position is available
        {
          printf("invalid input\n");
        }
        else if (check_pos(board,pos) == 1) //if available
        {
          send(new_socket, input, strlen(input), 0 ); //send position to other player
          board[pos[0]][pos[1]] = 1;    //set that position as O
          play = 0;               //change player turn
        }
    }
    else        //this is when the opponent turn
    {
      printf("opponent's turn...\n");
      valread = read( new_socket , input, 20);    //read opponent position of X
      pos[0] = atoi(input);
      pos[1] = atoi(input+2);
      board[pos[0]][pos[1]] = -1;     //set that position as X
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
    printf("You Win\n");
  else if (win == -3)
    printf("You lose\n");
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
