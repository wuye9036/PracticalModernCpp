#include "CppVersion.h"

#include <gtest/gtest.h>

#include <array>
#include <string>
#include <string_view>
#include <iostream>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <range/v3/range.hpp>
#include <range/v3/view.hpp>

#include <absl/strings/string_view.h>
#include <absl/strings/str_cat.h>
#include <absl/strings/str_join.h>

#define OUT_VAR(v) std::cout << #v << " = " << v << std::endl;
#define ILLEGAL(v) std::cout << #v << " is ILLEGAL" << std::endl;

TEST(PMCPP_TextProcessing_Cpp23, String_Manip_Concatenate_Strings)
{
    // Homo-type instances concatenate
    std::string a1{"Alpha"}, b1{"Beta"}, c1{"Gammar"};

    // ====== BEST PRACTICE ==============
    auto o1 = a1 + b1 + c1;

    // ====== OTHER  OPTS ==============
    {
        // ...... 1 ...... via library {fmt}
        auto o2 = fmt::to_string(fmt::join({a1, b1, c1}, ""));
        EXPECT_EQ(o1, o2);

        // ...... 2 ...... via library {fmt}
    }
}

TEST(PMCPP_TextProcessing_Cpp23, String_Manip_Concatenate_StringViews)
{
    using ranges::views::join;
    using ranges::to;

    // Homo-type instances concatenate
    std::string_view a1{"Alpha"}, b1{"Beta"}, c1{"Gammar"};

    // ====== BEST PRACTICE ==============
    // The range-v3 lib is better than std::ranges on clang 14.0.x with C++23.
    std::string_view sv[] = {a1, b1, c1};
    auto o1 = sv | join | to<std:: string>();
}

TEST(PMCPP_TextProcessing_Cpp23, String_Manip_Concatenate_Hetero){
    // Hetero-type instances concatenate
    const char* a{"Alpha"};
    std::string_view b{"Beta"};
    std::string c{"Gammar"};

    // ====== BEST PRACTICE ==============
    std::string o1 = (std::stringstream{} << a << b << c).str();

    // ====== OTHER  OPTS ==============
    {
        // ...... 1 ...... Native C++11 string code
        // No "string + string_view", they may be reserved for "cheap" concat
        std::string o2{a};
        o2 += b;
        o2 += c;  
    }
}

TEST(PMCPP_TextProcessing_Cpp23, String_Manip_AbslStrCatJoin){
  using std::vector;
  using std::string;
// #if __cplusplus < 201703L
  using absl::string_view; // std::string_view in C++17
// #else
//   using std::string_view;
// #endif

  vector<string> svec{"tx", "12", "bl ah"};
  string a{"tx"}, b{"12"}, c{"bl ah"};
  const char* a1{"tx"}; string_view b1{"12"}; string c1{"bl ah"};

  // ... 1 ... Use absl::StrCat
  ILLEGAL( absl::StrCat(svec) );
  OUT_VAR( (absl::StrCat(a, b, c)) );
  OUT_VAR( (absl::StrCat(a1, b1, c1)) );

  // ... 2 ... Use absl::Join
  OUT_VAR( (absl::StrJoin(svec, "")) );
  OUT_VAR( (absl::StrJoin({a, b, c}, "")) );
  ILLEGAL( (absl::StrJoin({a1, b1, c1}, "")) );
}
