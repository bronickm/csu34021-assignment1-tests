#include <iostream>
#include <vector>

#include "test_utils.hpp"
#include "winapi_utils.hpp"

extern "C" int task2_main();
extern "C" int task2_recursiveLowerCase(char* s, int len);

int recursiveLowerCase(char* s, int len) {
    int aux = 0;
    if((*s > 64) && (*s < 91)) { aux++; *s += 32; }
    len--;
    if(len > 0) return(aux + recursiveLowerCase(++s, len));
    return(aux);
}

int main() {

    // Uncomment to also test the behaviour of the main procedure

    // const auto [task2MainReturnValue, task2MainOutput] = invokeWithStdOutRedirectedToStr([]() {
    //     return task2_main();
    // });

    // std::cout << "Testing Task #2 main standard output value:\n";
    // printTestResult("()", "", task2MainOutput);
    // std::cout << '\n';

    // std::cout << "Testing Task #2 main return value:\n";
    // printTestResult("()", 0, task2MainReturnValue);
    // std::cout << '\n';

    const std::vector<std::string> testInputsRecursiveLowerCase{
        "", "a", "A", "Ab", "aB", "ABC", "abc", "123", "AbC123", "hello world", 
        "HELLO WORLD", "Hello World", "1234567890", "Mixed123CASE", "singleword", 
        "SINGLEWORD", "PascalCase", "snake_case", "camelCaseExample", "lowercasewithspaces ", 
        "UPPERCASEWITHSPACES ", " Leading space", "Trailing space ", "   Multiple   spaces   ", 
        "\tTabbed", "\nNewline", "Special@#&Characters", "Numb3rsAndLetters", "UPPER_lower", 
        "MiXeD123w!th$pec!al", "Spaces And-Special#Chars", "With\nNewlines\nAnd\nTabs", 
        "ALLCAPS", "   leading and trailing spaces   ", "AlreadyLowercase", "12345Symbols!@", 
        "Hi There!", "StringWithManyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyCharacters", 
        "Test-With-Dashes", "test_with_underscores", "Whitespace   inside", "tabs\tin\tbetween", 
        "New\nLine\nAt\nEnd\n", "MixedCASEWithNumbersAndSymbols123!", "Z", "lower123", 
        "UPPER123", "CAPSandLower123", "Special&Symbols#@!", "  SurroundingSpaces  ", 
        "New\nLine\nAt\nStart\n", "CamelCaseExampleWithNumbers123",
        std::string(300, 'A'), std::string(300, 'a'), std::string(300, '@'),
        std::string(300, ' '), std::string(300, '\n'), std::string(300, '\t')
    };
    std::cout << "Testing recursiveLowerCase string output param:\n";
    for(const auto& input : testInputsRecursiveLowerCase) {
        std::string str = input;
        recursiveLowerCase(str.data(), static_cast<int>(str.size()));
        const std::string expectedResult = str;
        str = input;
        task2_recursiveLowerCase(str.data(), static_cast<int>(str.size()));
        const std::string actualResult = str;
        printTestResult(input, expectedResult, actualResult);
    }
    std::cout << '\n';
    std::cout << "Testing recursiveLowerCase return value:\n";
    for(const auto& input : testInputsRecursiveLowerCase) {
        std::string str = input;
        const auto expectedReturnValue = recursiveLowerCase(str.data(), static_cast<int>(str.size()));
        str = input;
        const auto actualReturnValue = task2_recursiveLowerCase(str.data(), static_cast<int>(str.size()));
        printTestResult(input, expectedReturnValue, actualReturnValue);
    }
    std::cout << '\n';

    std::cout << "All tests completed.";

    return 0;
}
