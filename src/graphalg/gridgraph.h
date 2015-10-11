#ifndef GRIDGRAPH_H
#define GRIDGRAPH_H

#include <fstream>      // for std::ifstream
#include <sstream>      // for std::istringstream
#include <array>
#include <vector>
#include <algorithm>    // for std::reverse
#include <unordered_set>
#include <assert.h>
#include "gridlocation.h"

template <typename Node_T, size_t width, size_t height>
class GridGraph
{
public:
    using Node = GridLocation;

    void load(std::string mapfile_path, std::function<std::unique_ptr<Node_T>(std::string)> get_node_instance) {
        std::ifstream mapFile(mapfile_path);
        std::string line;
        std::unordered_set<GridLocation> locs;

        size_t row(0), node_idx(0);
        while (std::getline(mapFile, line)) {
            size_t column = 0;
            assert(row < height);
            std::istringstream iss(line);
            std::string token;
            while (std::getline(iss, token, ' ')) {
                assert(column < width);
                m_grid.at(node_idx) = get_node_instance(token);
                locs.insert(GridLocation(column, row));
                column++;
                node_idx++;
            }
            row++;
        }

        split_regions(locs);
    };

    Node_T* at(int x, int y) const { return m_grid.at(y * height + x).get(); };

    std::vector<GridLocation> neighbors(GridLocation loc) const {
        int x, y, dx, dy;
        std::tie(x, y) = loc;
        std::vector<GridLocation> results;
        for (auto direction : DIRS) {
            std::tie(dx, dy) = direction;
            GridLocation next(x + dx, y + dy);
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
    inline int cost(GridLocation a, GridLocation b) const { return 1; };

private:
    inline bool in_bounds(GridLocation loc) const {
        int x, y;
        std::tie(x, y) = loc;
        return x >= 0 && x < width && y >= 0 && y < height;
    };

    inline bool passable(GridLocation loc) const {
        int x, y;
        std::tie(x, y) = loc;
        return m_grid.at(y * height + x)->passable();
    };

    void split_regions(std::unordered_set<GridLocation>& locs) {
        uint32_t reg(0);
        while (!locs.empty()) {
            reg++;
            auto it = locs.begin();
            // TODO: flood fill here
            int32_t x, y;
            std::tie(x, y) = *it;
            auto first_node = m_grid.at(y * height + x).get();
            first_node->set_region(reg);
            printf("Reg: %d {%d, %d}\n", reg, x, y);
            flood_fill(x, y, locs, first_node);
        }
    };

    void flood_fill(int32_t x, int32_t y, std::unordered_set<GridLocation>& locs, Node_T* first_node) {
        printf("     {%d, %d}\n", x, y);
        auto reg(first_node->region());

        if (!m_grid.at(y * height + x)->is_same_type(*first_node)) {
            return;
        }

        int32_t x1(x);

        // scan current line from start to the right end
        while (x1 < width && m_grid.at(y * height + x1)->is_same_type(*first_node)) {
            m_grid.at(y * height + x1)->set_region(reg);

            GridLocation loc {x1, y};
            locs.erase(loc);

            x1++;
        }

        // scan current line from start to the left end
        x1 = x - 1;
        while ((x1 >= 0) && m_grid.at(y * height + x1)->is_same_type(*first_node)) {
            m_grid.at(y * height + x1)->set_region(reg);

            GridLocation loc {x1, y};
            locs.erase(loc);

            x1--;
        }

        // test for new scanlines to the top
        x1 = x;
        while (x1 < width && m_grid.at(y * height + x1)->region() == reg) {
            if (y > 0 && m_grid.at((y - 1) * height + x1)->is_same_type(*first_node)
                      && m_grid.at((y - 1) * height + x1)->region() == 0) {
                flood_fill(x1, y - 1, locs, first_node);
            }
            x1++;
        }
        x1 = x - 1;
        while (x1 >= 0 && m_grid.at(y * height + x1)->region() == reg) {
            if (y > 0 && m_grid.at((y - 1) * height + x1)->is_same_type(*first_node)
                      && m_grid.at((y - 1) * height + x1)->region() == 0) {
                flood_fill(x1, y - 1, locs, first_node);
            }
            x1--;
        }

        // test for new scanlines to the bottom
        x1 = x;
        while (x1 < width && m_grid.at(y * height + x1)->region() == reg) {
            if ((y + 1) < height && m_grid.at((y + 1) * height + x1)->is_same_type(*first_node)
                                 && m_grid.at((y + 1) * height + x1)->region() == 0) {
                flood_fill(x1, y + 1, locs, first_node);
            }
            x1++;
        }
        x1 = x - 1;
        while (x1 >= 0 && m_grid.at(y * height + x1)->region() == reg) {
            if ((y + 1) < height && m_grid.at((y + 1) * height + x1)->is_same_type(*first_node)
                                 && m_grid.at((y + 1) * height + x1)->region() == 0) {
                flood_fill(x1, y + 1, locs, first_node);
            }
            x1--;
        }
    };

    std::array<std::unique_ptr<Node_T>, width * height> m_grid;
    static std::array<GridLocation, 4> DIRS;
};

template <typename Node_T, size_t width, size_t height>
std::array<GridLocation, 4> GridGraph<Node_T, width, height>::DIRS {
    GridLocation {1, 0},
    GridLocation {0, -1},
    GridLocation {-1, 0},
    GridLocation {0, 1}
};

#endif // GRIDGRAPH_H
