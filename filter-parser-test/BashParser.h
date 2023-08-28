#pragma once
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <memory>
#include <stack>
#include <algorithm>
#include "/opt/logtail/deps/include/json/json.h"

namespace parser_test {
enum TokenType {
    TOKEN_UNKNOWN = 0,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_BINARY_AND,
    TOKEN_BINARY_OR,
    TOKEN_BINARY_and,
    TOKEN_BINARY_or,
    TOKEN_BINARY_EQUAL,
    TOKEN_UNARY_NOT,
    TOKEN_KEY,
    TOKEN_VALUE,
};

struct Token {
    TokenType type;
    std::string token;

public:
    Token(TokenType _type, std::string _token) : type(_type), token(_token) {}
    ~Token() {}
};

class BashParser {
private:
    std::vector<Token> tokens;
    std::string bash_expr;
    Json::Value json_res;
    std::vector<char> seperators{' ', '\n', '\t', '\r'};
    std::unordered_set<char> preserved_id{'[', ']', '=', '&', '|', '!', '$', '\\'};
    size_t tokens_idx;

private:
    bool LexicalAnalysis();
    bool IsSeperator(const char& ch);
    bool Parse();
    bool RecursiveDescent();
    bool CheckExpr();
    bool CheckStat();
    Json::Value MakeJsonElement(std::vector<Token>&, Json::Value&);
    inline int GetPriority(TokenType& type) {
        switch (type) {
            case TOKEN_BINARY_EQUAL:
                return 4;
                break;
            case TOKEN_UNARY_NOT:
                return 3;
                break;
            case TOKEN_BINARY_OR:
                return 2;
                break;
            case TOKEN_BINARY_AND:
                return 1;
                break;
            default:
                return -1;
                break;
        }
    };
    bool MakeJson();
    void Reset();
public:
    BashParser() = default;
    ~BashParser() = default;
    Json::Value GetParsedJson(std::string expr);
    auto GetTokens() -> decltype(tokens) { return tokens; }
};

} // namespace parser_test
