#include <iostream>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <fstream>
#include <string>

// Defines static positions for horizontal alignment
#define dinoPos 6
#define hurdlePos 70

using namespace std;

// Console Output Handle
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

string name = "";
int dinoY = 0;      // Dino elevation height above ground
int speed = 35;     // Game loop speed in milliseconds
int gameover = 0;   // Game state status flag

// Function to reposition console cursor
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(console, coord);
}

// Function to hide cursor and prevent screen flickering
void set_cursor(bool visible, DWORD size)
{
    if (size == 0) size = 20;
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

// Initialize boundary borders, UI, and ground line
void init()
{
    system("cls");
    gameover = 0;

    // Header display
    gotoxy(3, 2);
    cout << "SCORE : 0";

    // Top and Bottom borders using '@'
    for (int i = 0; i < 79; i++)
    {
        gotoxy(1 + i, 1);
        cout << "@";
        gotoxy(1 + i, 25);
        cout << "@";
    }

    // Ground line using '#'
    gotoxy(1, 24);
    for (int i = 0; i < 79; i++)
    {
        cout << "#";
    }
}

// Draws Dino using '@' and '#' characters cleanly with animated legs
void move_dino(int jump = 0)
{
    static int foot = 0;

    // Update vertical jump height
    if (jump == 0) dinoY = 0;
    else if (jump == 1) dinoY++;
    else if (jump == 2) dinoY--;

    // Clear top and bottom line buffer to eliminate visual trails
    gotoxy(dinoPos, 14 - dinoY); cout << "                 ";
    gotoxy(dinoPos, 24 - dinoY); cout << "                 ";

    // Dino Body Frame (ASCII Layout)
    gotoxy(dinoPos, 15 - dinoY); cout << "        @@#@@@@@ ";
    gotoxy(dinoPos, 16 - dinoY); cout << "        @@@@@@@@ ";
    gotoxy(dinoPos, 17 - dinoY); cout << "        @@@@@### ";
    gotoxy(dinoPos, 18 - dinoY); cout << "@      @@@@@###  ";
    gotoxy(dinoPos, 19 - dinoY); cout << "@@@   @@@@@@@@@@ ";
    gotoxy(dinoPos, 20 - dinoY); cout << "#@@@@@@@@@@   #  ";
    gotoxy(dinoPos, 21 - dinoY); cout << "   #@@@@@@@#     ";

    // Dino Feet Alignment
    gotoxy(dinoPos, 22 - dinoY);
    if (jump > 0)
    {
        // Air state (Jumping)
        cout << "    @@# #@      ";
        gotoxy(dinoPos, 23 - dinoY);
        cout << "    @@   @@     ";
    }
    else if (foot == 0)
    {
        // Running animation step 1
        cout << "    #@@#  ###   ";
        gotoxy(dinoPos, 23 - dinoY);
        cout << "      @@        ";
        foot = 1;
    }
    else
    {
        // Running animation step 2
        cout << "     #@@ #@     ";
        gotoxy(dinoPos, 23 - dinoY);
        cout << "          @@    ";
        foot = 0;
    }

    if (jump > 0)
        Sleep(speed / 2);
    else
        Sleep(speed);
}

// Draw moving Cactus Hurdle and check Collision accurately
void draw_hurdle()
{
    static int plantX = 0;
    static int score = 0;

    int currentX = hurdlePos - plantX;

    // Clear trailing space on the right of the moving hurdle
    if (currentX < hurdlePos)
    {
        gotoxy(currentX + 6, 19); cout << " ";
        gotoxy(currentX + 6, 20); cout << " ";
        gotoxy(currentX + 6, 21); cout << " ";
        gotoxy(currentX + 6, 22); cout << " ";
        gotoxy(currentX + 6, 23); cout << " ";
    }

    // Draw Cactus shape at updated position
    if (currentX > 2)
    {
        gotoxy(currentX, 19); cout << " |  | ";
        gotoxy(currentX, 20); cout << " |  | ";
        gotoxy(currentX, 21); cout << " |__| ";
        gotoxy(currentX, 22); cout << "   |  ";
        gotoxy(currentX, 23); cout << "   |  ";
    }

    // Collision Detection Check
    if (currentX >= dinoPos + 2 && currentX <= dinoPos + 8 && dinoY < 5)
    {
        speed = 35;
        gotoxy(34, 10);
        cout << "  GAME OVER!  ";
        getch();
        gameover = 1;

        // Clean player name (replace spaces with '_') to prevent stream breaking
        string formattedName = name;
        if (formattedName.empty()) formattedName = "Player";
        for (size_t i = 0; i < formattedName.length(); i++)
        {
            if (formattedName[i] == ' ') formattedName[i] = '_';
        }

        // Save Score to score.txt
        ofstream myfile("score.txt", ios::app);
        if (myfile.is_open())
        {
            myfile << formattedName << " " << score << endl;
            myfile.close();
        }

        score = 0;
        plantX = 0;
        return;
    }

    plantX++;

    // Reset Cactus position when reaching the left border
    if (plantX >= hurdlePos - 2)
    {
        // Clear remaining plant visual artifacts at far left
        for (int row = 19; row <= 23; row++)
        {
            gotoxy(2, row); cout << "      ";
        }

        plantX = 0;
        score++;
        gotoxy(11, 2);
        cout << score;

        // Progressive speed increase
        if (speed > 15) speed--;
    }
}

// Main Play Function
void play()
{
    system("cls");
    char ch;
    init();

    while (true)
    {
        // Running loop while no key is pressed
        while (!kbhit())
        {
            if (gameover == 1) return;
            move_dino();
            draw_hurdle();
        }

        ch = getch();

        // Pause Game Mechanics (P Key)
        if (ch == 'p' || ch == 'P')
        {
            gotoxy(30, 10); cout << "=======================";
            gotoxy(30, 11); cout << "        PAUSED         ";
            gotoxy(30, 12); cout << "=======================";
            gotoxy(30, 13); cout << "Press 'p'/'P' to Resume";

            char pauseKey;
            do
            {
                pauseKey = getch();
            } while (pauseKey != 'p' && pauseKey != 'P');

            system("cls");
            init();
        }
        // Jump Mechanics (Spacebar Key)
        else if (ch == 32)
        {
            // Upward Jump Phase
            for (int i = 0; i < 10; i++)
            {
                move_dino(1);
                draw_hurdle();
                if (gameover) return;
            }
            // Downward Jump Phase
            for (int i = 0; i < 10; i++)
            {
                move_dino(2);
                draw_hurdle();
                if (gameover) return;
            }
        }
        // Exit to main menu (ESC Key)
        else if (ch == 27)
        {
            break;
        }
    }
}

// Instructions Menu
void instructions()
{
    system("cls");
    gotoxy(10, 4); cout << "INSTRUCTIONS";
    gotoxy(10, 5); cout << "===================================";
    gotoxy(10, 7); cout << "1. Press SPACEBAR to Jump.";
    gotoxy(10, 8); cout << "2. Press 'P' to Pause/Resume.";
    gotoxy(10, 9); cout << "3. Press 'ESC' to Exit to menu.";
    gotoxy(10, 12); cout << "Press any key to return.";
    getch();
}

// High Score Screen - Fixed to display Top 10 entries cleanly
void show()
{
    system("cls");
    ifstream open("score.txt");

    string names[100];
    int scores[100];
    int count = 0;

    // Load records safely into arrays using dynamic allocation pointers
    if (open.is_open())
    {
        while (count < 100 && open >> names[count] >> scores[count])
        {
            count++;
        }
        open.close();
    }

    // Sort scores in descending order (Bubble Sort)
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (scores[j] < scores[j + 1])
            {
                int tempScore = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tempScore;

                string tempName = names[j];
                names[j] = names[j + 1];
                names[j + 1] = tempName;
            }
        }
    }

    gotoxy(10, 3); cout << "PLAYER";
    gotoxy(35, 3); cout << "SCORE";
    gotoxy(10, 4); cout << "---------------------------------";

    int line = 5;
    if (count > 0)
    {
        // Display top 10 scores
        int limit = (count < 10) ? count : 10;
        for (int i = 0; i < limit; i++)
        {
            gotoxy(10, line); cout << names[i];
            gotoxy(35, line); cout << scores[i];
            line++;
        }
    }
    else
    {
        gotoxy(10, 6); cout << "No high scores recorded yet.";
        line = 7;
    }

    gotoxy(10, line + 2); cout << "Press any key to return.";
    getch();
}

// Program Main Entry Point
int main()
{
    set_cursor(false, 0);
    system("color f0");

    gotoxy(10, 4); cout << "Enter your name: ";
    getline(cin, name);

    while (true)
    {
        system("cls");
        gotoxy(10, 3); cout << "Welcome " << name << "!";
        gotoxy(10, 5); cout << "1. Start Game";
        gotoxy(10, 6); cout << "2. Instructions";
        gotoxy(10, 7); cout << "3. High Scores";
        gotoxy(10, 8); cout << "4. Exit";
        gotoxy(10, 10); cout << "Select Option: ";

        char op = getche();

        if (op == '1') play();
        else if (op == '2') instructions();
        else if (op == '3') show();
        else if (op == '4') exit(0);
    }

    return 0;
}
