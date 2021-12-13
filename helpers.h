#include <string>
#include <functional>
#include <fstream>
#include <chrono>
#include <ostream>
#include <vector>

class FileParser {
    mutable std::ifstream file;
public:
    FileParser(const std::string& filename) {
        file = std::ifstream(filename);
    }

    template<class T>
    constexpr T parseOne(const std::function<T(std::ifstream&)> process) const noexcept {
        return process(file);
    }

    template<class T>
    constexpr T parseOne() const noexcept {
        return T(file);
    }

    template<class T>
    constexpr std::vector<T> parseRest(const std::function<T(std::ifstream&)> process) const noexcept {
        std::vector<T> container;

        while (file && !file.eof()) {
            container.push_back(process(file));
        }

        return container;
    }

    template<class T>
    constexpr std::vector<T> parseRest() const noexcept {
        return parseRest<T>([](std::ifstream& ss) {
            return T(ss);
        });
    }

    ~FileParser() {
        file.close();
    }
};

class Timer {
public: 
    template<class T>
    static std::chrono::nanoseconds time(T func) {
        auto start = std::chrono::steady_clock::now();

        func();

        return std::chrono::steady_clock::now() - start;
    }

    static std::ostream& print(std::ostream& o, const std::chrono::nanoseconds& val) {
        return o << "[=======| time |======]" << std::endl
            << "s: " << std::chrono::duration_cast<std::chrono::seconds>(val).count() << std::endl
            << "ms: " << std::chrono::duration_cast<std::chrono::milliseconds>(val).count() << std::endl
            << "ns: " << val.count() << std::endl
            << "[=====================]" << std::endl;
    }
};

class DayPartHandler {
public:
    using PrintCallback = std::function<void(std::ostream&)>;
    using LambdaCallback = std::function<void(PrintCallback&)>;

    DayPartHandler() = default;

    void AddPart(LambdaCallback func) {
        functions.push_back(func);
    }

    void RunAll(std::ostream& o) const {
        PrintCallback printResults;

        int i = 1;
        for (const auto func : functions) {
            o << "Part " << i++ << ": " << std::endl;
            Timer::print(o, Timer::time([&]{
                func(printResults);
            }));

            printResults(o);
            o << std::endl << std::endl;
        }
    }
private:
    std::vector<LambdaCallback> functions;
};