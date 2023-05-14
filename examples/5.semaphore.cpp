/*
A semaphore is a synchronization construct that can be used to protect shared
resources in a concurrent scenario. It is simply a variable that is non-negative
and shared between threads. A semaphore has two atomic operations:
1. `wait()`: This operation decrements the value of the semaphore.
If the value becomes negative, the process executing the wait operation is
blocked until the value is greater or equal to zero.
2. `signal()`: This operation increments the value of the semaphore.
If the value is less or equal to zero, then a process that was blocked waiting
on the semaphore wakes up. Using these two operations, we can solve the
bounded-buffer problem.
We need two semaphores: one to count the empty buffer slots (call it `empty`)
and another to count the filled slots (call it `full`). We also need a mutex
(`mtx`) to protect the buffer from simultaneous access by the producer and
consumer. Here is the skeleton of how this can be done in C++:
*/
#include <condition_variable>
#include <mutex>
#include <queue>

std::mutex mtx;
std::condition_variable cv_empty, cv_full;
std::queue<int> buffer;  // shared resource
int buffer_size = 10;    // maximum buffer size

void producer() {
  while (true) {
    std::unique_lock<std::mutex> lock(mtx);
    cv_empty.wait(lock, [] {
      return buffer.size() != buffer_size;
    });  // block if buffer is full
    // add item to buffer
    buffer.push(1);
    lock.unlock();
    cv_full.notify_all();  // notify consumer that buffer is not empty
  }
}

void consumer() {
  while (true) {
    std::unique_lock<std::mutex> lock(mtx);
    cv_full.wait(lock,
                 [] { return !buffer.empty(); });  // block if buffer is empty
    // remove item from buffer
    buffer.pop();
    lock.unlock();
    cv_empty.notify_all();  // notify producer that buffer is not full
  }
}

/*
In this example, `std::mutex` and `std::condition_variable` are used instead of
semaphores. These are more common in C++ and can accomplish the same task as
semaphores in this scenario. `std::condition_variable::wait` is similar to
`wait()` operation on a semaphore, and `std::condition_variable::notify_all` is
similar to `signal()` operation on a semaphore.
*/

/*
Let's break it down:

1. `std::mutex`: This is a mutual exclusion object, used to protect shared data
from being simultaneously accessed by multiple threads. Mutex is an abbreviation
for "mutual exclusion".

In this scenario, `std::mutex mtx;` is creating a mutex object named `mtx`. This
mutex is then used to protect the shared resource (`buffer`).

2. `std::condition_variable`: This is a synchronization primitive that can be
used to block a thread, or multiple threads at the same time, until another
thread modifies the condition variable and notifies the blocked thread(s).

In this scenario, we have two condition variables: `cv_empty` and `cv_full`.
These are used to make the producer wait when the buffer is full, and make the
consumer wait when the buffer is empty.

3. `std::unique_lock`: This is a lock that provides exclusive ownership. It's a
wrapper for a mutex but provides more flexibility than just locking and
unlocking a mutex. It works in conjunction with condition variables to allow
waiting for conditions to be met.


In the code example provided, `std::unique_lock<std::mutex> lock(mtx);` is
creating a `unique_lock` object named `lock` that owns the mutex `mtx`. The
`unique_lock` object will automatically lock the mutex when it is created and
unlock it when it is destroyed.

In the context of the bounded-buffer problem, these constructs are used as
follows:

- Mutex `mtx` is used to protect access to the shared `buffer`. Only one thread
can own the mutex at a time, so this prevents the producer and consumer from
accessing the buffer at the same time.

- Condition variable `cv_empty` is used to make the producer wait when the
buffer is full. The producer thread will release the mutex and block until
another thread notifies `cv_empty`.

- Condition variable `cv_full` is used to make the consumer wait when the buffer
is empty. The consumer thread will release the mutex and block until another
thread notifies `cv_full`.

- `unique_lock` is used to manage the mutex in a way that integrates with the
condition variables. The `wait` function of a condition variable requires a
`unique_lock`.

- The `unique_lock` automatically releases the mutex when the
thread waits on the condition variable, and reacquires the mutex when the wait
is over. This is crucial for avoiding race conditions.
*/