* `[2]` Pointers, smart pointers and ownership (C++11/14/17)
* Data structures
  * `[1]` Contiguous data structures, iterators, views(C++17/20) and concepts(C++20)
  * Quick view of non-contiguous data structures
	  * `deque`
	  * `set`, `unordered_set`
	  * `map`, `unordered_map`
	  * `stack`, `queue`, `priority_queue`
  * interval data structure (区间数据结构) 区间树
  * `[1]` `tuple`(C++11), `variant`, `any`, `optional` (C++17 or Boost with C++11)
* `[1]` Algorithms
  * The forgotten `<algorithm>`
  * `std::execution` and parallel STL (C++17)
* `[2]` Memory management (C++17)
* `[^]` Most commonly used utility libraries
  * `<random>`, `<chrono>`, `<locale>` (C++11), `filesystem` (C++17), `<format>` (C++20)
* `[^]` Engineering C++
	* Use case study: Writing unit test for standard libraries.
	* Project organization and dependencies maintenance with modern CMake
	* ABI compatibility
* C++ core language features
  * `[1]` Out-of-the-box features
    * `override`, `final`, `noexcept`, `namespace A::B {}`, etc.
    * Literals: `100_km`, `0b0100`, `100'000ul`
    * Attributes (`[[*]]`): Common attributes in GCC, Clang and MSVC
  * `[1]` Enumerations
  * `[1]` Value categories (gl/pr/x/**l/r**)
    * Universal references
    * Perfect forwarding
    * Parameter pack(variadic arguments)
    * Ref-qualifier
    * Deducing `this`
    * https://www.zhihu.com/question/533946012/answer/2509921643
  * `[1]` Understand constancy: `const`, `constexpr` and `consteval`
  * `[1]` Constructors, destructors, assignments and implicit type conversion
  * `[1]` Initialization Hell
  * `[1]` Compile-time and runtime diagnostics
    * `assert` and `static_assert`
    * `source_location` and `basic_stacktrace`
    * `type_info` and `type_index`
    * `__FUNCTION__`, `__PRETTY_FUNCTION__` and `__func__`
    * Exceptions and system errors
  * `[2]` Template and automatic type deduction (`decltype`, `auto`)
  * `[1]` Polymorphic, CRTP, type erasure and polymorphic object
	  * `folly.poly`
	  * `dyno`
	  * `Boost.TypeErasure`
  * `[2]` "Generalized" functions and call-back in advance
    * Traditional functor, lambda and `std::function`
    * Closure and high-order function: capture and `std::bind`
    * Overloading resolving and CPO
* Concurrency and asynchronization
  * `[2]` Concurrency utilities
      * `thread` and `jthread`
    * Synchronization primitives - I
      * `mutex`s and `lock`s
    * Synchronization primitives - II
      * (C++11) `condition_variable` and `condition_variable_any`
      * (C++20) `counting_semaphore` and `binary_semaphore`
      * (C++20) `latch` and `barrier`
    * `atomic` and memory model
  * `[1]` Asynchronization in C++11
    * `promise` and `future` (`shared_future`)
    * `packaged_task` and `async`
  * `[1]` Coroutine (Language, C++20)
* “dialects” in C++
  * `[1]` _"SQL"_ in C++: `ranges` (Lib, C++20)
  * `[2]` From CPU to GPU: Executors (TS, C++26, Lib)
