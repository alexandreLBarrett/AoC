#include "../helpers.h"
#include "../common.h"
#include "18.h"

#include <queue>

using namespace std;

using MathNode = Node<int16_t>;

class MathProblem {
    MathNode* root = nullptr; 

    list<MathNode*> leafs;

    bool reduceOne() {
        return root->traverse(MathNode::TraversalType::PRE, [this](MathNode* node){
            if (node->type != MathNode::TYPE::VALUE) {
                if (node->depth == 4) {
                    explode(node);
                    return true;
                }
            }
            return false;
        }) || 
         root->traverse(MathNode::TraversalType::PRE, [this](MathNode* node){
            if (node->type == MathNode::TYPE::VALUE) {
                if (node->value >= 10) {
                    splits(node);
                    return true;
                }
            }
            return false;
        });
    }

    void reduce() {
        while (reduceOne());
    }
public:
    ~MathProblem() {
        leafs.clear();
        delete root;
        root = nullptr;
    }

    MathProblem(const MathProblem& src) {
        *this = src;
    }

    MathProblem(ifstream& stream) {
        root = new MathNode();
        root->type = MathNode::TYPE::NODE;
        MathNode* current = root;

        string line;
        getline(stream, line);

        stringstream ss(line);
        for (istreambuf_iterator<char> it(ss), end; it != end; ++it) {
            switch (*it) {
                case '[': {
                    auto n = new MathNode();
                    n->type = MathNode::TYPE::NODE;
                    n->parent = current;
                    n->depth = current->depth + 1;

                    current->left = n;
                    current = n;
                    break;
                }
                case ',': {
                    auto n = new MathNode();
                    n->type = MathNode::TYPE::NODE;
                    n->parent = current;
                    n->depth = current->depth + 1;

                    current->right = n;
                    current = n;
                    break;
                }
                case ']':
                    if (current != root)
                        current = current->parent;
                    break;
                default:
                    current->type = MathNode::TYPE::VALUE;
                    current->value = *it - '0';
                    leafs.push_back(current);
                    current = current->parent;
                    break;
            }
        }
        reduce();
    }

    string toString(MathNode* n) const {
        if (n->type == MathNode::TYPE::VALUE)
            return to_string(n->value);

        return "[" + toString(n->left) + "," + toString(n->right) + "]";
    }

    void print(ostream& os) const {
        os << toString(root) << endl;
    }

    void explode(MathNode* node) {
        auto leftIt = find(begin(leafs), end(leafs), node->left);
        if (leftIt != begin(leafs)) {
            (*prev(leftIt))->value += node->left->value;
        }

        auto rightIt = find(begin(leafs), end(leafs), node->right);
        if (next(rightIt) != end(leafs)) {
            (*next(rightIt))->value += node->right->value;
        }

        leafs.insert(leftIt, node);
        leafs.erase(leftIt);
        leafs.erase(rightIt);

        delete node->left;
        node->left = nullptr;
        delete node->right;
        node->right = nullptr;

        node->type = MathNode::TYPE::VALUE;
        node->value = 0;
    }

    void splits(MathNode* node) {
        double value = node->value;
        auto it = find(begin(leafs), end(leafs), node);

        auto n = new MathNode();
        n->type = MathNode::TYPE::VALUE;
        n->value = floor(value / 2);
        n->depth = node->depth + 1;
        node->left = n;

        leafs.insert(it, n);

        n = new MathNode();
        n->type = MathNode::TYPE::VALUE;
        n->value = ceil(value / 2);
        n->depth = node->depth + 1;
        node->right = n;

        leafs.insert(it, n);
        leafs.erase(it);

        node->type = MathNode::TYPE::NODE;
        node->value = 0;
    }



    int64_t getMagnitude() const {
        return getMagnitude(root);
    }

    int64_t getMagnitude(MathNode* node) const {
        if (node->type == MathNode::TYPE::VALUE) {
            return node->value;
        }

        return (3 * getMagnitude(node->left)) + (2 * getMagnitude(node->right));
    }

    MathProblem& operator+=(const MathProblem& mathP) {
        MathNode* n = new MathNode();
        n->left = root;
        root->parent = n;

        auto* node = new MathNode(*mathP.root);
        n->right = node;
        node->parent = n;

        root = n;

        leafs.clear();
        root->traverse(MathNode::TraversalType::PRE, [this](MathNode* node) {
            if (node->type == MathNode::TYPE::VALUE) {
                leafs.push_back(node);
            }
            return false;
        });

        root->updateDepth();

        reduce();

        return *this;
    }

    MathProblem operator+(const MathProblem& mathP) {
        MathProblem prob = *this;
        prob += mathP;
        return prob;
    }

    MathProblem& operator=(const MathProblem& src) {
        if (this == &src)
            return *this;

        leafs.clear();

        delete root;
        root = new MathNode(*src.root);

        root->traverse(MathNode::TraversalType::PRE, [this](MathNode* node) {
            if (node->type == MathNode::TYPE::VALUE) {
                leafs.push_back(node);
            }
            return false;
        });

        root->updateDepth();

        reduce();

        return *this;
    }
};

int main() {
    FileParser fp("2021/18-data");

    // Parse file
    auto problems = fp.parseRest<MathProblem>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        auto p = problems[0];
        for (int i = 1; i < problems.size(); i++) {
            p += problems[i];
        }
        out = [=](auto& o) { 
            p.print(o);
            o << p.getMagnitude() << endl; 
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        int64_t maxVal = 0;
        for (int i = 0; i < problems.size(); i++) {
            cout << endl;
            for (int j = i + 1; j < problems.size(); j++) {
                maxVal = max(
                    (problems[i] + problems[j]).getMagnitude(),
                    maxVal
                );
                maxVal = max(
                    (problems[j] + problems[i]).getMagnitude(),
                    maxVal
                );
            }
        }
        out = [=](auto& o) { 
            o << maxVal << endl;
        };
    });

    dph.RunAll(cout);
}
