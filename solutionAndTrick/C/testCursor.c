#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

#define COLS 60
#define ROWS 30

int main() {
  // Hide cursor
  printf("\e[?25l");

  // Switch to canonical mode, disable echo
  /*These lines configure the terminal \ 
  to operate in non-canonical mode (immediate input processing) \
  and to disable echoing of characters typed by the user.*/
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  int x[1000], y[1000];
  int quit = 0;
  while (!quit) {
    // Render table
    printf("┌");
    for (int i = 0; i < COLS; i++)
      printf("─");
    printf("┐\n");

    for (int j = 0; j < ROWS; j++) {
      printf("│");
      for (int i = 0; i < COLS; i++)
        printf("·");
      printf("│\n");
    }

    printf("└");
    for (int i = 0; i < COLS; i++)
      printf("─");
    printf("┘\n");
    printf("HELLOWORLD1\n");
	 
    // Move cursor back to top
    printf("\e[%iA", ROWS + 2);
    printf("HELLOWORLD2\n");
    printf("\e[%iB\e[%iC❤", 11, COLS/2 + 1); 
	 printf("\e[%iF", 10);
	 printf("HELLOWORLD3\n");
    sleep(7);
    }
    return 0;   
    
}
