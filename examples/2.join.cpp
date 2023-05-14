#include <iostream>
#include <thread>
using namespace std;
void proc(int &a) {
  cout << "我是子线程,传入参数为" << a << endl;
  cout << "子线程中显示子线程id为" << this_thread::get_id() << endl;
}
int main() {
  cout << "我是主线程" << endl;
  int a = 9;
  // std::ref
  // 是引用传递，如果不加引用传递，那么子线程中的a就是一个局部变量，而不是主线程中的a。
  thread th2(
      proc,
      ref(a));  // 第一个参数为函数名，第二个参数为该函数的第一个参数，如果该函数接收多个参数就依次写在后面。此时线程开始执行。
  cout << "主线程中显示子线程id为" << th2.get_id() << endl;
  // 此处省略多行，不要在创建完线程后马上join,应该在程序结束前join。
  // 这通常是一个好的做法，因为如果主线程在th2完成之前退出，
  // th2可能会被突然终止，这可能导致问题。需要注意的是，join只能被调用一次，
  // 如果你尝试再次join一个已经join过的线程，会抛出一个异常。
  th2.join();  // 此时主线程被阻塞直至子线程执行结束。
  return 0;
}
/*
我是主线程
主线程中显示子线程id为0x70000b215000
我是子线程,传入参数为9
子线程中显示子线程id为0x70000b215000
*/

// 一提到join,你脑海中就想起两个字，"等待"，而不是"加入"，这样就很容易理解join的功能。