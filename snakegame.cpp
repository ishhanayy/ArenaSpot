#include <bits/stdc++.h>
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

#define Max_Lenght 1000

// defining the characters for directions which will tell in which direction to go or what is the direction
const char Dir_Up = 'U';
const char Dir_down = 'D';
const char Dir_left = 'L';
const char Dir_Right = 'R';

const int SnakeColor = 10;   // Green
const int FoodColor = 12;    // Red
const int BackgroundColor = 14; // Black

int consoleWidht, consoleHeight;

void initScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidht = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

struct point // in the struct(structure) access modifier is bydefault public
{
    int xcoordinate;
    int ycoordinate;
    point()
    {
        // Empty constructor
    }
    point(int x, int y)
    {
        xcoordinate = x;
        ycoordinate = y;
    }
};

class snake // in the class access modifier is bydefault private
{
    int length;
    char direction;

public:
    point body[Max_Lenght];
    snake(int x, int y)
    {
        this->length = 1; // at the starting the snake will be intitiated with the body of length 1
        body[0] = point(x, y);
        this->direction = Dir_Right; // making the initial heading direction as right
    }
    // ~snake()
    // {
    //     //as the body array can be deallocated automatically, but for caution we are handling to delete it manually
    //     delete[] body;
    // }

    int getLength()
    {
        return length;
    }

    void changeDirection(char newDirection)
    {
        if (newDirection == Dir_Up && direction != Dir_down)
        {
            direction = newDirection;
        }
        else if (newDirection == Dir_down && direction != Dir_Up)
        {
            direction = newDirection;
        }
        else if (newDirection == Dir_Right && direction != Dir_left)
        {
            direction = newDirection;
        }
        else if (newDirection == Dir_left && direction != Dir_Right)
        {
            direction = newDirection;
        }
    }

    bool move(point food)
    {
        for (int i = length - 1; i > 0; i--)
        {
            body[i] = body[i - 1];
        }

        switch (direction)
        {
            int val;

        case Dir_Up:
            val = body[0].ycoordinate;
            body[0].ycoordinate = val - 1;
            break;

        case Dir_down:
            val = body[0].ycoordinate;
            body[0].ycoordinate = val + 1;
            break;

        case Dir_left:
            val = body[0].xcoordinate;
            body[0].xcoordinate = val - 1;
            break;

        case Dir_Right:
            val = body[0].xcoordinate;
            body[0].xcoordinate = val + 1;
            break;
        }

        // if the snake bite itslef
        for (int i = 1; i < length; i++)
        {
            if (body[0].xcoordinate == body[i].xcoordinate && body[0].ycoordinate == body[i].ycoordinate)
            {
                return false;
            }
        }

        // if the snake eats the food
        if (food.xcoordinate == body[0].xcoordinate && food.ycoordinate == body[0].ycoordinate)
        {
            body[length] = point(body[length - 1].xcoordinate, body[length - 1].ycoordinate);
            length++;
        }

        return true;
    }
};

class board
{
    snake *Snake;
    const char snake_body = 'o';
    point food;
    const char Target_food = '*';
    int score;

public:
    board()
    {
        spawnFood();
        Snake = new snake(10, 10);
        score = 0;
    }
    ~board()
    {
        // because the snake is created dynamically
        delete Snake;
    }

    void spawnFood()
    {
        // in the game we know the food will be created randomly if one food is eaten
        int x = rand() % consoleWidht;
        int y = rand() % consoleHeight;
        food = point(x, y);
    }

    int getScore()
    {
        return score;
    }

    void displayScore()
    {
        gotoxy(consoleWidht / 2, 0);
        cout << "Current Score: " << score << endl;
    }

    void gotoxy(int x, int y) // it iwll be used to draw a point on the console screen
    {
        COORD coordinate;
        coordinate.X = x;
        coordinate.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
    }

    void draw()
    {
        system("cls");
        for (int i = 0; i < Snake->getLength(); i++)
        {
            gotoxy(Snake->body[i].xcoordinate, Snake->body[i].ycoordinate);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), SnakeColor);
            cout << snake_body;
        }

        gotoxy(food.xcoordinate, food.ycoordinate);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FoodColor);
        cout << Target_food;

        displayScore();
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    }

    bool update()
    {
        bool isAlive = Snake->move(food);
        if (isAlive==false)
        {
            return false;
        }
        if(food.xcoordinate == Snake->body[0].xcoordinate && food.ycoordinate == Snake->body[0].ycoordinate)
        {
            score++;
            spawnFood();
        }
        return true;
    }

    void getInput()
    {
        if (kbhit())
        {
            int key = getch();
            if (key == 'W' || key == 'w')
            {
                Snake->changeDirection(Dir_Up);
            }
            else if (key == 'A' || key == 'a')
            {
                Snake->changeDirection(Dir_left);
            }
            else if (key == 'S' || key == 's')
            {
                Snake->changeDirection(Dir_down);
            }
            else if (key == 'D' || key == 'd')
            {
                Snake->changeDirection(Dir_Right);
            }
        }
    }
};

int main()
{
    srand(time(0));
    initScreen();
    board *BoardGame = new board();
    while (BoardGame->update())
    {
        BoardGame->getInput();
        BoardGame->draw();
        Sleep(100);
    }
    cout << "Game Over" << endl;
    cout << BoardGame->getScore() << endl;
    return 0;
}
