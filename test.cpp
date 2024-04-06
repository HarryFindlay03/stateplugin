#include <iostream>
#include <stdio.h>
#include <vector>

void foo(std::vector<int>& num)
{
    for(auto& v : num)
        v *= 2;

    return;
}

int main(void)
{
    printf("Hello, world!\n");

    auto printer = [](const std::vector<int>& vec)
    {
        for(const auto& v : vec)
            std::cout << v << ", ";
        std::cout << '\n';
    };

    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printer(vec);
    foo(vec);
    printer(vec);

    return 0;
}
