namespace MathLib {
    // ❌ 错误示范：在头文件中直接写实现
    // 这会导致 "Multiple Definition" (重定义) 链接错误
    double add(double a, double b) {
        return a + b;
    }

    double subtract(double a, double b) {
        return a - b;
    }
}
