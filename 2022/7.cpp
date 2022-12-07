#include "../helpers.h"
#include "../common.h"

#include <queue>

using namespace std;

// puzzle: https://adventofcode.com/2022/day/7

struct Node {
    enum class NodeType {
        FOLDER, FILE
    };

    int size = 0;

    virtual NodeType getType() const = 0;
    int getSize() {
        return size;
    }
};

struct File : public Node {
    string filename;

    File(int fileSize, const string& filename) : filename(filename) {
        size = fileSize;
    }

    NodeType getType() const { return Node::NodeType::FILE; };

    File(const File& src) {
        size = src.size;
        filename = src.filename;
    }
};

struct Folder : public Node {
    string foldername;
    Folder* parent;
    vector<Node*> content;

    Folder(const string& folderName, Folder* parent) : foldername(folderName), parent(parent) {}
    NodeType getType() const { return Node::NodeType::FOLDER; };

    Folder(const Folder& src) {
        for (Node* c : src.content) {
            Node* cpy;
            if (c->getType() == Node::NodeType::FOLDER) {
                Folder* fold = new Folder(*reinterpret_cast<Folder*>(c));
                fold->parent = this;
                cpy = fold;
            } else {
                cpy = new File(*reinterpret_cast<File*>(c));
            }
            
            content.push_back(cpy);
        }
        foldername = src.foldername;
    }

    void calculateSize() {
        int total = 0;
        for (Node* n : content) {
            if (n->getType() == Node::NodeType::FOLDER) {
                reinterpret_cast<Folder*>(n)->calculateSize();
            }
            total += n->size;
        }
        size = total;
    }
};

class Filesystem {
    Folder root = Folder("/", nullptr);
public:
    Filesystem(istream& is) {
        Folder* currentDir = &root;

        string s;

        while (getline(is, s)) {
            if (s[0] == '$') {
                // command
                string command = s.substr(2, 2);
                if (command == "cd") {
                    string arg = s.substr(5);
                    if (arg == "..") {
                        currentDir = currentDir->parent;
                    } else if (arg == "/") {
                        currentDir = &root;
                    } else {
                        auto it = find_if(
                            currentDir->content.begin(), 
                            currentDir->content.end(),
                            [&currentDir, &arg](Node* node) {
                                if (node->getType() == Node::NodeType::FOLDER) {
                                    Folder* folder = reinterpret_cast<Folder*>(node);
                                    return folder->foldername == arg;
                                }
                                return false;
                            }
                        );

                        if (it == currentDir->content.end())
                            throw runtime_error("what the shit dir doesnt exist");

                        currentDir = reinterpret_cast<Folder*>(*it);
                    }
                }
            } else {
                int spacePos = s.find_first_of(' ');
                string prefix = s.substr(0, spacePos);
                string suffix = s.substr(spacePos + 1);

                Node* node;
                if (prefix == "dir") {
                    // directory
                    node = new Folder(suffix, currentDir);
                } else {
                    // file
                    node = new File(stoi(prefix), suffix);
                }
                currentDir->content.push_back(node);
            }
        }
    }

    int calculateCleanup() {
        int total = 0;
        root.calculateSize();

        queue<Folder*> folders;
        folders.push(&root);
        while (folders.size() > 0) {
            Folder* current = folders.front();
            folders.pop();

            int size = current->getSize();
            if (size <= 100000) {
                total += size;
            }

            for (Node* n : current->content) {
                 if (n->getType() == Node::NodeType::FOLDER) {
                    folders.push(reinterpret_cast<Folder*>(n));
                }
            }
        }

        return total;
    }

    int calculateCleanupRoom() {
        const int totalSpace = 70000000;
        const int minCleanupSize = 30000000;
        root.calculateSize();

        const int cleanupRemaining = minCleanupSize - (totalSpace - root.getSize());

        Folder* best = &root;

        queue<Folder*> folders;
        folders.push(&root);
        while (folders.size() > 0) {
            Folder* current = folders.front();
            folders.pop();

            int size = current->getSize();
            if (size > cleanupRemaining) {
                if (size < best->getSize())
                    best = current;

                for (Node* n : current->content) {
                    if (n->getType() == Node::NodeType::FOLDER)
                        folders.push(reinterpret_cast<Folder*>(n));
                }
            }
        }

        return best->getSize();
    }
};


int main() {
    FileParser fp("2022/7-data");

    // Parse file
    auto fs = fp.parseOne<Filesystem>();

    DayPartHandler<ostream> dph;

    // Part 1
    dph.AddPart([=](auto& out) mutable {
        auto res = fs.calculateCleanup();
        out = [=](auto& o) {
            o << res << endl;
        };
    });

    // Part 2
    dph.AddPart([=](auto& out) mutable {
        auto res = fs.calculateCleanupRoom();
        out = [=](auto& o) {
            o << res << endl;
        };
    });

    dph.RunAll(cout);
}