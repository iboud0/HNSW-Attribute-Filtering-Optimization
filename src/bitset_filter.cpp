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

std::bitset<NUM_ATTRIBUTES> generateRandomAttributes(std::mt19937 &rng) {
    std::bitset<NUM_ATTRIBUTES> attributes;
    std::uniform_int_distribution<> distrib(0, 1);
    for (int i = 0; i < NUM_ATTRIBUTES; ++i) {
        attributes[i] = distrib(rng);
    }
    return attributes;
}

class BitsetFilter : public hnswlib::BaseFilterFunctor {
    std::bitset<NUM_ATTRIBUTES> target;
    const std::unordered_map<hnswlib::labeltype, std::bitset<NUM_ATTRIBUTES>>& attribute_map;

public:
    BitsetFilter(const std::bitset<NUM_ATTRIBUTES>& target,
                 const std::unordered_map<hnswlib::labeltype, std::bitset<NUM_ATTRIBUTES>>& attribute_map)
        : target(target), attribute_map(attribute_map) {}

    bool operator()(hnswlib::labeltype label_id) {
        auto it = attribute_map.find(label_id);
        if (it == attribute_map.end()) return false;
        const std::bitset<NUM_ATTRIBUTES>& attributes = it->second;
        return (attributes & target) == target;
    }
};

int main() {
    std::mt19937 rng(47);
    hnswlib::L2Space space(DIM);
    hnswlib::HierarchicalNSW<float> alg_hnsw(&space, MAX_ELEMENTS, 16, 200);

    float* data = new float[DIM * MAX_ELEMENTS];
    std::uniform_real_distribution<> distrib_real;
    std::unordered_map<hnswlib::labeltype, std::bitset<NUM_ATTRIBUTES>> attribute_map;

    for (int i = 0; i < DIM * MAX_ELEMENTS; ++i) {
        data[i] = distrib_real(rng);
    }

    for (int i = 0; i < MAX_ELEMENTS; ++i) {
        std::bitset<NUM_ATTRIBUTES> attributes = generateRandomAttributes(rng);
        attribute_map[i] = attributes;
        alg_hnsw.addPoint(data + i * DIM, i);
    }

    std::bitset<NUM_ATTRIBUTES> target;
    target[0] = 1;
    target[2] = 1;

    float query_point[DIM] = {0};
    for (int i = 0; i < DIM; ++i) {
        query_point[i] = distrib_real(rng);
    }

    BitsetFilter filter(target, attribute_map);

    auto start = std::chrono::high_resolution_clock::now();
    auto result = alg_hnsw.searchKnnCloserFirst(query_point, 10, &filter);

    std::ofstream file("bitset_filter_results.txt");
    for (const auto& item : result) {
        file << "ID: " << item.second << ", Distance: " << item.first << "\n";
    }
    file.close();

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Bitset filtering time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";

    delete[] data;
    return 0;
}
