#include "map.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

Map::Map() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    for (int y = 0; y < MAP_H; ++y) {
        for (int x = 0; x < MAP_W; ++x) {
            if (x == 0 || y == 0 || x == MAP_W - 1 || y == MAP_H - 1)
                map[y][x] = '%';  // outer wall
            else
                map[y][x] = '#';  // default interior wall
        }
    }
}

void Map::generate() {
    // Clear map and build outer wall
    for (int y = 0; y < MAP_H; ++y) {
        for (int x = 0; x < MAP_W; ++x) {
            if (x == 0 || y == 0 || x == MAP_W - 1 || y == MAP_H - 1)
                map[y][x] = '%';
            else
                map[y][x] = '#';
        }
    }

    const int roomH = 10;
    const int roomW = 15;
    const int gap = 1;
    int top = 1;

    while (top + roomH < MAP_H - 1) {
        int numRooms = 2 + std::rand() % 7; // 2 to 8 rooms
        int availableW = MAP_W - 2;
        int requiredW = numRooms * (roomW + gap) - gap;

        if (requiredW > availableW) numRooms = availableW / (roomW + gap);

        int left = 1 + std::rand() % std::max(1, (availableW - requiredW + 1)); // random horizontal offset

        for (int i = 0; i < numRooms; ++i) {
            int roomX = left + i * (roomW + gap);
            if (roomX + roomW >= MAP_W - 1) break;

            for (int y = top; y < top + roomH; ++y) {
                for (int x = roomX; x < roomX + roomW; ++x) {
                    map[y][x] = '.';
                }
            }
        }

        top += roomH + gap;
    }
}
void Map::splitNode(BSPNode& node, int minSize, int depth) {
    if (depth <= 0) {
        leaves.push_back(&node);
        return;
    }

    bool splitH = std::rand() % 2;
    if (node.w > node.h && node.w / node.h >= 1.25) splitH = false;
    else if (node.h > node.w && node.h / node.w >= 1.25) splitH = true;

    int max = (splitH ? node.h : node.w) - minSize;
    if (max <= minSize) {
        leaves.push_back(&node);
        return;
    }

    int splitAt = std::rand() % (max - minSize + 1) + minSize;

    if (splitH) {
        node.left = std::make_unique<BSPNode>(node.x, node.y, node.w, splitAt);
        node.right = std::make_unique<BSPNode>(node.x, node.y + splitAt, node.w, node.h - splitAt);
    }
    else {
        node.left = std::make_unique<BSPNode>(node.x, node.y, splitAt, node.h);
        node.right = std::make_unique<BSPNode>(node.x + splitAt, node.y, node.w - splitAt, node.h);
    }

    splitNode(*node.left, minSize, depth - 1);
    splitNode(*node.right, minSize, depth - 1);
}

void Map::carveRoom(BSPNode& node) {
    const int minRoomSize = 4;

    int safeX = std::max(node.x, 1);
    int safeY = std::max(node.y, 1);
    int safeW = std::min(node.w, MAP_W - safeX - 1);
    int safeH = std::min(node.h, MAP_H - safeY - 1);

    if (safeW <= minRoomSize || safeH <= minRoomSize) return;

    int rw = minRoomSize + std::rand() % (safeW - minRoomSize + 1);
    int rh = minRoomSize + std::rand() % (safeH - minRoomSize + 1);

    int rx = safeX + std::rand() % (safeW - rw + 1);
    int ry = safeY + std::rand() % (safeH - rh + 1);

    for (int y = ry; y < ry + rh; ++y) {
        for (int x = rx; x < rx + rw; ++x) {
            if (x >= 1 && x < MAP_W - 1 && y >= 1 && y < MAP_H - 1)
                map[y][x] = '.';
        }
    }
}

void Map::connectNearbyRooms() {
    const int maxDist = 15;

    for (size_t i = 0; i < leaves.size(); ++i) {
        BSPNode* a = leaves[i];
        int ax = a->x + a->w / 2;
        int ay = a->y + a->h / 2;

        for (size_t j = i + 1; j < leaves.size(); ++j) {
            BSPNode* b = leaves[j];
            int bx = b->x + b->w / 2;
            int by = b->y + b->h / 2;

            int dx = std::abs(ax - bx);
            int dy = std::abs(ay - by);

            if (dx + dy <= maxDist) {
                int x = ax, y = ay;
                while (x != bx && x > 0 && x < MAP_W - 1) {
                    map[y][x] = '.';
                    x += (bx > x) ? 1 : -1;
                }
                while (y != by && y > 0 && y < MAP_H - 1) {
                    map[y][x] = '.';
                    y += (by > y) ? 1 : -1;
                }
            }
        }
    }
}

void Map::print() const {
    const int viewHeight = 40;
    const int viewWidth = 100;
    const int offsetY = 0;
    const int offsetX = 0;

    for (int y = offsetY; y < offsetY + viewHeight && y < MAP_H; ++y) {
        for (int x = offsetX; x < offsetX + viewWidth && x < MAP_W; ++x) {
            std::cout << map[y][x];
        }
        std::cout << '\n';
    }
    std::cout << "\n[Showing " << viewWidth << "x" << viewHeight << " from ("
        << offsetX << "," << offsetY << ")]\n";
}

const std::array<std::array<char, MAP_W>, MAP_H>& Map::getMap() const {
    return map;
}
