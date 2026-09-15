#include <iostream>
#include <vector>
#include <chrono>
#include <future>
#include <exception>

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

value_t calculateParallel(const data_t& data, size_t threadCount) {
  const size_t baseSize = data.size() / threadCount;
  const size_t remainder = data.size() % threadCount;

  std::vector<std::future<value_t>> futures;
  futures.reserve(threadCount);

  size_t currStart = 0;
  for (size_t i = 0; i < threadCount; ++i) {
    const size_t currentSize = baseSize + (i < remainder ? 1 : 0);
    const size_t currEnd = currStart + currentSize;

    futures.push_back(
      std::async(std::launch::async, workerSum, std::cref(data), currStart, currEnd)
    );

    currStart = currEnd;
  }

  value_t totalSum = 0;
  for (size_t i = 0; i < threadCount; ++i) {
    totalSum += futures[i].get();
  }

  return totalSum;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Error: please enter thread count as argument\n";
    return 1;
  }

  size_t threadCount = 0;
  try {
    threadCount = std::stoull(argv[1]);
  } catch (const std::exception& e) {
    std::cerr << "Error of parsing argument: " << e.what() << "\n";
    return 1;
  }

  if (threadCount == 0) {
    std::cerr << "Error: thread count must be more than zero\n";
    return 1;
  }

  constexpr size_t size = 100000000;
  const data_t values(size, 1);

  Clicker cl;
  const value_t result = calculateParallel(values, threadCount);
  const double total = cl.millisec();

  std::cout << "Threads: " << threadCount
            << "\nSum: " << result
            << "\nTime: " << total << " ms\n";

  return 0;
}