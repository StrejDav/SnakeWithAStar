#include "AutoPlay.h"
#include <algorithm>
#include <iostream>

/**
* Generates nodes for each console field and will push neighbors to each node
*/
AutoPlay::AutoPlay(const short& width, const short& height, const unsigned int& period) {
    this->width = width;
    this->height = height;
    gameplanArea = width * height;

    for (int i = 0; i < gameplanArea; i++) {
        A_Star::Node2D temp{
            COORD{(short)(i % width), (short)(floor(i / width))}
        };
        gameplan.push_back(temp);
    }

    for (int i = 0; i < gameplanArea; i++) {
        A_Star::Node2D* currentNode = &gameplan[i];

        if (i >= width)                 currentNode->neighbors.push_back(&gameplan[i - width]); // top neighbour
        if (i < width * (height - 1))   currentNode->neighbors.push_back(&gameplan[i + width]); // bottom neighbour
        if (i % width != 0)             currentNode->neighbors.push_back(&gameplan[i - 1]);     // left neighbour
        if (i % width != width - 1)     currentNode->neighbors.push_back(&gameplan[i + 1]);     // right neighbour
    }

    snake = new Snake(width, height, period);
    hConsole = snake->getConsoleHandle();
}

/**
* Destructor
*/
AutoPlay::~AutoPlay() {
    delete snake;
}

/**
* Marks the node with passed coordinates as invalid, so the A* algorithm will ignore it
*/
void AutoPlay::deleteNeighboursFromNodes(const COORD& coordsToDelete) {
    int x = coordsToDelete.X;
    int y = coordsToDelete.Y;

    int pos = y * width + x;

    A_Star::Node2D* nodeToDelete = &gameplan[pos];

    // Had to resort to ditching this because I could not make it work and also there is a problem that when two neighboring
    // nodes delete each other's pointer then that situation is irreversible
    /*for (auto& i : nodeToDelete->neighbors) {
        int pos = 0;
        for (auto& j : i->neighbors) {
            if ((j->coords.X == coordsToDelete.X) && (j->coords.Y == coordsToDelete.Y)) break;
            pos++;
        }
        i->neighbors.erase(i->neighbors.begin() + pos);
    }*/

    nodeToDelete->bInvalid = true;
}

/**
* Unmarks the node with passed coordinates as invalid, so the A* algorithm will not ignore it
*/
void AutoPlay::addNeighbourToGameplan(const COORD& coordsToAdd) {
    int x = coordsToAdd.X;
    int y = coordsToAdd.Y;

    int pos = y * width + x;

    A_Star::Node2D* nodeToAdd = &gameplan[pos];
    /*
    for (auto& i : nodeToAdd->neighbors) {
        bool contains = false;
        for (auto& j : i->neighbors) {
            if ((j->coords.X == coordsToAdd.X) && (j->coords.Y == coordsToAdd.Y)) {
                contains = true;
                break;
            }
        }
        if (!contains) i->neighbors.push_back(nodeToAdd);
    }*/

    nodeToAdd->bInvalid = false;
}

void AutoPlay::playGame(const A_Star::Heuristic2D& heuristic) {

    // Variable initialization
    char dir = 0;                       // Movement direction
    bool cont = true;                   // Should continue?
    bool ateFood;                       // Did snake eat food?
    bool firstIter = true;              // Is it the first iteration?
    bool rendered;                      // Was snake drawn in the current pass through the while cycle?
    int snakeLength = 1;
    COORD snakeHeadPos, foodPos;
    std::deque<COORD> snakeCoords;
    std::vector<COORD> path;
    int startNodePos, finishNodePos;
    A_Star::Node2D *startNode, *finishNode;

    while (cont) {
        snake->move(dir);
        snake->render(&cont, &ateFood, &rendered);
        if (ateFood) snakeLength++;

        if (rendered || firstIter) {
            if (ateFood || firstIter) {
                snakeCoords = snake->getSnakeCoords();
                snakeHeadPos = snakeCoords.front();

                // Marks all nodes occupied by the snake as invalid
                for (auto i = snakeCoords.begin() + 1; i != snakeCoords.end(); i++) {
                    deleteNeighboursFromNodes(*i);
                }

                foodPos = snake->getFoodCoords();

                // Defines starting node = snake's head
                startNodePos = snakeHeadPos.Y * width + snakeHeadPos.X;
                startNode = &gameplan[startNodePos];

                // Defines finish node = food
                finishNodePos = foodPos.Y * width + foodPos.X;
                finishNode = &gameplan[finishNodePos];

                // Finds the path from snake's head to food
                path = A_Star::findPath2D(startNode, finishNode, heuristic);

                // Marks all nodes occupied by the snake as valid
                for (auto i = snakeCoords.begin() + 1; i != snakeCoords.end(); i++) {
                    addNeighbourToGameplan(*i);
                }

                // If no path was found, then end the game
                if (path.empty()) {
                    cont = false;
                    system("cls");
                    SetConsoleCursorPosition(*hConsole, { 1, 1 });
                    std::cout << "No available path was found.\nGame Over!\nThe score was: " << std::to_string(snakeCoords.size() - 1) << "\n";
                    return;
                }

                // Draws path as green 'x's
                SetConsoleTextAttribute(*hConsole, 10); // 10 = green
                for (auto i = path.begin() + 1; i != path.end(); i++) {
                    SetConsoleCursorPosition(*hConsole, *i);
                    WriteConsole(*hConsole, (char*)"x", 1, NULL, NULL);
                }
                SetConsoleTextAttribute(*hConsole, 15);
            }

            // Defines direction in which the snake should go
            short x = snake->getSnakeCoords().front().X;
            short y = snake->getSnakeCoords().front().Y;
            COORD nextCoord = path.back(); path.pop_back();
            COORD direction = { x - nextCoord.X, y - nextCoord.Y };

            // Defines direction for the snake object itself
            if (direction.X == 0 && direction.Y == 1) dir = UP;
            else if (direction.X == 0 && direction.Y == -1) dir = DOWN;
            else if (direction.X == 1 && direction.Y == 0) dir = LEFT;
            else if (direction.X == -1 && direction.Y == 0) dir = RIGHT;

            firstIter = false;
        }
    }
}