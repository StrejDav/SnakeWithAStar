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
    std::vector<COORD> obstacles;
    COORD food;
    COORD winDimensions;
    char direction;
    HANDLE hConsole;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point now;
    unsigned int period;


    bool collisionCheck();
    const void generateFood();
    const void generateObstacles(const int& numOfObstacles);
public:
    Snake(const short& width, const short& height, const unsigned int& numberOfObstacles = 0, const unsigned int& period = 500);
    const void move(const char& direction);
    const void render(bool* cont, bool* ateFood = nullptr, bool* rendered = nullptr);
    const void gameLoop();

    std::deque<COORD> getSnakeCoords();
    COORD getFoodCoords();
    HANDLE* getConsoleHandle();
    std::vector<COORD> getObstacles();
};