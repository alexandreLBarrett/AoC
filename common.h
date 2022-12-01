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

template<class T = uint8_t>
struct Point {
    int64_t x = 0, y = 0;
    T* value = nullptr;

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

    friend Point<T> operator+(const Point<T>& a, const Point<T>& b) {
        return {a.x + b.x, a.y + b.y};
    }

    bool operator==(const Point<T>& b) {
        return x == b.x && y == b.y;
    }

    Point<T>& operator=(const Point<T>& b) {
        x = b.x;
        y = b.y;
        return *this;
    }

    bool operator<(const Point<T>& a) const {
        if (x == a.x) {
            return y < a.y;
        } else {
            return x < a.x;
        }
        return false;
    }
};