// lib/lazy_adapters.h
#pragma once

#include <vector>
#include <iterator>
#include <algorithm>
#include <type_traits>
#include <utility>

namespace lazy {

// ===== filter =====

template<typename Pred>
struct filter_fn {
    Pred pred;
    filter_fn(Pred p): pred(p) {}
    template<typename Container>
    auto operator()(Container const& c) const {
        using std::begin; using std::end;
        using T = std::decay_t<decltype(*begin(c))>;
        std::vector<T> out;
        for (auto const& e : c)
            if (pred(e))
                out.push_back(e);
        return out;
    }
};
template<typename Pred>
inline auto filter(Pred p) { return filter_fn<Pred>(p); }


// ===== transform =====

template<typename Func>
struct transform_fn {
    Func func;
    transform_fn(Func f): func(f) {}
    template<typename Container>
    auto operator()(Container const& c) const {
        using std::begin; using std::end;
        using U = std::decay_t<decltype(func(*begin(c)))>;
        std::vector<U> out;
        out.reserve(std::distance(begin(c), end(c)));
        for (auto const& e : c)
            out.push_back(func(e));
        return out;
    }
};
template<typename Func>
inline auto transform(Func f) { return transform_fn<Func>(f); }


// ===== take =====

struct take_fn {
    size_t n;
    take_fn(size_t n_): n(n_) {}
    template<typename Container>
    auto operator()(Container const& c) const {
        using std::begin; using std::end;
        using T = std::decay_t<decltype(*begin(c))>;
        std::vector<T> out;
        size_t cnt = 0;
        for (auto const& e : c) {
            if (cnt++ >= n) break;
            out.push_back(e);
        }
        return out;
    }
};
inline auto take(size_t n) { return take_fn(n); }


// ===== drop =====

struct drop_fn {
    size_t n;
    drop_fn(size_t n_): n(n_) {}
    template<typename Container>
    auto operator()(Container const& c) const {
        using std::begin; using std::end;
        using T = std::decay_t<decltype(*begin(c))>;
        std::vector<T> out;
        size_t cnt = 0;
        for (auto const& e : c) {
            if (cnt++ < n) continue;
            out.push_back(e);
        }
        return out;
    }
};
inline auto drop(size_t n) { return drop_fn(n); }


// ===== reverse =====

struct reverse_fn {
    template<typename Container>
    auto operator()(Container const& c) const {
        using std::begin; using std::end;
        using T = std::decay_t<decltype(*begin(c))>;
        std::vector<T> out(begin(c), end(c));
        std::reverse(out.begin(), out.end());
        return out;
    }
};
inline auto reverse() { return reverse_fn(); }


// ===== keys =====

template<typename Map>
auto keys(Map const& m) {
    using std::begin; using std::end;
    using Key = std::decay_t<decltype(begin(m)->first)>;
    std::vector<Key> out;
    out.reserve(std::distance(begin(m), end(m)));
    for (auto const& p : m)
        out.push_back(p.first);
    return out;
}


// ===== values =====

template<typename Map>
auto values(Map const& m) {
    using std::begin; using std::end;
    using Val = std::decay_t<decltype(begin(m)->second)>;
    std::vector<Val> out;
    out.reserve(std::distance(begin(m), end(m)));
    for (auto const& p : m)
        out.push_back(p.second);
    return out;
}


// ===== pipeline operator =====

template<typename C, typename F>
auto operator|(C const& c, F f) {
    return f(c);
}
template<typename C, typename F>
auto operator|(C&& c, F f) {
    return f(c);
}

} // namespace lazy
