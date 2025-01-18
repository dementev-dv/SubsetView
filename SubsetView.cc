#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>
#include <bitset>

namespace ranges = std::ranges;
namespace views = ranges::views;

template <ranges::range R>
class subset_view : public ranges::view_interface<subset_view<R>> {
  R data_;
  size_t distance_;

 public:
  class iterator;
  struct sentinel;

 public:
  subset_view(R &data) : data_(data), distance_(ranges::distance(data_)) { }
  subset_view(R &&data) : data_(std::move(data)), distance_(ranges::distance(data_)) { }
  auto begin();
  auto end();
};

template <ranges::range R>
struct subset_view<R>::sentinel {
  std::size_t size_;
};

template <ranges::range R>
class subset_view<R>::iterator {
  R *parent_ = nullptr;
  std::size_t index_ = 0;
  std::size_t limit_ = 0;

 public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = std::vector<ranges::range_value_t<R>>;
  // using difference_type = std::ptrdiff_t; ??

  iterator(subset_view<R> &parent)
    : parent_(&parent.data_),
    index_(0),
    limit_(1U << parent.distance_) { }

  value_type operator*() const {
    value_type subset;
    std::size_t pos = 0;
    for (std::size_t mask = index_; mask > 0; mask >>= 1, ++pos) {
      if (mask & 1) {
        // std::cout << "index = " << index_ << std::endl;
        // std::cout << "mask = " << mask << std::endl;
        subset.push_back((*parent_)[pos]);
      }
    }
  return subset;
  }

  iterator &operator++() {
    ++index_;
    return *this;
  }

  iterator operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  bool operator==(const iterator &other) const = default;
  bool operator==(const subset_view<R>::sentinel &s) const { return index_ == s.size_; }
};

template <ranges::range R>
auto subset_view<R>::begin() {
  return subset_view<R>::iterator{*this};
}

template <ranges::range R>
auto subset_view<R>::end() {
  return subset_view<R>::sentinel{1U << distance_};
}

struct subset_closure {
  template <ranges::range R>
  auto operator()(R&& r) const {
    return subset_view(std::forward<R>(r));
  }
};

struct subset_adaptor {
  template <ranges::range R>
  auto operator()(R&& r) const {
    return subset_view(std::forward<R>(r));
  }

  constexpr auto operator()() const {
    return subset_closure{};
  }
};

template <ranges::range R>
auto operator|(R&& r, const subset_closure& c) {
  return c(std::forward<R>(r));
}

inline constexpr subset_adaptor subset;

int main() {
  auto vec = std::vector{1, 2, 3};

  // auto w = subset_view(vec);

  auto w = vec | views::all | subset();

  for (const auto &subset : w) {
    std::cout << "{";
    for (std::size_t i = 0; i < subset.size(); ++i) {
      std::cout << subset[i];
      if (i + 1 < subset.size()) std::cout << ", ";
    }
    std::cout << "} ";
  }
  std::cout << std::endl;
  return 0;
}
