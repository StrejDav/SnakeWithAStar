#pragma once
#include <vector>
#include <windows.h>
#include <queue>
#include <cmath>
#include <limits>
#include <algorithm>
#include <stdexcept>

namespace A_Star {
    struct Node2D {
        COORD coords;
        std::vector<Node2D*> neighbors = {};
        float f = std::numeric_limits<float>::infinity();
        float g = std::numeric_limits<float>::infinity();
        Node2D* parent = nullptr;
        bool bInOpen = false;
        bool bInClosed = false;
        bool bVisited = false;
        bool bInvalid = false;

        // Function will reset the following parameters to their default state
        void reset() {
            f = std::numeric_limits<float>::infinity();
            g = std::numeric_limits<float>::infinity();
            parent = nullptr;
            bInOpen = false;
            bInClosed = false;
            bVisited = false;
        }
    };

    enum class Heuristic2D { EUCLIDEAN, OCTILE, MANHATTAN };

    std::vector<COORD> findPath2D(Node2D* start, Node2D* finish, const Heuristic2D& heuristic);

    float CalculateEuclideanDistance(COORD a, COORD b);
    float CalculateManhattanDistance(COORD a, COORD b);
    float CalculateOctileDistance(COORD a, COORD b);

    struct CmpNodePtr
    {
        bool operator()(const Node2D* lhs, const Node2D* rhs) const
        {
            return lhs->f > rhs->f;
        }
    };
}
