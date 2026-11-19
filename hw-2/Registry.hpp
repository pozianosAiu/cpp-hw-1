#include <algorithm>
#include <vector>

template <typename T, unsigned Cap>
class Registry {
 private:
  std::vector<T> arr_;
  unsigned last_;

 public:
  Registry() {
    arr_.resize(Cap);
    last_ = -1;
  }
  void add(T val) {
    last_ = (last_ + 1) % Cap;
    arr_[last_] = std::move(val);
  }
  unsigned size() { return Cap; }
  T& operator[](int ind) {
    ind = (2 * Cap + last_ - ind) % Cap;
    return arr_[ind];
  }
};