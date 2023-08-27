#include "BashParser.h"
#include "iostream"
int main() {
    std::string str(R"([ $&&student == \&&Alice ] || [ $$$value == fre&&shman ] && [ $$age == 18 || $height == \[167 ])");
    std::string tmp;
    std::getline(std::cin, tmp);
    if (tmp.size()) str = tmp;
    parser_test::BashParser parse;
    parse.GetParsedJson(str);
    auto res = parse.GetTokens();
    for (auto k: res) {
        std::cout << k.token << std::endl;
    }
    std::cout << str << std::endl;
}
