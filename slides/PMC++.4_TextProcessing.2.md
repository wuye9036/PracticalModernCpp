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
    section ul {
      font-size: 0.95em;
    }

    section iframe {
      margin-top: 25px;
      width: 100% !important;
      height: 90%;
    }

    section iframe.h8 {
      width: 100% !important;
      height: 80%;
    }

    section iframe.h10 {
      width: 100% !important;
      height: 100%;
    }
---
<!-- theme: gaia -->

# Text Processing II

<style scoped>
h1 {
    text-align: center;
    position: relative;
    top: 30%;
    font-size: 1.8em;
}
</style>

---
<style scoped>
  p {
    font-size: 0.9em;
  }

  ul {
    font-size: 0.825em;
  }
</style>

## 回顾

String representation

Manipulation
* Create
* Join and Concatenate 
* Split
* Substring
* Trim
* Examples based on C++11/14/17/20/23 with or without third party libraries
  * (C++11) *boost string_view/string algorithm/tokenizer*, *fmt*
  * (C++14) *abseil*, *range-v3*

---
<style scoped>
  p {
    margin-top: 0.5em;
    margin-bottom: 0.5em;
  }
</style>

## 回顾

Don't use C string

Use standard or third-party `string_view`

Use the latest C++ standard if possible

Use 3rd party string libs (boost/folly/abseil/range-v3) to simplify your code

  * C++11: stream + fmt (join, concate, string_view) + boost (split, trim, string_view)
  * C++14/17/20: *abseil* or *range-v3* (for join, concate, split, string_view)


---
<style scoped>
  p {
    font-size: 0.9em;
  }

  ul {
    font-size: 0.85em;
  }
</style>

Why is the "manipulation" so complicated?

“既要… 又要…”
  * Intuitive 直观
  * Concise 简洁
  * Performant 高性能

---

什么是：直观和简洁

1 *Unique-solution* for simple problem
2 *Flexible/extensible-form* for complex problem
3 *Completeness* for broad domains
4 *Significant differences* between usages

``` C++
// Traverse vector's elements.
for(auto v: vec) { doSomething(v); }
for(auto const& v: vec) { doSomething(v); }
for(size_t i = 0; i < vec.size(); ++i) { doSomething(i, vec[i]); }
```

---
<style scoped>
  p {
    margin-top: 0.5em;
    margin-bottom: 0em;
  }
  ul {
    margin-top: 0em;
    font-size: 0.7em;
  }
</style>
``` C++
// Endswith
EXPECT_EQ(
  /* C++11 */
  s.size() >= ending.size() && std::equal(ending.rbegin(), ending.rend(), s.rbegin()), 
  /* C++20 */
  s.ends_with(ending)
);
```

All cases in previous charper formed as:

> In C++11, we should blah blah blah, but in C++14, with library such_a_lib we blah blah blah.

Have problems 3 and 4. 
 * *Completeness* for broad domains
 * *Clear and definite border* for cases

---

* Manipulation -> How to handle visibility spreading?
  * Lifetime management
    * Manual / Auto (RAII)
  * Visibility spreading
    * Duplicate (Value)
    * Reference semantic
      * Lifetime No-change / Extended / Transferred(Moved)
  * Visibility(maybe with Lifetime) Separation of the *Entity* and *Parts of Entity*

(Unwanted) "*Domain Specific Language*" for everything

---

To our own library design and coding rules
* Layered libs, clear responsibility
* Explicitly lifetime management
  * *value* / `*` / `shared_ptr` / `unique_ptr` / `weak_ptr`
* Return by value or *generic* pointer if possible
* Pass by value or reference/pointer
* **NO** pointer calculation
* Performance should be the first and last thing you consider
  * In system desgin stage and and pre-release/optimization stage
  * **DON'T** focus on perf in sub component level design and impl

---

## Examination

就没有这么事儿

- get *length*
- *find* (also called *index*)
- *count*

---

<style scoped>
  p {
    margin-top: 0.5em;
    margin-bottom: 0.2em;
  }
</style>

### `string` is a *string*

Operations from sequence types / (contigurous) iterators
* *length*
* *find* (*index*) / *rfind*
* *startswith*
* *endswith*

Operations are meaningful with writing system
* *isalpha*, *isalnum*, *isascii*, *isdigit*
* *isupper*, *islower*, ...

---
<style scoped>
  p {
    margin-top: 0.2em;
    margin-bottom: 0.2em;
  }
  pre {
    margin-top: 0.7em;
  }
</style>

### Length

``` C++
char const* cs = "...";
size_t len = strlen(cs); // cs is const char *

std::string s;
size_t len = s.length(); // size() is the same

std::string_view sv;
size_t len = s.length(); // size is the same
```

