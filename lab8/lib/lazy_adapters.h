#ifndef LAZY_PIPE_ADAPTERS_H
#define LAZY_PIPE_ADAPTERS_H

#include <utility>
#include <iterator>
#include <type_traits>
#include <algorithm>

// Проверка, является ли тип парой
template<typename T>
struct IsAssocPair : std::false_type {};

template<typename K, typename V>
struct IsAssocPair<std::pair<K, V>> : std::true_type {};

template<typename T>
inline constexpr bool IsAssocPairV = IsAssocPair<T>::value;

// Пайплайновый оператор
template<typename Collection, typename Pipe>
auto operator|(Collection&& src, Pipe&& stage) {
    return std::forward<Pipe>(stage)(std::forward<Collection>(src));
}

// === KeysAdapter ===
template<typename Coll>
class MapKeyAdapter {
    Coll input_;

    using Item = typename std::decay_t<decltype(*std::begin(input_))>;
    static_assert(IsAssocPairV<Item>, "Keys require range of key-value pairs");

public:
    MapKeyAdapter(Coll coll) : input_(std::forward<Coll>(coll)) {}

    class Iterator {
        using It = decltype(std::begin(std::declval<Coll&>()));
        It ptr_;

    public:
        Iterator(It p) : ptr_(p) {}

        auto operator*() const { return ptr_->first; }
        Iterator& operator++() { ++ptr_; return *this; }
        bool operator!=(const Iterator& rhs) const { return ptr_ != rhs.ptr_; }
    };

    auto begin() const { return Iterator(std::begin(input_)); }
    auto end() const { return Iterator(std::end(input_)); }
};

struct GetKeys {
    auto operator()() const {
        return [](auto&& container) {
            return MapKeyAdapter<decltype(container)>(std::forward<decltype(container)>(container));
        };
    }
};

inline GetKeys get_keys;

// === ValuesAdapter ===
template<typename Coll>
class MapValueAdapter {
    Coll input_;

    using Item = typename std::decay_t<decltype(*std::begin(input_))>;
    static_assert(IsAssocPairV<Item>, "Values require range of key-value pairs");

public:
    MapValueAdapter(Coll coll) : input_(std::forward<Coll>(coll)) {}

    class Iterator {
        using It = decltype(std::begin(std::declval<Coll&>()));
        It ptr_;

    public:
        Iterator(It p) : ptr_(p) {}

        decltype(auto) operator*() const { return ptr_->second; }
        Iterator& operator++() { ++ptr_; return *this; }
        bool operator!=(const Iterator& rhs) const { return ptr_ != rhs.ptr_; }
    };

    auto begin() const { return Iterator(std::begin(input_)); }
    auto end() const { return Iterator(std::end(input_)); }
};

struct GetValues {
    auto operator()() const {
        return [](auto&& container) {
            return MapValueAdapter<decltype(container)>(std::forward<decltype(container)>(container));
        };
    }
};

inline GetValues get_values;

// === FilterAdapter ===
template<typename Coll, typename Pred>
class ConditionalAdapter {
    Coll input_;
    Pred condition_;

public:
    ConditionalAdapter(Coll c, Pred p)
        : input_(std::forward<Coll>(c)), condition_(std::move(p)) {}

    class Iterator {
        using It = decltype(std::begin(std::declval<Coll&>()));
        It curr_;
        It end_;
        Pred cond_;

        void Skip() {
            while (curr_ != end_ && !cond_(*curr_)) ++curr_;
        }

    public:
        Iterator(It start, It finish, Pred pred)
            : curr_(start), end_(finish), cond_(std::move(pred)) {
            Skip();
        }

        auto operator*() const { return *curr_; }
        Iterator& operator++() { ++curr_; Skip(); return *this; }
        bool operator!=(const Iterator& rhs) const { return curr_ != rhs.curr_; }
    };

    auto begin() const { return Iterator(std::begin(input_), std::end(input_), condition_); }
    auto end() const { return Iterator(std::end(input_), std::end(input_), condition_); }
};

struct PickIf {
    template<typename Predicate>
    auto operator()(Predicate&& cond) const {
        return [c = std::forward<Predicate>(cond)](auto&& coll) {
            return ConditionalAdapter<decltype(coll), std::decay_t<Predicate>>(std::forward<decltype(coll)>(coll), std::move(c));
        };
    }
};

