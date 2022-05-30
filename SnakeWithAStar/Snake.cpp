#include "Snake.h"

bool operator==(COORD a, COORD b) {
    return a.X == b.X && a.Y == b.Y;
}

/**
* Initialize the console with the basic setup of width, height and disables resizing
* Generates first part of snake and food
*/
Snake::Snake(const short& width, const short& height, const unsigned int& period = 500) {
    HWND consoleWindow = GetConsoleWindow();
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFOEX consolesize;
    CONSOLE_CURSOR_INFO cursorInfo;
    consolesize.cbSize = sizeof(consolesize);

    GetConsoleScreenBufferInfoEx(hConsole, &consolesize);
    winDimensions = { width, height };
    consolesize.dwSize = winDimensions;
    consolesize.dwMaximumWindowSize = winDimensions;
    consolesize.srWindow = { 0, 0, width, height };
    SetConsoleScreenBufferInfoEx(hConsole, &consolesize);
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

    snake.push_back({ (short)(width / 2), (short)(height / 2) });

    generateFood();

    SetConsoleCursorPosition(hConsole, snake.front());
    WriteConsole(hConsole, (char*)"#", 1, NULL, NULL);

    this->period = period;

    start = std::chrono::steady_clock::now();
}

/**
* Makes sure, that the given direction does not collide with the snake => it cannot go backwards to itself
*/
const void Snake::move(const char& direction) {
    if (this->direction + direction != 0) this->direction = direction;
}

/*
* Takes care of generating the snake, checking if it collided with the walls or itself, checking if the food was eaten and generates new food. Also draws the snake
* TODO: Should probably separate into different functions
*/
const void Snake::render(bool* cont, bool* ateFood = nullptr, bool* rendered = nullptr) {
    now = std::chrono::steady_clock::now();

    // Checks if the required period defined in ms already passed
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() >= period && direction != 0) {
        // Defines newPart as copy snake's head and depending on the direction will change the needed coordinate
        COORD newPart = snake.front();
        switch (direction) {
        case UP: // UP
            newPart.Y--;
            break;
        case LEFT: // LEFT
            newPart.X--;
            break;
        case DOWN: // DOWN
            newPart.Y++;
            break;
        case RIGHT: // RIGHT
            newPart.X++;
            break;
        }
        snake.push_front(newPart); // Pushes the new part to the snake deque

        // Checks for collision and returns if snake collided with either itself or the walls
        *cont = !collisionCheck();
        if (!(*cont)) {
            return;
        }

        // Draws snake's head
        SetConsoleCursorPosition(hConsole, snake.front());
        WriteConsole(hConsole, (char*)"#", 1, NULL, NULL);

        // Checks if snake ate food
        // If it did, it will generate and draw new food
        if (snake.front() == food) {
            if (snake.size() == winDimensions.X * winDimensions.Y) {
                std::cout << "Congratulations!\nYou won!";
            }
            generateFood();
            *ateFood = true;
        }
        // Else it will erase snake's tail from the deque and from the console window
        else {
            SetConsoleCursorPosition(hConsole, snake.back());
            WriteConsole(hConsole, (char*)" ", 1, NULL, NULL);
            snake.pop_back();

            *ateFood = false;
        }
        *rendered = true;
        start = std::chrono::steady_clock::now();
    }
    else *rendered = false;
}

/**
* Game loop of the snake game
*/
const void Snake::gameLoop() {
    char dir = 0;
    bool cont = true;
    bool rend, ate;

    // Game loop
    while (cont) {
        // Checks for pressed keys
        if (GetKeyState(VK_UP) & 0x8000) dir = UP;
        else if (GetKeyState(VK_DOWN) & 0x8000) dir = DOWN;
        else if (GetKeyState(VK_LEFT) & 0x8000) dir = LEFT;
        else if (GetKeyState(VK_RIGHT) & 0x8000) dir = RIGHT;
        move(dir);
        render(&cont, &ate, &rend);
    }

    system("cls");
    std::cout << "Game over!\nYour score was: " << snake.size() - 1;
}

/**
* Checks for collision. Returns true if collision occurred, otherwise returns false
*/
bool Snake::collisionCheck() {
    COORD head = snake.front();

    // Checks for collisions with the walls
    if (head.X >= winDimensions.X || head.Y >= winDimensions.Y || head.X < 0 || head.Y < 0) return true;

    // Checks for collisions with itself only if the snake has more than 3 parts, since 3-part snake cannot collide with itself
    else if (snake.size() > 3) return std::any_of(snake.begin() + 1, snake.end(), [head](COORD i) { return head == i; });
    return false;
}

/**
* Generates food at random coordinates and draws it
*/
const void Snake::generateFood() {
    // Continue generating food until it does not collide with the snake
    do {
        // Defines random device and distributions
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> xDistr(0, winDimensions.X - 1);
        std::uniform_int_distribution<> yDistr(0, winDimensions.Y - 1);

        short x = xDistr(gen);
        short y = yDistr(gen);

        // Sets the new coordinates for food
        food = { x, y };
    } while (std::any_of(snake.begin(), snake.end(), [this](COORD i) { return i == food; }));

    // Draws food with red color
    SetConsoleTextAttribute(hConsole, 12); // 12 = green
    SetConsoleCursorPosition(hConsole, food);
    WriteConsole(hConsole, (char*)"Q", 1, NULL, NULL);
    SetConsoleTextAttribute(hConsole, 15);
}

/**
* Returns copy of snake deque
*/
std::deque<COORD> Snake::getSnakeCoords() {
    return snake;
}

/**
* Returns copy of food coordinates
*/
COORD Snake::getFoodCoords() {
    return food;
}

/**
* Returns pointer to the console handle
*/
HANDLE* Snake::getConsoleHandle() {
    return &hConsole;
}