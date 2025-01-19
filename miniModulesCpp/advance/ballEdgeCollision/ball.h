/* Author: devh
 * Update: 19/1/2025
*/
#ifndef _BALL_H_
#define _BALL_H_

#include <iostream>
#include <gtk/gtk.h>
#include <cstdlib>
#include <ctime>
#include <chrono>  // For timestamp logging

#include <stdlib.h>
#include <time.h>
#include <random>
#include <vector>

#define NUM_BALLS 200
#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1400

#define CENTER_X_WD WINDOW_WIDTH/2
#define CENTER_Y_WD WINDOW_HEIGHT/2

float getFloatRandom(float minNum , float maxNum){
    //std::random_device rd;
    // random generator
    //srand(time(NULL));
    //return rand() % (maxNum - minNum + 1) + minNum;
    //std::default_random_engine gen;
    //
    // Engines
    //
    //std::mt19937 e2(rd());
    //std::knuth_b e2(rd());
    //std::default_random_engine e2(rd()) ;

    //
    // Distribtuions
    //
    //std::uniform_real_distribution<float> distribution(minNum, maxNum);
    //std::normal_distribution<> dist(2, 2);
    //std::student_t_distribution<> dist(5);
    //std::poisson_distribution<> dist(2);
    //std::extreme_value_distribution<> dist(0,2);

    //return distribution(gen);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<float> distribution(minNum, maxNum);
    float dist = distribution(rng);
    //std::cout << "dist = " << dist << std::endl;

    return dist;


}

struct ColorRGB {
    float fRed, fGreen, fBlue;
};

struct Ball {
    float x, y;          // Position of the ball
    float dx, dy;        // Direction and speed of movement
    float radius;        // Radius of the ball
    float loopEdgeX, loopEdgeY;
    ColorRGB rgb_t;
};


class WBall {
public:
    struct Ball mball;
    WBall(){

    };
    void initDefaultBall();
    void moveBall(GtkWidget *widget);
    //gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data);
    //gboolean on_timeout(gpointer data);
};

struct CallbackData {
    //WBall* ball;
    std::vector<WBall> *lsBall;
    GtkWidget* drawing_area;
};

// The ball object
Ball ball;

// Function to initialize the ball's attributes
void WBall::initDefaultBall() {
    mball.x = CENTER_X_WD;
    mball.y = CENTER_Y_WD;
    mball.dx = getFloatRandom(0.5f, 4.0f);
    mball.dy = getFloatRandom(0.5f, 4.0f);
    mball.radius = getFloatRandom(20.0f, 50.0f);
    mball.rgb_t.fRed = getFloatRandom(0.0f, 1.0f);
    mball.rgb_t.fGreen = getFloatRandom(0.0f, 1.0f);
    mball.rgb_t.fBlue = getFloatRandom(0.0f, 1.0f);
}

// Simple logger function with timestamp
//void log_collision(const std::string& axis) {
//    using namespace std::chrono;
//    auto now = system_clock::now();
//    auto time = system_clock::to_time_t(now);
//    std::cout << "[" << std::ctime(&time) << "] Collision detected, reverse direction on the " << axis << " axis." << std::endl;
//    std::cout << "ball.x = " << ball.x << " ball.dx = " << ball.dx << std::endl;
//    std::cout << "ball.y = " << ball.y << " ball.dy = " << ball.dy << std::endl;
//}

// Function to update the ball's position
void WBall::moveBall(GtkWidget *widget) {
    // Get the actual size of the window (widget)
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    // Update the position based on speed
    mball.x += mball.dx;
    mball.y += mball.dy;

    // Check for collision with the window borders
    if (mball.x - mball.radius <= 0 || mball.x + mball.radius >= width) {
        mball.dx = -mball.dx;  // Reverse direction on the X-axis
        mball.x += mball.dx;
        //std::cout << "Reverse direction on the X-axis \n";
    }
    if (mball.y - mball.radius <= 0 || mball.y + mball.radius >= height) {
        mball.dy = - mball.dy;  // Reverse direction on the Y-axis
        mball.y += mball.dy;
        //std::cout << "Reverse direction on the Y-axis \n";
    }

    // TODO : APP still work when be resized - But if you scroll a edge of window to near mid-ball -> have problems
    if (mball.x - mball.radius <= 0 || mball.x + mball.radius >= width) {
        int centerXforward = mball.x - (width/2); //get direction forward
        mball.loopEdgeX ++;
        mball.x = (width/2) + centerXforward + mball.loopEdgeX*mball.dx;
    } else {
        mball.loopEdgeX = 0.0f;
    }
    if (mball.y - mball.radius <= 0 || mball.y + mball.radius >= height) {
        int centerYforward = mball.y - (height/2);
        mball.loopEdgeY ++;
        mball.y = (height/2) + centerYforward + mball.loopEdgeY*mball.dy;
    } else {
        mball.loopEdgeY = 0.0f;
    }

    // Trigger a redraw
    gtk_widget_queue_draw(widget);
}

