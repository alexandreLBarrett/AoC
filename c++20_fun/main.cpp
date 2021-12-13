#include <iostream>

using namespace std;

template <class T>
concept Outputable = requires(T arg) {
    { cout << arg };
    { arg.yeet() } -> std::integral;
};

struct Outputer {
    constexpr Outputer() noexcept = default;

    template<Outputable Out>
    void constexpr Print(ostream& o, Out const& printer) const noexcept {
        o << printer;
    }
};

struct X {
    constexpr X() noexcept = default;

    friend ostream& operator<<(ostream& o, X x) noexcept {
        return o << "Hello World" << x.yeet() + 351;
    }
    constexpr int yeet() const noexcept {
        return 69;
    }
};

struct Y {
    constexpr Y() noexcept = default;
};

int main() {
    constexpr Outputer t;
    constexpr X x;
    t.Print(cout, x);
}

