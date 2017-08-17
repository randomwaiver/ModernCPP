#include <thread>
#include <future>
#include <iostream>
#include <deque>

struct Context
{
    // setup some interthread communications
    std::mutex mutex;
    std::condition_variable cv;
    std::deque<std::promise<int>> work_queue;

    void worker()
    {
        std::unique_lock<std::mutex> lock(mutex);
        auto count = 0;
        while (true)
        {
            while (!work_queue.empty())
            {
                work_queue.back().set_value(++count);
                work_queue.pop_back();
            }
            cv.wait(lock);
        }
    }
};


int main()
{
    Context context;
    auto workerID = std::thread(&Context::worker, &context);
    workerID.detach();
    std::deque<std::future<int>> futures;
    // put some work on the queue
    for (int i=0; i!= 100; ++i)
    {
        // only keeping the lock long enough to be afe
        // this gives the worker a chance
        std::unique_lock<std::mutex> lock(context.mutex);

        // create a promise to send to worker
        auto promise = std::promise<int>();

        // get the future to be satisfied by the promise
        auto future = promise.get_future();

        context.work_queue.push_back(std::move(promise));
        futures.push_back(std::move(future));

        // tell the worker there is some work
        context.cv.notify_one();
    }
    for (auto&& f:futures)
    {
        std::cerr << "future " << f.get() << '\n';
    }
    return 0;
}
