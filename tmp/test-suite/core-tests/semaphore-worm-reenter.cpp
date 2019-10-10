#include "rodos.h"
#include "random.h"


/** Warning:
	this test shows if a thread may reenter
	a semaphore severaltimes without deadlock.
**/

static Application module("semaphoretest");

#define MAX_X 30
#define MAX_Y 30
#define RIGHT_LIMIT 20

#define MAX_STEPS 1000      //Maximale Anzahl der Schritte eines Wurms

Semaphore lowerWindow;
Semaphore printProtector;

bool occupied[MAX_X][MAX_Y] = { { 0, 0, 0, 0, 0, 0 }, { 0, 0, 0 } };

static int prio = 1;        //Priorität des Threads, priority ist bereits in Rodos vergeben

void MAIN() {
    PRINTF(SCREEN_CLEAR);
    for(int i = 0; i < MAX_X; i++) {
        PRINTF(SCREEN_MOVE_CURSOR_TO, i, RIGHT_LIMIT + 1);
        PRINTF(":");
    }
}

static char wormLetter = 'A';

class worm : public Thread {
    int  x, y, speed_x, speed_y;
    char letter;

  public:
    worm() : Thread("Worm", prio) {
        x       = randomTT800Positive() % RIGHT_LIMIT;
        y       = randomTT800Positive() % MAX_Y + 1;
        speed_x = (randomTT800Positive() % 3) - 1;
        speed_y = (randomTT800Positive() % 3) - 1;
        letter  = wormLetter++;
        
        prio += 100;
    }

    void run() {

        int last_x, last_y;
        for (int i = 0; i < MAX_STEPS; i++) {
            occupied[x][y] = false;
            last_x         = x;
            last_y         = y;
           
            bool inUpperSide = x <= RIGHT_LIMIT;
            bool inLowerSide = x > RIGHT_LIMIT;


            // Change speed randomly
            if(randomTT800Positive() % 10 < 2) {
                speed_x = (randomTT800Positive() % 3) - 1;
                speed_y = (randomTT800Positive() % 3) - 1;
                if(speed_x == 0 && speed_y == 0) speed_y = 1;
            }


            if(x >= MAX_X-1) speed_x = -1;
            if(y >= MAX_Y-1) speed_y = -1;
            if(x <= 1) speed_x = 1;
            if(y <= 1) speed_y = 1;
            x += speed_x;
            y += speed_y;

            if(occupied[x][y]) {
                x = last_x;
                y = last_y;
            }
            occupied[x][y] = true;

            printProtector.enter();
            printProtector.enter();
            printProtector.enter();
            PRINTF("%c %d %d \n", letter, y, x);
            printProtector.leave();
            printProtector.leave();
            printProtector.leave();

            if(inUpperSide && x > RIGHT_LIMIT) {
                lowerWindow.enter();
                lowerWindow.enter();
            }
            if(inLowerSide && x <= RIGHT_LIMIT) {
                lowerWindow.leave();
                lowerWindow.leave();
            }

            suspendCallerUntil(NOW() + 100 * MILLISECONDS);
        }
    }
};


worm wrms[3];
