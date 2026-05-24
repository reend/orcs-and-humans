#include "Pathfinder.h"
#include "engine/logging/Logger.h"
#include <cmath>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

namespace Engine {

struct PathNode {
    int   x, y;
    float g, h, f;
    int   parentKey = -1;
};

static constexpr float SQRT2              = 1.414f;
static constexpr int   PACK_MULTIPLIER   = 10000;
static constexpr int   CARDINAL_DIR_COUNT = 4;

static inline int Pack(int x, int y) { return x * PACK_MULTIPLIER + y; }

static float Octile(int x1, int y1, int x2, int y2) {
    float dx = (float)std::abs(x2 - x1);
    float dy = (float)std::abs(y2 - y1);
    return (dx + dy) + (SQRT2 - 2.0f) * std::min(dx, dy);
}

std::vector<raylib::Vector2> Pathfinder::FindPath(
    raylib::Vector2 start, raylib::Vector2 goal, IsPassableFunc isPassable)
{
    int sx = (int)start.x, sy = (int)start.y;
    int gx = (int)goal.x,  gy = (int)goal.y;

    if (!isPassable(sx, sy) || !isPassable(gx, gy)) {
        LOG_WARN("Pathfinding: start or goal is not passable");
        return {};
    }
    if (sx == gx && sy == gy) return {start};

    std::unordered_map<int, PathNode> nodes;
    std::unordered_set<int> closed;

    using Entry = std::tuple<float, int, int>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> open;

    int sk = Pack(sx, sy);
    PathNode& sn = nodes[sk];
    sn.x = sx; sn.y = sy; sn.g = 0;
    sn.h = Octile(sx, sy, gx, gy);
    sn.f = sn.h;
    open.push({sn.f, sx, sy});

    constexpr int dx[] = { 0,  1, 0, -1, -1,  1,  1, -1};
    constexpr int dy[] = {-1,  0, 1,  0, -1, -1,  1,  1};

    int goalKey = -1;

    while (!open.empty()) {
        auto [f, cx, cy] = open.top();
        open.pop();

        int ck = Pack(cx, cy);
        if (closed.count(ck)) continue;
        closed.insert(ck);

        if (cx == gx && cy == gy) { goalKey = ck; break; }

        float cg = nodes[ck].g;

        for (int i = 0; i < 8; i++) {
            int nx = cx + dx[i], ny = cy + dy[i];
            if (!isPassable(nx, ny)) continue;
        if (i >= CARDINAL_DIR_COUNT && (!isPassable(cx, cy + dy[i]) || !isPassable(cx + dx[i], cy))) continue;

            int nk = Pack(nx, ny);
            if (closed.count(nk)) continue;

            float newG = cg + (i < CARDINAL_DIR_COUNT ? 1.0f : SQRT2);
            auto it = nodes.find(nk);
            if (it == nodes.end() || newG < it->second.g) {
                PathNode& nb = nodes[nk];
                nb.x = nx; nb.y = ny;
                nb.g = newG;
                nb.h = Octile(nx, ny, gx, gy);
                nb.f = nb.g + nb.h;
                nb.parentKey = ck;
                open.push({nb.f, nx, ny});
            }
        }
    }

    if (goalKey == -1) {
        LOG_WARN("No path from (%d,%d) to (%d,%d)", sx, sy, gx, gy);
        return {};
    }

    std::vector<raylib::Vector2> path;
    for (int key = goalKey; key != -1; key = nodes.at(key).parentKey)
        path.push_back({(float)nodes.at(key).x, (float)nodes.at(key).y});
    std::reverse(path.begin(), path.end());

    LOG_DEBUG("Path found: %d nodes", (int)path.size());
    return path;
}

}
