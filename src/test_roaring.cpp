#include <iostream>
#include "roaring/roaring.hh" // Include the Roaring header

int main() {
    // Create a Roaring Bitmap
    roaring::Roaring r1;

    // Add elements to the bitmap
    for (uint32_t i = 0; i < 1000; i += 2) {
        r1.add(i); // Add even numbers
    }

    // Check cardinality (number of elements)
    std::cout << "Cardinality (number of elements): " << r1.cardinality() << std::endl;

    // Check if specific elements exist
    std::cout << "Contains 4? " << (r1.contains(4) ? "Yes" : "No") << std::endl;
    std::cout << "Contains 5? " << (r1.contains(5) ? "Yes" : "No") << std::endl;

    // Iterate over elements
    std::cout << "Elements in the bitmap: ";
    for (auto value : r1) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // Remove an element and check again
    r1.remove(4);
    std::cout << "Contains 4 after removal? " << (r1.contains(4) ? "Yes" : "No") << std::endl;

    return 0;
}
