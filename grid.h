#pragma once
#include <vector>
#include <set>

#include "common.h"

template<class T>
struct GridNode {
    enum class Sibling {
        TOP_LEFT = 0, 
        BOTTOM_RIGHT,
        TOP, 
        BOTTOM, 
        TOP_RIGHT,
        BOTTOM_LEFT, 
        LEFT,
        RIGHT
    };

    Point<T> offset(Sibling s) {
        switch (s) {
            case Sibling::LEFT:         return { -1, 0 };
            case Sibling::TOP_LEFT:     return { -1, -1 };
            case Sibling::TOP:          return { 0, -1 };
            case Sibling::TOP_RIGHT:    return { 1, -1 };
            case Sibling::RIGHT:        return { 1, 0 };
            case Sibling::BOTTOM_RIGHT: return { 1, 1 };
            case Sibling::BOTTOM:       return { 0, 1 };
            case Sibling::BOTTOM_LEFT:  return { -1, 1 };
        }
        return { 0,0 };
    }

    Sibling opposite(Sibling s) {   
        uint8_t sibIndex = (uint8_t)s;
        return static_cast<Sibling>(sibIndex + (sibIndex % 2 == 0 ? 1 : -1 ));
    }

    T value;
    Point<T> pos{0,0};

    GridNode() = default;
    GridNode(const T& value) 
        : value(value)
    { }

    GridNode<T>* sibs[8] { nullptr };

    void set(const T& val) {
        value = val;
    }

    GridNode<T>* get(Sibling s) {
        return sibs[(uint8_t)s];
    }
};

template<class T>
class Grid {
    GridNode<T>* origin = new GridNode<T>(); 
    Point<T> min = { 0, 0 };
    Point<T> max = { 0, 0 };

    mutable std::set<Point<GridNode<T>>> nodes { Point<GridNode<T>>{ 0, 0, origin } };
public:
    using Sibling = GridNode<T>::Sibling;

    Grid() = default;
    Grid(const Grid& src) {
        *this = src;
    }

    ~Grid() {
        std::for_each(std::begin(nodes), std::end(nodes), [](auto* node) {
            delete node;
            node = nullptr;
        });
    }

    Grid<T>& operator=(const Grid<T>& src) {
        std::for_each(std::begin(src.nodes), std::end(src.nodes), [this](auto* node) {
            set(node->pos.x, node->pos.y, node->value);
        });
        return *this;
    }

    void bind(GridNode<T>* a, GridNode<T>* b, Sibling aTob) const {
        if (a)
            a->sibs[(uint8_t)aTob] = b;
        if (b)
            b->sibs[(uint8_t)a->opposite(aTob)] = a;
    }

    auto findNode(int64_t x, int64_t y) const {
        return std::find_if(nodes.begin(), nodes.end(), [=](auto node) {
            return node == Point<GridNode<T>>{ x, y, nullptr };
        });
    }

    void foreachPixel(std::function<void(GridNode<T>*)> func) const {
        for (int64_t y = min.y - 1; y <= max.y + 1; y++) {
            for (int64_t x = min.x - 1; x <= max.x + 1; x++) {
                func(get(x, y));
            }
        }
    }

    void foreachNode(std::function<void(GridNode<T>*)> func) const {
        std::for_each(nodes.begin(), nodes.end(), func);
    }

    bool exists(int64_t x, int64_t y) const {
        return findNode(x, y) != nodes.end();
    }

    void print(std::ostream& os, std::function<char(const T&)> func) const {
        for (int64_t y = min.y - 1; y <= max.y + 1; y++) {
            for (int64_t x = min.x - 1; x <= max.x + 1; x++) {
                os << func(get(x, y)->value);
            }
            os << std::endl;
        }
    }

    void set(int64_t x, int64_t y, const T& value) {
        get(x, y)->set(value);

        if (value != T()) {
            min.x = std::min(min.x, x);
            max.x = std::max(max.x, x);
            min.y = std::min(min.y, y);
            max.y = std::max(max.y, y);
        }
    }

