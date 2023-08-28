#include "BashParser.h"
#include <iostream>
#include <fstream>
int main() {
    std::vector<std::string> unit_tests{
        R"([ $student == Alice ])",
        R"([$student == Alice ])",
        R"([ $student == \&Alice&Bob ] && [ ! $age == 19 || $addr == London ])",
        R"([ $student == Bob&Alice && $age == 22 ] || [ $student == Alice && $age == 19 || $addr == London ])",
        R"([ $info == WARNING|ERROR || $suffix == *.log ] && [ $user == .{2,5}sys* ])",
        R"(![ $student == Alice ])",
        R"([ $info == \$post&&$get || $date == 2017\07\06 ] && [ !$LEVEL == WARNING ])",
        R"([ $$usr == sys || $LEVEL == WARNING|ERROR ] && [ $date == */09/* ])",
        R"([ Alice == $student ])",
        R"([ $student == \Alice ])",
        R"([ $student == Alice && [ $age == 22 || $height == 167 ] ])",
        R"([ $student == Alice ] && [ $age == 22 || $height == 167 ])",
        R"([ $and == \&\&id])",
        R"([ $and == \&&id])",
        R"([ $&&student == \&&Alice ] || [ $$$value == fre&&shman ] && [ $$age == 18 || $height == \[167 ])"
    };
    parser_test::BashParser parser;
    for (auto& str : unit_tests) {
        std::cout << "=========================================================================================" << std::endl;
        std::cout << "parsing \"" << str << "\"..." <<std::endl;
        auto res = parser.GetParsedJson(str);
        if (!res.empty())
            std::cout << res << std::endl;
    }
}
