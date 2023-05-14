#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>

class FooBar {
 private:
  int n;
  std::mutex mtx1, mtx2;

 public:
  FooBar(int n) {
    this->n = n;
    mtx2.lock();
  }

  void foo(std::function<void()> printFoo) {
    for (int i = 0; i < n; i++) {
      mtx1.lock();
      // printFoo() outputs "foo". Do not change or remove this line.
      printFoo();
      mtx2.unlock();
    }
  }

  void bar(std::function<void()> printBar) {
    for (int i = 0; i < n; i++) {
      mtx2.lock();
      // printBar() outputs "bar". Do not change or remove this line.
      printBar();
      mtx1.unlock();
    }
  }
};