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

