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

template<class IndexType = int64_t, class ValueType = char>
struct Point {
    IndexType x = 0, y = 0;
    ValueType value;

    Point() = default;
    Point(IndexType x, IndexType y) : x(x), y(y) {}
    Point(IndexType x, IndexType y, ValueType value) : x(x), y(y), value(value) {}

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

    friend bool operator==(const Point& a, const Point& b) {
        return a.x == b.x && a.y == b.y;
    }

    Point& operator=(const Point& b) {
        x = b.x;
        y = b.y;
        value = b.value;
        return *this;
    }

    friend Point operator-(Point& p1, Point& p2) {
        return Point(
            p2.x - p1.x,
            p2.y - p1.y
        );
    }
    
    friend Point operator^(const Point& p1, const Point& p2) {

    }

    friend double operator*(const Point& p1, const Point& p2) {
        return p1.x * p2.x + p1.y + p2.y;
    }

    bool operator<(const Point& a) const {
        if (x == a.x) {
            return y < a.y;
        } else {
            return x < a.x;
        }
        return false;
    }
};

template<class IndexType, class ValueType = char>
std::ostream& operator<<(std::ostream& o, const Point<IndexType, ValueType>& p) {
    o << "(" << p.x << "," << p.y << ")";
    return o;
}