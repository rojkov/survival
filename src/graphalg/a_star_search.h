#ifndef A_STAR_SEARCH
#define A_STAR_SEARCH

#include <queue>
#include <unordered_map>
#include <algorithm>

template<typename T, typename Number=int>
struct PriorityQueue {
    using PQElement = std::pair<Number, T>;
    std::priority_queue<PQElement, std::vector<PQElement>,
                        std::greater<PQElement>> elements;

    inline bool empty() { return elements.empty(); }

    inline void put(T item, Number priority) {
        elements.emplace(priority, item);
    }

    inline T get() {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};

template<typename Graph>
std::vector<typename Graph::Node> a_star_search(const Graph &graph,
                                                typename Graph::Node start,
                                                typename Graph::Node goal,
                                                std::function<int(typename Graph::Node, typename Graph::Node)> heuristic)
{
    std::unordered_map<typename Graph::Node, typename Graph::Node> came_from;
    std::unordered_map<typename Graph::Node, int> cost_so_far;
    PriorityQueue<typename Graph::Node> frontier;

    frontier.put(start, 0);
    came_from[start] = start;
    cost_so_far[start] = 0;

    while (!frontier.empty()) {
        auto current = frontier.get();

        if (current == goal) {
            break;
        }

        for (auto next : graph.neighbors(current)) {
            int new_cost = cost_so_far[current] + graph.cost(current, next);
            if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
                cost_so_far[next] = new_cost;
                int priority = new_cost + heuristic(next, goal);
                frontier.put(next, priority);
                came_from[next] = current;
            }
        }
    }

    // Generate path
    std::vector<typename Graph::Node> path;
    auto current = goal;
    path.push_back(current);
    while (current != start) {
        current = came_from[current];
        path.push_back(current);
    }
    std::reverse(path.begin(), path.end());
    return path;
}

#endif // A_STAR_SEARCH