    GridNode<T>* get(int64_t x, int64_t y) const {
        auto found = nodes.find({ x, y, nullptr });

        auto nodeIt = findNode(x, y);
        if (nodeIt != nodes.end())
            return nodeIt->value;

        GridNode<T>* current = origin;
        GridNode<T>* temp = nullptr;
        Sibling sib;

        while(x != 0 || y != 0) {
            if (x > 0 && y < 0) {
                temp = current->sibs[(uint8_t)Sibling::TOP_RIGHT];
                if (!temp) {
                    temp = new GridNode<T>();
                    temp->pos = current->pos + current->offset(Sibling::TOP_RIGHT);

                    bind(temp, current, Sibling::BOTTOM_LEFT);
                    bind(temp, current->get(Sibling::TOP),  Sibling::LEFT);
                    bind(temp, current->get(Sibling::RIGHT),  Sibling::BOTTOM);
                    
                    auto[x, y, _] = temp->pos;
                    if (auto it = findNode(x - 1, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP_LEFT);
                    if (auto it = findNode(x, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP);
                    if (auto it = findNode(x + 1, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP_RIGHT);
                    if (auto it = findNode(x + 1, y); it != nodes.end()) bind(temp, it->value, Sibling::RIGHT);
                    if (auto it = findNode(x + 1, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM_RIGHT);
                }
            } else if (x > 0 && y == 0) {
                temp = current->sibs[(uint8_t)Sibling::RIGHT];
                if (!temp) {
                    temp = new GridNode<T>();
                    temp->pos = current->pos + current->offset(Sibling::RIGHT);

                    bind(temp, current, Sibling::LEFT);
                    bind(temp, current->get(Sibling::TOP),  Sibling::TOP_LEFT);
                    bind(temp, current->get(Sibling::BOTTOM),  Sibling::BOTTOM_LEFT);
                    bind(temp, current->get(Sibling::TOP_RIGHT),  Sibling::TOP);
                    bind(temp, current->get(Sibling::BOTTOM_RIGHT), Sibling::BOTTOM);
                    
                    auto[x, y, _] = temp->pos;
                    if (auto it = findNode(x + 1, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP_RIGHT);
                    if (auto it = findNode(x + 1, y); it != nodes.end()) bind(temp, it->value, Sibling::RIGHT);
                    if (auto it = findNode(x + 1, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM_RIGHT);
                }
            } else if (x > 0 && y > 0) {
                temp = current->sibs[(uint8_t)Sibling::BOTTOM_RIGHT];
                if (!temp) {
                    temp = new GridNode<T>();
                    temp->pos = current->pos + current->offset(Sibling::BOTTOM_RIGHT);

                    bind(temp, current, Sibling::TOP_LEFT);
                    bind(temp, current->get(Sibling::RIGHT),  Sibling::TOP);
                    bind(temp, current->get(Sibling::BOTTOM),  Sibling::LEFT);

                    auto[x, y, _] = temp->pos;
                    if (auto it = findNode(x - 1, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM_LEFT);
                    if (auto it = findNode(x, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM);
                    if (auto it = findNode(x + 1, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP_RIGHT);
                    if (auto it = findNode(x + 1, y); it != nodes.end()) bind(temp, it->value, Sibling::RIGHT);
                    if (auto it = findNode(x + 1, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM_RIGHT);
                }
            } else if (x == 0 && y > 0) {
                temp = current->sibs[(uint8_t)Sibling::BOTTOM];
                if (!temp) {
                    temp = new GridNode<T>();
                    temp->pos = current->pos + current->offset(Sibling::BOTTOM);

                    bind(temp, current, Sibling::TOP);
                    bind(temp, current->get(Sibling::BOTTOM_RIGHT),  Sibling::RIGHT);
                    bind(temp, current->get(Sibling::LEFT),  Sibling::TOP_LEFT);
                    bind(temp, current->get(Sibling::RIGHT),  Sibling::TOP_RIGHT);
                    bind(temp, current->get(Sibling::BOTTOM_LEFT), Sibling::LEFT);
                    
                    auto[x, y, _] = temp->pos;
                    if (auto it = findNode(x - 1, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM_LEFT);
                    if (auto it = findNode(x, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM);
                    if (auto it = findNode(x + 1, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM_RIGHT);
                }
            } else if (x == 0 && y < 0) {
                temp = current->sibs[(uint8_t)Sibling::TOP];
                if (!temp) {
                    temp = new GridNode<T>();
                    temp->pos = current->pos + current->offset(Sibling::TOP);

                    bind(temp, current, Sibling::BOTTOM);
                    bind(temp, current->get(Sibling::RIGHT),  Sibling::BOTTOM_RIGHT);
                    bind(temp, current->get(Sibling::TOP_LEFT),  Sibling::LEFT);
                    bind(temp, current->get(Sibling::TOP_RIGHT),  Sibling::RIGHT);
                    bind(temp, current->get(Sibling::LEFT), Sibling::BOTTOM_LEFT);
                    
                    auto[x, y, _] = temp->pos;
                    if (auto it = findNode(x - 1, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP_LEFT);
                    if (auto it = findNode(x, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP);
                    if (auto it = findNode(x + 1, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP_RIGHT);
                }
            } else if (x < 0 && y < 0) {
                temp = current->sibs[(uint8_t)Sibling::TOP_LEFT];
                if (!temp) {
                    temp = new GridNode<T>();
                    temp->pos = current->pos + current->offset(Sibling::TOP_LEFT);

                    bind(temp, current, Sibling::BOTTOM_RIGHT);
                    bind(temp, current->get(Sibling::TOP),  Sibling::RIGHT);
                    bind(temp, current->get(Sibling::LEFT), Sibling::BOTTOM);
                    
                    auto[x, y, _] = temp->pos;
                    if (auto it = findNode(x - 1, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM_LEFT);
                    if (auto it = findNode(x - 1, y); it != nodes.end()) bind(temp, it->value, Sibling::LEFT);
                    if (auto it = findNode(x - 1, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP_LEFT);
                    if (auto it = findNode(x, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP);
                    if (auto it = findNode(x + 1, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP_RIGHT);
                }
            } else if (x < 0 && y == 0) {
                temp = current->sibs[(uint8_t)Sibling::LEFT];
                if (!temp) {
                    temp = new GridNode<T>();
                    temp->pos = current->pos + current->offset(Sibling::LEFT);

                    bind(temp, current, Sibling::RIGHT);
                    bind(temp, current->get(Sibling::TOP_LEFT),    Sibling::TOP);
                    bind(temp, current->get(Sibling::TOP),         Sibling::TOP_RIGHT);
                    bind(temp, current->get(Sibling::BOTTOM),      Sibling::BOTTOM_RIGHT);
                    bind(temp, current->get(Sibling::BOTTOM_LEFT), Sibling::BOTTOM);
                    
                    auto[x, y, _] = temp->pos;
                    if (auto it = findNode(x - 1, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP_LEFT);
                    if (auto it = findNode(x - 1, y); it != nodes.end())     bind(temp, it->value, Sibling::LEFT);
                    if (auto it = findNode(x - 1, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM_LEFT);
                }
            } else if (x < 0 && y > 0) {
                temp = current->sibs[(uint8_t)Sibling::BOTTOM_LEFT];
                if (!temp) {
                    temp = new GridNode<T>();
                    temp->pos = current->pos + current->offset(Sibling::BOTTOM_LEFT);

                    bind(temp, current, Sibling::TOP_RIGHT);
                    bind(temp, current->get(Sibling::LEFT), Sibling::TOP);
                    bind(temp, current->get(Sibling::BOTTOM), Sibling::RIGHT);
                    
                    auto[x, y, _] = temp->pos;
                    if (auto it = findNode(x - 1, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM_LEFT);
                    if (auto it = findNode(x - 1, y); it != nodes.end()) bind(temp, it->value, Sibling::LEFT);
                    if (auto it = findNode(x - 1, y - 1); it != nodes.end()) bind(temp, it->value, Sibling::TOP_LEFT);
                    if (auto it = findNode(x, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM);
                    if (auto it = findNode(x + 1, y + 1); it != nodes.end()) bind(temp, it->value, Sibling::BOTTOM_RIGHT);
                }
            }

            current = temp;

            if (x > 0) x--;
            if (x < 0) x++;
            if (y > 0) y--;
            if (y < 0) y++;

            nodes.insert({ current->pos.x, current->pos.y, current });
        }

        return current;
    }
};