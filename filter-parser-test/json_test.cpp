#include "BashParser.h"
#include "iostream"

int main() {
    Json::Value v;
    v["some"];
    v["some"].append("abcdefg");
    v["operands"].append("qop");
    Json::StreamWriterBuilder builder;
    std::string str = Json::writeString(builder, v);
    std::cout << str << std::endl;
}