inline PickIf pick_if;

// === TransformAdapter ===
template<typename Coll, typename Func>
class MapperAdapter {
    Coll input_;
    Func transformer_;

public:
    MapperAdapter(Coll coll, Func func)
        : input_(std::forward<Coll>(coll)), transformer_(std::move(func)) {}

    class Iterator {
        using It = decltype(std::begin(std::declval<Coll&>()));
        It iter_;
        Func func_;

    public:
        Iterator(It i, Func f) : iter_(i), func_(std::move(f)) {}

        auto operator*() const { return func_(*iter_); }
        Iterator& operator++() { ++iter_; return *this; }
        bool operator!=(const Iterator& rhs) const { return iter_ != rhs.iter_; }
    };

    auto begin() const { return Iterator(std::begin(input_), transformer_); }
    auto end() const { return Iterator(std::end(input_), transformer_); }
};

struct Apply {
    template<typename Func>
    auto operator()(Func&& f) const {
        return [fn = std::forward<Func>(f)](auto&& data) {
            return MapperAdapter<decltype(data), std::decay_t<Func>>(std::forward<decltype(data)>(data), std::move(fn));
        };
    }
};

inline Apply apply;

// === TakeAdapter ===
template<typename Coll>
class HeadAdapter {
    Coll input_;
    size_t limit_;

public:
    HeadAdapter(Coll c, size_t n) : input_(std::forward<Coll>(c)), limit_(n) {}

    class Iterator {
        using It = decltype(std::begin(std::declval<Coll&>()));
        It it_, end_;
        size_t remain_;

    public:
        Iterator(It i, It e, size_t r) : it_(i), end_(e), remain_(r) {}

        auto operator*() const { return *it_; }
        Iterator& operator++() { ++it_; if (remain_) --remain_; return *this; }
        bool operator!=(const Iterator& rhs) const { return it_ != rhs.it_ && remain_ != rhs.remain_; }
    };

    auto begin() const { return Iterator(std::begin(input_), std::end(input_), limit_); }
    auto end() const { return Iterator(std::end(input_), std::end(input_), 0); }
};

struct First {
    auto operator()(size_t count) const {
        return [count](auto&& coll) {
            return HeadAdapter<decltype(coll)>(std::forward<decltype(coll)>(coll), count);
        };
    }
};

inline First first;

// === DropAdapter ===
template<typename Coll>
class SkipAdapter {
    Coll input_;
    size_t count_;

public:
    SkipAdapter(Coll c, size_t n) : input_(std::forward<Coll>(c)), count_(n) {}

    class Iterator {
        using It = decltype(std::begin(std::declval<Coll&>()));
        It ptr_, end_;

    public:
        Iterator(It p, It e) : ptr_(p), end_(e) {}

        auto operator*() const { return *ptr_; }
        Iterator& operator++() { ++ptr_; return *this; }
        bool operator!=(const Iterator& rhs) const { return ptr_ != rhs.ptr_; }
    };

    auto begin() const {
        auto it = std::begin(input_), end = std::end(input_);
        size_t n = count_;
        while (n-- && it != end) ++it;
        return Iterator(it, end);
    }

    auto end() const { return Iterator(std::end(input_), std::end(input_)); }
};

struct Skip {
    auto operator()(size_t count) const {
        return [count](auto&& coll) {
            return SkipAdapter<decltype(coll)>(std::forward<decltype(coll)>(coll), count);
        };
    }
};

inline Skip skip;

// === ReverseAdapter ===
template<typename Coll>
class BackwardAdapter {
    Coll input_;

    using BaseIter = decltype(std::begin(std::declval<Coll&>()));
    using Tag = typename std::iterator_traits<BaseIter>::iterator_category;

    static_assert(std::is_base_of_v<std::bidirectional_iterator_tag, Tag>, "Reverse requires bidirectional iterators");

public:
    BackwardAdapter(Coll c) : input_(std::forward<Coll>(c)) {}

    auto begin() const { return std::rbegin(input_); }
    auto end() const { return std::rend(input_); }
};

struct Backwards {
    auto operator()() const {
        return [](auto&& coll) {
            return BackwardAdapter<decltype(coll)>(std::forward<decltype(coll)>(coll));
        };
    }
};

inline Backwards backwards;

#endif // LAZY_PIPE_ADAPTERS_H
