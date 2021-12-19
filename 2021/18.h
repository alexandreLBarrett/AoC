#pragma once
#include <queue>

template<class T>
class Node {
public:
    enum class TYPE {
        NODE,
        VALUE,
    } type = TYPE::NODE;
    Node() = default;

    Node(TYPE type) : type(type) {}
    Node(const Node& src) {
        depth = src.depth;
        type = src.type;
        value = src.value;

        if (src.left) {
            left = new Node(*src.left);
            left->parent = this;
        }

        if (src.right)  {
            right = new Node(*src.right);
            right->parent = this;
        }
    }

    Node<T>* parent = nullptr;

    Node<T>* left = nullptr;
    Node<T>* right = nullptr;

    int64_t depth = 0;

    T value = T();
};