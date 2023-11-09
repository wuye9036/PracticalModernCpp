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
# Pointers, smart pointers and ownership, III

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
Make a choice between `unique_ptr`, `shared_ptr`, `weak_ptr` and _raw pointers_

* It is not a simple question.

* Following rules may help you.

---

Q1: How many reference points for the object? <br>

If the answer is "1", `unique_ptr`. <br><br>

``` C++
class UniqueHwModule
{
public:
    std::vector<bytes> Read(address_t start, size_t length) {
        // ...
    }
private:
    std::unique_ptr<ScratchPadMemory> m_privateMem;
};
```

---

Q2: How many pointers **indeed own** the object at the same time?

If the answer is "more than 1",
AND
equals to the number of ref points, `shared_ptr`

``` C++
class Processor {
  std::shared_ptr<Cache> m_sharedCache;
};

class Chip {
  std::vector<Processor> m_processors;
};
```
---

Q2: How many pointers **indeed own** the object at the same time?

If the answer is "1" please consider `unique_ptr` OR _value_ + _raw pointers_

``` C++
class Processor {
  Cache* m_sharedCache;
};

class Chip {
  std::unique_ptr<Cache> m_sharedCache;
  // OR
  //    Cache m_sharedCache; 
  // if polymorphism or replaceablity is not required.
  std::vector<Processor> m_processors;
};
```

---
Q2: How many pointers **indeed own** the object at the same time?

If the answer is "more than 1",
AND
Not all reference points are owners.

For e.g., 

* Messages are passing in a complex, asynchronized system
* The object will be shared out of our system
* **CAUTION**: Circular references may be overlooked in complicated scenarios.

---

Q3: Could the lifetime of (not owned) ref points be longer than the lifetime of the object? If **NO**,
`shared_ptr` for owners, and _raw pointers_ for the others

``` C++
class InstInterpreter{
    void ProcessInstPacket(InstPacket const* inst);
};

class ShaderExecutor{
  void RunOneInstruction() {
    // ...
    auto instPacket = make_shared<InstPacket>(/* args ...*/);
    m_ifcToOtherBlocks->send(instPacket);
    m_interpreters[instPacket->Category()]->ProcessInstPacket(instPacket.get());
  }
  std::unordered_map<InstCategory, std::unique_ptr<InstInterpreter>> m_interpreters;
};
```

---

Could the lifetime of (not owned) ref points be longer than the lifetime of the object? If **YES**
Q4: Do you **try** to use it even the object has been deleted?
  * NO, indicates the BAD design. Potential pointer dangling issue here.
  * YES, Q5: Are you **eager** to resurrect it?
    * YES That means the lifetime is too short. Please consider:
      * Own it by `shared_ptr` OR
      * Own a copy.
    * NO, how to get its living/available state?

---
``` C++
// (Continued) NO, how to get its living/available state? Solution 1
// Main App
class DomNode {
  Node* m_parent;
  std::unordered_map<NodeIds, std::shared_ptr<Node>> m_children;
  void remove(NodeId nodeId) { m_children.erase(nodeId); }
};
// Plug-in
class Plugin {
  std::weak_ptr<Node> m_node;
  void Process() {
    if (!m_node.expired()) { ProcessLivingNode(m_node.lock().get()); }
  }
};
```
* Any issue in `Process()`?

---

``` C++
// (Continued) NO, how to get its living/available state? Solution 1

// Main App
class DomNode {
  Node* m_parent;
  std::unordered_map<NodeIds, std::shared_ptr<Node>> m_children;
  void remove(NodeId nodeId) { m_children.erase(nodeId); }
};

// Plug-in
class Plugin {
  std::weak_ptr<Node> m_node;
  void Process() {
    if (auto node = m_node.lock()) { ProcessLivingNode(node.get()); }
  }
};
```

---

``` C++
// (Continued) NO, how to check whether it is living/available? Solution 2

// Main App
class DomNode {
  Node* m_parent;
  std::unordered_map<NodeIds, std::shared_ptr<Node>> m_children;
  void invalidate() { /* ... */ }
  bool isValid() const noexcept { /* ... */ }
  void remove(NodeId nodeId) {
    m_children[nodeId]->invalidate();
    m_children.erase(nodeId);
  }
};

// Plug-in
class Plugin {
  std::shared_ptr<Node> m_node;
  void Process() {
    if (m_node && m_node->isValid()) { ProcessNode(m_node.get()); } 
    else { m_node.reset(); }
  }
};
```

---

Example: A design of the message passing system

``` C++
struct TypeDefinition {
  using MessageType = /* ??? */;
  template <typename MessageType> using QueueType = /* ??? */;
};

class ModuleA {
  QueueType<MessageType> m_messageQueue;
};

class ModuleB {
  QueueType<MessageType> m_messageQueue;
};
```

