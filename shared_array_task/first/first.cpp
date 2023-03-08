#include "shared_array.hpp"

int main() {
    shared_array arr_1("arr", 10);
    
    while (true) {
        std::cout << "Value of the first element: " << arr_1[0] << std::endl;
        std::cout << "Value of the last element: " << arr_1[arr_1.size() - 1] << std::endl;
        sleep(1);
    }
    
    return 0;
    
}

