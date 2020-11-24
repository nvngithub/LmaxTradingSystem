template <typename T>
void BlockingQueue<T>::Push(T&& t)
{
    {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->queue.push_front(std::move(t));
    }

    this->conditionVariable.notify_one();
}

template <typename T>
T BlockingQueue<T>::Pop()
{        
    std::unique_lock<std::mutex> lock(this->mutex);
    this->conditionVariable.wait(lock, [=]  {  return !this->queue.empty(); });

    T o(std::move(this->queue.back()));

    this->queue.pop_back();

    return o;
}

template <typename T>
int BlockingQueue<T>::Count()
{
    return this->queue.size();
}