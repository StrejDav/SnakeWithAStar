#include "A_star.h"

namespace A_Star {

    /**
    * Find path between two nodes with COORD struct -> only available for Windows
    */
    std::vector<COORD> findPath2D(Node2D* start, Node2D* finish, const Heuristic2D& heuristic) {
        // If the start and finish coordinates are the same, returns empty vector
        if ((start->coords.X == finish->coords.X) && (start->coords.Y == finish->coords.Y))
            return {};

        // Initialize function pointer for heuristic function
        float (*h)(COORD a, COORD b);


        switch (heuristic) {
        case Heuristic2D::EUCLIDEAN:
            h = CalculateEuclideanDistance;
            break;
        case Heuristic2D::OCTILE:
            h = CalculateOctileDistance;
            break;
        case Heuristic2D::MANHATTAN:
            h = CalculateManhattanDistance;
            break;
        default:
            throw std::invalid_argument("Invalid heuristic!");
            break;
        }

        // Init open priority queue
        std::priority_queue<Node2D*, std::vector<Node2D*>, CmpNodePtr> open;

        // Init visited vector, which is used for resetting values in visited nodes
        std::vector<Node2D*> visited;

        // Init path vector
        std::vector<COORD> path;

        // Initial values for starting node
        start->f = h(start->coords, finish->coords);
        start->g = 0;
        open.push(start);

        // current is the node which is currently being searched
        Node2D* current;
        float tentScore;

        // A* algorithm itself
        while (open.size() != 0) {
            current = open.top(); open.pop();
            current->bInClosed = true;
            if ((current->coords.X == finish->coords.X) && (current->coords.Y == finish->coords.Y)) break;

            for (auto& neighbor : current->neighbors) {
                if (!neighbor->bInvalid) {
                    tentScore = current->g + 1;
                    if (tentScore < neighbor->g) {
                        neighbor->parent = current;
                        neighbor->g = tentScore;
                        neighbor->f = tentScore + h(neighbor->coords, finish->coords);
                        if (!neighbor->bInOpen) {
                            open.push(neighbor);
                            neighbor->bInOpen = true;
                        }
                    }
                    if (!neighbor->bVisited) {
                        neighbor->bVisited = true;
                        visited.push_back(neighbor);
                    }
                }
            }
        }

        // This part will reconstruct the path from the finish node up to the starting node and returns it
        Node2D* currentToStart = finish;
        if (finish->parent == nullptr) return {};
        path = { currentToStart->coords };

        while (true) {
            currentToStart = currentToStart->parent;
            if ((currentToStart->coords.X == start->coords.X) && (currentToStart->coords.Y == start->coords.Y)) break;
            path.push_back(currentToStart->coords);
        }

        for (auto& i : visited) i->reset();

        return path;
    }

    float CalculateEuclideanDistance(COORD a, COORD b) {
        return std::sqrt(std::pow(b.X - a.X, 2) + std::pow(b.Y - a.Y, 2));
    }

    float CalculateManhattanDistance(COORD a, COORD b) {
        return std::abs(b.X - a.X) + std::abs(b.Y - a.Y);
    }

    float CalculateOctileDistance(COORD a, COORD b) {
        return std::max<short>(std::abs(a.X - b.X), std::abs(a.Y - b.Y)) + (std::sqrt(2) - 1)*std::min<short>(std::abs(a.X - b.X), std::abs(a.Y - b.Y));
    }
}