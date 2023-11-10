# 二 "But where's an outsider like yourself to begin?"

在理解 C++ Execution 是如何完成 `arg + 42` 这个请求之前，我们需要先了解用什么样的姿势对它祈祷才能获得它的认可。

``` C++
// ...
    sender auto s = just(in_value) | then([](int i) {return i + 42;});
    int out_value = sync_wait(s).value();
// ...
```

例子一共两句代码。第一句代码构造了一个数据流动的“管道”，`just` 会从外部获得一个数据（`in_value`），然后发送给 `then`；`then` 拿到这个数据后，调用参数上的匿名函数，获得一个增加42之后的结果，然后再试图传递给下一级。数据传递的关系使用了管道符('|')表示，就和linux命令行类似。

在第一句结束之后，这个数据的管道就构建完毕了。这个时候，这个数据管道并没有正式启动。返回值 `s` 是一个符合 `sender` 概念的类型的对象，它只表示了这个数据管道，而不是直接执行并返回 `arg + 42` 的结果。关于 `sender` 概念，我们会在之后的章节中进行解析。

只有在第二句使用 `sync_wait` 调用 `s` 的时候，才会真正的驱动 `s` 的执行，并返回一个 `std::optional<int>` 的对象。这个对象中包含有计算之后的结果。

这种 _表达（声明）_ 与 _执行_ 两段式的做法，在C++中并不罕见，比如 _Expression templates_。其它语言中也可能会因为其它的原因（比如Immutation的传递性）而设计出类似的机制，比如Haskell中的`Monad` 。

因为一个数据管道被 _声明_ 后未必会被立刻执行 —— 尽管它的代码看起来就和执行了一样，所以这也被称作 _延迟求解 (lazy evaluation)_。这里并不是说它非延迟不可。通过特定的实现，它完全可以做到 _立即求解_ (eager evaluation)。比如：

``` C++
int out_value = then(just(in_value), [](int i){ return i + 42; });
```

只需要

``` C++
auto just(auto v) {
  return v;
}
auto then(auto v, auto f) {
  return f(v);
}
```

就可以做到这一点[https://godbolt.org/z/s5aEKxhfM]。当然，你应该已经发现了，这里 `out_value` 计算的表达式和例子中略有不同。这里我们没有使用管道符进行传递，而是使用了嵌套的函数调用。这两者是等价的，只要能正确的重载 `operator |`。比如我们这样：

``` C++
auto operator | (auto v, auto callable) {
  return callable(v);
}
```

最终可以得到我们的“人偶”：

``` C++
#include <iostream>

auto just(auto v) { return v; }

auto then(auto v, auto f) { return f(v); }

auto then(auto f) {
  return [&f](auto v) { return then(v, f); };
}

// 这里不加约束地、粗暴地重载了 operator | 会在和许多第三方库联合编译时产生错误，比如{fmt}。
// 这并不是正确的写法，只是为了最直接的解决问题。
auto operator | (auto v, auto c) { return c(v); }

int main()
{
  int in_value = 1;
  auto ret1 = then(just(in_value), [](int i) {return i + 42;});
  auto ret2 = just(in_value) | then([](int i) {return i + 42;});
  std::cout << ret1 << " " << ret2 << std::endl;  // Output: 43 43
  return 0;
}
```

为了实现完整的管道调用，除了增加了 `operator |`，我们还为`then`增加一种重载。
除了采用的是`eager evaluation`导致的返回值类型不同，其它简直可以说和我们可爱的祭品一模一样。

这个时候可能有人会说，那我这么写：

``` C++
auto foo = [in_value]() {
  return just(in_value) | then([](int i ){return i + 42;});
};
```

这不也是 _延期求解_ 吗？也是不调用 `foo` 就不求解。是的，它当然也可以算作是广义上的 _延迟求解_。但是和一般意义上的 _延迟求解_ 不同，被函数封装后的表达式已经不再是它自身。这样使得表达式的自然组合被这个刻意的函数所隔断。

这样的 “_延迟求解_” 并不能让你自然地从较小的 _延迟求解_ 的结构去拼接成一个更大的 _延迟求解_ 的结构。在所有需要延迟的地方，都得手动设计代码以规避 _立即求解_。

那么这里 _立即求解_ 有什么缺点呢？为什么我们要舍弃 _立即求解_ 的直观和简单，而使用绕了一道弯的 _延迟求解_ 呢？

> 但是，像你一样的外乡人要从哪里开始了解C++ Execution呢？简单，你只需要给自己来点 `auto` 和 `template` …
