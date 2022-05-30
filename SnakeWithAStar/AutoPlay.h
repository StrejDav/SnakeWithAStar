#pragma once
#include <vector>
#include "A_star.h"
#include "Snake.h"

class AutoPlay
{
private:
    short width;
    short height;
    int gameplanArea;
    std::vector<A_Star::Node2D> gameplan;
    std::vector<A_Star::Node2D*> removedNodes;
    Snake* snake;
    const HANDLE* hConsole;

public:
    AutoPlay(const short& width, const short& height, const unsigned int& period);
    ~AutoPlay();
    void deleteNeighboursFromNodes(const COORD& coordsToDelete);
    void addNeighbourToGameplan(const COORD& coordsToAdd);
    void playGame(const A_Star::Heuristic2D& heuristic);
};

