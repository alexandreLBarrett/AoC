#pragma once

#include <string>
#include <functional>
#include <fstream>
#include <chrono>
#include <iostream>
#include <vector>

#include <clocale>


class FileParser {
    mutable std::ifstream file;
public:
    FileParser(const std::string& filename) 
        : file{filename}
    {
    }

    template<class T>
    constexpr T parseOne(const std::function<T(std::ifstream&)> process) const noexcept {
        return process(file);
    }

    template<class T>
    constexpr T parseOne() const noexcept {
        T val;
        file >> val;
        return val;
    }

    template<class T>
    constexpr std::vector<T> parseRest(const std::function<T(std::ifstream&)> process) const noexcept {
        std::vector<T> container;

        while (file && !file.eof()) {
            container.push_back(parseOne<T>(process));
        }

        return container;
    }

    template<class T>
    constexpr std::vector<T> parseRest() const noexcept {
        std::vector<T> container;

        while (file && !file.eof()) {
            container.push_back(parseOne<T>());
        }

        return container;
    }
};

template<class StreamType>
class DayPartHandler {
    template<class T>
    static std::chrono::nanoseconds time(T func) {
        auto start = std::chrono::steady_clock::now();

        func();

        return std::chrono::steady_clock::now() - start;
    }

    static StreamType& print(StreamType& o, const std::chrono::nanoseconds& val) {
        o << "[=======| time |======]" << std::endl;

        double time = std::chrono::duration_cast<std::chrono::duration<double>>(val).count();
        if (time > 1) {
            o << time << "s" << std::endl;
        } else {
            auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(val).count();
            auto nano = std::chrono::duration_cast<std::chrono::nanoseconds>(val).count();

            double millis = milli + nano / 1000.0f;

            o << millis << "ms" << std::endl;
        }
        o << "[=====================]" << std::endl;
        return o;
    }
public:
    using PrintCallback = std::function<void(StreamType&)>;
    using LambdaCallback = std::function<void(PrintCallback&)>;

    DayPartHandler() {
        setlocale(LC_ALL, "en_US.UTF-8");
    };

    void AddPart(LambdaCallback func) {
        functions.push_back(func);
    }

    void RunAll(StreamType& o) const {
        PrintCallback printResults = [](auto&){};

        int i = 1;
        for (const auto func : functions) {
            o << "Part " << i++ << ": \n";

            print(o, time([&]{
                func(printResults);
            }));

            printResults(o);
            o << std::endl << std::endl;
        }
    }
private:
    std::vector<LambdaCallback> functions;
};

int constexpr length(const char* str)
{
    return *str ? 1 + length(str + 1) : 0;
}