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
        if (type == TYPE::VALUE) {
            value = src.value;
        } else {
            if (src.left != nullptr)
                left = new Node(*src.left);

            if (src.right != nullptr) 
                right = new Node(*src.right);
        }
    }

    Node<T>* parent = nullptr;

    Node<T>* left = nullptr;
    Node<T>* right = nullptr;

    int64_t depth = 0;

    T value = T();
};