#pragma once

#include <algorithm>
#include <format>
#include <string>
#include <type_traits>


inline unsigned int clobberFunctionCall(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e) {
    return a * b * c * d * e;
}

inline unsigned int clobberFunctionCall() {
    return clobberFunctionCall(29, 31, 37, 41, 43);
}

template<typename T>
std::string customFormat(const T& input) {
    if constexpr (std::is_convertible_v<T, std::string>) {
        const auto inputString = std::string(std::move(input));
        std::string escapedString;
        escapedString.reserve(inputString.size() + 2);

        escapedString += '"';
        std::for_each(inputString.cbegin(), inputString.cend(), [&escapedString](const auto ch) {
            switch(ch) {
                case '\n':  escapedString += "\\n"; break;
                case '\t':  escapedString += "\\t"; break;
                case '\r':  escapedString += "\\r"; break;
                default:    escapedString += ch;    break;
            }
        });
        escapedString += '"';

        return escapedString;
    } else {
        return std::to_string(input);
    }
}

void printTestResult(const std::string& input, const auto& expectedResult, const auto& actualResult) {
    if(expectedResult == actualResult) {
        std::cout << std::format(
            "\033[32mPASSED\033[0m Input: {}. Output: {}.\n", 
            customFormat(input), customFormat(actualResult)
        );
    } else {
        std::cout << std::format(
            "\033[31mFAILED\033[0m Input: {}. Expected: {}. Actual: {}.\n", 
            customFormat(input), customFormat(expectedResult), customFormat(actualResult)
        );
    }
}
