#pragma once

#include <coroutine>
#include <exception>
#include <utility>

constexpr int NFIB = 1000;
constexpr int NBMK = 100;
constexpr int FM = 10;

int sumfib_coro(int n);
int sumfib(int n);

int startup_coro(int n);
int startup(int n);

template <typename T> struct generator {
  struct promise_type {
    T current_value;
    using coro_handle = std::coroutine_handle<promise_type>;
    auto get_return_object() { return coro_handle::from_promise(*this); }
    auto initial_suspend() { return std::suspend_always(); }
    auto final_suspend() noexcept { return std::suspend_always(); }
    void return_void() {}
    void unhandled_exception() { std::terminate(); }
    auto yield_value(T value) {
      current_value = value;
      return std::suspend_always{};
    }
  };

  using coro_handle = std::coroutine_handle<promise_type>;
  bool move_next() {
    return handle_ ? (handle_.resume(), !handle_.done()) : false;
  }
  T current_value() const { return handle_.promise().current_value; }
  generator(coro_handle h) : handle_(h) {}
  generator(generator const &) = delete;
  generator(generator &&rhs) : handle_(rhs.handle_) { rhs.handle_ = nullptr; }
  ~generator() {
    if (handle_)
      handle_.destroy();
  }

private:
  coro_handle handle_;
};


