#include <iostream>
#include <bitset>
#include <unordered_map>
#include <random>
#include <vector>
#include <fstream>
#include <chrono>
#include "hnswlib/hnswlib.h"

// Constants
const int NUM_ATTRIBUTES = 1000;
const int MAX_ELEMENTS = 10000;
const int DIM = 16;

// Function to generate random attributes for a data point
std::bitset<NUM_ATTRIBUTES> generateRandomAttributes(std::mt19937 &rng) {
    std::bitset<NUM_ATTRIBUTES> attributes;
    std::uniform_int_distribution<> distrib(0, 1);
    for (int i = 0; i < NUM_ATTRIBUTES; ++i) {
        attributes[i] = distrib(rng);
    }
    return attributes;
}

// Linear filter class for attribute-based filtering
class LinearFilter : public hnswlib::BaseFilterFunctor {
    std::bitset<NUM_ATTRIBUTES> target;
    const std::unordered_map<hnswlib::labeltype, std::bitset<NUM_ATTRIBUTES>>& attribute_map;

public:
    LinearFilter(const std::bitset<NUM_ATTRIBUTES>& target,
                 const std::unordered_map<hnswlib::labeltype, std::bitset<NUM_ATTRIBUTES>>& attribute_map)
        : target(target), attribute_map(attribute_map) {}

    bool operator()(hnswlib::labeltype label_id) override {
        auto it = attribute_map.find(label_id);
        if (it == attribute_map.end()) return false; // Label not found
        const std::bitset<NUM_ATTRIBUTES>& attributes = it->second;
        for (int i = 0; i < NUM_ATTRIBUTES; ++i) {
            if (target[i] && !attributes[i]) return false; // Target requires a bit that is missing
        }
        return true; // All required bits are present
    }
};

int main() {
    std::mt19937 rng(47);
    hnswlib::L2Space space(DIM);
    hnswlib::HierarchicalNSW<float> alg_hnsw(&space, MAX_ELEMENTS, 16, 200);

    float* data = new float[DIM * MAX_ELEMENTS];
    std::uniform_real_distribution<> distrib_real;
    std::unordered_map<hnswlib::labeltype, std::bitset<NUM_ATTRIBUTES>> attribute_map;

    // Generate data points
    for (int i = 0; i < DIM * MAX_ELEMENTS; ++i) {
        data[i] = distrib_real(rng);
    }

    // Generate attributes and add points to the index
    for (int i = 0; i < MAX_ELEMENTS; ++i) {
        std::bitset<NUM_ATTRIBUTES> attributes = generateRandomAttributes(rng);
        attribute_map[i] = attributes;
        alg_hnsw.addPoint(data + i * DIM, i);
    }

    // Define a target bitset for filtering
    std::bitset<NUM_ATTRIBUTES> target;
    target[0] = 1; // Example: Require the first attribute
    target[2] = 1; // Example: Require the third attribute

    // Define a query point
    float query_point[DIM] = {0};
    for (int i = 0; i < DIM; ++i) {
        query_point[i] = distrib_real(rng);
    }

    // Create a LinearFilter instance
    LinearFilter filter(target, attribute_map);

    // Perform search with the filter
    auto start = std::chrono::high_resolution_clock::now();
    auto result = alg_hnsw.searchKnnCloserFirst(query_point, 10, &filter);

    // Save results to file
    std::ofstream file("linear_filter_results.txt");
    for (const auto& item : result) {
        file << "ID: " << item.second << ", Distance: " << item.first << "\n";
    }
    file.close();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Linear filtering time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    delete[] data;
    return 0;
}
