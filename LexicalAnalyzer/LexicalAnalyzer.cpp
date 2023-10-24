#pragma execution_character_set("utf-8")

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <Windows.h>

#include "Console.h"

class LexicalAnalyzer
{
    std::unordered_set<std::string> _keywords = {
        "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor",
        "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t",
        "class", "compl", "concept", "const", "consteval", "constexpr", "constinit",
        "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype", "default",
        "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export",
        "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int",
        "long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr",
        "operator", "or", "or_eq", "private", "protected", "public", "register", "reinterpret_cast",
        "requires", "return", "short", "signed", "sizeof", "static", "static_assert",
        "static_cast", "struct", "switch", "template", "this", "thread_local", "throw",
        "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using",
        "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq", "pragma",
        "define", "undef", "ifdef", "ifndef", "if", "ifn", "endif", "elifdef", "elifndef", "include",
        "line", "error", "warning", "defined", "__has_include", "__has_cpp_attribute", "export", "module"
    };

    std::unordered_set<std::string> _symbols = {"#", "<", ">", "(", ")", "{", "}", ";", ","};
    std::unordered_set<std::string> _operators = {
        "+", "-", "*", "/", "=", "!", "&", "|", "<<", ">>", "^", "~", "&&", "||",
        "+=", "-=", "*=", "/=", "&=", "|=", "^=", "<<=", ">>="
    };
    std::map<std::string, std::vector<std::string>> _tokenLists;

public:
    void Analyze(const std::string& fileName)
    {
        std::ifstream file(fileName);
        if (!file.is_open())
        {
            std::cerr << "Error opening file." << '\n';
            return;
        }

        std::string line, content;
        while (std::getline(file, line))
        {
            content += line + "\n";
        }

        std::regex rgx(
            R"(\/\/.+$|\/\*[\s\S]+\*\/|R["]\(.+\)["]|["](?:\\.|[^"\n])*["]|\b[^;\s{}()\[\]<>#*+\-\/=:,]+\b|(\.)|(\+\+)|(--)|(<<)|(>>)|(&&)|(\|\|)|(::)|(([*+\-\/&|^]|<<|>>)=)|(->)|[;'{}()\[\]<>#*+\-=,])");
        std::smatch match;

        while (std::regex_search(content, match, rgx))
        {
            std::string token = match[0];

            std::string category;
            if (_keywords.contains(token))
            {
                category = "Keyword";
            }
            else if (token[0] == '"' && token[token.length() - 1] == '"' ||
                token.substr(0, 3) == "R\"(" && token.substr(token.length() - 2, 2) == ")\"")
            {
                category = "String Literal";
            }
            else if (std::isdigit(token[0]) || token[0] == '-' ||
                std::regex_match(token, std::regex("[-]?[0-9]*\\.[0-9]+([uUlLzZ]|[uU]?(ll|LL)|f)?")) ||
                std::regex_match(token, std::regex("'.'")))
            {
                category = "Constant";
            }
            else if (_symbols.contains(token))
            {
                category = "Symbol";
            }
            else if (_operators.contains(token))
            {
                category = "Operator";
            }
            else if (token.substr(0, 2) == "//" || (token.substr(0, 2) == "/*" && token.substr(token.length() - 2, 2) == "*/"))
            {
                category = "Comment";
            }
            else
            {
                category = "Identifier";
            }

            _tokenLists[category].push_back(token);
            PrintSummary(token, category);

            content = match.suffix().str();
        }

        PrintFinal();
        file.close();
    }

    void PrintSummary(const std::string& token, const std::string& category)
    {
        std::cout << "┌────────────────────────────────────┐\n";
        std::cout << "│ " << Center("[" + token + "]: " + category, 34) << " │\n";
        std::cout << "├────────────────────────────────────┤\n";
        for (auto& [fst, snd] : _tokenLists)
        {
            std::cout << "│ " << Right(fst + ": " + std::to_string(snd.size()), 34) << " │\n";
        }
        std::cout << "└────────────────────────────────────┘\n";
    }

    void PrintFinal()
    {
        std::cout << "──────────── FINAL LIST ────────────\n";
        for (auto& [fst, snd] : _tokenLists)
        {
            if (!snd.empty())
            {
                std::cout << fst << ": [";
                for (size_t i = 0; i < snd.size(); ++i)
                {
                    std::string token = std::regex_replace(snd[i], std::regex("[\r\n]"), "");
                    if (token.empty()) continue;

                    std::cout << token;
                    if (i < snd.size() - 1) std::cout << ", ";
                }
            }
            std::cout << "]\n";
        }
    }
};

int main()
{
    SetConsoleOutputCP(65001);

    std::cout << "Enter file name: ";
    std::string fileName;
    std::cin >> fileName;

    LexicalAnalyzer analyzer;
    analyzer.Analyze(fileName);
}
