#include <format>
#include <iostream>
#include <vector>

#include "test_utils.hpp"
#include "winapi_utils.hpp"

extern "C" int task1_main();
extern "C" int task1_my_product3_wrapper(int n);
extern "C" int task1_my_product3(int a, int b, int c);

int my_product3(int a, int b, int c) {
    int x = 0;
    if(a <= 1) { return a; }
    x = a--;
    if(b >= 1) { x *= b--; }
    if(c >= 1) { x *= c--; }
    return(x * my_product3(a, b, c));
}

int my_product3_wrapper(int N) {
    return(my_product3(N, N-1, N-2));
}

int main() {

    // Uncomment to also test the behaviour of the main procedure

    // const auto [task1MainReturnValue, task1MainOutput] = invokeWithStdOutRedirectedToStr([]() {
    //     return task1_main();
    // });

    // std::cout << "Testing Task #1 main standard output value:\n";
    // printTestResult("()", "288", task1MainOutput);
    // std::cout << '\n';

    // std::cout << "Testing Task #1 main return value:\n";
    // printTestResult("()", 0, task1MainReturnValue);
    // std::cout << '\n';

    const std::vector testInputsProductWrapper{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << "Testing my_product3_wrapper:\n";
    for(const auto input : testInputsProductWrapper) {
        const auto expectedResult = my_product3_wrapper(input);
        clobberFunctionCall();
        const auto actualResult = task1_my_product3_wrapper(input);
        printTestResult(std::format("({})", input), expectedResult, actualResult);
    }
    std::cout << '\n';

    const std::vector<std::tuple<int, int, int>> testInputsProduct{
        { 0, 0, 0 }, { 10, 10, 10 }, { 0, 10, 0 }, { 10, 0, 10 }, { 5, 5, 5 }, 
        { 0, 5, 10 }, { 10, 5, 0 }, { 1, 1, 1 }, { 9, 9, 9 }, { 2, 8, 4 },
        { 3, 7, 5 }, { 6, 4, 3 }, { 4, 6, 2 }, { 7, 3, 6 }, { 5, 2, 8 },
        { 1, 9, 0 }, { 8, 1, 10 }, { 2, 0, 9 }, { 9, 2, 1 }, { 3, 10, 2 },
        { 10, 3, 7 }, { 4, 4, 4 }, { 6, 6, 6 }, { 7, 7, 7 }, { 5, 8, 3 },
        { 8, 5, 9 }, { 0, 6, 4 }, { 3, 5, 1 }, { 7, 1, 8 }, { 10, 4, 5 },
        { 4, 10, 3 }, { 6, 9, 2 }, { 1, 3, 7 }, { 5, 1, 6 }, { 9, 4, 2 },
        { 2, 7, 3 }, { 6, 8, 1 }, { 7, 10, 0 }, { 8, 0, 6 }, { 10, 2, 9 },
        { 4, 8, 5 }, { 0, 7, 2 }, { 2, 3, 8 }, { 5, 10, 1 }, { 1, 6, 9 },
        { 8, 3, 0 }, { 3, 2, 10 }, { 10, 9, 3 }, { 0, 4, 7 }, { 7, 5, 9 },
        { 9, 8, 2 }, { 6, 0, 10 }, { 2, 6, 5 }, { 3, 8, 6 }, { 9, 1, 4 },
        { 1, 10, 8 }, { 4, 9, 6 }, { 7, 2, 5 }, { 5, 9, 0 }, { 6, 7, 10 },
        { 10, 9, 8 }, { 9, 8, 7 }, { 8, 7, 6 }, { 7, 6, 5 }, { 6, 5, 4 },
        { 5, 4, 3 }, { 4, 3, 2 }, { 3, 2, 1 }, { 2, 1, 0 }, { 1, 0, -1 }
    };
    std::cout << "Testing my_product3:\n";
    for(const auto& input : testInputsProduct) {
        const auto expectedResult = std::apply(my_product3, input);
        clobberFunctionCall();
        const auto actualResult = std::apply(task1_my_product3, input);
        const auto [arg1, arg2, arg3] = input;
        printTestResult(std::format("({}, {}, {})", arg1, arg2, arg3), expectedResult, actualResult);
    }
    std::cout << '\n';

    std::cout << "All tests completed.";

    return 0;
}
