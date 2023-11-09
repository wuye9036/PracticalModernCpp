#include "CppVersion.h"
#include <gtest/gtest.h>

#include <string>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <boost/utility/string_view.hpp>

TEST(PMCPP_TextProcessing_Cpp11, String_Manip_Concatenate_Strings)
{
    // Homo-type instances concatenate
    std::string a1{"Alpha"}, b1{"Beta"}, c1{"Gammar"};

    // ====== BEST PRACTICE ==============
    auto o1 = a1 + b1 + c1;
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
    // boost::string_view
    std::string o1 = (std::stringstream{} << a << b1 << c).str();
    // fmt::string_view
    fmt::string_view sv[] = {a, b2, c};
    auto o2 = fmt::to_string(fmt::join(sv, ""));


    EXPECT_EQ(o1, o2);
}