/*

- **std::promise**: A `std::promise` object can store a value of a
specific type to be retrieved by a `std::future` object, thereby
communicating a result between two threads. The `std::promise`
object is meant to be used only once.

- **std::future**: A `std::future` can be thought of as a handle
to a value that may not yet be computed, often because the
computation is running in another thread. It provides a mechanism
to access the result of a computation happening in a separate
thread. You can query the status of the value, wait for the
computation to finish, or fetch the value (waiting if necessary).

These are typically used in conjunction with `std::async`, which
runs a function asynchronously (potentially in a separate thread)
and returns a `std::future` that will eventually hold the result
of that function call.

These concepts are crucial to creating a thread pool because
you'll often want to get results back from your tasks. When you
submit a task to a thread pool, it can return a `std::future` to
you. Then you can use the `std::future` to get the result of the
task once it's done.

Here's a simple example of how promises and futures work:
*/
#include <future>
#include <iostream>

int compute() { return 7; }

int main() {
  // Create a promise
  std::promise<int> prom;

  // Get the future from the promise
  std::future<int> fut = prom.get_future();

  // Pass the promise to a new thread
  std::thread([&prom]() {
    int result = compute();  // Do some computation
    prom.set_value(result);  // Set the value in the promise
  }).detach();

  // Get the result (this will wait for the result to be set)
  int x = fut.get();
  std::cout << "result: " << x << "\n";

  return 0;
}
/*

This example shows how you can use a promise and a future to get the result of
a computation that's happening in another thread. The `fut.get()` call will
block until the result is ready.

Now that you understand these concepts, you're ready to start building a thread
pool!
*/

// Q:one more question: why using "detach" here?

/*
Good question! In C++, a `std::thread` can either be joined or detached.
When a thread is joined using `std::thread::join()`, the function blocks
until the thread finishes its execution. This means that the thread that
calls `join()` (often the main thread) cannot continue until the joined
thread has finished.

On the other hand, detaching a thread using `std::thread::detach()`
separates the thread from the `std::thread` object, allowing execution
to continue independently. Once detached, a thread cannot be re-joined.
Its resources will be automatically released when the thread finishes
execution. This is useful when you don't need to wait for the thread
to finish, or if you want to "fire and forget" a thread.

NOTE:
In the example I gave, `detach()` is used because we don't need to
wait for the thread to finish -- we're going to wait on the
`std::future` instead. If we used `join()` instead of `detach()`,
then we would be waiting **twice**: once for the thread to join, and
once for the future to be ready. This would be unnecessary in this case.

However, it's crucial to make sure that you don't accidentally access
a `std::thread` object after it has been detached, as this could lead
to undefined behavior. And if a `std::thread` object is destructed
without being either joined or detached, `std::terminate` is called,
which by default terminates the program.

So, while `detach()` can be useful, it should be used with caution.
Often, it's better to use `join()`, or to use higher-level abstractions
like `std::async` or a thread pool, which manage this for you.
*/