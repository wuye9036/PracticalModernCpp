
# 一  “Our eyes are yet to open, fear the C plus plus.”

“和我签订契约，成为马猴烧酒吧！” —— 说个鬼故事，小圆脸已经是2011年的作品了。同年，C++11正式发布了。

十年过去，C++新特性更多，表达能力更强，当然写出来的代码也可能会更加令人抓狂，如同血源中拥有无穷知识，却又令人发狂的上位者们。

在探究C++古神的奥秘之前，先从一个简单的例子来回顾一下现状：

> 设计一个功能单元，输入一个数字`arg`，返回`arg + 42`
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

当然，最基本的办法就是也不管什么单元了，就直接了当的写：

``` C++
// ...
    int out_value = in_value + 42;
// ...
```

如果稍微讲究一些可以写成

``` C++
int add42(int a) {
    return a + 42;
}

// ...
    int out_value = add42(in_value);
// ...
```



``` C++
int add(int a, int b) {
    return a + b;
}

auto add42 = [](int a) { return a + 42;}

template <typename T>
T add(T a) {
    return a + 42;
}

template <typename T, int v> T add(T a){
    return a + v;
}

template <int C>
struct Add {
    template <int A>
    class Apply {
        static const int Value = A + C;
    };
};

template <integral C>
struct Add {
    constexpr auto apply(integral auto a) {
        return a + C;
    }
}

```

在 C++ Execution 的加持下，我们可以获得一种新的魔法：

``` C++
sender auto s = just(v) | then([](int i) {i += 42});

sync_wait(s).value();
```

表达了数据的流动以及在流动过程中的变换。



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

