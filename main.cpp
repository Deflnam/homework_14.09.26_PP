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

int main() {
  return 0;
}