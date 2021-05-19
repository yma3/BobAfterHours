#include <iostream>
#include "../sqlhandler.h"

int main() {
    std::cout << "Hello World!2" << std::endl;

    SqlHandler sqlhand = SqlHandler("test.db");
    sqlhand.insertDataTable();

    return 0;
}