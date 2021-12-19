#pragma once

#include <iostream>
#include <algorithm>
#include <sstream>
#include <cassert>

struct VectorHash {
    size_t operator()(const std::vector<std::string>& v) const {
        std::hash<std::string> hasher;
        size_t seed = 0;
        for (std::string i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};

struct Point {
    int64_t x = 0, y = 0;

    int toIndex(Point p, uint64_t lineLength) {
        return x + y * lineLength;
    }

    void fromIndex(uint64_t i, uint64_t lineLength) {
        x = i % lineLength;
        y = i / lineLength;
    }

    bool valid() {
        return x > 0 && y > 0;
    }

    friend Point operator+(const Point& a, const Point& b) {
        return {a.x + b.x, a.y + b.y};
    }

    bool operator==(const Point& b) {
        return x == b.x && y == b.y;
    }
};