---
1 Source, 1 Sink *OR* N Sinks but 1 Consumer

``` C++
// OPT 1
using MessageType = std::unique_ptr<Message>; // OR
using MessageType = Message;

template <typename M> using QueueType = std::shared_ptr<Queue<M>>;

// OPT 2
class Subsystem {
  std::unique_ptr<ModuleA> m_A;
  std::unique_ptr<ModuleB> m_B;
  std::unique_ptr<Queue<MessageType>> m_MsgQueAB;
  // Optional
  ObjectPool<Message> m_messagePool;
};

template <typename M> using QueueType = std::shared_ptr<Queue<M>>;
```

---

1 Source, N Sinks, K Consumers (1 < K <= N)
``` C++
// OPT 1
using MessageType = std::shared_ptr<Message>; // shared object
// OR
using MessageType = Message;                  // value semantic

// OPT 2
// Allocate multiple times, free once
class Subsystem {
  // ...
  ObjectManager<Message> m_messageManager;
};

using MessageType = Message*;
```

---

<style scoped>
    h1 {
        position: relative;
        top: 35%;
        text-align: center;
    }

    ul {
        position: relative;
        top: 30%;
        text-align: center;
        font-size: 50pt;
        list-style-type: none;
    }
</style>
# Anything else?

* YES

---

Performance

* The ops with same cost as _raw pointers_, a.k.a, zero-cost abstraction
  * All ops in `unique_ptr`.
    * `make_unique`
    * `unique_ptr<T>(new T())`
    * Movement of `unique_ptr`
    * `unique_ptr<T>::get()` / `unique_ptr<T>::operator ->()`
    * Deleting `unique_ptr` by static typed `deleter`
  * `shared_ptr<T>::get()` / `shared_ptr<T>::operator ->()`

---

<style scoped>
    /* ul ul {
      font-size: 0.9em;
    }
    ul ul ul{
      font-size: 0.9em;
    } */
</style>
Performance

* A little reasonable cost
  * `make_shared`
    * Time cost: 1.2x of `new T()`
    * Space cost: 1 CB per shared object and 2x size per pointer
  * `weak_ptr<T>::lock()`
    * Creating a copy of `shared_ptr`

---

<style scoped>
    /* ul ul {
      font-size: 0.9em;
    }
    ul ul ul{
      font-size: 0.9em;
    } */
</style>
Performance

* More expensive than you think
  * `shared_ptr<T>(new T);` 2x time cost compare to `new T()`
  * Copy operation of `shared_ptr`
    * The time cost of `copy` would be up to ~30% in sub-system
    * Shared counters are `std::atomic<>` for thread safety copy
  
---

Performance

Optimize the `shared_ptr` performance issue
  * If not a bottleneck, just accept it
  * Review your design and reconsider the lifetime management of objects
  * If all pointers of the object are used in the same thread, try `boost::intrusive_ptr` or `boost::local_shared_ptr`

---

Thread safety of `shared_ptr`
Quiz: In `threaded_func`, which statement(s) is(are) thread-safety?

``` C++
void threaded_func(shared_ptr<Dog>& spDog) {
   shared_ptr<Dog> spDog2 = spDog;
   spDog2.use_count();
   spDog.use_count();
   spDog2.reset();
   spDog->Bark();
   spDog.reset();
}

void main(){
  auto spDog = std::make_shared<Dog>("Boxer", "Meow", 3);
  std::jthread t1([&spDog](){ threaded_func(spDog); });
  std::jthread t2([&spDog](){ threaded_func(spDog); });
}
```

---

<style scoped>
blockquote {
    position: relative;
    top: 5%;
    color: #1155aa;
    font-size: 0.9em;
}
</style>

> All member functions (including copy constructor and copy assignment) can be called by multiple threads on different instances of `shared_ptr` without additional synchronization even if these instances are copies and share ownership of the same object.

> If multiple threads of execution access the **same** `shared_ptr` without synchronization and any of those accesses uses a non-const member function of shared_ptr then a data race will occur; ...

---

Thread safety of `shared_ptr`
Quiz: In `threaded_func`, which statement(s) is(are) thread-safety?

``` C++
void threaded_func(shared_ptr<Dog>& spDog) {
  shared_ptr<Dog> spDog2 = spDog;  // Yes
  spDog2.use_count();              // Yes
  spDog.use_count();               // Yes
  spDog2.reset();                  // Yes
  spDog->Bark();                   // Depends on Dog's thread safety
  spDog.reset();                   // No
}

void main(){
  auto spDog = std::make_shared<Dog>("Boxer", "Meow", 3);
  std::jthread t1([&spDog](){ threaded_func(spDog); });
  std::jthread t2([&spDog](){ threaded_func(spDog); });
}
// For thread safety read/write shared_ptr, use `atomic_*(shared_ptr<T>)`
// Or std::atomic<shared_ptr<T>> from C++20
```
