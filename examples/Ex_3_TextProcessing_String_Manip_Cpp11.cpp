
// !!!!!!!!!!!!!!!!!!!!!!!!!!!
// Check version number of C++ standard.
#include <fmt/core.h>
static_assert(__cplusplus == 201103L, "Not C++11");
// !!!!!!!!!!!!!!!!!!!!!!!!!!!

#include <gtest/gtest.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/utility/string_view.hpp>
#include <sstream>
#include <string>

TEST(PMCPP_TextProcessing_Cpp11, String_Manip_Concatenate_Strings)
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
    }
}

TEST(PMCPP_TextProcessing_Cpp11, String_Manip_Concatenate_StringViews)
{
    // Homo-type instances concatenate
    boost::string_view a{"Alpha"}, b{"Beta"}, c{"Gammar"};

    // ====== BEST PRACTICE ==============
    std::string o1 = (std::stringstream{} << a << b << c).str();
}

TEST(PMCPP_TextProcessing_Cpp11, String_Manip_Concatenate_Hetero){
        
    // Hetero-type instances concatenate
    const char* a{"Alpha"};
    boost::string_view b1{"Beta"};
    fmt::string_view b2{"Beta"};
    std::string c{"Gammar"};

    // ====== BEST PRACTICE ==============
    std::string o1 = (std::stringstream{} << a << b1 << c).str();
    // Illegal: std::string o1 = (std::stringstream{} << a << b2 << c).str(); 
    //          Because fmt::string_view doesn't support stream.

    // ====== OTHER  OPTS ==============
    {
        // ...... 1 ...... Native C++11 string code
        // No "string + string_view" because it may be reserved for "cheap" concat
        // "+=", string::append(string_view) are available since C++20.
        std::string o2{a};
        o2.append(b1.cbegin(), b1.cend());
        o2 += c;

        // ...... 2 ...... {fmt}
        fmt::string_view sv[] = {a, b2, c};
        auto o3 = fmt::to_string(fmt::join(sv, ""));

        EXPECT_EQ(o1, o2);
        EXPECT_EQ(o1, o3);
    }
}
