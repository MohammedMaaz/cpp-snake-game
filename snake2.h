#include "console_formatting.h"
#include <queue>

using namespace std;

//global variables for increasing performance
queue<char> keys;
bool gameOverCondition=false;
char direction;
bool pause = false;

class Snake
{
private:
    struct bodyUnit
    {
        int x;
        int y;
    };

    vector<struct bodyUnit> snake;
    struct bodyUnit last, food, bounty, bountySize={3,2}, scorePos={23,0}, bScorePos={37,0};
    const int height=26, width=51;
    bool walls=false;
    string snakeColor = "L Red", wallsColor = "L Green", foodColor = "L Yellow", bountyColor="L Cyan";
    int score=0, bScore=0;
    int snakeColorCode = clrStrToCode(snakeColor), wallsColorCode = clrStrToCode(wallsColor);
    int newHeadColor;
    short bountyTimer=-1, bountyPeriod=60, bCounter=0, bountyFreq=10;

    void createFood()
    {
    bool repeat = true;
    int initFColor;

    while(repeat) //decide where to place food, it should not be on the body of snake
    {
        food.x = rand()%(width-1)+1;
        food.y = rand()%(height-1)+2;

        initFColor = getPixelColor(food.x, food.y);
        if( initFColor!=0 ) //if it is not black
            repeat = true;
        else
            repeat = false;
    }
    colorPixel(food.x, food.y, foodColor);
    }

    void createBounty()
    {
    repeatBlock:
        bounty.x = rand()%(width-bountySize.x)+1;
        bounty.y = rand()%(height-bountySize.y)+2;

        for(int i=bounty.x; i<bounty.x+bountySize.x; ++i)
        {
            for(int j=bounty.y; j<bounty.y+bountySize.y; ++j)
            {
                if(getPixelColor(i,j)!=0)// if it is not black
                {
                    goto repeatBlock;
                }
            }
        }
        clrdLine(bountyColor, bounty.x, bounty.y, bounty.x+bountySize.x, bounty.y+bountySize.y);
    }

    void displayBountyMeter()
    {
        for(int i=1; i<=20; ++i)
            colorPixel(i, 0, bountyColor);
    }

    void clearBountyMeter()
    {
        for(int i=1; i<=20; ++i)
            colorPixel(i,0, "Black");
    }

public:
    int level = 28;

    Snake(int startX,int startY,char initialDirection,int initLength=2)
    {
        //fix console dimensions
        setConsoleDimensions(width+1,height+2);
        //hide scrollbars
        setConsoleScrollBarVisibility(false);
        //hide cursor
        cursorVisAndSize(0);
        //display initial score
        Locate(scorePos.x, scorePos.y);
        cout << "Score: " << score;
        //display initial bounty score position
        Locate(bScorePos.x, bScorePos.y);
        cout << "Bounty: " << bScore;
        //set initial direction
        direction = initialDirection;
        keys.push(direction);
        switch(initialDirection)
        {
        case 'L':
            for(int i=startX; i<startX+initLength; ++i)
                snake.push_back({i, startY});
            break;
        case 'R':
            for(int i=startX; i>startX-initLength; --i)
                snake.push_back({i, startY});
            break;
        case 'U':
            for(int i=startY; i<startY+initLength; ++i)
                snake.push_back({startX, i});
            break;
        case 'D':
            for(int i=startY; i>startY-initLength; --i)
                snake.push_back({startX, i});
            break;
        }

        //creating walls
        for(int i=0; i<=width; ++i)
        {
            colorPixel(i,1, wallsColor);
            colorPixel(i,height+1, wallsColor);
        }
        for(int i=1; i<=height+1; ++i)
        {
            colorPixel(0,i, wallsColor);
            colorPixel(width,i, wallsColor);
        }

        //create snake initially
        for(int i=0; i<snake.size(); ++i)
            colorPixel(snake[i].x, snake[i].y, snakeColor);

        //create food initially
        createFood();
    }

