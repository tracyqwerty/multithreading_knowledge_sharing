// 产生死锁的四个必要条件（面试考点）：

// 1.互斥（资源同一时刻只能被一个进程使用）
// 2.请求并保持（进程在请资源时，不释放自己已经占有的资源）
// 3.不剥夺（进程已经获得的资源，在进程使用完前，不能强制剥夺）
// 4.循环等待（进程间形成环状的资源循环等待关系）

#include <iostream>
#include <mutex>
#include <thread>

std::mutex mutex1;
std::mutex mutex2;

void thread1() {
  std::unique_lock<std::mutex> lock1(mutex1, std::defer_lock);
  std::unique_lock<std::mutex> lock2(mutex2, std::defer_lock);

  std::cout << "Thread 1 acquiring mutex1..." << std::endl;
  lock1.lock();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::cout << "Thread 1 acquiring mutex2..." << std::endl;
  lock2.lock();

  // std::cout << "Thread 1 acquiring mutex1 and mutex2..." << std::endl;
  // std::lock(lock1, lock2);

  std::cout << "Thread 1 acquired both mutexes." << std::endl;
}

void thread2() {
  std::unique_lock<std::mutex> lock1(mutex2, std::defer_lock);
  std::unique_lock<std::mutex> lock2(mutex1, std::defer_lock);

  std::cout << "Thread 2 acquiring mutex2..." << std::endl;
  lock1.lock();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::cout << "Thread 2 acquiring mutex1..." << std::endl;
  lock2.lock();

  // std::cout << "Thread 2 acquiring mutex2 and mutex1..." << std::endl;
  // std::lock(lock1, lock2);

  std::cout << "Thread 2 acquired both mutexes." << std::endl;
}

int main() {
  std::thread t1(thread1);
  std::thread t2(thread2);

  t1.join();
  t2.join();

  return 0;
}
// 在这个例子中，我们有两个线程（thread1和thread2）和两个互斥锁（mutex1和mutex2）。thread1首先尝试获取mutex1，然后是mutex2；而thread2首先尝试获取mutex2，然后是mutex1。

// 由于两个线程之间的竞争和资源获取顺序不同，可能出现以下情况：

// thread1获取了mutex1，然后在尝试获取mutex2之前暂停。
// thread2在thread1暂停的时候获取了mutex2，然后尝试获取mutex1。
// 此时，thread1和thread2都在等待对方释放互斥锁，从而产生死锁。
// 要解决死锁问题，可以采用以下方法之一：
// 1.按照相同的顺序请求资源。在上述例子中，如果两个线程都按照相同的顺序（例如，先获取mutex1，再获取mutex2）来请求互斥锁，就不会发生死锁。
// 2.使用std::lock()函数来同时锁定多个互斥锁。这个函数可以保证在锁定多个互斥锁时不会产生死锁。你可以使用`std::lockstd::lock()函数可以同时锁定多个std::unique_lock，
// 这样可以防止死锁的发生。当你调用std::lock()时，它会尝试锁定所有给定的std::unique_lock，如果无法立即锁定，那么它会阻塞直到可以锁定所有的std::unique_lock。在这个过程中，std::lock()会避免出现死锁。

// 死锁预防：
// 破坏死锁产生的四个条件（完全杜绝死锁）
// 死锁避免：
// 对分配资源做安全性检查，确保不会产生循环等待（银行家算法）
// 死锁检测：
// 允许死锁的发生，但提供检测方法
// 死锁解除：
// 已经产生了死锁，强制剥夺资源或者撤销进程