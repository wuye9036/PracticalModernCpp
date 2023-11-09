
# 一  “Our eyes are yet to open, fear the C plus plus.”

在远古时代，C++11正式发布了。十年过去，它增添了许多新特性，表达能力更强，当然写出来的代码也可能会更加令人抓狂 ———— 正如那些有着无穷知识，却又令人发狂的上位者们。

本系列中是对其中一名上位者 —— C++ Execution —— 进行探索的癫狂之旅。

探究上位者秘辛之前，需要一个例子作为祭品：

> 设计一个功能单元，输入一个数字`arg`，返回`arg + 42`，比如：
>  ``` C++
>  #include <cstdlib>
>  #include <iostream>
>
>  int main(int argc, char* argv[]) {
>    int in_value = atoi(argv[0]);
>    /*******************
>      ... 你的代码 ...
>    ********************/
>    std::cout << "Result: " << out_value << std::endl;  
>  }
>  ```

对于这个问题，最基本的办法自然是直接了当地写：

``` C++
// ...
    int out_value = in_value + 42;
// ...
```

如果考虑把实现细节封装起来并提供可复用性，可以写成：

``` C++
int add42(int a) {
    return a + 42;
}

// ...
    int out_value = add42(in_value);
// ...
```

再泛化一下`arg`的类型，使得它可以用于更广泛的场合：

``` C++
template <typename T>
auto add42(T arg) {
    return arg + 42;
}
```

或者再宽泛一点，42也不是什么奇妙的、独一无二的数字：

``` C++
template <auto I, typename T>
auto addK(T arg) {
    return arg + I;
}

// ...
    int out_value = addK<42>(in_value);
// ...
```

如果只针对题目本身，可以说以上几种解答都是可以满足要求的。只不过不同的解法都有一些额外的特性，比如有些*封装了实现细节*，有一些则提供了*泛型*性。当这些特性连同题目的要求一起被需要的时候，那么这些解就不再是面目可憎，而的确是不同组合条件下的较优解。

