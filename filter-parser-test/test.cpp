#include "BashParser.h"
#include "iostream"
int main() {
    std::string str(R"([ $student == Alice ])");
    std::string tmp;
    std::getline(std::cin, tmp);
    if (tmp.size()) str = tmp;
    parser_test::BashParser parse;
    auto res = parse.GetParsedJson(str);
    std::cout << res << std::endl;
}
