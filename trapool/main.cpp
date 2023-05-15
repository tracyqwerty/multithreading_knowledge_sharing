#include "gptpool.h"
#include <bits/stdc++.h>
int main() {
  GPTPool pool(4);
  std::vector<std::future<int> > results;

  for (int i = 0; i < 8; ++i) {
    results.emplace_back(pool.enqueue([i] {
      //std::cout << "hello " << i << std::endl;
      //std::this_thread::sleep_for(std::chrono::seconds(1));
      //std::cout << "world " << i << std::endl;
      return i * i;
    }));
  }
  // The blocking happens when you call get() on the std::future object. When you call res.get(), it will block the calling thread until the task has finished executing and the result is available.
  for (auto&& result : results) std::cout << result.get() << ' ';
  std::cout << std::endl;
  return 0;
}
