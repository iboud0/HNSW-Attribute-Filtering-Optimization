#include <iostream>
#include <roaring/roaring.hh>
#include <bitset>
#include <unordered_set>
#include <random>
#include <vector>
#include <cassert>
#include <map>

// Constants
const int NUM_ATTRIBUTES = 100000; // Total number of attributes
const int NUM_DATA_POINTS = 1000;  // Number of data points to generate
const double ACTIVE_PERCENTAGE = 10.0; // Percentage of active attributes

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

// Test function for both Roaring and Bitset
void testGenerateRandomAttributes() {
    std::mt19937 rng(47); // Fixed seed for reproducibility

    // Track statistics for Roaring
    int total_active_roaring = 0;
    std::map<int, int> roaring_distribution;

    // Track statistics for Bitset
    int total_active_bitset = 0;
    std::map<int, int> bitset_distribution;

    for (int i = 0; i < NUM_DATA_POINTS; ++i) {
        // Roaring attributes
        roaring::Roaring roaring_attributes = generateRandomAttributesRoaring(rng, ACTIVE_PERCENTAGE);
        total_active_roaring += roaring_attributes.cardinality();
        for (auto idx : roaring_attributes) {
            roaring_distribution[idx]++;
        }

        // Bitset attributes
        std::bitset<NUM_ATTRIBUTES> bitset_attributes = generateRandomAttributesBitset(rng, ACTIVE_PERCENTAGE);
        total_active_bitset += bitset_attributes.count();
        for (int idx = 0; idx < NUM_ATTRIBUTES; ++idx) {
            if (bitset_attributes[idx]) {
                bitset_distribution[idx]++;
            }
        }

        // Validate correctness
        int expected_active = static_cast<int>(NUM_ATTRIBUTES * ACTIVE_PERCENTAGE / 100.0);
        assert(roaring_attributes.cardinality() == expected_active && "Roaring: Incorrect number of active attributes!");
        assert(bitset_attributes.count() == expected_active && "Bitset: Incorrect number of active attributes!");
    }

    // Print results
    std::cout << "Test Passed: All data points have the correct number of active attributes.\n";

    std::cout << "Roaring Stats:\n";
    std::cout << "  Average active attributes per data point: "
              << static_cast<double>(total_active_roaring) / NUM_DATA_POINTS << "\n";
    std::cout << "  Most common indices (Top 5):\n";
    int count = 0;
    for (const auto &pair : roaring_distribution) {
        std::cout << "    Index: " << pair.first << ", Frequency: " << pair.second << "\n";
        if (++count == 5) break;
    }

    std::cout << "\nBitset Stats:\n";
    std::cout << "  Average active attributes per data point: "
              << static_cast<double>(total_active_bitset) / NUM_DATA_POINTS << "\n";
    std::cout << "  Most common indices (Top 5):\n";
    count = 0;
    for (const auto &pair : bitset_distribution) {
        std::cout << "    Index: " << pair.first << ", Frequency: " << pair.second << "\n";
        if (++count == 5) break;
    }
}

int main() {
    testGenerateRandomAttributes();
    return 0;
}
