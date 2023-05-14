/*
Sure, a condition variable is a synchronization primitive that enables
threads to wait until a particular condition occurs. Standard usage always
involves a mutex, because the condition variable is meant to synchronize
access to some condition of the program state shared among different threads.
It's important to understand that condition variables are always used with
a `std::unique_lock<std::mutex>`. This is because they are designed to
have a predictable behavior when the lock is unlocked prior to the condition
variable being notified, then relocked after the notification is received.

Here's a bit more detail:

- **Waiting on a condition**: To wait on a condition variable, a thread will use
one of the wait functions (`wait`, `wait_for`, `wait_until`). This will
automatically release the mutex and suspend execution of the thread. The thread
will remain blocked until woken up by another thread that calls the `notify_one`
or `notify_all` function on the same condition variable.

- **Notifying a condition**: A thread can notify a condition variable using the
`notify_one` or `notify_all` functions. This will unblock at least one or all of
the threads currently waiting on the condition variable respectively. If no
threads are waiting on the condition variable, the call to `notify_one` or
`notify_all` has no effect.

- **Race conditions and spurious wakeups**: In multi-threaded code, race
conditions can occur if the condition variable is checked, and then the thread
is put to sleep in two distinct steps. This is solved by checking the condition
inside a loop that surrounds the call to wait, effectively making the check and
sleep an atomic operation. This also handles "spurious wakeups", which is a
situation where a thread might be awakened from its wait state even though no
thread signaled the condition variable. Here's a simple example:
*/
#include <condition_variable>
#include <mutex>
std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void print_id(int id) {
  std::unique_lock<std::mutex> lock(mtx);
  while (!ready)
    cv.wait(lock);  // Wait for the ready condition
                    // ... print id ...
}

void go() {
  std::unique_lock<std::mutex> lock(mtx);
  ready = true;     // Change the condition
  cv.notify_all();  // Notify all threads
}
/*
In this example, `print_id` will print the id only after the `ready` condition
is set by the `go` function. The `cv.wait(lock)` function atomically releases
the mutex and puts the thread to sleep. When `cv.notify_all()` is called, the
condition variable wakes up all the threads, which then reacquire the mutex and
check the condition. The mutex ensures that the check and the call to wait is
atomic, thereby preventing any race conditions.
*/