`Length` is the counted by bytes, not the number of characters.

``` C++
// UTF-8 narrow multibyte encoding
const std::string_view str = "z\u00df\u6c34\U0001f34c"; // or u8"zß水🍌"
std::cout << std::quoted(str) << " is " << str.size() << " bytes.";

// "zß水🍌" is 10 bytes.
```

---

### *find*

``` Python
s = "The quick brown fox jumps over the lazy dog"
k = "fax"

assert k in s
assert s.index(k) == 0
assert s.find(k) != -1
assert s.find(k, 5, 20) != -1 # find(substr, spos, epos)
```

For C++
``` C++
std::string s = "The quick brown fox jumps over the lazy dog";
std::string k = "fox"; // OR C++14: auto k{"fax"s};
```

---

### *find* subsequence

``` C++
// C++11, naive implementation
assert(s.find(k) != string::npos);
assert(s.find(k, 5, 15) != string::npos);  // find(substr, spos, len)

// C++17
assert(std::search(s.begin(), s.end(), k.begin(), k.end()) != s.end());
// boyer_moore_searcher / boyer_moore_horspool_searcher (C++17)

// range style: range-v3 + C++14 or C++20
assert(!ranges::search(s, k).empty());
assert(!ranges::search(string_view(s).substr(5, 15), k).empty());

// C++23 contains
assert(!s.contains(k));
```

---
<style scoped>
  p {margin-top: 0.75em;}
  ul {margin-top: 0.5em;}
  h3 {margin-bottom: 0.5em;}
</style>
### *find* subsequence

**Similar functions discrimination**

C++11 `std::find` in `<algorithm>`

* Find an element in sequence

C++17 `std::includes`

* Interval in interval
  * `vector{1, 2, 3, 4, 7, 11}.includes(vector{3, 7});` <= True
  * `vector{1, 2, 3, 4, 7, 11}.includes(vector{3, 6});` <= False
---

### *find* subsequence

`rfind`

``` C++
// C++11
assert(s.rfind(s, 'o') != string::npos);
assert(s.rfind(s, "o") != string::npos);

// C++17 find_end
assert( find_end(s.begin(), s.end(), k.begin(), k.end()) != string::npos );
// ranges ver
assert( !ranges::find_end(s, k).empty() );
```

`std::string::find` <-> `std::string::rfind`
`std::search` <-> `std::find_end`

---
### *find* element

`find_if` / `find_first_of` / `find_last_of`
`find_first_not_of` / `find_last_not_of`

``` C++
std::string haystack = "the beatles";
std::string needles = "abba";

find_first_of(haystack.begin(), haystack.end(), needles.begin(), needles.end());
find_first_of(haystack, needles);

// Quiz: how about find_last_of?
```

More features - such as "Pred" - Will talk in the chapter "algorithm"

---
<style scoped>
  p {margin-top: 0.25em;}
  pre {margin-top: 0.3em;}
  ul {margin-top: 0.5em;}
  h3 {margin-bottom: 0.5em;}
</style>
### *count*

Counting element

``` C++
cout << count(s.begin(), s.end(), 'o') << endl;
cout << ranges::count(s, 'o') << endl;
```

Counting sub-sequence
``` C++
// From stackoverflow
while ((pos = s.find(k, pos )) != std::string::npos) {
  ++ occurrences;
  pos += k.length();
}
```

* Time complexity?
* How to do better solution?

---
<style scoped>
  pre {
    margin-top: 0.25em;
    margin-bottom: 0.25em;
  }
  ul {
    margin-top: 0.2em;
    margin-bottom: 0.5em;
  }
</style>
### *find_all*

Implements by `find`

Boost.find_all/ifind_all

* C++11/14/17
``` C++
std::vector<boost::iterator_range<std::string::iterator>> output;
boost::algorithm::find_all(output, s, k);
```
* C++20
``` C++
std::vector<std::string_view> output;
boost::algorithm::find_all(output, s, k);
```

---

`starts_with`

``` C++
// C++20
s.starts_with(k);

// Boost
boost::algorithm::starts_with(s, k);

// C++11
s.find(k) == 0;
// Quiz: any issue?
```

---

`ends_with`

``` C++
// C++11
bool ends_with(std::string_view s, std::string_view ending) {
  s.size() >= ending.size() && std::equal(ending.rbegin(), ending.rend(), s.rbegin());
}

// Boost
boost::algorithm::end_with(s, k);

// C++20
s.ends_with(k);
```

---

## Take Away

* Use member functions in `std::string` / `std::string_view`
* Use `<algorithm>`, but you need to be alert to the misuse.
* Don't forget *Boost.StringAlgorithm*. Find the examples from web.
  * The document is not friendly.

---
# つづく

Character systems

* Comparison of strings will be talked in this charpter