接下来，就要看看上面这个淳朴的祭品在上位者 C++ Execution 眼中扭曲的样子 [https://godbolt.org/z/rv41cqPeq]：

``` C++
// ...
    sender auto s = just(in_value) | then([](int i) {return i + 42;});
    sync_wait(s).value();
// ...
```

Hmm，欢迎你，外乡人。

> 我们的眼界尚不足以意识到C++的可怖之处。 —— Master Willem

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

# 三 "We are born of the async, made men by the async, undone by the async."

**软件工程基本定理**（Fundamental theorem of software engineering, FTSE）说，

> “通过添加间接层，我们可以解决任何问题。”

或者我们也可以说：增加间接层的目的，一般是为了解决某个问题。

在此处，和 _立即求解_ 的版本相比，_延期求解_ 版本的 Execution 额外提供了 “声明（表达）” 这一间接层，使我们得以将问题的 *表达* 和 *求解（执行）* 分离开来。

这样，拥有**完整**问题信息的、或者说全知、先知的框架，就可以根据情况，选择和调整具体的 *执行* 过程，比如：
* 为任务可以选择一个特定的执行时间；
* 将任务部署到某个线程或者某个核心上；
* 增加、去除、重排、修改某些执行步骤；
* 将任务部署到不同设备，比如GPU上。

这些 *执行* 层面上的变化，都可以另行定制，基本不需要对 `表达` 进行修改。这样便同时达到了既灵活，又可控的要求。

作为一个上位者，`std::execution` 的别名是 “ *异步(asynchronization)* 与 *异构(heterogeneous)* 的 *处刑人(executor)* ”。可以看出， _延期求解_ 的设计恰好迎合了上位者的挑剔的口味。

实现 *延期求解* 有多种办法，比如使用设计模式中的*intepreter*或者*builder*。不过，因为C++本身就提供了相对完善地元编程能力，比如模板、泛型和操作符重载。如果能充分运用这些特性，编译器就可以获得充足的信息以充分优化代码。因此在C++中完成*延期求解*这一行为，更加流行的方法是“血疗”：**表达式模板（expression template）**。

这里我们用一个简单的祭品：**表达式求值** 来帮助大家回顾一下“血疗”(本血疗手册参考自：[https://en.wikipedia.org/wiki/Expression_templates])。

表达式求值几乎是所有语言的入门课。在C++中对表达式求值可以看作是立即执行的。我们需要进行一些设计，把立即执行的表达式求值变成*延期求解*的。

*表达式*是一个递归的树状结构。也就是说，*表达式*的一部分也是一个*表达式*。`a + 2 + b` 是一个加法表达式，它将两个子表达式 `a + 2` 和 `b` 用加号连接了起来。我们可以设计一个空的结构`struct Expr`，作为表达式这个概念的根节点。

可以通过判断一个类或者对象是否继承自Expr来认定它是不是一个表达式。

表达式树的叶节点，一般是一个字面值或者一个变量。为了简单，我们认为叶节点只有变量一种形式、运算符也只有加号。于是我们设计出以下结构：

``` C++
struct Expr {};

template <typename T>
struct Value: Expr {
  T const& get() const { return v; }
  T const& v;
  explicit Value(T const& a): v(a) {}
};
```

那么，两个`Expr`求和之后的结果是什么呢？
答案是：*两个Expr之和*。啊，对你没看错，就是这么一句废话。

``` C++
template <typename E0, typename E1>
struct Add_: Expr {
  E0 a;
  E1 b;
};

template <typename E0, typename E1>
Add_<E0, E1> operator + (E0 e0, E1 e1) {
  return {{}, e0, e1}; // 初始化列表中第一个参数 {} 是用于构造父类 Expr 的。
}
```

这段代码距离真正能运行的代码还差了两步：
- *表达*是*表达*了，那真到了*求值*的时候应该怎么办呢？
- 如何限制我们的 `operator +` 仅适用于两个参数都是`Expr`子类的时候？

对于第一个问题，简单，给它增加一个函数 `get`，在这个函数内完成实际的*求值*动作就好了。
对于第二个问题，我们在这里使用`concepts`，告诉它这个操作符重载仅适用于特定的条件。


所以完整的`Add_`和`operator +`长这样：

``` C++
template <typename E0, typename E1>
struct Add_: Expr {
  E0 a;
  E1 b;
  auto get() const {
    return a.get() + b.get();
  }
};

auto operator + (std::derived_from<Expr> auto e1, std::derived_from<Expr> auto e2) {
  return Add_<decltype(e1), decltype(e2)>{{}, e1, e2};
}

// 或者写成下面这样：

template<
  std::derived_from<Expr> E1,   // 使用Concept要求E1必须要继承自Expr。
  std::derived_from<Expr> E2>
auto operator + (E1 e1, E2 e2) {
  return Add_<E1, E2>{{}, e1, e2};
}
```

`Add_::get()`的实现中调用了小弟们的`get()`，这说明`Add_`求值的时候不能就他一个人惨，还要带着它的小弟们一起惨。总之，在增加了必要的代码后，下面这段代码就可以执行了：

``` C++
  int a{1}, b{2}, c{3};
  Value<int> va{a}, vb{b}, vc{c};
  auto r = va + vb + vc;      // The type of r is: Add_<Add_<Value, Value>, Value>
  fmt::print("{}", r.get()); // Output: 6
```

完整的代码在 [https://godbolt.org/z/Ev7n7WeYr]。

你看，这是不是就和我们的祭品长得差不多了？

当然在这个演示语法的例子因为太简单，并不太能看得出*表达式模板*的作用；问题开头给出的“血疗”参考的链接中更能说明*表达式模板*的作用。
*表达式模板*最早、也是最广泛的应用是在线性代数库中。这是因为它的*延迟求解*的特性，可以调整实际的计算路径，完成诸如循环融合、矩阵乘法的重组等一系列对性能大有助益的优化动作。迄今为止它也是*Eigen*等C++数值库所采用的主要优化方法之一。

除此之外，C++中还常使用*表达式模板*用来构造方言（dialect）。这些方言通常是类似于LINQ或者SQL那种声明式语言，其具体的执行步骤往往和方言的语法语义不完全一致。此类方言可用于构造解释器等概念简单、实现复杂的功能。这一类“方言”库比较典型的有Boost.Spirit和Boost.Proto.

但是和这里演示的*表达式模板*相比，Execution对*表达式模板*的使用又有所不同。因为：一切为了异步。

> 我等，因异步而成人，因异步而超人，因异步而非人。无知者啊，敬畏异步吧！

# 四 “the push and the pull are one.”

观察上一节中的*表达式模板*，可以发现它有两个特征：

1. 在表达式求值这个例子中，求值函数 `get()` 的调用顺序是先调用最靠近结果的`Expr`，然后依次递归，最后调用叶节点上Value的`get()`。这一从结果到源头的调用顺序，我们可称之为 "Pull Mode"，也就是说，只要表达式不去拉取子表达式的结果，子表达式就啥都不做。
2. 整个求值过程在一个当前线程中同步执行完成。它的效果和直接的递归并没有什么两样。

我们要模仿的上位者Execution，在这两点上都和我们的表达式模板略有不同。上位者的执行可以是异步的，不同表达式之间的执行可以并行；其次Execution的求值驱动顺序是*push mode*，也就是先计算前面一级，然后把结果向数据管道的后一级 `push`，以驱动后面的管道执行自己的操作。

根本上讲*push/pull*并不存在对立性，它们的实际执行顺序皆由数据间的依赖关系所决定。比如在表达式这个例子中，表面上看起来，`Add_::get()` 是最先*执行*的，`Value::get()`是最后执行的，但实际上，因为求和必须要在两个子树都计算出结果之后才能执行，因此从运算顺序上，`Value::get()` 反而是最先*完成*的。而优化后的执行代码，也往往会变成从叶节点往根节点执行。表达式例子中实际翻译出来的代码会变成下面这样：

``` nasm
mov DWORD PTR [rsp+4], 1
mov DWORD PTR [rsp+8], 2
mov DWORD PTR [rsp+12], 3
mov eax, DWORD PTR [rsp+4]    ;; o  = a
mov ecx, DWORD PTR [rsp+8]
mov edx, DWORD PTR [rsp+12]
add eax, ecx                  ;; o += b
lea rcx, [rsp+16]
add eax, edx                  ;; o += c
mov edx, 1
mov DWORD PTR [rsp+16], eax
```

但从表达上来说，如果任务是从一个起点到一个或多个可能的终点这样的树状结构会更适合*push*的结构；相应的，如果任务由不同的起点汇聚到一个终点（比如表达式求值），那*pull*会更加适合。

![](TaskGraph.png)

特别是当树或者图中只有部分路径会被执行的时候，如果执行顺序不太合适就可能会导致无谓的计算，此时需要做额外的结构避免此类的性能劣化。

我们也可以为表达式模板增加异步计算的功能([示例](https://godbolt.org/z/5qf7WT6c5))。

假设我们的设备是一个特别缓慢的设备，取一个数字需要`2ms`, 执行一次加法需要`3ms`，执行一次乘法需要`7ms`：

``` C++
auto slow_fetch(auto const &v) {
  std::this_thread::sleep_for(2ms);
  return v;
}

struct _slow_add {
  auto operator() (auto a, auto b) const {
    std::this_thread::sleep_for(3ms);
    return a + b;
  }
};
inline constexpr auto slow_add = _slow_add{}; 

// 这里 slow_add 使用函数对象，因为我们需要将这个泛型函数作为参数，传递给别的函数以用于回调：
//    async_eval(slow_add, a, b);
// 此时如果写成
//    auto slow_add(auto a, auto b) { ... }
// 会因为 slow_add 本身不是一个变量，会导致编译错误。

struct _slow_mul {
  auto operator() (auto a, auto b) const {
    std::this_thread::sleep_for(7ms);
    return a * b;
  }
};
inline constexpr auto slow_mul = _slow_mul{}; 
```

那么我们可以把原先是同步的程序，使用`std::future`和`std::async`改造成异步程序：

``` C++
template <typename T> struct Value : Expr {
  future<T> eval() {
    return async(
      [this]() { return slow_fetch(v); }
    );
  }
  T const &v;
  explicit Value(T const &a) : v(a) {}
};

template <typename OpT1, typename OpT2> struct Add_ : Expr {
  OpT1 a;
  OpT2 b;
  auto eval() {
    return async(
      [this]() mutable {
        auto a_future = a.eval();
        auto b_future = b.eval();
        return slow_add(a_future.get(), b_future.get()); 
    });
  }
};

template <typename OpT1, typename OpT2> struct Mul_ : Expr {
  OpT1 a;
  OpT2 b;
  auto eval() {
    return async(
      [this]() mutable {
        auto a_future = a.eval();
        auto b_future = b.eval();
        return slow_mul(a_future.get(), b_future.get()); 
    });
  }
};

auto operator+(std::derived_from<Expr> auto e1, std::derived_from<Expr> auto e2) {
  return Add_<decltype(e1), decltype(e2)>{{}, e1, e2};
}

auto operator*(std::derived_from<Expr> auto e1, std::derived_from<Expr> auto e2) {
  return Mul_<decltype(e1), decltype(e2)>{{}, e1, e2};
}

int main(int argc, char* argv[]) {
                                   // current time: 0ms
  int a{1}, b{2}, c{3}, d{4};
  Value<int> va{a}, vb{b}, vc{c}, vd{4};
  auto r = (va + vb) * (vc + vd);
  auto r_future = r.eval();               
  fmt::print("{}", r_future.get()); // start at ~0ms, end at ~12ms = 2ms(4T) + 3ms(2T) + 7ms(1T)
  return 0;
}
```

当然，经过观察可以发现，`Add_::eval` 和 `Mul_::eval` 具有相似的结构和逻辑：
1. 调用子表达式的 `eval`，触发子表达式的执行，并获得一个 `std::future` 用于等待值的完成；
2. 启动一个异步执行的函数，这个函数在执行的时候会等待子表达式计算完成、并完成自身的求值。同时返回一个 `std::future` 可以让别人等他的结果。

唯一的差别点就在于求值本身是调用`slow_add`还是`slow_mul`。

因此我们可以把这两个函数提取出一个公共函数 `async_eval` 来 —— 甚至我们还可以把`Add_`和`Mul_`抽象成`BinaryOp_`([示例](https://godbolt.org/z/hdnr1Gbh1))：

``` C++
// 亿点点小技巧
template <typename ImmFn, typename... SubExprsT> 
auto async_eval(ImmFn&& fn, SubExprsT&&... subExprs) {
  auto future_tuple = make_tuple(subExprs.get()...);
  return async([&fn, future_tuple = std::move(future_tuple)]() mutable{
    auto invoke_with_future_eval = [&fn](auto&&... future_args) {
      return fn(future_args.get()...);
    };
    return apply(invoke_with_future_eval, std::move(future_tuple));
  });
}

// 合并之后的二元运算符
template <typename OpT1, typename OpT2, typename OpFunc>
struct BinaryOpExpr_ : Expr {
  OpT1 a;
  OpT2 b;
  OpFunc op;
  auto get() {
    return async_eval(op, a, b);
  }
};

auto operator+(std::derived_from<Expr> auto e1, std::derived_from<Expr> auto e2) {
  return BinaryOpExpr_<decltype(e1), decltype(e2), decltype(slow_add)>{
    {}, e1, e2, slow_add};
}

auto operator*(std::derived_from<Expr> auto e1, std::derived_from<Expr> auto e2) {
  return BinaryOpExpr_<decltype(e1), decltype(e2), decltype(slow_mul)>{
    {}, e1, e2, slow_mul};
}
```

这样我们就仿照上位者Execution，根据自己的知识构造了“眼” —— 虽然看起来还是挺畸形的。

当我们以为窥明神秘之时，上位者正在呢喃。

> THE PUSH AND PULL ARE ONE.

# Backlog

* Expression Templates
* Monadic
* Continuation-passing style

吾等，因血而成人，因血而超人，因血而非人。无知者啊，敬畏血吧！

## used techniques

Features:

* Expression structure building and evaluation are separated.
  * Similar concepts:
    * *expression templates*
    * *monadic*
    * *continuation-passing style*
  * Support lazy evaluation
* Structured (asynchronous) data flow
  * Embedded structure with clear boundary
  * Strong-typed data flow
  * RAII is well preserved
* Evaluation and scheduling are separated
* Rich and flexible customization (i.e. hookable or injectable) points
  * Related design patterns:
    * Decorator
    * Visitor

https://www.gcores.com/articles/95998

