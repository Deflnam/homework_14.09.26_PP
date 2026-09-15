#include <iostream>
#include <vector>
#include <chrono>

class Clicker {
public:
  Clicker() :
    start_(std::chrono::steady_clock::now())
  {}

  double millisec() const {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    const auto t = std::chrono::steady_clock::now();
    return static_cast<double>(duration_cast<milliseconds>(t - start_).count());
  }

private:
  std::chrono::steady_clock::time_point start_;
};

using data_t = std::vector<unsigned long long>;
using value_t = data_t::value_type;

value_t workerSum(const data_t& data, size_t start, size_t end) {
  value_t sum = 0;
  for (size_t i = start; i < end; ++i) {
    sum += data[i];
  }
  return sum;
}

int main() {
  return 0;
}