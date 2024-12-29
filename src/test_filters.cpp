#include <iostream>
#include <roaring/roaring.hh>
#include <bitset>
#include <unordered_set>
#include <random>
#include <vector>
#include <cassert>

// Constants
const int NUM_ATTRIBUTES = 50; // Reduced total number of attributes
const int NUM_DATA_POINTS = 5;  // Reduced number of data points
const double ACTIVE_PERCENTAGE = 20.0; // Percentage of active attributes
const int NUM_TARGET_BITS = 5;         // Number of active bits in the target

// Function to generate random attributes using Roaring Bitmaps
roaring::Roaring generateRandomAttributesRoaring(std::mt19937 &rng, double active_attributes_percentage) {
    roaring::Roaring attributes;
    std::uniform_int_distribution<> distrib(0, NUM_ATTRIBUTES - 1);

    int num_active = static_cast<int>(NUM_ATTRIBUTES * active_attributes_percentage / 100.0);
    std::unordered_set<int> activated_indices;

    while (activated_indices.size() < num_active) {
        int index = distrib(rng);
        if (activated_indices.find(index) == activated_indices.end()) {
            attributes.add(index);
            activated_indices.insert(index);
        }
    }

    return attributes;
}

// Function to generate random attributes using std::bitset
std::bitset<NUM_ATTRIBUTES> generateRandomAttributesBitset(std::mt19937 &rng, double active_attributes_percentage) {
    std::bitset<NUM_ATTRIBUTES> attributes;
    std::uniform_int_distribution<> distrib(0, NUM_ATTRIBUTES - 1);

    int num_active = static_cast<int>(NUM_ATTRIBUTES * active_attributes_percentage / 100.0);
    std::unordered_set<int> activated_indices;

    while (activated_indices.size() < num_active) {
        int index = distrib(rng);
        if (activated_indices.find(index) == activated_indices.end()) {
            attributes.set(index);
            activated_indices.insert(index);
        }
    }

    return attributes;
}

// Filter function for Roaring Bitmap
std::vector<int> filterRoaring(const roaring::Roaring &target, const std::unordered_map<int, roaring::Roaring> &attribute_map) {
    std::vector<int> filtered_ids;
    for (const auto &pair : attribute_map) {
        if ((pair.second & target) == target) {
            filtered_ids.push_back(pair.first);
        }
    }
    return filtered_ids;
}

// Filter function for Bitset
std::vector<int> filterBitset(const std::bitset<NUM_ATTRIBUTES> &target, const std::unordered_map<int, std::bitset<NUM_ATTRIBUTES>> &attribute_map) {
    std::vector<int> filtered_ids;
    for (const auto &pair : attribute_map) {
        if ((pair.second & target) == target) {
            filtered_ids.push_back(pair.first);
        }
    }
    return filtered_ids;
}

// Helper function to print Roaring attributes
void printRoaringAttributes(const roaring::Roaring &attributes) {
    for (auto idx : attributes) {
        std::cout << idx << " ";
    }
    std::cout << std::endl;
}

// Helper function to print Bitset attributes
void printBitsetAttributes(const std::bitset<NUM_ATTRIBUTES> &attributes) {
    for (int i = 0; i < NUM_ATTRIBUTES; ++i) {
        if (attributes[i]) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
}

// Test filtering for both Roaring and Bitset
void testFilteringFunction() {
    std::mt19937 rng(47); // Fixed seed for reproducibility

    // Generate data for Roaring
    std::unordered_map<int, roaring::Roaring> roaring_attribute_map;
    for (int i = 0; i < NUM_DATA_POINTS; ++i) {
        roaring_attribute_map[i] = generateRandomAttributesRoaring(rng, ACTIVE_PERCENTAGE);
    }

    // Generate data for Bitset
    std::unordered_map<int, std::bitset<NUM_ATTRIBUTES>> bitset_attribute_map;
    for (int i = 0; i < NUM_DATA_POINTS; ++i) {
        bitset_attribute_map[i] = generateRandomAttributesBitset(rng, ACTIVE_PERCENTAGE);
    }

    // Generate a common target
    roaring::Roaring roaring_target = generateRandomAttributesRoaring(rng, NUM_TARGET_BITS);

    // Convert Roaring target to Bitset for consistency
    std::bitset<NUM_ATTRIBUTES> bitset_target;
    for (auto idx : roaring_target) {
        bitset_target.set(idx);
    }

    // Print the target attributes
    std::cout << "Roaring Target Attributes: ";
    printRoaringAttributes(roaring_target);

    std::cout << "Bitset Target Attributes: ";
    printBitsetAttributes(bitset_target);

    // Print data points
    std::cout << "\nRoaring Data Points:\n";
    for (const auto &pair : roaring_attribute_map) {
        std::cout << "Data Point " << pair.first << ": ";
        printRoaringAttributes(pair.second);
    }

    std::cout << "\nBitset Data Points:\n";
    for (const auto &pair : bitset_attribute_map) {
        std::cout << "Data Point " << pair.first << ": ";
        printBitsetAttributes(pair.second);
    }

    // Assign targets to ensure at least one match
    roaring_attribute_map[0] = roaring_target; // Ensure at least one match for Roaring
    bitset_attribute_map[0] = bitset_target;   // Ensure at least one match for Bitset

    // Filter data points
    auto roaring_filtered = filterRoaring(roaring_target, roaring_attribute_map);
    auto bitset_filtered = filterBitset(bitset_target, bitset_attribute_map);

    // Validate Roaring results
    for (int id : roaring_filtered) {
        assert((roaring_attribute_map[id] & roaring_target) == roaring_target && "Roaring: Incorrect filtering result!");
    }

    // Validate Bitset results
    for (int id : bitset_filtered) {
        assert((bitset_attribute_map[id] & bitset_target) == bitset_target && "Bitset: Incorrect filtering result!");
    }

    // Print results
    std::cout << "\nRoaring Filtering Test Passed: " << roaring_filtered.size() << " data points matched.\n";
    std::cout << "Bitset Filtering Test Passed: " << bitset_filtered.size() << " data points matched.\n";

    // Optional: Compare results between Roaring and Bitset
    assert(roaring_filtered.size() == bitset_filtered.size() && "Mismatch between Roaring and Bitset results!");
    std::cout << "Roaring and Bitset filtering results are consistent.\n";
}

int main() {
    testFilteringFunction();
    return 0;
}
