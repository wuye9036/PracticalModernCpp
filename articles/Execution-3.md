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
