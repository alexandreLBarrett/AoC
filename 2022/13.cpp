#include "../helpers.h"
#include "../common.h"

using namespace std;

// puzzle: https://adventofcode.com/2022/day/13

enum NodeType { VALUE, CONTAINER };
class Node {
public:
    virtual NodeType getType() const = 0;
    virtual unique_ptr<Node> clone() const = 0;
    // virtual int compare(const ValueNode& node) const = 0;
    // virtual int compare(const ContainerNode& node) const = 0;
};

class ValueNode : public Node {
public:
    uint32_t value;
    ValueNode(uint32_t value) : value(value) {}
    // int compare(const ValueNode& node) const override;
    // int compare(const ContainerNode& node) const override;

    NodeType getType() const { return VALUE; }
    unique_ptr<Node> clone() const {
        return make_unique<ValueNode>(value);
    }
};

class ContainerNode : public Node {
public:
    vector<unique_ptr<Node>> children;
    ContainerNode() = default;
    ContainerNode(const ContainerNode& node) {
        *this = node;
    }
    ContainerNode(istream& is) {
        char c;
        is >> c;
        while (c != ']') {
            if (c == ',')
                is >> c;

            if (c == '[') {
                children.push_back(make_unique<ContainerNode>(is));
                is >> c;
            } else {
                string currentVal = "";
                while (c != ',' && c != ']') {
                    currentVal += c;
                    is >> c;
                }
                children.push_back(make_unique<ValueNode>(stoi(currentVal)));
            }
        }
    }

    // int compare(const ValueNode& node) const override;
    // int compare(const ContainerNode& node) const override;

    ContainerNode& operator=(const ContainerNode& other) {
        for (int i = 0; i < other.children.size(); ++i) {
            children.push_back(other.children[i]->clone());
        }
        return *this;
    }

    NodeType getType() const { return CONTAINER; }
    unique_ptr<Node> clone() const {
        unique_ptr<ContainerNode> node = make_unique<ContainerNode>();
        for (int i = 0; i < children.size(); ++i) {
            node->children.push_back(children[i]->clone());
        }
        return node;
    }
};

int compare(const ValueNode& value, const ContainerNode& cont);
int compare(const ContainerNode& cont, const ValueNode& value);

int compare(const ValueNode& value1, const ValueNode& value2) {
    if (value1.value == value2.value)
        return 0;

    if (value1.value < value2.value)
        return -1;

    return 1;
}

int compare(const ContainerNode& cont1, const ContainerNode& cont2) {
    for (int i = 0; i < cont1.children.size(); ++i) {
        const unique_ptr<Node>& node1 = cont1.children[i];

        if (i >= cont2.children.size()) {
            return 1;
        }

        const unique_ptr<Node>& node2 = cont2.children[i];

        int comp = 0;
        if (node1->getType() == VALUE) {
            if (node2->getType() == VALUE) {
                ValueNode* n1 = static_cast<ValueNode*>(node1.get());
                ValueNode* n2 = static_cast<ValueNode*>(node2.get());
                comp = compare(*n1, *n2);
            } else {
                ValueNode* n1 = static_cast<ValueNode*>(node1.get());
                ContainerNode* n2 = static_cast<ContainerNode*>(node2.get());
                comp = compare(*n1, *n2);
            }
        } else {
            if (node2->getType() == CONTAINER) {
                ContainerNode* n1 = static_cast<ContainerNode*>(node1.get());
                ContainerNode* n2 = static_cast<ContainerNode*>(node2.get());
                comp = compare(*n1, *n2);
            } else {
                ContainerNode* n1 = static_cast<ContainerNode*>(node1.get());
                ValueNode* n2 = static_cast<ValueNode*>(node2.get());
                comp = compare(*n1, *n2);
            }
        }

        if (comp != 0)
            return comp;
    }

    if (cont1.children.size() == cont2.children.size())
        return 0;

    return -1;
}

int compare(const ValueNode& value, const ContainerNode& cont) {
    ContainerNode node;
    node.children.push_back(value.clone());
    return compare(node, cont);
}

int compare(const ContainerNode& cont, const ValueNode& value) {
    ContainerNode node;
    node.children.push_back(value.clone());
    return compare(cont, node);
}


class Comparison {
public:
    unique_ptr<ContainerNode> left;
    unique_ptr<ContainerNode> right;
    Comparison(const Comparison& other) {
        left = unique_ptr<ContainerNode>(static_cast<ContainerNode*>(other.left->clone().release()));
        right = unique_ptr<ContainerNode>(static_cast<ContainerNode*>(other.right->clone().release()));
    }
    Comparison(istream& is) {
        char c;
        is >> c; // burn [
        left = make_unique<ContainerNode>(is);
        is >> c; // burn [
        right = make_unique<ContainerNode>(is);
        string s;
        getline(is, s);
    }

    bool inOrder() const {
        return compare(*left, *right) < 0;
    }
};

int main() {
    FileParser fp("2022/13-data");

    // Parse file
    auto comps = fp.parseRest<Comparison>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        int total = 0;
        for (int i = 0; i < comps.size(); ++i) {
            if (comps[i].inOrder()) {
                total += (i + 1);
            }
        }

        out = [=](auto& o) {
            o << total << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        vector<unique_ptr<ContainerNode>> nodes;
        unique_ptr<ContainerNode> sep1 = make_unique<ContainerNode>();
        unique_ptr<ContainerNode> sep11 = make_unique<ContainerNode>();
        sep11->children.push_back(make_unique<ValueNode>(2));
        sep1->children.push_back(move(sep11));
        nodes.emplace_back(static_cast<ContainerNode*>(sep1->clone().release()));

        unique_ptr<ContainerNode> sep2 = make_unique<ContainerNode>();
        unique_ptr<ContainerNode> sep22 = make_unique<ContainerNode>();
        sep22->children.push_back(make_unique<ValueNode>(6));
        sep2->children.push_back(move(sep22));
        nodes.emplace_back(static_cast<ContainerNode*>(sep2->clone().release()));

        for (const auto& comp : comps) {
            nodes.emplace_back(static_cast<ContainerNode*>(comp.left->clone().release()));
            nodes.emplace_back(static_cast<ContainerNode*>(comp.right->clone().release()));
        }

        sort(nodes.begin(), nodes.end(), [&](const auto& n1, const auto& n2) {
            return compare(*n1, *n2) < 0;
        });

        int sep1Pos = 0;
        int sep2Pos = 0;
        for (int i = 0; i < nodes.size(); ++i) {
            if (sep1Pos == 0 && compare(*nodes[i], *sep1) == 0)
                sep1Pos = i + 1;

            if (sep2Pos == 0 && compare(*nodes[i], *sep2) == 0)
                sep2Pos = i + 1;
        }

        out = [=](auto& o) {
            o << sep1Pos * sep2Pos << endl;
        };
    });

    dph.RunAll(cout);
}