//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// height and width of game's paddle
#define PHEIGHT 10
#define PWIDTH 60

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // initial x and y velocity of the ball
    double xvelocity = drand48() + 0.5;
    double yvelocity = drand48() + 0.5;
    
    // object of collision
    GObject collision_object;

    waitForClick();
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // TODO
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        if(event != NULL)
        {
            if(getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - getWidth(paddle) / 2;
                setLocation(paddle, x, HEIGHT - 50);
            }
        }
        
        move(ball, xvelocity, yvelocity);
        
        if(getX(ball) + getWidth(ball) >= getWidth(window) || getX(ball) <= 0)
        {
            xvelocity = -xvelocity;
        }
        if(getY(ball) <= 0)
        {
            yvelocity = -yvelocity;
        }
        if(getY(ball) + getHeight(ball) >= getHeight(window))
        {
            lives--;
            setLocation(paddle, (WIDTH - PWIDTH) / 2, HEIGHT - 50);
            setLocation(ball, (WIDTH - 2 * RADIUS) / 2, (HEIGHT - 2 * RADIUS) / 2);
            xvelocity = drand48() + 0.5;
            yvelocity = drand48() +0.5;
            waitForClick();            
                        
        }
        
        collision_object = detectCollision(window, ball);
        
        if(collision_object != NULL && strcmp(getType(collision_object), "GRect") == 0)
        {
            yvelocity = -yvelocity;
            if(collision_object != paddle)
            {
                points+=10;
                bricks--;
                removeGWindow(window, collision_object);
                updateScoreboard(window, label, points);
            }
        }
        
        pause(2);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // TODO
    
    string colours[] = {"RED","YELLOW","GREEN","ORANGE","BLUE"};
    double x_start;
    int y_start = 10;
    for (int i = 0; i < ROWS; i++)
    {
        x_start = 2.5;
        for (int j = 0; j < COLS; j++) 
        {
            GRect brick = newGRect(x_start, y_start, 35, 10);
            setFilled(brick, true);
            setColor(brick, colours[i]);
            add(window, brick);
            x_start = x_start + 40;
        }

        y_start = y_start + 15;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // TODO
    GOval ball = newGOval((WIDTH - 2 * RADIUS) / 2, (HEIGHT - 2 * RADIUS) / 2, RADIUS, RADIUS);
    setFilled(ball, true);
    setColor(ball, "BLACK");
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // TODO
    GRect paddle = newGRect((WIDTH - PWIDTH)/2, HEIGHT - 50, PWIDTH, PHEIGHT);
    setFilled(paddle, true);
    setColor(paddle, "BLUE");
    add(window, paddle);
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO
    GLabel label = newGLabel("0");
    double x, y;
    setFont(label, "SansSerif-18");
    x = (getWidth(window) - 180);
    y = (getHeight(window) + getFontAscent(label)) / 2;
    setLocation(label, x, y);
    add(window, label);
    setColor(label, "GRAY");
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
