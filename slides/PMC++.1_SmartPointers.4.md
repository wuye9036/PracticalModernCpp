---
marp: true
paginate: true
style: |
    section {
        background-color: #fffff2;
        font-family: 'Palatino', 'Charter', 'STHeiti', 'Segoe UI Emoji';
    }
    section pre {
        font-size: 0.9em;
    }
---

<!-- theme: gaia -->
# Pointers, smart pointers and ownership, IV

<style scoped>
h1 {
    text-align: center;
    position: relative;
    top: 30%;
    font-size: 1.8em;
}
</style>

---
* 第一期
  * 为什么要有智能指针，为什么要用智能指针
  * 智能指针的基本使用
  * 一些帮助大家理解智能指针的题目
---

* 第二期
  * 指向数组的智能指针在不同C++标准中的可用性
  * 智能指针的三种创建方式及他们的不同
  * `shared_ptr`与垃圾回收机制
    * 理解基于引用计数的 `shared_ptr` 的特点与陷阱
  * `weak_ptr` 的作用、使用与陷阱

---
* 第三期
  * 指针的选择与系统设计
  * 性能问题
  * `shared_ptr` 的线程安全性

---

* 第四期
  * 如何从对象的 *raw pointer* 获取它的 `shared_ptr`
  * 理解自定义指针销毁器（`deleter`）
  * 其它非标准的智能指针

---

How to get `shared_ptr` by _raw pointer_ or _reference_?
Is following code correct?

``` C++
class Node {
public:
  std::shared_ptr<Node> getShared() { return std::shared_ptr<Node>(this); }
};

void foo() {
  // Consider following case
  Node n;
  auto sp1 = n.getShared();  // What happened?

  Node* pn = new Node();
  auto sp2 = pn->getShared(); // What happened?
}
```

---

How to get `shared_ptr` by _raw pointer_ or _reference_?
Is following code correct?

``` C++
class Node {
public:
  std::shared_ptr<Node> getShared() { return std::shared_ptr<Node>(this); }
};

void foo() {
  // Consider following case
  Node n;
  auto sp1 = n.getShared();   // !!! WRONG!!! Attempting to delete object twice.

  Node* pn = new Node();
  auto sp2 = pn->getShared(); // !!!DANGER!!! Invisible errors.
}
```

---
How to get `shared_ptr` by _raw pointer_ or _reference_?
Is following code correct?

``` C++
class Node {
private:
  std::shared_ptr<Node> self;
public:
  void setMyself(std::shared_ptr<Node> const& selfPtr) {
    if (myself.get() != this) {
      throw std::invalid_argument(" ... ");
    }
    self = selfPtr;
  }
  std::shared_ptr<Node> getShared() {
    return self;
  }
};
```

---

How to get `shared_ptr` by _raw pointer_ or _reference_?
Use `weak_ptr` intrusively.
``` C++
class Node {
private:
  std::weak_ptr<Node> self;
public:
  void setMyself(std::shared_ptr<Node> const& selfPtr) {
    if (myself.get() != this) {throw std::invalid_argument(" ... ");}
    self = selfPtr;
  }
  std::shared_ptr<Node> getShared() {
    return self.lock();
  }
};
```

---

How to get `shared_ptr` by _raw pointer_ or _reference_?
Use `std::enable_shared_from_this<T>`.

``` C++
class Node: public std::enable_shared_from_this<Node> {
  // ...
  std::shared_ptr<Node> getShared() {
    return shared_from_this();
  }
  // ...
};

void foo() {
  Node n;
  n.getShared();      
    // Throws the exception "bad_weak_ptr"
  std::shared_ptr<Node> pNode{ new Node() };
  pNode->getShared(); 
    // Yes, the shared_ptr's constructor
    // will initialize the __weak_this embedded in Node
}
``` 

---

How to get `shared_ptr` by _raw pointer_ or _reference_?
Or prevent users creating objects by `new` or _from the stack_

```C++
class Node {
private:
  Node( /* ... */ ) { /* ... */ } 

public:
  // The only way to create an object is by specified factory method.
  template <typename ...Args> shared_ptr<T> Create(Args&&... args);
};
```

---

User-defined `deleter`.

```C++
struct FnCloseFile { void operator() (FILE* pf) const { fclose(pf); } };

// type of unique_ptr: std::unique_ptr<T, Deleter>, 
//   Deleter should be instantiated and callable.
// type of shared_ptr: std::shared_ptr<T>

// deleter is a functor
std::unique_ptr<FILE, FnCloseFile> uf1(fopen("file.txt", "r"));
std::shared_ptr<FILE> sf1(fopen("file.txt", "r"), FnCloseFile{});
```

--- 

```C++
// deleter is a C function
std::unique_ptr<FILE, int (*)(FILE*)> uf2a(fopen("file.txt", "r"), fclose);
// COMPILATION FAILED:
//   std::unique_ptr<FILE, decltype(fclose)> uf2b(fopen("file.txt", "r"), fclose);
std::unique_ptr<FILE, decltype(fclose)*> uf2b(fopen("file.txt", "r"), fclose);
std::shared_ptr<FILE> sf2(fopen("file.txt", "r"), fclose);

// deleter is a lambda function
std::unique_ptr<FILE, decltype([](FILE* p){fclose(p);})> uf3_cpp20(
  fopen("file.txt", "r"));
std::unique_ptr<FILE, std::function<void(FILE*)>> uf3_cpp11(
  fopen("file.txt", "r"), [](FILE* p){fclose(p);});
std::shared_ptr<FILE> sf3(fopen("file.txt", "r"), [](FILE* p){fclose(p);});
```