    void program()
    {
        //preserve last body unit to be erased later
        last = snake[snake.size()-1];

        //every bodyUnit except the head follows its next one
        for(int i=snake.size()-1; i>0; --i)
        {
            snake[i] = snake[i-1];
        }

        //if snake eats food
        if(snake[0].x==food.x && snake[0].y==food.y)
        {
            score = score+level; //update score
            bCounter++; //update bounty counter
            //display score
            Locate(scorePos.x+7,scorePos.y);
            cout << score;
            snake.push_back(last); //then push one unit in the end and don't erase the tail
            createFood(); //create new food;

            if(bCounter%bountyFreq == 0)
            {
                createBounty();
                displayBountyMeter();
                bountyTimer=0;
            }
        }
        else
        {
            colorPixel(last.x, last.y, "Black"); //erase last tail
        }

        //if snake eats bounty
        if(snake[0].x>=bounty.x&&snake[0].x<bounty.x+bountySize.x &&
            snake[0].y>=bounty.y&&snake[0].y<bounty.y+bountySize.y)
        {
            //then clear the bounty from screen (snake head also get's clear)
            for(int i=bounty.x; i<bounty.x+bountySize.x; ++i)
            {
                for(int j=bounty.y; j<bounty.y+bountySize.y; ++j)
                {
                    colorPixel(i,j, "Black");
                }
            }
            colorPixel(snake[0].x, snake[0].y, snakeColor); //reset snake's head back to it's color

            //clear bounty meter
            clearBountyMeter();
            //update bounty score
            bScore = bScore + (bountyPeriod-bountyTimer)*level;
            //display bounty score
            Locate(bScorePos.x+8,bScorePos.y);
            cout << bScore;

            bounty.x=200; bounty.y=200; //reset bounty position
            bountyTimer=-1; //set bounty expiry condition to true
        }

        //checking bounty state
        if(bountyTimer>-1) //bounty present
        {
            colorPixel(bountyTimer/3, 0, "Black");
            if(bountyTimer==bountyPeriod) //if bounty expires
            {
                bountyTimer=-1; //set bounty expire condition to true
                //remove bounty
                for(int i=bounty.x; i<bounty.x+bountySize.x; ++i)
                    for(int j=bounty.y; j<bounty.y+bountySize.y; ++j)
                        colorPixel(i,j, "Black");
                bounty.x=200; bounty.y=200; //reset bounty position
            }
            else //if bounty not expires
            {

                ++bountyTimer;
            }
        }


        //head follows the given direction
        switch(direction)
        {
        case 'L':
            if(snake[0].x == (1-walls))
            {
                snake[0].x = width-1;
            }
            else
                snake[0].x = snake[0].x-1;
            break;
        case 'R':
            if(snake[0].x == width-(1-walls))
            {
                snake[0].x = 1;
            }
            else
                snake[0].x = snake[0].x+1;
            break;
        case 'U':
            if(snake[0].y == 1+(1-walls))
            {
                snake[0].y = height;
            }
            else
                snake[0].y = snake[0].y-1;
            break;
        case 'D':
            if(snake[0].y == height+1-(1-walls))
            {
                snake[0].y = 2;
            }
            else
                snake[0].y = snake[0].y+1;
            break;
        }
        //game over condition
        newHeadColor = getPixelColor(snake[0].x, snake[0].y);
        if( newHeadColor==snakeColorCode || (newHeadColor==wallsColorCode && walls) )
            gameOverCondition = true;
        else
            colorPixel(snake[0].x, snake[0].y, snakeColor); //move the head to the new position
    }

    resetData()
    {
        queue<char> dummy;
        keys = dummy;
        gameOverCondition=false;
        pause = false;
    }

};

void collectKeyEvents()
{
while(1)
{
    if (_kbhit())
    {
        switch (_getch())
        {
        case 75:
            if(keys.back()!='R' && !pause)
                keys.push('L');
            break;

        case 77:
            if(keys.back()!='L' && !pause)
                keys.push('R');
            break;

        case 72:
            if(keys.back()!='D' && !pause)
                keys.push('U');
            break;

        case 80:
            if(keys.back()!='U' && !pause)
                keys.push('D');
            break;

        case 32:
            pause=!pause;
            if(pause)
                WriteTextAtLoc("Paused", 23,13);
            else
                WriteTextAtLoc("      ", 23,13);
            break;
        }//end switch
    }
    Sleep(1);
}//end while
}


