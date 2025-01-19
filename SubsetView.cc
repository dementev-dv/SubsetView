#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <vector>
#include <bitset>
#include <fstream>

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
  using difference_type = std::ptrdiff_t; // ??

  iterator(subset_view<R> &parent)
    : parent_(&parent.data_),
    index_(1),
    limit_(1U << parent.distance_) { }

  value_type operator*() const {
    value_type subset;
    std::size_t pos = 0;
    for (std::size_t mask = index_; mask > 0; mask >>= 1, ++pos) {
      if (mask & 1) {
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

#ifdef OLD_STD
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

#else

struct subset_adaptor : public ranges::range_adaptor_closure<subset_adaptor> {
  template <ranges::viewable_range R>
  auto operator()(R&& r) const {
    return subset_view(std::forward<R>(r));
  }
};
#endif

inline constexpr subset_adaptor subset;

void ConsoleUse();
bool Test(const std::string& input_path, const std::string& answer_path);

int main(int argc, char** argv) {
  if (argc == 1) {
    ConsoleUse();
    return 0;
  }
  if (argc == 3) {
    std::string input = std::string(argv[1]);
    std::string ans = std::string(argv[2]);
    if (Test(input, ans)) {
      std::cout << "Test passed for: " << input << "\n";
      return 0;
    }
    std::cerr << "Test failed for: " << input << "\n";
    return 1;
  }

  std::cerr << "Invalid arguments" << std::endl;
  return 2;
}

void ConsoleUse() {
  unsigned size;
  unsigned tmp;
  std::vector<unsigned> vec;

  std::cin >> size;
  for (std::size_t i = 0; i < size; ++i) {
    std::cin >> tmp;
    vec.push_back(tmp);
  }

#ifdef OLD_STD
  auto w = vec | views::all | subset();
#else
  auto w = vec | views::all | subset;
#endif

  for (const auto &v : w) {
    std::cout << "{";
    for (std::size_t i = 0; i < v.size(); ++i) {
      std::cout << v[i];
      if (i + 1 < v.size()) std::cout << ", ";
    }
    std::cout << "} ";
  }
  std::cout << std::endl;
}

bool Test(const std::string& input_path, const std::string& answer_path) {
  std::ifstream in(input_path);
  std::ifstream ans(answer_path);
  
  if (!in.is_open()) {
    std::cerr << "Error: Could not open input file: " << input_path << "\n";
    return false;
  }
  if (!ans.is_open()) {
    std::cerr << "Error: Could not open answer file: " << answer_path << "\n";
    return false;
  }

  std::vector<int> vec;
  int value;
  while (in >> value) {
    vec.push_back(value);
  }

#ifdef OLD_STD
  auto w = vec | views::all | subset();
#else
  auto w = vec | views::all | subset;
#endif

  unsigned tmp;
  for (const auto& v : w) {
    for (std::size_t i = 0; i < v.size(); ++i) {
      ans >> tmp;
      std::cout << tmp << " " << v[i] << std::endl;
      // if (v[i] != tmp)
        // return false;
    }
  }
  return true;
}
