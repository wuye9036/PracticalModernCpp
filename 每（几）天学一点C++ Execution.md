
# 一  “Our eyes are yet to open, fear the template metaprogramming.”

“和我签订契约，成为马猴烧酒吧！” —— 说个鬼故事，马猴烧酒已经是2011年的作品了。同年，C++11正式发布了。自此C++开始往古神的道路上越走越远。

实现一个可复用的结构，以实现 `arg + 42` ，你能想到几种写法？

``` C++

int add42(int a) {
    return a + 42;
}


int add(int a, int b) {
    return a + b;
}

auto add42 = [](int a) { return a + 42;}

template <typename T>
T add(T a) {
    a + 42;
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

```

在 C++ Execution 的加持下，我们可以获得一种新的魔法：

``` C++
sender auto s = just(v) | then([](int i) {i += 42});

sync_wait(s).value();
```

表达了数据的流动以及在流动过程中的变换。



# Backlog

吾等，因血而成人，因血而超人，因血而非人。无知者啊，敬畏血吧！


数据流的表达
Event chain的构造
调度、状态持有和执行点

