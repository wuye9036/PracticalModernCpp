# C++ Async Execution Models

## Synchronization Primitives

* Mutex
* Atomic
* Conditional Variables
* Semaphores

## Message Passing Style (MPI)

## Completion

## Promise/Future

## Coroutine

## `std::execution` / `libunifex`

### Concepts

#### Schedulers

#### Senders

#### Receivers

#### Operation State

#### Execution Context

### Features

#### Continuation-Passing Style 

#### Unix pipe

#### Type inference and constraint

#### Lifetime management and copy-free design

#### Lazy evaluation
* `let_*`
* Lazy senders could be optimized before submitted

#### Error handling and cancellation
* `set_error` and `set_done`

#### CPO

* Order of resolution of CPO in `std::execution`
  * `tag_invocable` > others

### Implementation analysis


#### `libunifex`

#### Python version execution `pex`

### Related techniques

#### Monadic

### Design Patterns

#### Command

#### Decorator

#### Composition

### References

https://github.com/NVIDIA/stdexec/blob/main/include/stdexec/execution.hpp
https://github.com/facebookexperimental/libunifex