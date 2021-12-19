#include <queue>
#include <algorithm>

using namespace std;

template<class T>
class Node {
public: 
    enum class TraversalType { PRE, IN, POST };
private:
    bool traverse(Node<T>* nextNode, TraversalType type, function<bool(Node<T>*)> apply) {
        bool stop = false;
        if (!stop && type == TraversalType::PRE)
            stop = apply(nextNode);

        if (!stop && nextNode->left)
            stop = traverse(nextNode->left, type, apply);

        if (!stop && type == TraversalType::IN)
            stop = apply(nextNode);

        if (!stop && nextNode->right)
            stop = traverse(nextNode->right, type, apply);

        if (!stop && type == TraversalType::POST)
            stop = apply(nextNode);

        return stop;
    }

    void updateDepth(Node<T>* node, int newDepth) {
        node->depth = newDepth;
        if (node->left)
            updateDepth(node->left, newDepth + 1);
        if (node->right)
            updateDepth(node->right, newDepth + 1);
    }
public:
    Node<T>* parent = nullptr;
    Node<T>* left = nullptr;
    Node<T>* right = nullptr;
    int64_t depth = 0;
    T value = T();
    enum class TYPE {
        NODE,
        VALUE,
    } type = TYPE::NODE;

    Node() = default;
    Node(TYPE type) : type(type) {}
    Node(const Node& src) {
        *this = src;
    }
    ~Node() {
        delete left;
        left = nullptr;
        delete right;
        right = nullptr;
    }

    Node<T>& operator=(const Node<T>& src) {
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
        return *this;
    }

    bool traverse(TraversalType type, function<bool(Node<T>*)> apply) {
        return traverse(this, type, apply);
    }

    void updateDepth() {
        updateDepth(this, 0);
    }
};

