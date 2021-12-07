/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
                or disclosure of this file or its contents without the prior
                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "Core.h"
#include "TMath.h"
#include <queue>

namespace Tempest::algo
{
    struct a_star_node
    {
        int x;
        int y;
        int parentX = -1;
        int parentY = -1;

        float gCost = .0f;
        float hCost = .0f;
        float fCost = .0f;
    };

    inline bool operator<(const a_star_node& lhs, const a_star_node& rhs)
    {
        return lhs.fCost < rhs.fCost;
    }
   
    inline double calculate_H(int x0, int y0, int x1, int y1) {
        double H = (sqrt((x0 - x1) * (x0 - x1)
            + (y0 - y1) * (y0 - y1)));
        return H;
    }
    template<typename Visit>
    tvector<glm::ivec2> a_star(int x0, int y0, int x1, int y1, Visit visit)
    {
        const tvector<tpair<int, int>> dir = { {0,1}, {0,-1}, {1,0}, {-1,0} };
        tmap<int, tmap<int, bool>>& visited;
        tmap<int, tmap<int, a_star_node>>& closed;
        tmap<int, tmap<int, float>>& cost;
        std::priority_queue<a_star_node> pq;
        pq.push({ x0, y0 });

        a_star_node end;
        /*while (!pq.empty())
        {
            Node p = pq.top();
            pq.pop();

            visited[p.x][p.y] = true;

            for (auto [x, y] : dir)
            {
                if (p.x + x == x1 && p.y + y == y1)
                    return tvector<glm::ivec2>();

                if (visited[p.x + x][p.y + y])
                    continue;

                if (visit(p.x, p.y, p.x + x, p.y + y))
                    continue;

                double gNew = p.gCost + 1.f, hNew = calculate_H(p.x + x, p.y + y, x1, y1), fNew = gNew + hNew;

                Node n;
                n.x = p.x + x;
                n.y = p.y + y;
                n.parentX = p.x;
                n.parentX = p.y;

                if(cost[p.x + x][p.y + y]
                q.push(n);
            }

        }*/

    }

    struct dijkstras_node
    {
        int x;
        int y;
        int dist = INT_MAX;
    };

    inline bool operator<(const dijkstras_node& lhs, const dijkstras_node& rhs)
    {
        return lhs.dist < rhs.dist;
    }


    inline tvector<glm::ivec2> make_path(int x0, int y0, int x1, int y1, tmap<int, tmap<int, tpair<int, int>>>& prev)
    {
        tvector<glm::ivec2> output = { {x1, y1} };
        while (!(x1 == x0 && y1 == y0) && prev.count(x1) && prev[x1].count(y1))
        {
            auto [n_x, n_y] = prev[x1][y1];
            output.push_back({ n_x, n_y });
            x1 = n_x;
            y1 = n_y;
        }
        return output;
    }

    template<typename Visit>
    tvector<glm::ivec2> bfs(int x0, int y0, int x1, int y1, int steps, Visit visit)
    {
        const uint32_t range = steps < 0 ? 0 : static_cast<uint32_t>(steps);
        const tvector<tpair<int, int>> dir = { {0,1}, {0,-1}, {1,0}, {-1,0} };
        tmap<int, tmap<int, bool>> visited;
        tmap<int, tmap<int, uint32_t>> distance;
        tmap<int, tmap<int, tpair<int,int>>> prev;

        std::queue<glm::ivec2> q;
        q.push({ x0, y0 });

        while (!q.empty())
        {
            auto p = q.front();
            q.pop();

            if (visited[p.x][p.y])
                continue;

            visited[p.x][p.y] = true;

            auto new_dist = distance[p.x][p.y] + 1;
            if (new_dist > range)
                continue;

            for (auto [x, y] : dir)
            {
                if (visited[p.x + x][p.y + y])
                    continue;

                if (visit(p.x, p.y, p.x + x, p.y + y))
                    continue;


                prev[p.x + x][p.y + y] = std::make_pair(p.x, p.y);
                if (p.x + x == x1 && p.y + y == y1)
                {
                    return make_path(x0, y0, x1, y1, prev);
                }

                distance[p.x + x][p.y + y] = new_dist;
                q.push({ p.x + x, p.y + y });
            }

        }

        return tvector<glm::ivec2>();
    }
}