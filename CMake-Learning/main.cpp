#include <iostream>
#include "MathLib/MathFunctions.h"

int main() {
    double a = 10.0;
    double b = 5.0;

    std::cout << "CMake Learning Project" << std::endl;
    std::cout << "----------------------" << std::endl;
    
    // 使用 MathLib 中的函数
    // Use functions from MathLib
    std::cout << a << " + " << b << " = " << MathLib::add(a, b) << std::endl;
    std::cout << a << " - " << b << " = " << MathLib::subtract(a, b) << std::endl;

    std::cout << "\n按任意键退出..." << std::endl;
    system("pause");
    
    return 0;
}
