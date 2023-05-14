#include <iostream>
// C++ 11 提供管理线程的类和函数。
#include <thread>
void hello_world() { std::cout << "Hello, world!" << std::endl; }
int main() {
  // 列表初始化（也称为统一初始化或初始值设定项初始化）
  // 当函数的名字被当做一个值来使用的时候，实际上使用的是函数的指针。
  std::thread t{hello_world};
  // 第二个是使用了直接初始化。
  // std::thread t(hello_world);
  // 成员函数 t.join()，确保主线程在子线程退出之后才退出。
  t.join();

  return 0;
}