// 在C++多线程编程中，`detach`是`std::thread`类的一个成员函数，
// 用于将线程与当前线程对象分离。分离后，线程会独立地执行，而不受创建它的线程对象的影响。
// 当分离后的线程执行完毕时，其资源会自动被操作系统回收。

// 通常，我们在不关心线程何时结束或者不需要在主线程中等待子线程执行完毕的情况下使用`detach`。
// 当线程被`detach`后，我们不能再使用`join`来等待线程结束。
// 需要注意的是，如果主线程在分离的子线程完成之前退出，这些子线程可能会被突然终止，这可能导致问题。

#include <chrono>
#include <iostream>
#include <thread>

void worker() {
  std::cout << "Worker thread starts working..." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout << "Worker thread finished working." << std::endl;
}

int main() {
  std::thread t(worker);

  std::cout << "Main thread detaches the worker thread." << std::endl;
  t.detach();  // 将线程t与当前线程对象分离

  std::cout << "Main thread continues its work." << std::endl;
  // std::this_thread::sleep_for(std::chrono::seconds(2));
  std::cout << "Main thread finished working." << std::endl;

  return 0;
}
// 在这个例子中，我们创建了一个名为`worker`的线程，然后使用`detach`将其与主线程分离。
// 在此之后，主线程和工作线程将独立运行，互不影响。这个示例中，主线程在2秒后结束，而工作线程在5秒后结束。
// 请注意，线程的执行顺序是不确定的，所以你可能会看到不同的输出顺序。

/*
(main等2s，worker等5s，main先结束， worker强制结束)
在主线程结束后，整个程序就结束了，所有的子线程（包括已经分离的线程）都会被立即终止。
Main thread detaches the worker thread.
Main thread continues its work.
Worker thread starts working...
Main thread finished working.

(main等2s，worker等1s，worker先结束)
Main thread detaches the worker thread.
Main thread continues its work.
Worker thread starts working...
Worker thread finished working.
Main thread finished working.
*/

// 在C++中，如果一个线程对象被销毁，但是该线程仍在执行（即它已经被启动，但还没有完成），则会调用`std::terminate`来终止整个程序。这是因为，如果线程仍在运行，但其对应的线程对象被销毁，那么线程的状态就无法再被程序所知，这可能会导致不可预测的结果。

// 所以，我们在结束主线程之前，需要确保所有已启动的线程都已完成。这通常是通过调用`std::thread::join()`来完成的，它会阻塞主线程，直到对应的子线程结束。如果你不希望主线程等待子线程结束，那么可以调用`std::thread::detach()`，它会将子线程的执行与线程对象分离，使得子线程可以在后台自行结束，而主线程可以立即继续执行。

// 如果你没有调用`join()`或`detach()`，那么当线程对象被销毁时，就会调用`std::terminate`来终止整个程序。这就是为什么“直接删掉t1.join()是不行的”。

// 要注意的是，`detach()`并不是一个解决所有问题的万能药。使用`detach()`会使得你失去对子线程的控制，你无法知道它何时结束，也无法获取它的返回值。而且，如果分离的子线程访问了已经被主线程销毁的资源，那么可能会导致未定义的行为。所以，在使用`detach()`时，需要谨慎对待线程间的同步和共享资源的管理问题。