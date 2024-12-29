#include <iostream>
#include <roaring/roaring.hh>
#include <unordered_map>
#include <random>
#include <vector>
#include <fstream>
#include <chrono>
#include "hnswlib/hnswlib.h"

// Constants
const int NUM_ATTRIBUTES = 100000;
const int MAX_ELEMENTS = 10000;
const int DIM = 16;

// Function to generate random attributes for a data point
roaring::Roaring generateRandomAttributes(std::mt19937 &rng, double active_attributes_percentage) {
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

roaring::Roaring generateRandomTarget(std::mt19937 &rng, int num_active_bits) {
    roaring::Roaring target;
    std::uniform_int_distribution<> distrib(0, NUM_ATTRIBUTES - 1);
    std::unordered_set<int> activated_indices;

    while (activated_indices.size() < num_active_bits) {
        int index = distrib(rng);
        if (activated_indices.find(index) == activated_indices.end()) {
            target.add(index);
            activated_indices.insert(index);
        }
    }

    return target;
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
        return (attributes & target) == target;
    }
};

size_t estimateMemoryUsage(const std::unordered_map<hnswlib::labeltype, roaring::Roaring>& attribute_map) {
    size_t total_size = 0;
    for (const auto& pair : attribute_map) {
        total_size += sizeof(pair.first) + pair.second.getSizeInBytes();
    }
    return total_size;
}

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

    // Test with three percentages
    std::vector<double> percentages = {10.0, 50.0, 90.0};
    std::ofstream file("roaring_filter_results.txt");

    for (double percentage : percentages) {
        // Generate attributes with the current percentage of active attributes
        attribute_map.clear();
        for (int i = 0; i < MAX_ELEMENTS; ++i) {
            roaring::Roaring attributes = generateRandomAttributes(rng, percentage);
            attribute_map[i] = attributes;
            alg_hnsw.addPoint(data + i * DIM, i);
        }

        // Generate a random target with a fixed number of active bits
        int num_target_bits = 5; // Adjust as needed
        roaring::Roaring target = generateRandomTarget(rng, num_target_bits);

        // Define a query point
        float query_point[DIM] = {0};
        for (int i = 0; i < DIM; ++i) {
            query_point[i] = distrib_real(rng);
        }

        // Create a RoaringFilter instance
        RoaringFilter filter(target, attribute_map);

        // Perform search with the filter
        auto start = std::chrono::high_resolution_clock::now();
        auto result = alg_hnsw.searchKnnCloserFirst(query_point, 10, &filter);
        auto end = std::chrono::high_resolution_clock::now();

        size_t memory_usage = estimateMemoryUsage(attribute_map);
        double filtering_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        // Output results
        file << "Percentage of active attributes: " << percentage << "%\n";
        file << "Filtering time: " << filtering_time << " ms\n";
        file << "Memory usage: " << memory_usage / 1024.0 << " KB\n\n";

        for (const auto& item : result) {
            file << "ID: " << item.second << ", Distance: " << item.first << "\n";
        }
        file << "-----------------------------\n";
    }

    file.close();
    delete[] data;
    return 0;
}
