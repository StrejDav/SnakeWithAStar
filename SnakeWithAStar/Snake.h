#pragma once
#include <vector>
#include <windows.h>
#include <chrono>
#include <deque>
#include <algorithm>
#include <string>
#include <random>
#include <iostream>

#define UP -1
#define LEFT 2
#define DOWN 1
#define RIGHT -2
//#define PERIOD 500 // ms

class Snake
{
private:
    std::deque<COORD> snake;
    std::vector<COORD> obstacle;
    COORD food;
    COORD winDimensions;
    char direction;
    HANDLE hConsole;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point now;
    unsigned int period;


    bool collisionCheck();
    const void generateFood();

public:
    Snake(const short& width, const short& height, const unsigned int& period);
    const void move(const char& direction);
    const void render(bool* cont, bool* ateFood, bool* rendered);
    const void gameLoop();

    std::deque<COORD> getSnakeCoords();
    COORD getFoodCoords();
    HANDLE* getConsoleHandle();
    std::vector<COORD> getObstacles();
};