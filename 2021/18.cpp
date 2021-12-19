#include "../helpers.h"
#include "../common.h"
#include "18.h"

#include <queue>
#include <set>

using namespace std;

using MathNode = Node<int16_t>;

class MathProblem {
public:
    MathNode root; 
    MathProblem(ifstream& stream) {
        root.type = MathNode::TYPE::NODE;
        MathNode* current = &root;

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
                    if (current != &root)
                        current = current->parent;
                    break;
                default:
                    current->type = MathNode::TYPE::VALUE;
                    current->value = *it - '0';
                    current = current->parent;
                    break;
            }
        }
    }

    string toString(MathNode* n) {
        if (n->type == MathNode::TYPE::VALUE)
            return to_string(n->value);

        return "[" + toString(n->left) + "," + toString(n->right) + "]";
    }

    void print(ostream& os) {
        os << toString(&root) << endl;
    }

    void explode(MathNode* node) {
        if (node == node->parent->left) {
            node->parent->value += node->right->value;
        } else if (node == node->parent->right) {
            node->parent->value += node->left->value;
        }

        delete node->left;
        delete node->right;
        node->type = MathNode::TYPE::VALUE;
        node->value = 0;
    }

    void splits(MathNode* node) {
        double value = node->value;

        auto n = new MathNode();
        n->type = MathNode::TYPE::VALUE;
        n->value = floor(value / 2);

        node->left = n;

        n = new MathNode();
        n->type = MathNode::TYPE::VALUE;
        n->value = ceil(value / 2);

        node->right = n;
    }

    void reduce() {
        queue<MathNode*> q;
        q.push(&root);

        while (q.size() > 0) {
            auto current = q.front();
            q.pop();

            if (current->depth == 4) {
                explode(current);
            }

            if (current->type == MathNode::TYPE::VALUE) {
                if (current->value >= 10) {
                    splits(current);
                }
                cout << current->value << endl;
            } else {
                q.push(current->left);
                q.push(current->right);
            }
        }
    }
};

int main() {
    FileParser fp("2021/18-data-test");

    // Parse file
    auto problems = fp.parseRest<MathProblem>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        for (auto prob : problems) {
            prob.reduce();
            prob.print(cout);
        }
        out = [=](auto& o) {};
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        out = [=](auto& o) {};
    });

    dph.RunAll(cout);
}
