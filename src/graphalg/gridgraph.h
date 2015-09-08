#ifndef GRIDGRAPH_H
#define GRIDGRAPH_H

#include <fstream>      // for std::ifstream
#include <sstream>      // for std::istringstream
#include <array>
#include <vector>
#include <algorithm>    // for std::reverse
#include <assert.h>

template <typename Node_T, size_t width, size_t height>
class GridGraph
{
public:
    void load(std::string mapfile_path, std::function<Node_T*(std::string)> get_node_instance) {
        std::ifstream mapFile(mapfile_path);
        std::string line;

        size_t row(0), node_idx(0);
        while (std::getline(mapFile, line)) {
            size_t column = 0;
            assert(row < height);
            std::istringstream iss(line);
            std::string token;
            while (std::getline(iss, token, ' ')) {
                assert(column < width);
                m_grid.at(node_idx) = get_node_instance(token);
                column++;
                node_idx++;
            }
            row++;
        }
    };

    typedef std::tuple<int, int> Location;
    typedef Location Node;

    Node_T* at(int x, int y) const { return m_grid.at(y * height + x); };
    std::vector<Location> neighbors(Location loc) const {
        int x, y, dx, dy;
        std::tie(x, y) = loc;
        std::vector<Location> results;
        for (auto direction : DIRS) {
            std::tie(dx, dy) = direction;
            Location next(x + dx, y + dy);
            if (in_bounds(next) && passable(next)) {
                results.push_back(next);
            }
        }

        if ((x + y) % 2 == 0) {
            // aesthetic improvement on square grids
            std::reverse(results.begin(), results.end());
        }

        return results;
    };
    // This is for future references in case I want
    // to implemenent different movement costs.
    inline int cost(Location a, Location b) const { return 1; };

private:
    inline bool in_bounds(Location loc) const {
        int x, y;
        std::tie(x, y) = loc;
        return x >= 0 && x < width && y >= 0 && y < height;
    };

    inline bool passable(Location loc) const {
        int x, y;
        std::tie(x, y) = loc;
        return m_grid.at(y * height + x)->passable();
    };

    std::array<Node_T*, width * height> m_grid;
    static std::array<Location, 4> DIRS;
};

template <typename Node_T, size_t width, size_t height>
std::array<std::tuple<int, int>, 4> GridGraph<Node_T, width, height>::DIRS {
    GridGraph::Location {1, 0},
    GridGraph::Location {0, -1},
    GridGraph::Location {-1, 0},
    GridGraph::Location {0, 1}
};

#endif // GRIDGRAPH_H
