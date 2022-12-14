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

template<class T = char>
struct Point {
    int64_t x = 0, y = 0;
    T value;

    Point() = default;
    Point(int64_t x, int64_t y) : x(x), y(y) {}
    Point(int64_t x, int64_t y, T value) : x(x), y(y), value(value) {}

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

    friend bool operator==(const Point<T>& a, const Point<T>& b) {
        return a.x == b.x && a.y == b.y;
    }

    Point<T>& operator=(const Point<T>& b) {
        x = b.x;
        y = b.y;
        value = b.value;
        return *this;
    }

    friend Point<T> operator-(Point& p1, Point<T>& p2) {
        return Point<T>(
            p2.x - p1.x,
            p2.y - p1.y
        );
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