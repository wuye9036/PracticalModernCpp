---
marp: true
paginate: true
style: |
    section {
        background-color: #fffff2;
    }
---
<!-- theme: gaia -->

<style scoped>
  h1 {
    text-align: center;
    position: absolute;
    top: 40%;
  }
</style>

# Practical Modern C++

---

# Introduction 1/2

* It is a series; Prefer talking about features that
  * _The most frequently used_ and
  * _Easiest to use_ OR _powerful_
  * “性价比”
* Our departure and goal
  * For ~~(精通C++)~~ intermediate experienced C++ users
  * Not to be a flawless “language lawyer”
  * For more proficient reading, writing and debugging modern C++
  
---

# Introduction 2/2

* Refers features from C++11 to C++2x (TS & proposals)
  * C++ 11/14/17 are our major narrative threads
  * C++ 17/20/2x are references to help us
    * Understand the motivation & limitation
    * Know the future
* Hope it could be the pleasant journey

---
# Agenda 1/5

<style scoped>
  ul {
    font-size: 0.9em
  }
</style>

* Pointers, smart pointers and ownership (C++11/14/17)
* Data structures
  * Contiguous data structures, iterators, views(C++17/20) and concepts(C++20)
  * `tuple`(C++11), `variant`, `any`, `optional` (C++17 or Boost with C++11)
* Algorithms
  * `<algorithm>`
  * `std::execution` and parallel STL (C++17)
* Memory management (C++17)
  
---

# Agenda 2/5

<style scoped>
  ul {
    font-size: 0.9em
  }
</style>

* Most commonly used utility libraries
  * `<random>`, `<chrono>`, `<locale>` (C++11) and `<format>` (C++20)
* C++ core language features
  * Out-of-the-box features
    * `override`, `final`, `noexcept`, `namespace A::B {}`, etc.
    * Literals: `100_km`, `0b0100`, `100'000ul`
    * Attributes (`[[*]]`): Common attributes in GCC, Clang and MSVC
  * Enumerations
  
---

# Agenda 3/5

<style scoped>
  ul {
    font-size: 0.9em
  }
</style>

* C++ core language features
  * Value categories (gl/pr/x/**l/r**), universal references, perfect forwarding and parameter pack(variadic arguments)
  * Understand constancy: `const`, `constexpr` and `consteval`
  * Constructors, destructors, assignments and implicit type conversion
  * Initialization
  * Compile-time and runtime diagnostics
  * Template and automatic type deduction (`decltype`, `auto`)
  * Functor, lambda, `std::function` and `std::bind`

---

# Agenda 4/5

<style scoped>
  ul {
    font-size: 0.9em
  }
  pre {
    font-size: 0.9em
  }
</style>
* Concurrency, asynchronization
  * `thread`, `mutex/lock` and `condition_variable`
  * `atomic` and memory model
  * Asynchronization in C++11: `future`, `promise` and `packaged_task`
  * Coroutine (Language, C++20)
``` C++
auto const values = {0,1,2,3,4,5};
auto even = [](int i) { return 0 == i % 2; };
auto square = [](int i) { return i * i; };

for (int v : values | std::views::filter(even) | std::views::transform(square)) {
  std::cout << v << ' ';
} // Output: 0 4 16
```

---

# Agenda 5/5

<style scoped>
  ul {
    font-size: 0.9em
  }
  pre {
    font-size: 0.9em
  }
</style>

* “dialects” in C++
  * _"SQL"_ in C++: `ranges` (Lib, C++20)
  * From CPU to GPU: Executors (TS, C++26, Lib)
* Engineering
  * Project organization and dependencies maintenance with modern CMake
  * ABI compatibility
* **Won't talk**
  * Advanced template meta-programming
  * Design a "dialect/DSL" in C++ (for e.g. write something like `boost.proto`)

---

# Resources

<style scoped>
  ul {
    font-size: 0.7em
  }
</style>

* 语言特性
  * https://en.cppreference.com/w/cpp （不推荐 https://www.cplusplus.com）
  * 标准草案：**N3337(C++11)**/N4140(C++14)/N4659(C++17)/**N4868(C++20)**
  * 其它提案：https://github.com/cplusplus/draft
* 标准库
  * https://en.cppreference.com/w/cpp （不推荐 https://www.cplusplus.com）
  * https://github.com/llvm/llvm-project/tree/main/libcxx
* Cpp教程与行业用例
  * https://www.youtube.com/user/CppCon / https://github.com/CppCon
  * 如果404，B站上有搬运工
* 最佳实践
  * C++ Core Guidelines
  * Effective Modern C++
* 其它
  * https://en.cppreference.com/w/cpp/links

---
<style scoped>
h1 {
    text-align: center;
    position: relative;
    top: 40%;
    font-size: 2em;
    color: darkorange;
}
</style>

# Enjoy!