// Drawing function to render the ball
gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    std::vector<WBall>* pBall = static_cast<std::vector<WBall>*>(data);

    for (auto &vBall : *pBall) {  // Dereference pBall to access the vector
        cairo_set_source_rgb(cr, vBall.mball.rgb_t.fRed, vBall.mball.rgb_t.fGreen, vBall.mball.rgb_t.fBlue); // Set the color for the ball
        cairo_arc(cr, vBall.mball.x, vBall.mball.y, vBall.mball.radius, 0, 2 * G_PI);  // Draw the ball
        cairo_fill(cr);  // Fill the circle with the chosen color
    }


    return FALSE;
}

// Timer callback to update the ball's position
gboolean on_timeout(gpointer data) {
    // Cast the data back to the CallbackData struct
    CallbackData* callback_data = static_cast<CallbackData*>(data);

    // Access both aBall and drawing_area
    std::vector<WBall>* pBall = callback_data->lsBall;
    GtkWidget* widget = callback_data->drawing_area;
    for(auto &vBall : *pBall){
        vBall.moveBall(widget);  // Move the ball based on the physics
    }

    return TRUE;        // Continue the timer
}

void wBallListInit(std::vector<WBall>& BallVec) {
    for (auto& nBall : BallVec) {
        nBall.initDefaultBall();  // Initialize each WBall object
    }
}

void changeColorVecBall(std::vector<WBall>& BallVec) {
    for (auto& nBall : BallVec) {
        nBall.mball.rgb_t.fRed = getFloatRandom(0.0f, 1.0f);  // update color
        nBall.mball.rgb_t.fGreen = getFloatRandom(0.0f, 1.0f);
        nBall.mball.rgb_t.fBlue = getFloatRandom(0.0f, 1.0f);
    }
}

void changeDelxyVecBall(std::vector<WBall>& BallVec) {
    for (auto& nBall : BallVec) {
        nBall.mball.dx = -nBall.mball.dx;  // update color
        nBall.mball.dy = -nBall.mball.dy;
    }
}

void speedUpDelxyVecBall(std::vector<WBall>& BallVec) {
    for (auto& nBall : BallVec) {
        nBall.mball.dx += ((int)nBall.mball.dx & (1 << (sizeof(nBall.mball.dx) * 8 - 1)))==0?(1):(-1);//check negative number
        nBall.mball.dy += ((int)nBall.mball.dy & (1 << (sizeof(nBall.mball.dy) * 8 - 1)))==0?(1):(-1);
    }
}

void speedDownDelxyVecBall(std::vector<WBall>& BallVec) {
    for (auto& nBall : BallVec) {
        nBall.mball.dx -= ((int)nBall.mball.dx & (1 << (sizeof(nBall.mball.dx) * 8 - 1)))==0?(1):(-1);//check negative number
        nBall.mball.dy -= ((int)nBall.mball.dy & (1 << (sizeof(nBall.mball.dy) * 8 - 1)))==0?(1):(-1);
    }
}


// Key event callback to move the ball
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    std::vector<WBall>* pBall = static_cast<std::vector<WBall>*>(data);

    // Process key events to move the ball
    switch (event->keyval) {
        case GDK_KEY_Up:
            changeColorVecBall(*pBall);  // Move up (increase the vertical speed)
            break;

        case GDK_KEY_Down:
            changeDelxyVecBall(*pBall);  // Move down (decrease the vertical speed)
            break;

        case GDK_KEY_Left:
            speedDownDelxyVecBall(*pBall);  // Move left (decrease the horizontal speed)
            break;

        case GDK_KEY_Right:
            speedUpDelxyVecBall(*pBall);  // Move right (increase the horizontal speed)
            break;

        default:
            break;
    }

    gtk_widget_queue_draw(widget);  // Request a redraw after moving the ball
    return TRUE;  // Indicate that the event has been handled
}




int ball_main(int argc, char *argv[]) {

    std::vector<WBall> BallVec(NUM_BALLS);  // A vector of 10 WBall objects
    // Initialize all balls
    wBallListInit(BallVec);

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Moving Ball");
    gtk_window_set_default_size(GTK_WINDOW(window), 1400, 1000);

    // Create a drawing area to display the ball
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    // Connect the key press event signal to the callback
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), &BallVec);

    // Connect signals
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw), &BallVec);


    // Create an instance of CallbackData and set the parameters
    CallbackData callback_data = { &BallVec, drawing_area };

    // Set up a timer to move the ball every 16ms (~60 FPS)
    g_timeout_add(16, on_timeout, &callback_data);

    // Show all the widgets
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}


#endif
