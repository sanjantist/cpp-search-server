#pragma once

#include <ostream>
#include <vector>

template <typename It>
class IteratorRange {
   public:
    IteratorRange(It begin, It end) : begin_(begin), end_(end) {};
    size_t size() { return distance(begin_, end_); }
    It begin() const { return begin_; }
    It end() const { return end_; }

   private:
    It begin_, end_;
};

template <typename It>
std::ostream& operator<<(std::ostream& out, const IteratorRange<It>& range) {
    for (auto i = range.begin(); i != range.end(); advance(i, 1)) {
        out << *i;
    }
    return out;
}

template <typename It>
class Paginator {
   public:
    Paginator(It begin, It end, size_t page_size);
    auto begin() const { return pages_.begin(); }
    auto end() const { return pages_.end(); }

   private:
    std::vector<IteratorRange<It>> pages_;
};

template <typename It>
inline Paginator<It>::Paginator(It begin, It end, size_t page_size) {
    size_t pages_until_break = 0;
    It page_begin = begin;
    for (auto i = begin; i != end; advance(i, 1)) {
        if (pages_until_break == page_size) {
            pages_.push_back(IteratorRange<It>(page_begin, i));
            page_begin = i;
            pages_until_break = 0;
        }
        ++pages_until_break;
    }
    if (page_begin != end) {
        pages_.push_back(IteratorRange<It>(page_begin, end));
    }
}

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}