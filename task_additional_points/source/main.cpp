#include <iostream>
#include <vector>
#include <algorithm>

#include "../include/memory_allocator.h"
#include "../include/operations_index_mask.h"
#include "../include/number_generator.h"

class SomeClass {
public:
    SomeClass() { std::cout << "Default constructor\n"; }
    SomeClass(int x, int y, int z) { std::cout << "Constructor with args: " << x << ", " << y << ", " << z << "\n"; }
    ~SomeClass() { std::cout << "Destructor\n"; }
};

int main() {
    try {
        std::cout << "--- SimpleRNG Example ---\n";
        SimpleRNG generator(5, 0.2, 1);  
        generator.reset(0.4); 
        std::vector<double> vec;  
        std::copy(generator.begin(), generator.end(0.5), std::back_inserter(vec));  

        std::cout << "Generated values: ";
        for (const auto& val : vec) {
            std::cout << val << " ";  
        }
        std::cout << "\n";

        generator.reset(0);
        std::cout << "Generated values after reset: ";
        for (auto it = generator.begin(); it != generator.end(0.5); ++it) {
            std::cout << *it << " ";  
        }
        std::cout << "\n";

        std::cout << "--- Mask Example ---\n";
        Mask<3> mask = {1, 0, 1};  
        std::vector<int> data = {1, 2, 3, 4, 5, 6};  

        std::cout << "Original data: ";
        for (const auto& val : data) {
            std::cout << val << " "; 
        }
        std::cout << "\n";

        mask.slice(data);  
        std::cout << "After slicing: ";
        for (const auto& val : data) {
            std::cout << val << " ";  
        }
        std::cout << "\n";

        
        data = {1, 2, 3, 4, 5, 6};  
        auto transformed = mask.transform(data, [](int x) { return x * 2; }); 
        std::cout << "After transformation: ";
        for (const auto& val : transformed) {
            std::cout << val << " "; 
        }
        std::cout << "\n";

        auto sliced_transformed = mask.slice_and_transform(data, [](int x) { return x * 2; });
        std::cout << "After slice and transform: ";
        for (const auto& val : sliced_transformed) {
            std::cout << val << " ";  
        }
        std::cout << "\n";

        std::cout << "--- MemReserver Example ---\n";
        MemReserver<SomeClass, 2> reserver;  
        auto obj1 = reserver.create();  
        auto obj2 = reserver.create(1, 2, 3);  

        try {
            reserver.create();
        } catch (const NotEnoughSlotsError& e) {
            std::cout << e.what() << "\n";  
        }

        reserver.remove(reserver.position(obj1));

        try {
            auto obj3 = reserver.get(0);
        } catch (const EmptySlotError& e) {
            std::cout << e.what() << "\n"; 
        }

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";  
    }

    return 0;  
}

