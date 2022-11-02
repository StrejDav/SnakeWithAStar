// SnakeWithAStar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include "AutoPlay.h"

int main()
{
    // Asks for game mode
    std::string strGameMode;
    int gameMode = 1;

    std::cout << "Welcome to snake!\nPlease choose a game mode: 1 = AI will play, 2 = You will play\n";
    while (true) {
        strGameMode = std::getchar();
        gameMode = std::stoi(strGameMode);
        if ((gameMode != 1) && (gameMode != 2)) {
            std::cout << "Wrong numbers entered. Try again.\n";
        }
        else break;
    }

    // Asks for parameters for the game
    std::string inserted;
    std::stringstream ssin;
    std::string arguments[4];
    unsigned int width, height, period, numberOfObstacles;

    std::cout << "Please choose window width and height and period between movements (in ms) in the following manner: width height numberOfObstacles period\ni. e. 20 20 10 250\n";
    while (true) {
        std::getline(std::cin, inserted);

        int i = 0;
        ssin = std::stringstream(inserted);
        while (ssin.good() && i < 5)
        {
            ssin >> arguments[i];
            i++;
        }
        try {
            width = std::stoi(arguments[0]);
            height = std::stoi(arguments[1]);
            numberOfObstacles = std::stoi(arguments[2]);
            period = std::stoi(arguments[3]);
            break;
        }
        catch (const std::exception& e) {
            std::cout << "Try again!";
        }
    }

    // AI driven game
    if (gameMode == 1) {
        char charHeuristic;
        A_Star::Heuristic2D heuristic;

        std::cout << "Please choose heuristic function: E = Euclidean, O = Octile, M = Manhattan\n";
        while (true) {
            charHeuristic = std::getchar();

            if (charHeuristic == *"E") {
                heuristic = A_Star::Heuristic2D::EUCLIDEAN;
                break;
            }
            else if (charHeuristic == *"O") {
                heuristic = A_Star::Heuristic2D::OCTILE;
                break;
            }
            else if (charHeuristic == *"M") {
                heuristic = A_Star::Heuristic2D::MANHATTAN;
                break;
            }
            else std::cout << "Try again!";
        }

        system("cls");
        AutoPlay play(width, height, numberOfObstacles, period);
        play.playGame(heuristic);
    }
    
    // Player driven game
    else if (gameMode == 2) {
        system("cls");
        Snake snake(width, height, numberOfObstacles, period);
        snake.gameLoop();
    }

    system("pause");
}
