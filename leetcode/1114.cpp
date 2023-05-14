#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

class Foo1 {
 private:
  std::mutex mtx;
  std::condition_variable cv;
  int count = 1;

 public:
  Foo1() {}
  void first(std::function<void()> printFirst) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return count == 1; });
    // printFirst() outputs "first". Do not change or remove this line.
    printFirst();
    count = 2;
    cv.notify_all();
  }

  void second(std::function<void()> printSecond) {
    std::unique_lock<std::mutex> lock(mtx);
    // printSecond() outputs "second". Do not change or remove this line.
    cv.wait(lock, [this]() { return count == 2; });
    printSecond();
    count = 3;
    cv.notify_all();
  }

  void third(std::function<void()> printThird) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this]() { return count == 3; });
    // printThird() outputs "third". Do not change or remove this line.
    printThird();
  }
};

class Foo2 {
 private:
  std::mutex mtx1, mtx2;

 public:
  Foo2() { mtx1.lock(), mtx2.lock(); }

  void first(std::function<void()> printFirst) {
    // printFirst() outputs "first". Do not change or remove this line.
    printFirst();
    mtx1.unlock();
  }

  void second(std::function<void()> printSecond) {
    std::lock_guard<std::mutex> guard(mtx1);
    // printSecond() outputs "second". Do not change or remove this line.
    printSecond();
    mtx2.unlock();
  }

  void third(std::function<void()> printThird) {
    std::lock_guard<std::mutex> guard(mtx2);
    // printThird() outputs "third". Do not change or remove this line.
    printThird();
  }
};

class Foo3 {
 private:
  std::mutex mtx1, mtx2;

 public:
  Foo3() { mtx1.lock(), mtx2.lock(); }

  void first(std::function<void()> printFirst) {
    // printFirst() outputs "first". Do not change or remove this line.
    printFirst();
    mtx1.unlock();
  }

  void second(std::function<void()> printSecond) {
    mtx1.lock();
    // printSecond() outputs "second". Do not change or remove this line.
    printSecond();
    mtx2.unlock();
  }

  void third(std::function<void()> printThird) {
    mtx2.lock();
    // printThird() outputs "third". Do not change or remove this line.
    printThird();
  }
};
