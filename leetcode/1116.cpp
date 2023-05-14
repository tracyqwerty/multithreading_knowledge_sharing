#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
using namespace std;
class ZeroEvenOdd {
 private:
  int n;
  int tmp = 1;
  std::mutex mtx1, mtx2, mtx3;
  // std::condition_variable cv;

 public:
  ZeroEvenOdd(int n) {
    this->n = n;
    mtx2.lock();
    mtx3.lock();
  }

  // printNumber(x) outputs "x", where x is an integer.
  void zero(function<void(int)> printNumber) {
    for (int i = 0; i < n; ++i) {
      mtx1.lock();

      printNumber(0);
      cout << 0 << endl;

      if (tmp % 2 == 0)
        mtx2.unlock();
      else
        mtx3.unlock();
    }
  }

  void even(function<void(int)> printNumber) {
    for (int i = 2; i <= n; i += 2) {
      mtx2.lock();
      printNumber(tmp);
      cout << tmp << endl;
      tmp++;

      mtx1.unlock();
    }
  }

  void odd(function<void(int)> printNumber) {
    for (int i = 1; i <= n; i += 2) {
      mtx3.lock();
      printNumber(tmp);
      cout << tmp << endl;
      tmp++;

      mtx1.unlock();
    }
  }
};