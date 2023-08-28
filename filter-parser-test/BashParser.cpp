#include "BashParser.h"

namespace parser_test {

bool BashParser::LexicalAnalysis() {
    int idx = 0, n = this->bash_expr.size();
    std::string id_key, id_value;
    while (idx < n) {
        switch (bash_expr[idx]) {
            case ' ':
                ++idx;
                break;
            case '\t':
                ++idx;
                break;
            case '\n':
                ++idx;
                break;
            case '\r':
                ++idx;
                break;
            case '\\':
                if (idx + 1 < n) {
                    if (preserved_id.count(bash_expr[idx + 1])) {
                        ++idx;
                        while (idx < n and !IsSeperator(bash_expr[idx])) {
                            id_value += bash_expr[idx];
                            ++idx;
                        }
                        tokens.push_back({TOKEN_VALUE, id_value});
                        id_value.clear();
                    } else {
                        std::cout << "error occurs when matching '\\'. It seems '\\" + bash_expr[idx + 1]
                                + std::string("' is not a valid id");
                        std::cout << std::endl;
                        return false;
                    }
                } else {
                    std::cout << "error occurs when matching '\\'. It seems you don't have enough space for next char";
                    std::cout << std::endl;
                    return false;
                }
                break;
            case '[':
                if (idx + 1 < n) {
                    if (bash_expr[idx + 1] == ' ') {
                        tokens.push_back({TOKEN_LEFT_BRACKET, "["});
                        idx += 2;
                    } else {
                        std::string err("[");
                        err += bash_expr[idx + 1];
                        std::cout << "error occurs when matching '['. Couldn't parse '" << err << "'" << std::endl;
                        return false;
                    }
                } else {
                    std::cout << "error occurs when matching '['. It seems you don't have enough space for next char";
                    std::cout << std::endl;
                    return false;
                }
                break;
            case ']':
                if (idx > 0) {
                    if (bash_expr[idx - 1] == ' ') {
                        tokens.push_back({TOKEN_RIGHT_BRACKET, "]"});
                        ++idx;
                    } else {
                        std::string err("]");
                        err += bash_expr[idx - 1];
                        std::cout << "error occurs when matching ']'. Couldn't parse " << err << "'" << std::endl;
                        return false;
                    }
                } else {
                    std::cout << "error occurs when matching ']'. It seems you don't have enough space for last char";
                    std::cout << std::endl;
                    return false;
                }
                break;
            /*
            case '-':
                if (idx + 1 < n) {
                    if (bash_expr[idx + 1] == 'a') {
                        tokens.push_back({TOKEN_BINARY_and, "-a"});
                        idx += 2;
                    } else if (bash_expr[idx + 1] == 'o') {
                        tokens.push_back({TOKEN_BINARY_or, "-o"});
                        idx += 2;
                    } else {
                        std::string err("-");
                        err += bash_expr[idx + 1];
                        std::cout << "error occurs when matching '-'. Couldn't parse " << err << "'" << std::endl;
                        return false;
                    }
                } else {
                    std::cout << "error occurs when matching '-'. It seems you don't enough space for next char";
                    std::cout << std::endl;
                    return false;
                }
                break;
                */
            case '=':
                if (idx + 1 < n) {
                    if (bash_expr[idx + 1] == '=') {
                        tokens.push_back({TOKEN_BINARY_EQUAL, "=="});
                        idx += 2;
                    } else {
                        tokens.push_back({TOKEN_BINARY_EQUAL, "="});
                        ++idx;
                    }
                } else {
                    std::cout << "error occurs when matching '='. It seems you don't have enough space for next char";
                    std::cout << std::endl;
                    return false;
                }
                break;
            case '|':
                if (idx + 1 < n) {
                    if (bash_expr[idx + 1] == '|') {
                        tokens.push_back({TOKEN_BINARY_OR, "||"});
                        idx += 2;
                    } else {
                        std::string err("|");
                        err = +bash_expr[idx + 1];
                        std::cout << "error occurs when matching '|'. Couldn't parse " << err << "'" << std::endl;
                        return false;
                    }
                } else {
                    std::cout << "error occurs when matching '|'. It seems you don't have enough space for next char";
                    std::cout << std::endl;
                    return false;
                }
                break;
            case '&':
                if (idx + 1 < n) {
                    if (bash_expr[idx + 1] == '&') {
                        tokens.push_back({TOKEN_BINARY_AND, "&&"});
                        idx += 2;
                    } else {
                        std::string err("&");
                        err += bash_expr[idx + 1];
                        std::cout << "error occurs when matching '&'. Couldn't parse " << err << "'" << std::endl;
                        return false;
                    }
                } else {
                    std::cout << "error occurs when matching '&'. It seems you don't have enough space for next char";
                    std::cout << std::endl;
                    return false;
                }
                break;
            case '!':
                tokens.push_back({TOKEN_UNARY_NOT, "!"});
                ++idx;
                break;
            case '$':
                id_key.clear();
                while (idx < n and !IsSeperator(bash_expr[idx])) {
                    id_key += bash_expr[idx];
                    ++idx;
                }
                if (id_key.size() <= 1) {
                    std::cout << "error occurs when matching '$'. It seems you didn't input any character after '$'.";
                    std::cout << std::endl;
                    return false;
                }
                tokens.push_back({TOKEN_KEY, id_key});
                break;
            default:
                // TOKEN_VALUE, 一直往后找，期待一个空格的出现
                while (idx < n and !IsSeperator(bash_expr[idx])) {
                    id_value += bash_expr[idx];
                    ++idx;
                }
                tokens.push_back({TOKEN_VALUE, id_value});
                id_value.clear();
                break;
        };
    }
    return true;
}

bool BashParser::IsSeperator(const char& ch) {
    for (const auto& elem : seperators) {
        if (elem == ch) {
            return true;
        }
    }
    return false;
}

bool BashParser::Parse() {
    std::vector<Token> token_stream(std::move(tokens));
    tokens.clear();
    auto op_stk = std::stack<Token>();
    auto ord_stk = std::stack<Token>();
    int idx = token_stream.size() - 1;
    // std::cout << "idx = " << idx << std::endl;
    while (idx >= 0) {
        auto current_token = token_stream.at(idx);
        if (current_token.type == TOKEN_BINARY_and) {
            current_token.type = TOKEN_BINARY_AND;
            current_token.token = "&&";
        } else if (current_token.type == TOKEN_BINARY_or) {
            current_token.type = TOKEN_BINARY_OR;
            current_token.token = "||";
        }
        switch (current_token.type) {
            case TOKEN_KEY:
                ord_stk.push(current_token);
                --idx;
                break;
            case TOKEN_VALUE:
                ord_stk.push(current_token);
                --idx;
                break;
            case TOKEN_UNARY_NOT:

            case TOKEN_BINARY_AND:

            case TOKEN_BINARY_OR:

            case TOKEN_BINARY_EQUAL:

                if (op_stk.empty() or op_stk.top().type == TOKEN_RIGHT_BRACKET) {
                    op_stk.push(current_token);
                    --idx;
                } else if (GetPriority(op_stk.top().type) <= GetPriority(current_token.type)) {
                    op_stk.push(current_token);
                    --idx;
                } else {
                    while (op_stk.size() and GetPriority(op_stk.top().type) > GetPriority(current_token.type)) {
                        ord_stk.push(op_stk.top());
                        op_stk.pop();
                    }
                    if (op_stk.empty() or op_stk.top().type == TOKEN_RIGHT_BRACKET) {
                        op_stk.push(current_token);
                        --idx;
                    } else if (GetPriority(op_stk.top().type) <= GetPriority(current_token.type)) {
                        op_stk.push(current_token);
                        --idx;
                    } else {
                        std::cout << "error occurs in parsing. illegal syntax found in " << current_token.token
                                  << std::endl;
                        return -1;
                    }
                }
                break;
            case TOKEN_LEFT_BRACKET:
                while (op_stk.size() and op_stk.top().type != TOKEN_RIGHT_BRACKET) {
                    ord_stk.push(op_stk.top());
                    op_stk.pop();
                }
                if (op_stk.top().type == TOKEN_RIGHT_BRACKET) {
                    op_stk.pop();
                    --idx;
                } else {
                    std::cout << "error occurs in parsing. Couldn't find couples of '[]'. Syntax error." << std::endl;
                    return false;
                }
                break;
            case TOKEN_RIGHT_BRACKET:
                op_stk.push(current_token);
                --idx;
                break;
            default:
                std::cout << "error occurs in Parsing. Couldn't find approiate syntax for " << current_token.token
                          << std::endl;
                return false;
                break;
        }
    }
    while (op_stk.size()) {
        ord_stk.push(op_stk.top());
        op_stk.pop();
    }
    while (ord_stk.size()) {
        tokens.push_back(ord_stk.top());
        ord_stk.pop();
    }
    return true;
}

bool BashParser::RecursiveDescent() {
    tokens_idx = 0;
    return CheckExpr();
}

bool BashParser::CheckExpr() {
    const int n = tokens.size();
    if (tokens_idx >= n) {
        std::cout << "error occurs in CheckExpr. Invalid syntax." << std::endl;
        return false;
    }
    while (tokens_idx < n) {
        if (tokens.at(tokens_idx).type != TOKEN_LEFT_BRACKET) {
            std::cout << "error occurs in CheckExpr. Missing '['" << std::endl;
            return false;
        } else {
            ++tokens_idx;
        }
        bool flag = CheckStat();
        if (!flag) {
            std::cout << "error occurs in CheckStat. Parsing failed" << std::endl;
            return false;
        }
        if (tokens_idx >= n or tokens.at(tokens_idx).type != TOKEN_RIGHT_BRACKET) {
            std::cout << "error occurs in CheckExpr. Missing ']'" << std::endl;
            return false;
        } else {
            ++tokens_idx;
        }
        if (tokens_idx < n) {
            if (tokens.at(tokens_idx).type != TOKEN_BINARY_AND and tokens.at(tokens_idx).type != TOKEN_BINARY_OR) {
                break;
            } else {
                ++tokens_idx;
            }
        }
    }
    if (tokens_idx == n) {
        return true;
    } else if (tokens_idx > n) {
        std::cout << "error occurs in CheckExpr. Invalid Syntax: Unknown error." << std::endl;
        return false;
    } else {
        std::cout << "error occurs in CheckExpr. Invalid Syntax: '" << tokens.at(tokens_idx).token
                  << "' doesn't match any syntax." << std::endl;
        return false;
    }
}

bool BashParser::CheckStat() {
    const int n = tokens.size();
    if (tokens_idx >= n) {
        std::cout << "error occurs in CheckStat. Invalid Stat" << std::endl;
        return false;
    }
    while (tokens_idx < n) {
        if (tokens.at(tokens_idx).type == TOKEN_UNARY_NOT) {
            ++tokens_idx;
        }
        if (tokens_idx >= n) {
            std::cout << "error ocuurs in CheckStat. Invalid Stat. '!' doesn't matched anything." << std::endl;
            return false;
        }
        int key_idx = tokens_idx, value_idx = tokens_idx + 2, equal_idx = tokens_idx + 1;
        if (key_idx >= n) {
            std::cout << "error occurs in CheckStat. Missing Key('$key') part." << std::endl;
            return false;
        }
        if (equal_idx >= n) {
            std::cout << "error occurs in CheckStat. Missing Equal('==') part." << std::endl;
            return false;
        }
        if (value_idx >= n) {
            std::cout << "error occurs in CheckStat. Missing Value('value') part." << std::endl;
            return false;
        }
        if (tokens.at(key_idx).type != TOKEN_KEY) {
            std::cout << "error occurs in CheckStat. '" << tokens.at(key_idx).token << "' is not a valid 'Key'. "
                      << std::endl;
            return false;
        }
        if (tokens.at(equal_idx).type != TOKEN_BINARY_EQUAL) {
            std::cout << "error occurs in CheckStat. '" << tokens.at(equal_idx).token << "' should be '==' "
                      << std::endl;
            return false;
        }
        if (tokens.at(value_idx).type != TOKEN_VALUE) {
            std::cout << "error occurs in CheckStat. '" << tokens.at(value_idx).token << "' is not a valid 'Value'. "
                      << std::endl;
            return false;
        }
        tokens_idx += 3;
        if (tokens_idx < n) {
            if (tokens.at(tokens_idx).type != TOKEN_BINARY_AND and tokens.at(tokens_idx).type != TOKEN_BINARY_OR) {
                break;
            } else {
                ++tokens_idx;
            }
        }
    }
    return true;
}

Json::Value BashParser::MakeJsonElement(std::vector<Token>& tmp_vec, Json::Value& parent_json) {
    if (tmp_vec.empty())
        return parent_json;
    auto last_elem = tmp_vec.back();
    tmp_vec.pop_back();
    Json::Value child_json;
    if (last_elem.type == TOKEN_BINARY_EQUAL) {
        parser_test::Token k = tmp_vec.back();
        tmp_vec.pop_back();
        parser_test::Token v = tmp_vec.back();
        tmp_vec.pop_back();
        k.token.erase(0, 1);
        child_json["key"] = k.token;
        child_json["exp"] = v.token;
        child_json["type"] = "regex";
        if (parent_json.isMember("operands")) {
            parent_json["operands"].append(child_json);
        } else if (parent_json.isMember("filter_expression")) {
            parent_json["filter_expression"] = child_json;
        }
    } else if (last_elem.type == TOKEN_BINARY_AND) {
        child_json["operator"] = "and";
        child_json["operands"];
        MakeJsonElement(tmp_vec, child_json);
        MakeJsonElement(tmp_vec, child_json);
        // if parent_json has attribute named "operands", add child_json to it. Otherwise skip it.
        if (parent_json.isMember("operands")) {
            parent_json["operands"].append(child_json);
        } else if (parent_json.isMember("filter_expression")) {
            parent_json["filter_expression"] = child_json;
        }
    } else if (last_elem.type == TOKEN_BINARY_OR) {
        child_json["operator"] = "or";
        child_json["operands"];
        MakeJsonElement(tmp_vec, child_json);
        MakeJsonElement(tmp_vec, child_json);
        // if parent_json has attribute named "operands", add child_json to it. Otherwise skip it.
        if (parent_json.isMember("operands")) {
            parent_json["operands"].append(child_json);
        } else if (parent_json.isMember("filter_expression")) {
            parent_json["filter_expression"] = child_json;
        }
    } else if (last_elem.type == TOKEN_UNARY_NOT) {
        child_json["operator"] = "not";
        child_json["operands"];
        MakeJsonElement(tmp_vec, child_json);
        // if parent_json has attribute named "operands", add child_json to it. Otherwise skip it.
        if (parent_json.isMember("operands")) {
            parent_json["operands"].append(child_json);
        } else if (parent_json.isMember("filter_expression")) {
            parent_json["filter_expression"] = child_json;
        }
    } else {
        std::cout << "error occurs in MakeJsonElement, type error. Type here is " << last_elem.type << ", token is "
                  << last_elem.token << std::endl;
    }
    return parent_json;
}

bool BashParser::MakeJson() {
    json_res.clear();
    json_res["filter_expression"];
    auto tmp_vec(tokens);
    std::reverse(tmp_vec.begin(), tmp_vec.end());
    auto qop = MakeJsonElement(tmp_vec, json_res);
    if (qop.compare(json_res) == 0)
        return true;
    return false;
}

void BashParser::Reset() {

    
}

Json::Value BashParser::GetParsedJson(std::string expr) {
    if (expr.empty()) {
        std::cout << "GetParsedJson error, expr empty." << std::endl;
        return {};
    }

    tokens.clear();
    bash_expr = expr;
    json_res.clear();

    if (!LexicalAnalysis()) {
        std::cout << "Lexical Analysis error, couldn't parse the expression: " << expr << std::endl;
        return {};
    }
    std::cout << "Tokenization success." << std::endl;
    if (!RecursiveDescent()) {
        std::cout << "Syntax error, couldn't parse the expression: " << expr << std::endl;
        return {};
    }
    std::cout << "Syntax success." << std::endl;
    if (!Parse()) {
        std::cout << "Parse error, couldn't parse the expression: " << expr << std::endl;
        return {};
    }
    std::cout << "Parsing success." << std::endl;
    if (!MakeJson()) {
        std::cout << "Json error, couldn't parse the expression: " << expr << std::endl;
        return {};
    }
    std::cout << "Convert success." << std::endl;
    return json_res;
}

} // namespace parser_test