#include <iostream>
#include "snake2.h"
#include <stdlib.h>
#include <thread>

using namespace std;

int main()
{
    srand(time(NULL));
    Snake s1(24,13,'L',5);

    WriteTextAtLoc("Press any key to start...", 12, 12);
    getch();
    WriteTextAtLoc("                         ", 12, 12);
    int speed = (31-s1.level)*10;

    thread t1(collectKeyEvents); //inputs tracking key events simultaneously
    while(!gameOverCondition)
    {
        Sleep(speed);
        while(pause)
            Sleep(1);

        if(!keys.empty())
        {
            direction = keys.front();
            keys.pop();
        }
        s1.program();
    }
    t1.detach();

    Locate(21,11);
    cout << "Game Over!";
    Locate(16,13);
    cout << "Press 'ESC' to exit";
    Locate(15,14);
    cout << "'Enter' to Play Again";

    while(1)
    {
        switch(getch())
        {
        case '\r':
            system("cls");
            s1.resetData();
            return main();
        case 27:
            system("cls");
            return 0;
        }
    }
}
