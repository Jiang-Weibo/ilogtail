#include "BashParser.h"
#include "iostream"
int main() {
    
    std::string str("[ $student == Alice -a $$$value == fre&&shman ]");
    //[ $student == Alice -a $value == freshman ]
    std::string tmp;
    std::getline(std::cin, tmp);
    if (tmp.size()) str = tmp;
    parser_test::BashParser parse;
    parse.GetParsedJson(str);
    auto res = parse.GetTokens();
    for (auto & p : res) {
        std::cout << p.token << "  ";
        std::cout << p.type << std::endl;
    }
    std::cout << str << std::endl;

    

    /*
    Json::Value jsonObject;
     jsonObject["name"] = "Alice";
     jsonObject["age"] = 25;
     jsonObject["isStudent"] = true;

     Json::Value jsonArray;
     jsonArray.append("apple");
     jsonArray.append("banana");
     jsonArray.append("orange");

     Json::Value complexJson;
     complexJson["person"]["name"] = "Bob";
     complexJson["person"]["age"] = 30;
     complexJson["fruits"].append("apple");
     complexJson["fruits"].append("banana");

     Json::StreamWriterBuilder writer;
     std::string jsonString = Json::writeString(writer, complexJson);

     std::cout << jsonString << std::endl;

    */

    /*
    output:
    {
        "fruits" :
        [
                "apple",
                "banana"
        ],
        "person" :
        {
                "age" : 30,
                "name" : "Bob"
        }
    }
    */
}
