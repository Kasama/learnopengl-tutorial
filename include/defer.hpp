#ifndef TUTORIAL_DEFER_HPP
#define TUTORIAL_DEFER_HPP
#pragma once

template <typename Func>
struct ScopeExit {
    Func f;
    ScopeExit(const Func &f) : f(f) {}
    ~ScopeExit() { f(); }
};

struct ScopeExitHelper {
    template <typename Func>
            ScopeExit<Func> operator+(Func func) { return func; }
};

#define CONCAT_INTERNAL(x,y) x##y
#define CONCAT(x,y) CONCAT_INTERNAL(x,y)

#define defer const auto &CONCAT(scope_exit_, __LINE__) = ScopeExitHelper() + [&]()

#endif //TUTORIAL_DEFER_HPP
