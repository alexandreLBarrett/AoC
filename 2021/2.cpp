#include "../helpers.h"

#include <iostream>

using namespace std;

struct Command {
    enum class Direction {
        UP, DOWN, FORWARD, ERROR = -1
    } direction;
    int value;

    Command() = default;
    
    Command(istream& i) {
        i >> *this;
    }

    Command(string _direction, int _value) {
        direction = parseDirection(_direction);
        value = _value;
    }

    friend istream& operator>>(istream& i, Command& c) {
        string temp;
        i >> temp;
        c.direction = parseDirection(temp);
        i >> c.value;
        return i;
    }
private:
    static Direction parseDirection(const string& strDir) {
        if (strDir == "up")
            return Direction::UP;
        if (strDir == "down")
            return Direction::DOWN;
        if (strDir == "forward")
            return Direction::FORWARD;

        return Direction::ERROR;
    }
};

class Sub {
    struct Position {
        int horizontal = 0;
        int depth = 0;
        int aim = 0;

        friend ostream& operator<<(ostream& o, const Position& p) {
            o << "horizontal position: " << p.horizontal << ", depth: " << p.depth << " - " << p.depth * p.horizontal;
            return o;
        }
    } position;

public:
    Position getPosition() {
        return position;
    }

    void takeCommand(const Command& command) {
        switch(command.direction) {
            case Command::Direction::FORWARD:
                position.horizontal += command.value;
                break;
            case Command::Direction::DOWN:
                position.depth += command.value;
                break;
            case Command::Direction::UP:
                position.depth -= command.value;
                break;
        }
    }

    void takeCommand2(const Command& command) {
        switch(command.direction) {
            case Command::Direction::FORWARD:
                position.horizontal += command.value;
                position.depth += position.aim * command.value;
                break;
            case Command::Direction::DOWN:
                position.aim += command.value;
                break;
            case Command::Direction::UP:
                position.aim -= command.value;
                break;
        }
    }

    void takeCommands(const vector<Command>& commands) {
        for (auto command : commands) {
            takeCommand(command);
        }
    }

    void takeCommands2(const vector<Command>& commands) {
        for (auto command : commands) {
            takeCommand2(command);
        }
    }
};

int main() {
    FileParser fp("2021/2-data");
    
    auto commands = fp.parseRest<Command>();

    Sub sub1;
    Timer::print(cout, Timer::time([&]{
        sub1.takeCommands(commands);
    }));
    cout << "Part 1: " << sub1.getPosition() << endl;

    Sub sub2;
    Timer::print(cout, Timer::time([&]{
        sub2.takeCommands2(commands);
    }));
    cout << "Part 2: " << sub2.getPosition() << endl;
}