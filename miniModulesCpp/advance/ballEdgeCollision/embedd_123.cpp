/* Author: devh
 * Update: 19/1/2025
*/

// g++ -o embedd_123 embedd_123.cpp `pkg-config --cflags --libs gtk+-3.0`
// ./embedd_123 to run
#include "ball.h"

int main(int argc, char *argv[])
{
    ball_main(argc, argv);

    return 0;
}
