#include "../helpers.h"
#include "../common.h"

#include <map>
#include <queue>
#include <list>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/20

struct Node {
    Node* left = nullptr;
    Node* right = nullptr;
    int32_t value;

    Node(int32_t v) : value(v) {}
};

uint32_t nextPos(int32_t current, uint32_t size, int32_t offset) {
    if (current + offset < 0) {
        offset += size;
    }
    return (current + offset) % size;
}

Node* advance(Node* curr, Node* head, Node* tail, bool neg, uint32_t steps) {
    Node* current = curr;
    for (int i = 0; i < steps; ++i) {
        current = neg 
            ? current->left == nullptr ? tail : current->left 
            : current->right == nullptr ? head : current->right;
    }
    return current;
}

int64_t displace(const vector<int32_t>& input) {
    vector<Node*> nodes = { new Node(input[0]) };
    Node* head = nodes[0];
    Node* tail = nullptr;
    Node* zero = nullptr;
    
    for (int i = 1; i < input.size(); ++i) {
        auto node = new Node(input[i]);

        if (node->value == 0)
            zero = node;

        auto prev = nodes.back();

        node->left = prev;
        prev->right = node;

        nodes.push_back(node);
    }
    tail = nodes.back();

    for (auto node : nodes) {
        bool neg = node->value < 0;
        Node* current = advance(node, head, tail, neg, abs(node->value));

        if (current == node)
            continue;

        // remove node from its original position
        if (node->left != nullptr) {
            node->left->right = node->right;
        } else {
            head = node->right;
        }

        if (node->right != nullptr)  {
            node->right->left = node->left;
        } else {
            tail = node->left;
        }

        // If negative place to left
        if (neg) {
            if (current->left != nullptr) 
                current->left->right = node;
            node->left = current->left;
            node->right = current;
            current->left = node;
        // If pos place to right
        } else {
            if (current->right != nullptr) 
                current->right->left = node;
            node->right = current->right;
            node->left = current;
            current->right = node;
        }

        if (node->left == nullptr) head = node;
        if (node->right == nullptr) tail = node;
    }

    auto first = advance(zero, head, tail, false, 1000)->value;    
    auto second = advance(zero, head, tail, false, 2000)->value;  
    auto third = advance(zero, head, tail, false, 3000)->value;  

    for (auto node : nodes) {
        delete node;
    }

    return first + second + third;
}

int main() {
    FileParser fp("2022/20-data");

    // Parse file

    auto input = fp.parseRest<int32_t>([](auto& is) {
        int32_t i = 0;
        is >> i;
        return i;
    });

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        auto resp = displace(input);
        out = [=](auto& o) {
            o << resp;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {

        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}