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

# Text Processing I

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
ul {
  font-size: 0.9em;
}
</style>

## Perspective of the series

* Features and libraries
  * *Smart pointers*/*Containers*/...
* Application and Scenario
  * **Text processing**/...
* Paradigms and Design Patterns
  * Polymorphics/Concurrent/Functional/Data Driven/...
* Engineering
  * Project Organization/Build/Test
  * Dev and debugging environment setup ~~No Vim, Thanks~~
  * Redist/Deploy/...

---
<style scoped>
  p {
    font-size: 0.9em
  }
</style>

> The C++ std::string class templates are among the most used features of the C++ standard library. For instance, an article in the Google Chromium developer forum stated that std::string accounted for half of all calls to the memory manager in Chromium.

-- *Optimized C++* by *Kurt Guntheroth* (O' Reilly 动物书系列, 2016)
-- *Charpter 4. Optimize String Use: A Case Study*

---
<style scoped>
p {
  font-size: 0.9em
}
img {
  position: absolute;
  left: 65%;
  top: 46%;
  width: 330px
}
</style>

> The C++ std::string class templates are among the most used features of the C++ standard library. For instance, an article in the Google Chromium developer forum stated that std::string accounted for half of all calls to the memory manager in Chromium.

-- *Optimized C++* by *Kurt Guntheroth* (O' Reilly 动物书系列, 2016)
-- *Charpter 4. Optimize String Use: A Case Study*

![](media/notsimple.jpg)

---

## "Hello world"

``` C++
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Hello world!";
}
```

> The standard-library string (§4.2, Chapter 36) had its origins in early work by Jonathan Shopiro and me at Bell Labs but was revised and extended by several different individuals and groups during standardization.

---
## Scenario

* Web server / client
* Logging and log analyzer
* Human-readable and editable hardware or software traces
* ...

---

## "Disposable" text processing

* Scripts and/or Linux toolchains may be better choice
  * grep/awk/sed/sort/...
  * Perl/Python/...
* Cherish your "Eureka" moments
* ~~C++ script mode may be not a bad idea~~

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
# Agenda


---
## Go over
* String representation in C/C++
  * C string
  * `std::basic_string<CharT>`
    * `CharT`, `char`, `wchar_t`, `char*_t`
  * `std::basic_string_view<CharT>` or 3rd party `string_view`s
    * *Boost*, *abseil*, etc.

---

## Go over
* Manipulation
  * Construct
  * Copy
  * Concatenate
  * Join
  * Split (by *Position* or by *Separator*)
  * Trim (a.k.a Strip)
* Cons of C/C++ strings [before C++17]

---

<style scoped>
  ul {
    font-size: 0.9em;
  }
</style>
## Go over

* Examination
  * Get Byte Length (Get Character Counts will be talked later)
  * Comparison (includes *Lexical order* & *Case sensitivity*)
  * Find or Index
  * Startswith
  * Endswith
  * Counting
* Generation
  * Format (by a dedicated chapter)
* QnA / Supplements

---
## Character sets
* Character and its byte representation
  * 烫烫烫/屯屯屯/锟斤拷/锘锘锘
* Non-unicode Character sets
  * Legacy sets: ASCII, EBCDIC, Windows Code Pages (cp 936/cp 932)
  * ISO sets: ISO 8859-1 (Latin-1), ...

---
## Character sets
  * Unicode
    * Code point
      * ISO/IEC 10646
      * B(asic) M(ultilingual) P(lane)
    * UTFs
      * UTF-7/8
        * BOM
      * UTF-16 with Big/Little Endian
      * UTF-32

---
## Character sets

* Representation and string literals
  * `char`, `wchar_t` and `char8_t`, etc.
  * `UNICODE`, `_UNICODE`, `_T("text")`
  * `L"abcdefg"` / `u8"abcdefg"`
  * Escape characters: `\t\r\n\v\"\'\0\\`
    * Recusively espacing `\\\\path`
  * Raw string
    * `auto rawString = R"abcdefg()abcdefg\")abcdefg";`

---
<style scoped>
  ul {
    font-size: 0.9em;
  }
</style>
## Character sets
* Locale
  * Character classification
  * Conversion between: Storage Types / Character Sets
  * Digits/Concurrency Format
  * Identical glyphs on different code points
    * NOTE: Unicode doesn't encode glyph
    * Security issue such as IDN Homograph Attack `"а", "a"`
  * Input and Output
    * Keyboard, IME, output locale, etc.
* How to: Examples of processing C/C++ strings with localization

---
  
* Format and Conversion
  * C format operations
    * `*printf`s
    * `*scanf`s
    * `*to*`s
---

* Format and Conversion
  * C++ before C++20
    * `boost::lexical_cast`, `istream`, `ostream`, `std::to_string`, `std::sto*`
    * *{fmt}* library
      * Precursor of C++20 `<format>`
  * C++ 20 and later
    * `<format>`
    
---

* The overview of 3rd parth strings
  * *FBString*
  * MFC/ATL's *CString*
  * Qt's *QString*
  * LLVM's strings, `Twine`, `StringRef` ...

---

* Matching and Parsing
  * Regular Expression
    * Quick look
  * Pre-defined structural text-based file format and libraries
    * Yaml
    * Json
    * Xml
    * Proto
    * xSV

---
  
* Text processing strategies at different scales
  * Less than **Tens MB** / Can be opened by most text editors
    * Versioning and Compatibility
    * Validation: syntax, semantic/schema, integrity, etc.
    * Character set
    * Mapping to strong typed objects in the code
      * For e.g. *Serialization* and *ORM*
    * Binary storage
      * Base64 or hex

---

* Text processing strategies at different scales
  * **Tens MB** to **Tens GB** / Can be loaded into memory
    * Indexing and Fast Querying
    * (Advanced) Versioning
    * Partition and partial loading
    * Failure tolerance and recovery

---

* Text processing strategies at different scales
  * **Tens GB** to **TB** / Can be processed on single node in hours
    * Processing by our tool chain
      * Inherited from above
      * Concurrent processing
        * Multihreading or Multiprocessing
        * Asynchronization programming model
        * C++ or not C++?

---

* Text processing strategies at different scales
  * **Tens GB** to **TB** / Can be processed on single node in hours
    * Rely on single node or centralized database
      * RDBMS
      * Non-SQL database
      * *In-the-process* or *out-of-process*

---
* Text processing strategies at different scales
  * Beyond **TB**
    * Distributed processing
      * e.g. Apache data processing stack
        * Hive/Spark/...
  
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

# String representation in C/C++

---

``` C++
#include <cstring>

// ASCII only (0000 - 007F), i.e. 1 Char <==> 1 Byte
char const* cs1 = "Hello World";
char const cs2[] = "Hello World";
char const cs3[] = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '\0' };

void foo() {
    static_asert(sizeof(cs1) == 12);
    assert(strlen(cs1) == 11);
    assert(strcmp(cs1, cs2) == 0);
    assert(strcmp(cs2, cs3) == 0);
}
```
* C-style string
  * Null-terminated
  * Array of `char`s

---

<style scoped>
  ul {
    /* font-size: 2em; */
  }
</style>

``` C++
#include <string>
// using string = std::basic_string<char>;

void foo() {
    std::string s{"Hello World"};  // Allocation
    std::string s = "Hello World"; // Allocation
}
```

* null-terminated is not mandatory before C++11
  * allocated space covers `[begin, end)`
  * `c_str()` may be a trick in C++98/03
* Null terminated after C++11
  * allocated space covers `[begin, end]`
  * `c_str()` is equiv to `data()`

---

``` C++
template <typename CharT> class basic_string_view {
   CharT* start;
   size_t length;
   // ...
};
```
* `string_view` does not own or extend the lifetime of object
  * Pass by value due to it is already reference semantic. 
    `void doStuff(string_view sv);`
  * Don't return the `string_view` of local string out of function.


---
Pros & Cons of `string_view`

* `[O]` Copy is cheap, sub-string is cheap
* `[O]` Sub part of `string_view` is also a `string_view`
* `[X]` `string` to `view`, shallow copy. `view` to `string`, deep copy.
  * Prefer use `string_view` in calling stack.
* `[X]` Cannot do `insert` or `erase` that have to modify the length of orignal string
* `[X]` Cannot convert to C string directly
  * `std::string s{sv}; printf("%s", s.c_str());`
  * `fmt::print(sv);`

---

<style scoped>
    h1 {
        position: relative;
        top: 35%;
        text-align: center;
    }

    ul {
        position: relative;
        top: 35%;
        left: 40%;
        text-align: left;
        font-size: 16pt;
    }
</style>

# Manipulation

---

* Environment
  * C/C++ (<= C++14) **w/o** 3rd party libraries
  * C/C++ with standard libs
    * C++ 17 `string_view`
    * C++ 20 *ranges*, *format*, `starts_with`, `ends_with`
    * C++ 2b `contains`, `join_with`
  * C/C++ with **quasi-standard libs** (11 or 14 required)
    * *{fmt}*, *range-v3*, *abseil*, *boost*, *folly*
  * C/C++ with 3rd Party Libs

---

* Manipulation
  * Construct
  * Copy
  * Concatenate / Join
  * Split / Substring
  * Trim

---

Manip: Construct
``` C++
// Construction from literal
char const* cs1 = "Hello World";  // No Allocation
std::string ss1 = "Hello World";  // Allocation and copy/in-place construction
   // Note: some popular C++ has small string opt

// Construction from some read function (Typical senario)
size_t client_get_name_length(Client*);
size_t client_get_name(char* outBuf, size_t bufSize, Client*);

size_t nameLen = client_get_name_length(c);
char* cName = static_cast<char*>(malloc(nameLen + 1));
subst(cName, nameLen+1, c);

std::string ss2 = c->getName();
// OR std::string const& ss2 = client->getName();
```

---

Manip: Copy

``` C++
// C: Get length - malloc - copy

// strcpy(char restricted* dest, char restricted* src);

char dest[4096]; // OR char* dest = (char*)malloc(strlen(src) + 1);
strcpy(dest, src);
strncpy(dest, src, 10);
strcpy_s(dest, sizeof dest, src); // C11
```

* For copy to heap string, `strlen()` may be called.
* Security tips: Prefer `strncpy` or `strcpy_s` to `strcpy`

---

Manip: Copy and Move

``` C++
// C++
// ... initialize src ...
std::string dst1, dst2;
dst1 = src;
std::string dst3{src};
dst2 = std::move(src); 
// After line execution, src is null and dst2 take(steal) src's content.
``` 

* The copying of `std::string` needs heap allocation except using stack-based allocator.

---

Manip: Concatenate 1: C impl

``` C++
// C: Concatenate by strcat/strcpy
typedef char const* PCSTR;
PCSTR csa[] = {"aaa", "bbb", "ccc"};
char buffer[1024] = "";
// ... 1 ...
char* d = buffer;
for (int i = 0; i < 3; ++i){
  size_t sz = strlen(csa[i]);
  strcpy(d, csa[i]); d += sz;
}
// ... 2 ...
for (int i = 0; i < 3; ++i) {
    strcat(buffer, csa[i]);
}
```
Quiz: which one's performance is better?

---

Manip: Concatenate 2: by `std::string`

<iframe class="h8" width="800px" height="200px" src="https://godbolt.org/e#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:42,endLineNumber:21,positionColumn:42,positionLineNumber:21,selectionStartColumn:42,selectionStartLineNumber:21,startColumn:42,startLineNumber:21),source:'%23include+%3Ciostream%3E%0A%0A%23include+%3Cvector%3E%0A%23include+%3Cstring%3E%0A%0A%23define+OUT_VAR(v)+std::cout+%3C%3C+%23v+%3C%3C+%22+%3D+%22+%3C%3C+v+%3C%3C+std::endl%3B%0A%0Aint+main(int+argc,+char*+argv%5B%5D)+%7B%0A++++std::vector%3Cstd::string%3E+svec%7B%22tx%22,+%2212%22,+%22bl+ah%22%7D%3B%0A++++std::string+a%7B%22tx%22%7D,+b%7B%2212%22%7D,+c%7B%22bl+ah%22%7D%3B%0A%0A++++//+...+1+...%0A++++std::string+o1+%3D+a%3B+o1+%2B%3D+b%3B+o1+%2B%3D+c%3B%0A++++//+...+2+...%0A++++std::string+o2+%3D+a+%2B+b+%2B+c%3B%0A++++//+...+3+...%0A++++std::string+o3a,+o3b%3B%0A++++for+(auto+const%26+s:+svec)+%7B+o3a+%3D+o3a+%2B+s%3B+%7D+//+3a%0A++++for+(auto+const%26+s:+svec)+%7B+o3b+%2B%3D+s%3B+%7D++++++//+3b%0A%0A++++//+QUIZ:+Which+performance+is+better%3F%0A%0A++++OUT_VAR(o1)%3B+OUT_VAR(o2)%3B+OUT_VAR(o3a)%3B+OUT_VAR(o3b)%3B%0A%0A++++return+0%3B%0A%7D%0A'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:54.56924974869089,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:clang1500,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'0',intel:'0',libraryCode:'0',trim:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!((name:abseil,ver:trunk),(name:boost,ver:'180'),(name:fmt,ver:'811'),(name:rangesv3,ver:'0120')),options:'-std%3Dc%2B%2B11+-O2',selection:(endColumn:23,endLineNumber:9,positionColumn:23,positionLineNumber:9,selectionStartColumn:23,selectionStartLineNumber:9,startColumn:23,startLineNumber:9),source:1,tree:'1'),l:'5',n:'0',o:'x86-64+clang+15.0.0+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),header:(),k:45.430750251309114,l:'4',m:44.44444444444445,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+12.2',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+15.0.0+(Compiler+%231)',t:'0')),header:(),l:'4',m:55.55555555555556,n:'0',o:'',s:0,t:'0')),k:45.430750251309114,l:'3',n:'0',o:'',t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

* 3a < 3b == 2 == 1

---
<style scoped>
  ul {
    font-size: 0.8em;
  }
</style>
Manip: Concatenate 2+

``` C++
template <typename... Ts> requires (std::same_as<std::remove_cvref_t<Ts>, std::string> && ...)
std::string join(Ts&&... args) {
  return (std::forward<Ts>(args) + ...);
}
std::string a, b, c, d;
join(a, b);          // equiv. a + b
join(a, b, c, d);    // equiv. a + b + c + d

const char* a; std::string b; string_view c; // How to implement?
join(a, b, c, a);                            // eqv string(a) += b += c += a;
```

* Requied features
  * Variadic parameters (C++11)
  * Universal Reference & Perfect forward (C++11)
  * Fold expression (C++17)
  * Concept (C++20)

---
Manip: Concatenate 3: by `stream`

<iframe width="800px" height="200px" src="https://godbolt.org/e#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGEgOykrgAyeAyYAHI%2BAEaYxCBmAJykAA6oCoRODB7evgGp6ZkCoeFRLLHxSbaY9o4CQgRMxAQ5Pn5cgXaYDlkNTQQlkTFxCckKjc2teR22EwNhQ%2BUjSQCUtqhexMjsHOYAzGHI3lgA1CZ7bk7jxJis59gmGgCCjy9mBwxHXqfnbgBu3SIxHur32h2OmDOF2uYWAIOeYM%2BEKhbgU11uLHhbw%2BXx%2BF0IcSYQKxoPeWBo4ROAHk5AAVAD6ADUngAlCB/FYncboEAoDYEFG/M7vP6Ci7CsxQgAiErFbhOot%2BQu5vNctHOVgRzzCApYTDCEB1JyawGQpBOyAQTQAVMbiMA/iYAKxWJ1Szkmfyap4nE4Aej9Jzc1msICDAlEBEYRMh0QAnlyCMRYei7s9fSqQACHCRfpmYUZ7lzs56rGYzARVOYzObq1wzNXa%2BXorRjQhq56pRrXhmCDyQAXgMbS9XKx3/FLzdER%2BX6%2BPJxaZ2YW23592tT6zl6e77E/3B6mWFyFOvNxmFHLjZfopfTHtvbu97zBydUFxpceAHTXCArU8730A2pYhX0pWgFgAp8ByTWFX0lc4ZQgfMYKMQ8RwnS8mGvW8Vm/JNf1PR8aQZZk2TfP972pOkmVZCBUDMCiHy3LsN19Ed013ZDk1QpMMWPQjOL7Xk0BSOMkOzT9YmAA01mLbpP1cX9zUzdJeNYekCWIIlc2hIToO4uE9mwJCFBWRjIK42DUD2D80Tw4gCPvSDiJosi9nMjjO1BDibgITYGBODR/wnV4ODWWhOCdXg/A4LRSFQThg0sawuQ2LZIX2HhSAITQwrWABrEAnUkT8nQANkSMxJESfxJAADgqjQ6uSCKOEkaLcvizheAUEANGy3K1jgWAYEQPkWBSOg4nISg0Amqb4iOQxgC4J0NH6mhaCjYheogaJOuiMImjjTgsrmthBCpBhaBO2LeCwPUjHEO7SHwG4egBXqXswVRui8KNOp1GpOvA6ItOIOMPCwTqYJYQa%2BAMYAFEZPBMAAdypFJGFO3h%2BEEEQxHYKQZEERQVHUF7dC4fRlpQENLH0PBol62BmDYEAmGZzA6FIAF4iTLwGHyoa2fYaJUFU3mRm4OqtBF1h2CoFgCCl%2BIODqrgeHl9mtKMTAFD%2BPZVc0es5bWVAUjqBgvoAWm5BDTGSyxNZOG2qTMHqakBLIXAYdxPDaPQQgWMoKj0NIMitqZ2gKSOskGUORmproenqOZo70FOrb6ZoE%2BGeJk/TgPplmfo86WAu1gUNLtj0Xidi1/RIo6l6Eo4VQGptsrJAtAwjBOVbPw0IeTggJKyxsE5cEIEhhT2anw3m%2BgQMylZeByu6zNIBBbiweIlMKyQnU/SQ9g0fw1rMJ0uDPxJNekVr2tIOHJBK1/34/j%2BnVIGK4rbnq%2BoDU3qQYaY1%2BQpH%2BjNCAc1JrLwiArTgHcypdx7qaZAA8zCfg9kEfAQI8A8mpnjYQohxDEyIWTNQnUqakDRlpFIONwrNx/p1NuVJ/oQIFKgKgJwkEoN7stAex9h4aFHh4JecQ55cDXkAs229d4jCUo/XgcMnTH1UeojRGiyrMNbt1WwgCN6yMPsfU%2B59L7X1vvfJuHA9gtz/nowxeVrHYN/rwf%2BMinF8wyM4SQQA"></iframe>

---

Manip: Concatenate 4: by `accumulate`

<iframe width="800px" height="200px" src="https://godbolt.org/e#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:32,endLineNumber:27,positionColumn:32,positionLineNumber:27,selectionStartColumn:32,selectionStartLineNumber:27,startColumn:32,startLineNumber:27),source:'%23include+%3Ciostream%3E%0A%0A%23include+%3Cvector%3E%0A%23include+%3Cstring%3E%0A%23include+%3Cnumeric%3E%0A%0A%23define+OUT_VAR(v)+std::cout+%3C%3C+%23v+%3C%3C+%22+%3D+%22+%3C%3C+v+%3C%3C+std::endl%3B%0A%0Aint+main(int+argc,+char*+argv%5B%5D)+%7B%0A++std::cout+%3C%3C+%22C%2B%2B%22+%3C%3C+(__cplusplus+/+100+%25+100)+%3C%3C+%22+is+available.%22+%3C%3C+std::endl%3B%0A%0A++//+Concate+by+accumulate+(fold_left+before+C%2B%2B23)%0A++//+++std::reduce+is+two-way+merge%0A++//+++std::ranges::fold_*+is+C%2B%2B23+proposal%0A%0A++std::vector%3Cstd::string%3E+svec%7B%22tx%22,+%2212%22,+%22bl+ah%22%7D%3B%0A++//+...+1+...%0A++auto+o1+%3D+std::accumulate(svec.begin(),+svec.end(),+std::string%7B%7D)%3B%0A++OUT_VAR(o1)%3B%0A++//+...+2+...%0A++std::string+o2+%3D+std::accumulate(svec.begin(),+svec.end(),+std::string%7B%7D,%0A++++++%5B%5D(std::string+%26s,+const+std::string+%26piece)+-%3E+std::string%26+%7B+return+s+%2B%3D+piece%3B+%7D)%3B%0A++OUT_VAR(o2)%3B%0A%0A++//+Code+1+has+performance+issue+until+C%2B%2B20%0A++//+++--+Due+to+%22Schlemiel+the+Painter!'s+Algorithm%22+(named+by+Yiddish+Joke)%0A++//+Code+2+fixes+this+problem+by+customized+reducer+%5BIllegal+since+C%2B%2B20%5D%0A++//+Anyway,+it+is+not+a+good+practice%0A++return+0%3B%0A%7D%0A'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:54.56924974869089,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:clang1500,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'0',intel:'0',libraryCode:'0',trim:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!((name:abseil,ver:trunk),(name:boost,ver:'180'),(name:fmt,ver:'811'),(name:rangesv3,ver:'0120')),options:'-std%3Dc%2B%2B11+-O2',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1,tree:'1'),l:'5',n:'0',o:'x86-64+clang+15.0.0+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),header:(),k:45.430750251309114,l:'4',m:44.44444444444445,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+12.2',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+15.0.0+(Compiler+%231)',t:'0')),header:(),l:'4',m:55.55555555555556,n:'0',o:'',s:0,t:'0')),k:45.430750251309114,l:'3',n:'0',o:'',t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

---

Manip: Concatenate 5: by `range`, requires >C++14

<iframe width="800px" height="200px" src="https://godbolt.org/e#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:40,endLineNumber:23,positionColumn:40,positionLineNumber:23,selectionStartColumn:40,selectionStartLineNumber:23,startColumn:40,startLineNumber:23),source:'%23include+%3Ciostream%3E%0A%0A%23include+%3Cvector%3E%0A%23include+%3Cstring%3E%0A%23include+%3Cboost/utility/string_view.hpp%3E%0A%23include+%3Crange/v3/range.hpp%3E%0A%23include+%3Crange/v3/view.hpp%3E%0A%0A%23define+OUT_VAR(v)+std::cout+%3C%3C+%23v+%3C%3C+%22+%3D+%22+%3C%3C+v+%3C%3C+std::endl%3B%0A%23define+ILLEGAL(v)+%3B%0A%0Aint+main(int+argc,+char*+argv%5B%5D)+%7B%0A++std::cout+%3C%3C+%22C%2B%2B%22+%3C%3C+(__cplusplus+/+100+%25+100)+%3C%3C+%22+is+available.%22+%3C%3C+std::endl%3B%0A%0A++//+Concate+by+range%0A++using+ranges::views::join%3B%0A++using+ranges::to%3B%0A++using+std::string%3B+using+std::vector%3B%0A++using+boost::string_view%3B+//+std::string_view+in+C%2B%2B17%0A%0A++//+...+1+...%0A++std::vector%3Cstd::string%3E+svec%7B%22tx%22,+%2212%22,+%22bl+ah%22%7D%3B%0A++auto+o1+%3D+svec+%7C+join+%7C+to%3Cstring%3E()%3B%0A++//+...+2+...%0A++ILLEGAL((auto+o2+%3D+%7Ba,+b%7D+%7C+join+%7C+to%3Cstring%3E()))%3B%0A++//+...+3+...%0A++std::string+a%7B%22tx%22%7D,+b%7B%2212%22%7D,+c%7B%22bl+ah%22%7D%3B%0A++string_view+views1%5B%5D+%3D+%7Ba,+b,+c%7D%3B+%0A++string+o3+%3D+views1+%7C+join+%7C+to%3Cstring%3E()%3B%0A++//+...+4+...%0A++const+char*+a1%7B%22tx%22%7D%3B+string_view+b1%7B%2212%22%7D%3B+string+c1%7B%22bl+ah%22%7D%3B%0A++string_view+views2%5B%5D+%3D+%7Ba1,+b1,+c1%7D%3B+%0A++string+o4+%3D+views2+%7C+join+%7C+to%3Cstring%3E()%3B%0A%0A++OUT_VAR(o1)%3B+OUT_VAR(o3)%3B+OUT_VAR(o4)%3B%0A++return+0%3B%0A%7D%0A%0A/*******%0ARequirements:%0A++%3E+C%2B%2B11:+N/A%0A++%3E+C%2B%2B14:+range-v3+%2B+boost/fmt+string_view%0A++%3E+C%2B%2B17:+range-v3%0A++%3E+C%2B%2B23:+Supported.+(Clang+15+is+not+ready)%0A********/'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:46.25078764965343,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:clang1500,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'0',intel:'0',libraryCode:'0',trim:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!((name:abseil,ver:trunk),(name:boost,ver:'180'),(name:fmt,ver:'811'),(name:rangesv3,ver:'0120')),options:'-std%3Dc%2B%2B14+-O2',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1,tree:'1'),l:'5',n:'0',o:'x86-64+clang+15.0.0+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),header:(),k:45.430750251309114,l:'4',m:44.44444444444445,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+12.2',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+15.0.0+(Compiler+%231)',t:'0')),header:(),l:'4',m:55.55555555555556,n:'0',o:'',s:0,t:'0')),k:53.74921235034657,l:'3',n:'0',o:'',t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

---

Manip: Concatenate 6: by abseil's `StrCat` and `StrJoin`

<iframe width="800px" height="200px" src="https://godbolt.org/e#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxBIAnKQADqgKhE4MHt6%2BekkpjgJBIeEsUTFc8XaYDmlCBEzEBBk%2Bfly2mPZ5DDV1BAVhkdFxtrX1jVktCsM9wX3FA2UAlLaoXsTI7BzmAMzByN5YANQmm25OE8SYrEfYJhoAgjf3ZtsMu14HR24AbpVExFcPWx2e0wh2OZ2CwH%2Bd0BL2BoLcTAiCloAHpwUYFGiCMQIQB9T54TAAdwAdAgoY9nq93sdEcisTiMQzcaICGSKTDqSCPnTUejgJizrjtKhguzNtdodCnlgaCF9gB5OQAFVxADVbgAlCCfeb7CboEAoZYEeEfQ5PT5m44WsyggAitutbn2Vo%2B5oNRtctCOVmlm1l032AEkAgFsABxW4BHV6z3Grym902rZu47m8x2vAKENhyPRzPO/UEQ0gb2%2BgF3YKmlhMYIQav7OrAZCkfbIBB1ABUTeIwE%2BJgArFZB/a9SYAOx%2B277YultCJouZtzWVdmO3Jl0QXEshLeBR7rw5lH7LgaDQWwen8/j9Mp9f7bNNz51gwRegkwubudehjoH2bNODyzkeEI/iA3wOCQFZ3CBKRGOB/IwTO%2BygQhvJGvy%2BKEkSvqHLB%2BwoieZgaGYXD7Oc9BMEoj4KAwYAcKaaAsAkTCOO%2BIJEoQCCIdieIEsSlYoZBvwfEhEr6pBk5WOuBCqJmbaZlwZgKba75NuS66TvayGzvyTbSZmcmZtpbYRIZ67KSZE72m2phTpm6lMJpKk2bp7YCBM7adsQPZMFwFlmMZWluYBxaMsA2HEvsEQBQ5lkqSFOlhfpyBxTJZhOS52nIcBhEniShWnvshUkvsigghhIA1MQCIEHlobhlGMZNkitBGjVdUQAokF6vM7lKqqGravsEBVZ1bFjWZdnzH1A0quqWoQKN43Yl1/lmS07ZcLN%2Bz9YBQmzkRJVFXapXlTRVUAFKigweWDYtI1jW1HXYjd9Y9ZUilaeuu37dOs4PcNy3Pcir3EO9DAQNJTDTe2pm2pmf3uY1%2BYtaD7XVW9t3Q1OG0xVtaUIyZv1zQdUooecBArAw%2BwaLlNkcIs7UcIOvB%2BBwWikKgnArpY1j6ssqzck8PCkAQmhM4sADWICSAAbCSZiDhoE4ABwTvLkixPLg6bJImz6Jwkjs5L3OcLwCggBo4uS4scCwDAiDGixdDROQlDMQkbsxLshjAFwKs2zQtAENEVsQBEZsRMEdQAJ6cGLzFsIICoMLQCec7wWC1kY4hZ6Q%2BDnFU3xWwXmCqJUibrFz1ZtGbtB4BExDxx4WBm3xLB23wBgCmqOEKgkjCJ7w/CCCIYjsFIMiCBVahm7oLQGEYKBrjYjdIvAEDMGwIB0pgdCkN8MTYl4DDS/bO/sBEqDJAQR8DNwataJfrDsFQLD38fnBq1wPCv7vFuRhMA9UNt/TmykX6LFQAkDoZcAC0Bojj2lMPzSwUhLZtB%2BGkFwf5RjNFIIEaYRQSjZGSKkAQ%2BCyG5DSL0EhcxWjtGqJMKh4wsFVAEF0eodD%2BilCGN0Vh/DuHEN4RIRYCghZrD0NiTA6x/5G1ZqbAuPMOCqDVvLeBWt2zL2AKeQcJINAGNGnzGSNh9i4EICQC0mwtoeFdvQYg1idq8AllnWapAEAXCwDECAMs5b6INqrFWysuCbA0LEP%2B0gWYm1IF3BWSsdZmESck%2BWYTBykA5lzFRltra2zcaQB2zsTQJETB7CAXsfahDfpwNRGitF%2BwQoHAxBjeCYHwL8PAhoWhj2EKIcQgcZ7yCUPPAui9SBEhbgkEezNOBswyWbFRCpEwlNNKgKg%2BxamaMkNo/2ejmkXggHY72DinHzBcXbRYnimDeMoDMjgMSu56xJBODQmwXlvNee8icLRMm8GybYXJrioGkFlk8ic2syJPBVvrXWE4FGbCUVki2eTgUszMIiv5yKgVSwfsQFIzhJBAA"></iframe>

---

Manip: Join 1

``` C++
// C: Join by Standard C strcpy
typedef char const* PCSTR;
PCSTR csa[] = {"aaa", "bbb", "ccc"}; PCSTR sep = ", ";
const size_t sepLen = (sizeof sep) - 1;

char buffer[1024];
char* d = buffer;
for (int i = 0; i < 3; ++i){
  if (i > 0) {
    strcpy(d, sepLen);
    d += sepLen;
  }
  size_t sz = strlen(csa[i]);
  strcpy(d, csa[i]);
  d += sz;
}
```

---

Manip: Join 2

``` C++
// C: Join by POSIX stpcpy
typedef char const* PCSTR;
PCSTR csa[] = {"aaa", "bbb", "ccc"};
PCSTR sep = ", "; const size_t sepLen = (sizeof sep) - 1;

char buffer[1024] = "";
char* d = buffer;
for (int i = 0; i < 3; ++i){
  if (i > 0) {
    d = stpcpy(d, sepLen);
  }
  d = stpcpy(d, csa[i]);
}
```

---

Manip: Join 3 `stream`, but NOT `join`

<iframe width="800px" height="200px" src="https://godbolt.org/e#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGEgOykrgAyeAyYAHI%2BAEaYxCAAzACspAAOqAqETgwe3r4BaRlZAqHhUSyx8cm2mPaOAkIETMQEuT5%2BXIF2mA7Zjc0EpZExcYkpCk0tbfmdtpODYcMVo8kAlLaoXsTI7BzmCWHI3lgA1CYJbk4TxJis59gmGgCCjy9mBwxHXqfnbgBuPSIxHur32h2OmDOF2uYWAIOeYM%2BEKhbgU11uLHhbw%2BXx%2BF0IcSYQKxoPeWBo4ROAHk5AAVAD6ADUngAlCB/VYnCboEAoTYEFG/M7vP6Ci7CsxQgAiErFbhOot%2BQu5vNctHOVgRzzCApYTDCEB1J2awGQpBOyAQzQAVMbiMA/iYklYklLOSZ/JqnicLQIJharcRbWgGP6sLQ8CxpRLzeYzBrQc8fQB6ZMnNzWawgE6KSFMVKpVwnEjCqwJKXmjOWaxcLivFNpgB0zeLLEIBEw6BOzcbie9J1T6czlmzXiUXIIxFh6NYleHFlr9bOnqXPpVIABDhIv3XMKM9y5m49VjMZgIqjjsdPXHjp6vZmitGNCDjHqlCaTPonPJAGUn08nDETkjVJaAwTsP37NcCB/NBUgATwgVcvwUTdG1iYADXWQ8ekbVwIGw5DoJ/P8blYekCWIIlt2hGDeT3OEEmwQ0WFA8D0HNcNI1WVZIK/b96P/Ixiy4aMQLArB0Eba4CL4r8aQZZk2VQLheISL0fTfJdtM9M5P2IwSp2Ephjzjc9X38CsTmiUzr1veNLPNUwV1PR9nws991OQ9dSIAsio3E9i5J9JgvCICdiCeYhiGjY8mEbZB6Rk7DogSpLJwIpy0uSrSvP0gS%2BQQpD8p9DCDWuKLiGw/CKuiwiSoK0iMQojsqOJWifwY%2B4WLYyTONqbi1I0lC6JABji0lc4ZUCyTpIyobkIUplWQgVAzAWz8tK1KCThuAgtgYE4NEgrang4dZaE4JJeD8DgtFIVBOCrE8LC5TZtkhfYeFIAhNHO9YAGsQCSSRGySAA2ABOMxJEh/xJAADihjQEch/ROEkG6/oezheAUEANB%2Bv71jgWAYEQPlWLoOJyEoNAqfoeIjkMYAuCSDRCZoWhWvxiBomx6IwmaeDOG%2B%2Bm2EEKkGFoEW7t4LA9SMcQ5dIfAbl6AF8ZVzBVB6MLdnunVamxiNoio4h4I8LBsf/Fhib4AxgAURk8EwAB3KlC1u77%2BEEEQxHYKQZEEXM1Gx3QuH0FmUHnfQ8GifHYGYNgQCYBPMDoUgAXiScvAYAGSeT9holQP8s9GbgEa0QvWHYKgWAIcv4g4BHa3O0gICLkAqKMTBUISJvNBvav1lQVJ6lDTgAFpuSm0xq0sWsTinqkzDx2pAWyFwGHcTx2j0EJFnKSo9HSTIJ%2BmDpCnP7IhmP0ZI%2B6XoGnmS%2B9Cfif%2BhaO%2BRniR/X73jMOYAwf7LD/usBQ70dh6EArsHgF0rpYxVo9DgqgkZT3BpIC0BhhJs0bBofBJwIDPWsOaXAhASxfUrKgBmcRhQJFUrwX6cseKkAQLcLA8RMpA0kEkRskgEgaH8OzMwSQuCCMhrWaQl0OCY1IHbSQoNFHKJUSolIt17ooLxgTImLCO7kwgEgfkqQwq0wgPTVI1NiARFrpwNB4MMFYNNMgE4N5GxryCPgIEeAeSR19sIUQ4gg7%2BNDuoFWEdSBuyoqkUW7cZHXVIBo3gKCqRhRMQKVAVATj2Mcdglmri%2BEEI0EQjwtCYpfVWEw4m6x2FME4ZQBBsjeB2ySHw1p7SOkdPBok7GWjbA6OYSPUgPC%2BECKESIsREipHow4AkJBmjca6KGTIjxSScYcCqSw9Y2dMjOEkEAA"></iframe>

---

Manip: Join 4: by boost/range-v3/abseil/fmt `join`

<iframe width="800px" height="200px" src="https://godbolt.org/e#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxCAArBqkAA6oCoRODB7evnrJqY4CQSHhLFEx8baY9vkMQgRMxASZPn5cFVXptfUEhWGR0XEJCnUNTdmtQ109xaUDAJS2qF7EyOwc5gDMwcjeWADUJutuTkPEmKwH2CYaAIJXt2abDNteewduAG6YDiQXdxtbO0w%2B0OJ2CwF%2BN3%2BT0BwLcEVQKQIAHovI5aIQAJ5I0FGAD67zwmAA7gA6BCJRIQ%2B6PZ6vQ7wxFIsTAEiEBAsbEEYhgskUqlQ2lAt5UFjI/jEFhMAhk/kPAEvIWHEXI4iGYCYBQy9aXSGQuVUXa43HIRLeBSmrwKYGXdYAEV2Zg0Ug0ZgCfzl0IVsKYEQUtE53KMCgDuO0qGCWp11PldLcqqMmCR73WTNotF5lO17vWrjwVD%2BevWWBoIV2AHk5AAVXEANWuACUIO9ZrshugQChFgRYW99g93j3Dn2zMD7eYR29ewPJ0O2x3XLQDlZC8XgkCAJIBALYADi1wCTZbS4L12C3clwQgZ929WAyFIu2QCHqACob8RgO8TLErLFbUeAHZl2uXZWwIdtO1RQc3GHNxrHgswJ0OXsICNE0zQtK0kV2LgNA0PtYhwvCj2Qodx12PArSYd4mDoH16BJciZxgucQAXY9dRA3YkWwjwnilIEIgxXZ43VbM80NY1MMw60DntR1nVdO5QMtMERLVDUOwJYkFA7MNgiXbjsJIMCINEzSQG0oldJAbRlN2VSjHUhMbKIDjqVzfMbhU1InNYnFwXWCwHN84BTK0r4iGIdz/gNNDpLNWEHSdACNHWN1vJCtSGSGDsAvxQkiUMnjwpAfKrIohhdjgyxrC4ADs0qJR7McsL/K5MECuJGKHk8k9QM%2Bb5opBDqjAuVtBpMIDxwIVRxwfccuDMebhwiWgbwQccpttdzQICm8pqsRDZq2gDbQfCJDsW5bEO2h9TGmxC1o206dqC%2By0AYIZH2fYg3yYLgruOubbrOwzysK3YIkBx6zCW17wdGsLkBho6zGephNtBt7gPskqSQJnDdgJkldkUIELARIYSWuWgWW5Ah2RJWpA2AEkAClwwYB8aqOiwuEBzKmFRVBdlQLhRyhqmCA7ZlWUZlg9K5iAFEGhbEPVsxZkMitqzrRtxe197MpK0JRZZDBW08VF0l2Ihdn0qqmAu%2B7%2BqM4nCZHEmyaUXZlSV4IeYQ/nBa44X7dQOHJf9kAiFxALqFFAOGBVtXh012YjeC3XawbCBI64LP7PD0XI6Q%2B0Y7jhOY8diBDudqHXbOzWM6z8sq1zg3EKLzLN23PcD0TmXY9QeOkaH5O66AgGLtaR8YfO9Ph0zzOYqFkWoa4XFlWjpOyqRrqiqA6GSXQKUmAgeZN5JVIAC9MEv7bdpvDfI/WXfh6r8f7NAmvldY8URJ6joFxEwBQuICBeFNA/Gem9t6inuoXFuy8f67DbjnfW%2BcHg91uIWCS8UMKJV7ApVK6V7IxwhsSXYCBMAEGiKgIQX4fzfjHHafY0857Q3gQQRBT9jYeVoM1TKlCiTUNofQxh35fysLHBw2evCwb8I2H1deEczCSA/h2L%2BrMJ62WVjQuhxAGHvGQeOVeQV256zzpHSQODxJxSkoQy0sk2EKUkC6DKXF8aE3ft7PG2FQKlSsjZR2uIiRsiDrVSwDx/GBPLPWXYwTk6VWcuqBQABaZMkS%2BZSHsiIxJhUFCA2YX%2BSW9cXaPj4cFPJSMxbrAlnJApOkGkARgrXFaZj2EwTciNVmFxH78L2gfCqwTlolJkewiwsDoYKJxvsYRtTUDrHLk06yE5WkO2Vh07uXS7aoDeAFfpODQIYOsfU9BHdMFLK1mvLx2FvYaO9kJG8vpMB0GLr6WgeVhmQy4T6P0QM4Y3WWoo6pJtsLQ12JKYSUQpaIm%2BazQ%2BuwiQIDwE%2BCiVo0AsESFKPAa0gThMZi8v0YAOBWmuModcNTWZi0kA0th/yvkgBZpzS8qsvimO7s/faqBJArMZR2Fl/9wIdkAcA0B4DIHQLrplUCsCBX70RVZQ6J8z51EvrPG%2BeB76P2bvPeYqDtk3MGWBGlvL36NIVUKy85TG6VL1Ua45ljO75zpRcqxBs%2BXupdeauxvUGD4C8rgripxIHECqhoGKZ0ODzC%2BRwWIvA/AcC0KQVAnBebWCtksFYfZ6m8BlsmmN8wADWIBJAADYSRmHiABAAHABctkgACc5bYjrEkOsfQnBJCJs0LwNNHBeC6QSAWrQ8w4CwBgIgTs2K6DRHIJQLFiQ50xG2GqLg8QEg0FoIY3SEAIh9tIBEYI9QMScB4KQLFbBBBlgYLQM9hbSBYElEYcQj78CnAcHgT4ulH2YFUF8VEqwU1nkqIe9EERVTEAxB4LAh6OqK0LfMKgBhgAKBrIVMsiRGDnt4PwQQIgxDsCkDIQQ5M1CHt0K0AwRgUDB30Hi3SsBmBsBAP8t5tBSCfBiFyLwDBi3jpY%2BwHKPDuPntrWO0gEAhMgGVFx/oHBa0CxjVJmT5lVadrE8mpaY75ioESNUX96S2xyVMFEkOQ7KiRXSC4ANowWikECGuaY/RWi5DSAIezOQUgeYYFMPoMRxhWa/QIToIxrZjDaNZ0LwxujOYC3oCY4WsgOaS3FooCWkEKEWMsYjpAuSYFWDwWNnAE2kCTSmgdqha3lvSY2x8NGwobpJBoFruwIAZssA%2BXAhATIbDnh4Wd9BiC5sLvmvtmdSA0KYFgGIGrS2SFiCSDtGgALxGrVwdYGgm0C2kHGntpBFaSEkMt47Z3ztndiOVw9A6h0gBHRNqTU6IBIC7IkVEC6IBLpXaEVgqxqu1fq2upyzXWspswPgKKeB2ytHw8IUQ4gN2kfkEoCjj6qOkCJKqRIuGSvxt7Y%2BgdZZUTve7KgA0AO6saOB01pbYP2uDeXcN0bsxxtIfmNN2blA8cHcVrEJb/PBdC6F%2BW67hPOB3Ye%2Bz0gpa20kgAs2oF6x4jttbQBLtHB1gE8qxL/LE28dmG1/23Xo6i3yeIKkZwkggA%3D%3D%3D"></iframe>

---

Manip: Concat/Join EX

* For other languages
  * Do concat by `join` with empty sep
    * Languages with `join`, for e.g. Python
  * Do join by reduce + transform
    * No feature to do `join` directly OR FP with freq used `fold`

``` Python
def concatenate(lst):
    return "".join(lst)
```
---

Manip: Substring

``` C++
// C: Get substring by strncpy
char const* cs = "Hello World";
char o[4096];
strncpy(o, cs+2, 3);

// C++：Get substring by substr
std::string s {"Hello World"};
std::string o = s.substr(2, 3);
```

* *Substring* is usually used with *find* (discussed later)
* A very important operation for parsing/deserialization/etc.
* **Unavoidable** *copy* may cause performance issue

---

<style scoped>
    h1 {
        position: relative;
        top: 35%;
        text-align: center;
        font-size: 36pt;
    }

    ul {
        position: relative;
        top: 30%;
        text-align: center;
        font-size: 50pt;
        list-style-type: none;
    }

    p {
      position: relative;
      top: 35%;
      left: 42%;
    }
</style>

# "Anxiety about performance & Imprefect C++"

---

* No cheap substr b/c: A part of the str is not a str (C)
  No cheap substr b/c: `std::string` is always a value (C++)
* String on the stack cannot be a value object (C);
  A string value object cannot be placed on stack (C++);
* The copy/ref of a string is cheap but unreliable (C);
  The copy/ref of a string is reliable but expensive (C++);
* The headstrong Standard Commitee never gave you what you want ... until C++ 20/23
  * `contains` (C++23), `join` (C++20, via `ranges`), ...

---

* You don't know the best choice until you see everything.
  * `appFn(PCSTR)`->`bizFn(string_view)`-> ... ->`libFn(PCSTR)`
  * `appFn(SCREF)`->`bizFn(FancyString)`-> ... ->`libFn(SCREF)`
  * `appFn(?????)`->`bizFn(...........)`-> ... ->`libFn(!!!!!)`
* ***Cost = convert(in, im) + process(im) + convert(im, out)***
---

## Solutions from community - 1
* View of string
  * Not null-terminated; O(1) for `length()`
  * `std::string_view` (C++17)
  * `abseil::string_view` / `boost::string_view` (C++11)
  * `folly::StringPiece` / `llvm::StringRef` (C++11)

---

## Solutions from community - 2

* Reference counter based strings
  * `FBString` in *folly* 
  * `CStringT` in MFC (>= 4.0)
  * `String` in *"Inside the Object Model: The Sensible Use of C++"*, 11.4
    * 不是多数人看过的 Lippman's 《深入探索C++对象模型》
* *RC + Copy-On-Write* for String as a read-only copy
  * Not friendly for concurrent programming

---

## Solutions from community - 3

``` C++
// An illustration of the idea of "Small String"
template <typename CharT> class basic_string {
  union {
    CharT* __long;
    CharT __short[sizeof(CharT*)/sizeof(CharT)];
  };
  size_t __length;
};

```

* Small strings on stack
  * `llvm::SmallVector<char>`
  * `std::string` in modern C++, Microsoft's STL, libcxx, etc.

---

A quiz of string

``` C++
auto ps0 = s0.c_str();
string s1 = std::move(s0);

// Does the following assertion always pass?
assert(ps0 == s1.c_str());
```

* DON'T promise anything other than the interface
* DON'T make any assumptions about the implementation

---

## Solutions from community - 4

* For specific scenario
  * `FixedString`
  * `llvm::CachedHashString`

---
<style scoped>
    h2 {
        position: relative;
        top: 35%;
        text-align: center;
    }
</style>
## Guidelines

---
* No optimal solution except full-stack one
* (Perhaps, I believe) Better Practices
  * Use `string_view` rather than `string const&` or `const char*`
    * Of course, full stack solution is better (C++17 or 3rd libs)
  * Decide **carefully** whether to store a *view* or store a *copy*
  * Don't focus on optimization of string use in dev stage
    * 世上无难事，只要肯~~放弃~~trade-off
    * *CppCoreGuidelines*, Per.2 *Don't optmize prematurely*

---

Manip: Split 1

``` C++
// C: strtok
char input[] = "one + two * (three - four)!";
const char* delimiters = "! +- (*)";
char *token = std::strtok(input, delimiters);
while (token) {
  std::cout << std::quoted(token) << ' ';
  token = std::strtok(nullptr, delimiters);
}

```

* `strtok` is stateful function (not reenterable)

---

Manip: Split 2

``` C++
// C: strpbrk / strspn
unsigned int cnt = 0;
do {
    char* next = std::strpbrk(str, sep); // find next separator
    // copy or print [str, next)
    str = next;
    if(str) str += std::strspn(str, sep); // skip separator
    ++cnt; // increment word count
} while(str && *str);

```

---

Manip: Split 3, STL and `stream` (C++11)

<iframe width="800px" height="200px" src="https://godbolt.org/e#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:26,endLineNumber:15,positionColumn:26,positionLineNumber:15,selectionStartColumn:26,selectionStartLineNumber:15,startColumn:26,startLineNumber:15),source:'%23include+%3Ciostream%3E%0A%0A%23include+%3Cvector%3E%0A%23include+%3Cstring%3E%0A%23include+%3Csstream%3E%0A%0A%23define+OUT_VAR(v)+std::cout+%3C%3C+%23v+%3C%3C+%22+%3D+%22+%3C%3C+v+%3C%3C+std::endl%3B%0A%23define+ILLEGAL(v)+%3B%0A%0Aint+main(int+argc,+char*+argv%5B%5D)+%7B%0A++std::cout+%3C%3C+%22C%2B%2B%22+%3C%3C+(__cplusplus+/+100+%25+100)+%3C%3C+%22+is+available.%22+%3C%3C+std::endl%3B%0A%0A++//+Split+by+find/find_if/find_first_of/search+%26+loop%0A++//%0A++//+...+TRY+TO+DO+IT+...%0A++//%0A%0A++//+Split+by+stream+%2B+%3E%3E%0A++std::stringstream+ssInput1(%22hello+world%22)%3B%0A++for(std::string+tok%3B+!!ssInput1.eof()%3B)+%7B++%0A++++ssInput1+%3E%3E+tok%3B%0A++++std::cout+%3C%3C+tok+%3C%3C+std::endl%3B%0A++%7D%0A%0A++//+Split+by+stream+%2B+getline%0A++std::istringstream+ssInput2(%22a-b-c-d%22)%3B%0A++for+(std::string+line%3B+std::getline(ssInput2,+line,+!'-!')%3B+)+%7B%0A++++std::cout+%3C%3C+line+%3C%3C+!'%5Cn!'%3B%0A++%7D%0A++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:46.25078764965343,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:clang1500,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'0',intel:'0',libraryCode:'0',trim:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!((name:abseil,ver:trunk),(name:boost,ver:'180'),(name:fmt,ver:'811'),(name:rangesv3,ver:'0120')),options:'-std%3Dc%2B%2B11',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1,tree:'1'),l:'5',n:'0',o:'x86-64+clang+15.0.0+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),header:(),k:45.430750251309114,l:'4',m:44.44444444444445,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+12.2',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+15.0.0+(Compiler+%231)',t:'0')),header:(),l:'4',m:55.55555555555556,n:'0',o:'',s:0,t:'0')),k:53.74921235034657,l:'3',n:'0',o:'',t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

---
Manip: Split 4, by `tokenizer` and `split` from boost (C++11)

<iframe width="800px" height="200px" src="https://godbolt.org/e#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGIAMwArKSuADJ4DJgAcj4ARpjEEmakAA6oCoRODB7evgHBaRmOAuGRMSzxiVzJdpgOWUIETMQEOT5%2BQbaY9sUMjc0EpdFxCUm2TS1teZ0KE4MRwxWj1QCUtqhexMjsHOb%2BEcjeWADUJv5uTrPEmKxn2CYaAIIPz2b7DIdeJ2duAG51RGIdxeewOR0wp3OVwiwGBT1BH3BkLcsVQ6QIAHoxMASIQECwMdCjAA6BApFJw17vT7fc6o9EYryOWiEACehIIxBhAH1fnhMAB3UnkykImkQn702YYogAa0YeAAXglhRT/Pd4fC3lgaJFjgB5OQAFW5ADVHgAlCC/FbHWboEAoDYEZE/U5vX6u87usyQgAiPq9bmOnp%2BbvtjtctDOVi1/h1C2OAElQqFsABxR6ha22mMgp4RF0sJgRCCF47NYDIUjHZAIZoAKgrxGAvxMgSsgT9uYA7LHHsc7QQHU6mUGfW5rFOzL6w96INzucgUt4FCuvApjhjjlwNBp3YEd3vc%2Bc3eZfXhN0xfiWDLF6MTz%2BOIyAo3mnoOXoONzChyOibC/j9t%2BGRGH%2Bjr/A4JDvs8H5btuxKITuxyIcSxyxKyxwWGiszEkaqDygwSoJF%2BQ5cmBm5nAG55Gggl7HJeNYVscBCYLM54wYOUoEI6coKsqQJ0jhPEoPWxDckoKTNEwgI/HWzTAuqLEEYwCgQAoNbcY68niZJ0myecOl3BA541ueKwrJxxz8MQxwQEwTKoLWAjsWYABsymyiAnmqb2wGDuBo4unOwZyuOHE9pOM5WSYPZ%2BqRL5oGOIWBW%2BQGkaRGIIUhvqoZl25cZh2HosSjy0DiXIEPixKNORwA1SuhDuhYgUAflAVCF45IkC6yjXPgoisQlnK/lcXAmTOdEVrEAC0DYzY8FhzTNTAtRhmDmVZkEGW4bVKQokFWWuLIEOpkE1mNmnCY6l7coYrLcqgVATWYc3mVd6K8SpDBLqgLApNcCgKI9DCWUB8GnH2PpTUwsSmeei3wzOq3oayG0zpD8VwTZ9mOc5DCuR5creQddR%2BYFSXBae3phSlEVRWYMaY8NI6U8%2Bw6Rgw6DRulcEgm8eBUMci7Lqu66UeqVHHGYGgy7LoTtahxz%2BChiHtYVWHCaV5W4lVLA1SNJJCI1wWWHZUVWJYMu2gBvL8gK6vHJ13UtMcfWYANMkbXBL623ygp2r8XCxRYCixfFvMDiGALQVCHMgH79t3IHh2R4Ox2EGd52B1wH2zDdwP3Y9z1Pu96HXSAfE/Wg/2A8DAhg/5OMOUQ%2BOE55JO/JB5OJc64609Twb0xx4Phyz2l9ylL5pf2eyuILGVwdcBCbAwxwaDB4ccGstCcIEvB%2BBwWikKgnAW9YdobFsEpvDwpA8Uf29rF5kgAJzEj2kj%2BGYXD%2BG/r9uX8P4Lgbl9CcEkAfTQvBT4cF4AoEAGh75QLWHAWAMBEBOn%2BnQBI5BKA1xSNgxIhxDDAC4IEPcfA6CsWIPAiAsQoGkFiBEZorJOB3xrmwQQ%2BoGC0FYY/UgWBixGHEPw/A1x6j/HgfwzAqg6hMh2MfQs3QGEsliMQFhHgsAMJGiwZBfADDAAUKae2%2BoUiMDYbwfgggRBiHYFIGQghFAqHUPw3QucDBGBQNOGwqj4GwGYGwEAsMlB0FIP8RInIvAMFlCggJ7BuJhNGNwAAHFoWJrB2BUBYAQRJiQODJK4DwdJgT1FGDYr8fwuTNDVDSWsVAKRehSJmvaKiphLDWEKXA7oAIsguC5lMPwucwgLHKJUPQhRMgCAGeM9IkyGBDFGcsLoPQGhzGmbnWo9QBD9BaAskYVRxgDHWYc3ZIz9kSDWAoK%2B2w9CckwDsIpYCOD71IIfY%2BMDVDJLcjNNykhaweOADuQIxINAgvNt4msuBCAkHdMAmsHgsH0FsnsLgKxeAP1qaQBANwsCJAgM/EAkhgVfw0D2chZhAi/w0K/Qp0hd4cAgaQXRkhJDEhZeyjlHLghvOgZwOBCCkGPxQegiASBnQpCZLgiA%2BDCFRAyZwT53zfnHCrMgHcZhiRmF4B7aFXIHS5yscIUQ4h7GGqcWoBhbjSACnUSkCxO896QP4TA/UTIJUuiescRVPy/nELAmQkFYKIAIoIUi2FqL0XILWNipguLKAOoZbwXRgRgUprTem9NoCeUnz5bYAVGKn6kC8uQzVgQ3I9i%2BeQjQ/g3JuWSXSzg/gnXvNzQWiyTytWvIYTAyNQq1jhIyM4SQQA%3D%3D%3D"></iframe>

---

Manip: Split 5, by `absl::StrSplit` (C++14)

<iframe width="800px" height="200px" src="https://godbolt.org/e#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:36,endLineNumber:6,positionColumn:36,positionLineNumber:6,selectionStartColumn:36,selectionStartLineNumber:6,startColumn:36,startLineNumber:6),source:'%23include+%3Ciostream%3E%0A%0A%23include+%3Cvector%3E%0A%23include+%3Cstring%3E%0A%23include+%3Cabsl/strings/string_view.h%3E%0A%23include+%3Cabsl/strings/str_split.h%3E%0A%0A%23define+OUT_VAR(v)+std::cout+%3C%3C+%23v+%3C%3C+%22+%3D+%22+%3C%3C+v+%3C%3C+std::endl%3B%0A%23define+ILLEGAL(v)+std::cout+%3C%3C+%23v+%3C%3C+%22+is+ILLEGAL%22+%3C%3C+std::endl%3B%0A%0Aint+main(int+argc,+char*+argv%5B%5D)+%7B%0A++std::cout+%3C%3C+%22C%2B%2B%22+%3C%3C+(__cplusplus+/+100+%25+100)+%3C%3C+%22+is+available.%22+%3C%3C+std::endl%3B%0A%0A++using+std::vector%3B%0A++using+std::string%3B%0A%0A++absl::string_view+words%7B%22Hello-_*C%2B%2B*_-20-_*!!%22%7D%3B%0A++absl::string_view+delim%7B%22-_%22%7D%3B%0A++auto+parts1+%3D+absl::StrSplit(words,+delim)%3B%0A++for(auto+const%26+p:+parts1)+%7B+OUT_VAR(p)%3B+%7D%0A++auto+parts2+%3D+absl::StrSplit(%0A++++words,+absl::ByAnyChar(delim),+absl::SkipEmpty()%0A++++)%3B%0A++for(auto+const%26+p:+parts2)+%7B+OUT_VAR(p)%3B+%7D%0A%0A++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:49.74321349963316,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:clang1500,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'0',intel:'0',libraryCode:'0',trim:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!((name:abseil,ver:trunk),(name:boost,ver:'180'),(name:fmt,ver:'811'),(name:rangesv3,ver:'0120')),options:'-std%3Dc%2B%2B14',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1,tree:'1'),l:'5',n:'0',o:'x86-64+clang+15.0.0+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),header:(),k:45.430750251309114,l:'4',m:44.44444444444445,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+12.2',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+15.0.0+(Compiler+%231)',t:'0')),header:(),l:'4',m:55.55555555555556,n:'0',o:'',s:0,t:'0')),k:50.25678650036684,l:'3',n:'0',o:'',t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

---
Manip: Split 6, by STL `ranges` ~~好看不好用~~ （C++20）

<iframe width="800px" height="200px" src="https://godbolt.org/e#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:3,endLineNumber:26,positionColumn:3,positionLineNumber:26,selectionStartColumn:3,selectionStartLineNumber:26,startColumn:3,startLineNumber:26),source:'%23include+%3Ciostream%3E%0A%0A%23include+%3Cvector%3E%0A%23include+%3Cstring%3E%0A%23include+%3Cstring_view%3E%0A%23include+%3Cranges%3E%0A//+%23include+%3Crange/v3/all.hpp%3E%0A%0A%23define+OUT_VAR(v)+std::cout+%3C%3C+%23v+%3C%3C+%22+%3D+%22+%3C%3C+v+%3C%3C+std::endl%3B%0A%23define+ILLEGAL(v)+std::cout+%3C%3C+%23v+%3C%3C+%22+is+ILLEGAL%22+%3C%3C+std::endl%3B%0A%0Aint+main(int+argc,+char*+argv%5B%5D)+%7B%0A++std::cout+%3C%3C+%22C%2B%2B%22+%3C%3C+(__cplusplus+/+100+%25+100)+%3C%3C+%22+is+available.%22+%3C%3C+std::endl%3B%0A%0A++using+std::vector%3B%0A++using+std::string%3B%0A++using+std::string_view%3B%0A++using+std::ranges::views::split%3B%0A++using+std::ranges::views::join%3B%0A%0A++//+C%2B%2B20+required%0A++//+%5BX%5D+Clang+15.0%0A++//+%5BO%5D+GCC+12.2%0A++//+%5BO%5D+MSVC+19.32%0A++string_view+words%7B%22Hello-_-C%2B%2B-_-20-_-!!%22%7D%3B%0A++string_view+wor_ds%5B%5D+%3D+%7B%22Hello-_-C%22,+%22%2B%2B-_-20-_-!!%22%7D%3B%0A++string_view+delim%7B%22-_-%22%7D%3B%0A++for+(const+auto+w+:+words+%7C+split(delim))+%7B%0A++++OUT_VAR((%0A++++++string_view(w.begin(),+w.end())%0A++++++))%3B%0A++%7D%0A++for(+const+auto+w:+wor_ds+%7C+join+%7C+split(delim)+)+%7B%0A++++OUT_VAR((+%0A++++++string(w.begin(),+w.end())+%0A++++))%3B+//%3C+Why+not+string_view%3F%0A++%7D%0A%0A++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:49.74321349963316,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:g122,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'0',intel:'0',libraryCode:'0',trim:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!((name:abseil,ver:trunk),(name:boost,ver:'180'),(name:fmt,ver:'811'),(name:rangesv3,ver:'0120')),options:'-std%3Dc%2B%2B20',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1,tree:'1'),l:'5',n:'0',o:'x86-64+gcc+12.2+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),header:(),k:45.430750251309114,l:'4',m:44.44444444444445,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+12.2',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+gcc+12.2+(Compiler+%231)',t:'0')),header:(),l:'4',m:55.55555555555556,n:'0',o:'',s:0,t:'0')),k:50.25678650036684,l:'3',n:'0',o:'',t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

---

Manip: Split 7, by `absl::string_view` + *range-v3* (C++14)

<iframe width="800px" height="200px" src="https://godbolt.org/e#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:23,endLineNumber:7,positionColumn:23,positionLineNumber:7,selectionStartColumn:23,selectionStartLineNumber:7,startColumn:23,startLineNumber:7),source:'%23include+%3Ciostream%3E%0A%0A%23include+%3Cvector%3E%0A%23include+%3Cstring%3E%0A%0A%23if+__cplusplus+%3E%3D+201703L%0A%23include+%3Cstring_view%3E%0Ausing+std::string_view%3B%0A%23else%0A%23include+%3Cabsl/strings/string_view.h%3E%0Ausing+absl::string_view%3B%0A%23endif%0A%0A%23define+USE_STD_RANGE+0%0A%23if+USE_STD_RANGE+%3D%3D+1%0A%23include+%3Cranges%3E%0Ausing+std::ranges::views::split%3B%0Ausing+std::ranges::views::join%3B%0A%23else%0A%23include+%3Crange/v3/all.hpp%3E%0Ausing+ranges::views::split%3B%0Ausing+ranges::views::join%3B%0Ausing+ranges::to%3B%0A%23endif%0A%0A%23define+OUT_VAR(v)+std::cout+%3C%3C+%23v+%3C%3C+%22+%3D+%22+%3C%3C+v+%3C%3C+std::endl%3B%0A%23define+ILLEGAL(v)+std::cout+%3C%3C+%23v+%3C%3C+%22+is+ILLEGAL%22+%3C%3C+std::endl%3B%0A%0Aint+main(int+argc,+char*+argv%5B%5D)+%7B%0A++std::cout+%3C%3C+%22C%2B%2B%22+%3C%3C+(__cplusplus+/+100+%25+100)+%3C%3C+%22+is+available.%22+%3C%3C+std::endl%3B%0A%0A++using+std::vector%3B%0A++using+std::string%3B%0A%0A++//+C%2B%2B20+required%0A++//+%5BX%5D+Clang+15.0%0A++//+%5BO%5D+GCC+12.2%0A++//+%5BO%5D+MSVC+19.32%0A++string_view+words%7B%22Hello-_-C%2B%2B-_-20-_-!!%22%7D%3B%0A++string_view+wor_ds%5B%5D+%3D+%7B%22Hello-_-C%22,+%22%2B%2B-_-20-_-!!%22%7D%3B%0A++string_view+delim%7B%22-_-%22%7D%3B%0A++for+(const+auto+w+:+words+%7C+split(delim))+%7B%0A++++OUT_VAR(to%3Cstring%3E(w))%3B%0A++%7D%0A++for(+const+auto+w:+wor_ds+%7C+join+%7C+split(delim)+)+%7B%0A++++OUT_VAR(to%3Cstring%3E(w))%3B+//%3C+Why+not+string_view%3F%0A++%7D%0A%0A++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:49.74321349963316,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:clang1500,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'0',intel:'0',libraryCode:'0',trim:'0'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!((name:abseil,ver:trunk),(name:boost,ver:'180'),(name:fmt,ver:'811'),(name:rangesv3,ver:'0120')),options:'-std%3Dc%2B%2B14',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1,tree:'1'),l:'5',n:'0',o:'x86-64+clang+15.0.0+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),header:(),k:45.430750251309114,l:'4',m:44.44444444444445,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+12.2',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+clang+15.0.0+(Compiler+%231)',t:'0')),header:(),l:'4',m:55.55555555555556,n:'0',o:'',s:0,t:'0')),k:50.25678650036684,l:'3',n:'0',o:'',t:'0')),l:'2',n:'0',o:'',t:'0')),version:4"></iframe>

---

Manip: Split - Don't forgot *RegEx* tokenizer or *Lexer*

``` C++
// ... sregex_token_iterator / regex_match ...
// ... Or Lexical/Grammar Analyzer
// ... Boost.Spirit.Qi / Boost.Spirit.Lexer / Flex + Bison / ANTLR ...
```

---

Manip: Split X

<style scoped>
  tr {
    font-size: 0.7em;
  }
</style>

* Best practice
  * C++11
    * `stream`
    * `boost::tokenizer`
    * `boost::split`
  * C++14 and later
    * `absl::StrSplit`
      * Flexible Splitting
      * "Cheap" partition representation, i.e. copy avoided.
  * For Tomorrow
    * *range-v3* or `std::ranges`

---

Manip: Trim

<iframe width="800px" height="200px" src="https://godbolt.org/e#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxCCSGqQADqgKhE4MHt6%2BekkpjgJBIeEsUTFxtpj2eQxCBEzEBBk%2BflzllWk1dQQFYZHRsfEKtfWNWS2Dnd1FJf0AlLaoXsTI7BzmAMzByN5YANQma25Og8SYrPvYJhoAgpc3ZhsMW167%2B24AbpgOJOe365vbmD2B2OwWAP2uf0eAKBbgiqGSBAA9GJgCRCAgWIiQUYAHQIBIJcF3B5PF4HJgRBS0LEEYighTIhTIPB4PFE373PBUHYAfR5yAS3gUgq8CiBFzWABEdmYNFwAOwaNYBDkk6GvbHAHlvPCYADuRNFoJ2g3QIBAmu1uoNaysEPuFSUqv%2Bz0BrwpVJpdKMDMtOv1bLWF2uRqMOw9tHNfut%2BztxNcXN%2B9rWWBoIR2AHk5AAVHkANSuACUIG8ZiaCGaUAsCDDXnt7m9awd62YgdLzK3XnXG13m6bza5aLHVangoCAJIBALYADiVwCJbL/arXhrvbc9bWPYOdY7OzwYsn07nKrMnZ3fYrA4Y6CHtqTV2CNZYTGCECf4eIwGQpB2yAQdQAFSfsAbwmAArFY4GSmWJjynGOzlpWaCrk2G4dm41hYWeaE7BAfICkKIpioiOxcBoGj1uBZEUbBF7oThB7hm8r4GBE9A4nu65Idet7DhC1yIaGwA8SAHxfMQ/FXEJKRhsumpSbciGIqRG6GOgOzMI4Hw7JhljWPuLCCpgbCCEwVRinUgKoCwhAEJg6BKTsKk7Mo9BMEoJqYICBAIICeBGfQpm1FUOyoNyEbmjUdIJIBVxMiyADqCCEJgwpMMsnECdJzmkdmdIsOW3oiREACeOxwgiOIomivksE5momlwbYtohOwABIVLQqA7IlJC0BpiEdlJiGNQo57SpVgzmjVdJ1eatIBTydLAAgBD8qgCSlRAChcDMsa5TsgEbVtOxUKwdC6goTlZrmBbFrt%2B22pmOb5kWO1mE9CEVfC00gLN6IsAtBU8vQVAEB9X0vXd73jV9TlTQQM20Kic0YsDAU7XtB23W9D3Y/egl7MmXK8vyxHEeK%2BzSrKsoaGYKpEy5%2BUBUVoJWvqFXlYj1Uo7VGINVeFqLUYHN6iaYHwXufm0N1Ox6v1GnDfKkojdDeMQD9CLI6jgMYywJ3bQopY7PDyYJlQD7KXlBVc%2BGlJDkTkXC8VYsS2YEB7ohMtywrxADW1HZmzluP3ZrzvRXgCQBKc%2BBGPFzJ4MlqXpcsO1vJ9ptq6H7321SUWLQk%2BWsaCCdJSl9mp5g6eZ8HiE58W7tZ4TOUnAQiwMDsGiKSrHBzJGHDgbwfgcFopCoJwelWJYJoLEsbr3DwpBI6PfdzAA1rEACcOLypIaxmFwaySFvW8AGxrGsXBn/onCSMPmi8BPHC8AoIDxCvWhzHAsAwIgVZGToNEcglA0CAPoDELYhhgBcHAhRPgdB7LEDfhACIj9SARGCHUUqnAl5gOChmBgtAcGr1IFgF8RhxCkPwCcBweAPhv1IZgVQnxVwrDHk%2BCo6DaB4AiMQbBHgsDoMWkDVecwqAGGAAoPM1oMwJEYLg3g/BBAiDEOwKQMhBCKBUOoUhugWgGCMCgbCNgeGUngBAZgbB/qUkwHQUgHwYi0i8Awde38rHsERg4vo3AAAcX9SCWNYOwKgLACDeJiBwXxXAeDuOCSAfhRg0pvDWBEzQXBZRr3HgkCynAAC0ppqamH0pYKQr8KifCqC4G8IxmikECGOKYfQWg5FSAIWp2RkhtIYJMXoMQxgVLoQIDowxPBND0HYSp7QhhdEaX0iZMyOljBmb04ozS5gKDnssPQtJvKKP7pwIepAR5j2fqoXxZ88ln0kH%2BQxIlYE4g0I8vCU9rC/lwIQEgm4Wi6RsgkIBxBvkzF4J/LJfkmBYBiBADesRwI4n3hoeUcCzDgSPhoLeMTpAD3vqQIGkhJDwvxUS4lRLwLHPQc/V%2B79l6P2/n/CASBqwJFXCAiAYD/kQNCPEjg5zLnXJ2N%2BZAZEzA4jMLwByny6RmhaMo4QohxAaNldotQ6D9GkD1PwhI%2Bzb6DwfqQ5%2BGZVzMprOFHYvKrk3KgWGB5TzKIQA8OA6IQKQW0rmOCyFlADkcBxUDcCcK/UBsDYGm%2BJyn6cCpR/V1pBN5wNFeBM%2B8oLlwKVGfM%2BvisWcDWHq054aaViK9WK8l%2Brc2gpmHMRxKRnCSCAA"></iframe>

---
<style scoped>
  ul {
    font-size: 0.8em;
  }
</style>
## Manipulation: Take away

* Use text processing toolbox/scripts for ad-hoc work - and archive it.
* Avoid C string
  * Verbose, dangerous and perf is not better than C++ strings
* Use standard or third-party `string_view`
  * `string_view` + ... + `string_view` =\\=> `string_view`
  * `string_view` ===> `string_view` + ... + `string_view`
* Use 3rd party string libs (boost/folly/abseil/range-v3) to simplify your code
  * C++11: stream + fmt (join, concate, string_view) + boost (split, trim, string_view)
  * C++14/17/20: *abseil* or *range-v3* (for join, concate, split, string_view)
  * C++14之前并没有好用且兼容性高的`string_view`。性能不保证，图个方便。
* Use the latest C++ standard if possible
