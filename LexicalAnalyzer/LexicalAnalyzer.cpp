#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_set>
#include <map>

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
        "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq", "include"
    };

    std::unordered_set<std::string> _symbols = {"#", "<", ">", "(", ")", "{", "}", ";", "<<", ">>"};
    std::unordered_set<char> _operators = {'+', '-', '*', '/', '=', '!', '&', '|'};
    std::map<std::string, int> _summary;

public:
    LexicalAnalyzer()
    {
        _summary["Keywords"] = 0;
        _summary["Identifiers"] = 0;
        _summary["Operators"] = 0;
        _summary["Symbols"] = 0;
        _summary["String Literals"] = 0;
        _summary["Constants"] = 0;
    }

    void Analyze(const std::string& fileName)
    {
        std::ifstream file(fileName);
        if (!file.is_open())
        {
            std::cerr << "Error opening file.\n";
            return;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::istringstream ss(line);
            std::string word;

            while (ss >> word)
            {
                if (_keywords.find(word) != _keywords.end())
                {
                    _summary["Keywords"]++;
                    std::cout << word << " is a keyword\n";
                }
                else if (std::isdigit(word[0]))
                {
                    _summary["Constants"]++;
                    std::cout << word << " is a constant\n";
                }
                else if (_symbols.find(word) != _symbols.end())
                {
                    _summary["Symbols"]++;
                    std::cout << word << " is a symbol\n";
                }
                else if (word.size() == 1 && _operators.find(word[0]) != _operators.end())
                {
                    _summary["Operators"]++;
                    std::cout << word[0] << " is an operator\n";
                }
                else if (word[0] == '"' && word[word.size() - 1] == '"')
                {
                    _summary["String Literals"]++;
                    std::cout << word << " is a constant\n";
                }
                else if (word.size() == 3 && word[0] == '\'' && word[word.size() - 1] == '\'')
                {
                    _summary["Constants"]++;
                    std::cout << word << " is a constant\n";
                }
                else
                {
                    _summary["Identifiers"]++;
                    std::cout << word << " is an identifier\n";
                }

                PrintSummary();
            }
        }

        file.close();
    }

    void PrintSummary() const
    {
        std::cout << "\n---- SUMMARY ----\n";
        for (auto& pair : _summary)
        {
            std::cout << pair.first << ": " << pair.second << "\n";
        }
        std::cout << "-----------------\n";
    }
};

int main()
{
    std::cout << "Enter file name ";
    std::string fileName;
    std::cin >> fileName;

    LexicalAnalyzer analyzer;
    analyzer.Analyze(fileName);

    std::cin >> fileName;
    return 0;
}