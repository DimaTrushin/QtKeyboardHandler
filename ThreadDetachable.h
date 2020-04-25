#ifndef THREADDETACHABLE_H
#define THREADDETACHABLE_H

class CThreadDetachable {
  using CThread = std::thread;
public:
  template<class... TArgs>
  CThreadDetachable(TArgs... args) : Thread_(std::forward<TArgs>(args)...) {}
  ~CThreadDetachable() {
    if (Thread_.joinable())
      Thread_.detach();
  }
  CThreadDetachable(CThreadDetachable&&) noexcept = default;
  CThreadDetachable& operator=(CThreadDetachable&&) noexcept = default;

private:
  CThread Thread_;
};

#endif // THREADDETACHABLE_H
