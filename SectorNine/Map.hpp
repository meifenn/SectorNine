#pragma once
#include <vector>
#include <memory>
#include <array>

constexpr int MAP_W = 100;
constexpr int MAP_H = 40;

struct BSPNode {
    int x, y, w, h;
    std::unique_ptr<BSPNode> left, right;

    BSPNode(int x_, int y_, int w_, int h_)
        : x(x_), y(y_), w(w_), h(h_), left(nullptr), right(nullptr) {
    }

    bool isLeaf() const { return !left && !right; }
};

class Map {
public:
    Map();
    void generate();
    void print() const;
    const std::array<std::array<char, MAP_W>, MAP_H>& getMap() const;

private:
    std::array<std::array<char, MAP_W>, MAP_H> map;
    std::vector<BSPNode*> leaves;

    void splitNode(BSPNode& node, int minSize, int depth);
    void carveRoom(BSPNode& node);
    void connectNearbyRooms();
};