* Why?

---

For `unique_ptr`, we hope that the performance of following snippets are *exactly* same:

``` C++
{ // 1a:
  std::unique_ptr<FILE, FnCloseFile> uf1(fopen("file.txt", "r"));
  writeSomething(uf1);
}

{ // 1b:
  FILE* f1 = fopen("file.txt", "r");
  writeSomething(f1);
  fclose(f1);
}
```

Any type erasing will hurt performance. So the `Deleter` should be instantiated by default constructor.

---

* For `shared_ptr`, the `deleter` is stored in a function object which is similar to `std::function`. 
* To cost of calling a generic function object is close to a virtual function call plus 1+ indirect jumps.
* It is also called
* 虱多不痒，债多不愁

---

Stateful `deleter` of `unique_ptr`

``` C++
template <typename ObjT> class Pool
{
private:
  ObjT* alloc() { return nullptr; }
  void free(ObjT*) { /* ... */ }
public:
  struct PoolDeleter { Pool* pool; void operator()(ObjT* obj) {pool->free(obj);} };
  std::unique_ptr<ObjT, PoolDeleter> AllocateObject() {
    return std::unique_ptr<ObjT, PoolDeleter>{alloc(), PoolDeleter{.pool = this}};
   }
};

```

* `Deleter` of `unique_ptr` should be move-assignable.

---

Quiz: Which line(s) is(are) correct in `foo`?

```C++
class B1 { ~B() { /* ... */ } };
class D1: public B1 { // ...
  ~D1() { /* ... */}
};

class B2 { virtual ~B2() { /* ... */ } };
class D2 final: public B2 { // ...
  virtual ~D2() override { /* ... */}
};

void foo() {
  std::unique_ptr<B> b1 = std::make_unique<D1>();
  std::shared_ptr<B> b2 = std::make_shared<D1>();
  std::unique_ptr<B2> b3 = std::make_unique<D2>();
  std::shared_ptr<B2> b4 = std::make_shared<D2>();
}
```

---

> If `T` is a derived class of some base `B`, then `std::unique_ptr<T>` is implicitly convertible to `std::unique_ptr<B>`. The default deleter of the resulting `std::unique_ptr<B>` will use `operator delete` for `B`, leading to undefined behavior unless the destructor of `B` is `virtual`.

> Note that `std::shared_ptr` behaves differently: `std::shared_ptr<B>` will use the `operator delete` for the type `T` and the owned object will be deleted correctly even if the destructor of `B` is not `virtual`.

---

Handle with intrusive reference counting object

``` C++
template <typename T>
class SharedArray {
public:
  SharedArray<T>* Create(size_t sz) {
    auto pArray = new SharedArray<int8_t>(sz);
    pArray->AddRef();
    return pArray;
  }
  void AddRef() { ++m_referenceCount; }
  void Release() { if(--m_referenceCount <= 0) { delete this; } }
private:
  explicit SharedArray(size_t sz) {
    m_data = new T[sz]; m_size = sz; m_refCnt = 0;
  }
  T*     m_data;
  size_t m_size;
  size_t m_refCnt;
};
```

---

``` C++
// OPT 1: Use boost::intrusive_ptr by add 2 utility functions
template <typename T>
void intrusive_ptr_add_ref(SharedArray<T>* p) { p->AddRef(); }
void intrusive_ptr_release(SharedArray<T>* p) { p->Release(); }
void Foo() {
  // ...
  intrusive_ptr pIntArray{new SharedArray<int8_t>()}; 
    // CTAD (class template argument deduction) from C++17 used
  // ...
}

// OPT 2: Use shared_ptr instead intrusive ref counting
shared_ptr<IWhatever> make_shared_from_COM(IWhatever * p){
  p->AddRef();
  shared_ptr<IWhatever> pw(p, mem_fn(&IWhatever::Release));
  return pw;
}

// OPT 3: unique_ptr, similar as above.
// ...
```

---

Misc

* `allocate_shared`
  * Create object by user defined allocator
* `shared_ptr<const T>` != `const shared_ptr<T>`
  * `const T*` and `T* const`
  * Why is this form rarely seen?
    * Too long
    * `std::shared_ptr<Queue<std::unique_ptr<Message>>>` v.s. `Queue<Message*>*`

---

<style scoped>
ul {
    font-size: 0.9em;
}
</style>

# 总结

* 使用*智能指针*管理对象的*生存期*与*所有权*
  * 尽管它**不是**一个“开箱即用”的特性
  * 需要谨慎思考并合理运用
  * 但是和 _裸指针_ 相比仍然好上许多
* 使用`make_*`创建智能指针；并考虑使用 _工厂方法_ 限制对象的创建途径
* 指向*数组*的*智能指针*是有用的，但使用时要注意细节
* 多线程时，要注意 `shared_ptr` 的线程安全性
* 如果要进行指针类型的转换，请使用 `std::*_pointer_cast<U>`
* **不要忘了** 在特定场合 _裸指针_ 和 `weak_ptr` 才是最佳选择

---
<style scoped>
h1 {
    text-align: center;
    position: relative;
    top: 40%;
    font-size: 2em;
    color: darkblue;
}
</style>

# Q & A

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

# Thank you!
