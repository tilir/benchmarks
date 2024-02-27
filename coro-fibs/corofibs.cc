//-----------------------------------------------------------------------------
//
// Sum of fibonacci numbers benchmark, part 1
//
// for part 2 see sumfib_module.cc
//
//-----------------------------------------------------------------------------

#include "benchmark/cppbenchmark.h"

#include <coroutine>
#include <exception>
#include <utility>

constexpr int NBMK = 1000;

namespace {

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

}

generator<int> fibs() {
  int a = 1, b = 0;
  for (;;) {
    co_yield b;
    b = std::exchange(a, (a + b) % 10) % 10;
  }
}

int sumfib_coro(int n) {
  int sum = 0;
  auto nums = fibs();

  for (int i = 0; i < n; ++i) {
    nums.move_next();
    auto y = nums.current_value();    
    sum = sum + y;
  }

  return sum;
}

int sumfib(int n) {
  int sum = 0;
  int a = 1, b = 0;

  for (int i = 0; i < n; ++i) {    
    sum = sum + b;
    b = std::exchange(a, (a + b) % 10) % 10;
  }

  return sum;
}

int startup_coro(int n);
int startup(int n);

BENCHMARK("sumfib_coro") {
  startup_coro(NBMK);
}

BENCHMARK("sumfib") {
  startup(NBMK);
}

BENCHMARK_MAIN()
