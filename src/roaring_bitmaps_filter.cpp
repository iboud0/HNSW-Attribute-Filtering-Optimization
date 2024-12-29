#include <iostream>
#include <roaring/roaring.hh>
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
roaring::Roaring generateRandomAttributes(std::mt19937 &rng) {
    roaring::Roaring attributes;
    std::uniform_int_distribution<> distrib(0, 1);
    for (int i = 0; i < NUM_ATTRIBUTES; ++i) {
        if (distrib(rng)) {
            attributes.add(i); // Add the attribute if it's active
        }
    }
    return attributes;
}

// Custom filter class for Roaring Bitmap-based filtering
class RoaringFilter : public hnswlib::BaseFilterFunctor {
    roaring::Roaring target;
    const std::unordered_map<hnswlib::labeltype, roaring::Roaring>& attribute_map;

public:
    RoaringFilter(const roaring::Roaring& target,
                  const std::unordered_map<hnswlib::labeltype, roaring::Roaring>& attribute_map)
        : target(target), attribute_map(attribute_map) {}

    bool operator()(hnswlib::labeltype label_id) {
        auto it = attribute_map.find(label_id);
        if (it == attribute_map.end()) return false; // Label not found
        const roaring::Roaring& attributes = it->second;
        // Check if the target is a subset of attributes
        bool is_match = (attributes & target) == target;
        return is_match;
    }
};

int main() {
    std::mt19937 rng(47);
    hnswlib::L2Space space(DIM);
    hnswlib::HierarchicalNSW<float> alg_hnsw(&space, MAX_ELEMENTS, 16, 200);

    float* data = new float[DIM * MAX_ELEMENTS];
    std::uniform_real_distribution<> distrib_real;
    std::unordered_map<hnswlib::labeltype, roaring::Roaring> attribute_map;

    for (int i = 0; i < DIM * MAX_ELEMENTS; ++i) {
        data[i] = distrib_real(rng);
    }

    for (int i = 0; i < MAX_ELEMENTS; ++i) {
        roaring::Roaring attributes = generateRandomAttributes(rng);
        attribute_map[i] = attributes;
        alg_hnsw.addPoint(data + i * DIM, i);
    }

    // Define a target bitmap for filtering
    roaring::Roaring target;
    target.add(0); // Example: Filter for points where the first attribute is active
    target.add(2); // Example: Filter for points where the third attribute is active

    float query_point[DIM] = {0};
    for (int i = 0; i < DIM; ++i) {
        query_point[i] = distrib_real(rng);
    }

    RoaringFilter filter(target, attribute_map);

    auto start = std::chrono::high_resolution_clock::now();
    auto result = alg_hnsw.searchKnnCloserFirst(query_point, 10, &filter);

    std::ofstream file("roaring_filter_results.txt");
    for (const auto& item : result) {
        file << "ID: " << item.second << ", Distance: " << item.first << "\n";
    }
    file.close();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Roaring filtering time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    delete[] data;
    return 0;
}
