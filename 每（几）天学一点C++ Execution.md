
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
# Backlog

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

