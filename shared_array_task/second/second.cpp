#include "shared_array.hpp"

int main() {
    srand(time(NULL));
    shared_array arr_1("arr", 10);
    
    while (true) {
        int elem = rand() % 1000;
        std::cout << "Setting up the first elem: " << elem << std::endl;
        arr_1[0] = elem;
        elem = rand() % 1000;
        std::cout << "Setting up the last elem: " << elem << std::endl;
        arr_1[arr_1.size() - 1] = elem;
        sleep(1);
    }
    
    return 0;
}