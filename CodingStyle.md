# `#include <xxx>`

# Naming the entities

## General naming rules
* **This rule is not applied to the name that shared with other systems.**
  * For e.g., FIFO/HW/Reg Module names
* **Don't** abuse abbreviation for variable/function/class name
  * **DON'T**: the head words
    * For e.g. barycentric**Point**
  * **DON'T**: short words (In general, <= 8 characters)
    * For e.g. `Mode` -> `md` , `Module` -> `Mdu`, `Core` -> `Cor`, `Port` -> `Prt`, etc.
  * **DON'T**: ambigous abbreviations
  * **DO**: Abbr. CAN be used where there is clear context *WITHOUT* misunderstanding
    * e.g. <br/> `Context ctx;` <br/> `std::function<void()> func;`
    * Common abbreviations: https://github.com/kisvegabor/abbreviations-in-code
* **PLEASE FIX TYPOs!**

## Naming convention

* Macro
  * All capital, underscore as separator
    * `HAS_DEFAULT_CONSTRUCTORS(MyClass);`
* Type name
  * starts with capital letter, and capital letter for each new word.
  * `ThisIsAType`
* Variable name
  * Local varaible as `localVariable`
  * Class member variable as `mFieldName`
  * Struct member variable as `FieldName`
  * Static member variable as `sFieldName`
  * `const static` or `const local variable as `kValue`
* Function name
  * Regular functions have mixed case; accessors and mutators may be named like variables. <br/> Ordinarily, functions should start with a capital letter and have a capital letter for each new word.
  * Prefer verb-object phrase
    * For e.g. `AddTriangle`
* Enumeration
  * `enum`
    * The name of `enum` is following type naming rule.
    * Field names in `enum` like the macros and should have a prefix to identify the `enum`.
    * e.g. `enum PrimitiveTopology{ PRIMTOPO_TRIANGLE_LIST = 0; };`
  * `enum class`
    * The name of `enum class` and fields follows the naming conversion of structure.

``` C++

struct MyStruct {
    int Value;
};

template <typename T>
class MyClass {
public:
    void DoSomething() {
        int localVariable;
        // ...
    }
private:
    T mSomeField;
    static int sField;
    static int const kMaxValue;
};
```

# Statements

## `if` - `else`
``` C++
// Rule: `cond` should be a bool
if (cond) { return false; }

if (cond) {
  return false; 
}

if (cond) 
{
  return false; 
}

if (cond)
{
    // ... do something ...
}

// Rule: Braces are requied even only one statement in the branch.
if (cond) 
{
    return localVariable; 
}

// Rule: Convert to bool explicitly, except pointer.
if (iValue != 0) 
{
    // ... do something ...
}
```

## `switch`

``` C++
// Rule: Pass-thru is only available for shared logic
switch (v) {
    case A:
        // ... do something ...
        break;  // don't pass through
        
    // Pass-thru is only allowed for following case
    case B:
    case C:
    case D:
        // ... do something for B, C and D ...
        break;

// Rule: Default is required
    default:  
        // ... do something or validation ...
        break;
}
```

# Function

``` C++
class SmallStruct {
    void* p;
    int v;
};

class BigClass {
    int arr[20];
}

// Rule: For small structure (<= 2 * sizeof(void*)), passing by value
// Rule: For big class, passing by const reference
void Foo(SmallStruct smallValue, BigClass const& bigValue)
{
// Rule: var
    int apple = 5;
    // ... do something ...
}
```

# `struct`/`class` with template

## Naming convension



## Guides
### Don't use macro in `class/struct`
Don't
``` C++
struct Value {
    int v;
#if defined(DEBUG)
    DebugInfo dbg;  // !!! Danger !!!
#endif
};
```
Do
```
struct Value {
    int v;
    std::unqiue_ptr<DebugInfo> dbg;
};
```

# `assert` and `expect`
# Standard library
