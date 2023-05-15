
#include <iostream>
#include <future>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;
void hello() {
    std::cout << "Hello from thread " << std::this_thread::get_id() << std::endl;
}
//this function will create a number of threads equal to the number of cores on the machine
//and then wait for all of them to finish.
void concurrent_task() {
    auto start_time = std::chrono::system_clock::now();
    unsigned concurrent_count = std::thread::hardware_concurrency();
    std::cout << "Number of concurrent threads supported: " << concurrent_count << std::endl;
    vector<future<void>> futures;
    for (unsigned i = 0; i < concurrent_count; ++i) {
        //create a new thread and move it into the vector
        futures.push_back(std::async(std::launch::async, hello));
    }
    //wait for all the threads to finish
    for (auto& f : futures) {
        f.get();
    }
    auto end_time = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << "Time taken: " << elapsed_seconds.count() << "s\n";
}

int main() {
    concurrent_task();
   return 0;
}

// In this code, multithreading is achieved using the `std::async` function with the `std::launch::async` policy. This function, provided by the C++ Standard Library, allows you to start a new thread and associate it with a task, which is specified as a function (or function-like object).

// In this specific case, the task is the function `hello`. For every iteration of the loop, `std::async(std::launch::async, hello)` creates a new thread and starts executing `hello` on that thread.

// The `std::launch::async` policy argument tells `std::async` to start the task asynchronously on a separate thread, as opposed to potentially running it on the same thread at a later time.

// The return value of `std::async` is a `std::future` that will eventually hold the result of the function call. In this case, since `hello` is expected to return `void`, the futures are of type `std::future<void>`. These futures are stored in a `std::vector`.

// The final loop in the code waits for all the threads to finish. This is done by calling `get` on each `std::future` in the vector. The `get` method blocks until the task associated with the `std::future` has completed.

// So, the multithreading here is achieved by starting each task on a new thread with `std::async`, and then waiting for all threads to complete. The tasks are run in parallel, taking advantage of multiple CPU cores if they are available.