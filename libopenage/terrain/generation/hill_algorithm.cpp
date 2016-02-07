
#include "hill_algorithm.h"
#include <cmath>
#include <random>
#include <memory>

namespace openage {

struct point {
    unsigned int r;
    unsigned int c;
};

float z(double mid_r, double mid_c, point p, float r, float &max_val) {
    auto val = std::pow(r, 2) - (std::pow(p.c - mid_c, 2) + std::pow(p.r - mid_r, 2));

    if (val > max_val) {
        max_val = val;
    }

    return val > 0 ? val : 0;
}

void keep_boundary(int size, double &x) {
    if (x > size) {
        x -= size;
    }
    else if(x < 0) {
        x += size;
    }
}

float ** hill_generation(long size, int iterations) {

    // step 1 - initialize an empty terrain
    float** terrain = new float*[size];
    for(auto i = 0; i != size; ++i) {
        terrain[i] = new float[size]();
    }

    auto minimum = 0.5 * size * 0.1;
    auto maximum = 0.5 * size * 0.6;
    auto max_val = 0.0f;


    // initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.4f, 0.6f);

    for (auto i = 0; i != iterations; ++i) {

        // step 2 - find mid point and radius
        auto m1 = dis(gen);
        auto m2 = dis(gen);

        std::normal_distribution<> nd1(m1 * size, 0.3f * size);
        std::normal_distribution<> nd2(m2 * size, 0.3f * size);
        auto mid_r = nd1(gen);
        auto mid_c = nd2(gen);

        keep_boundary(size, mid_r);
        keep_boundary(size, mid_c);

        auto rad = minimum + ((maximum - minimum) * std::generate_canonical<float, 1>(gen));

        for (unsigned int row = 0; row != size; ++row) {
            for (unsigned int col = 0; col != size; ++col) {
                terrain[row][col] += z(mid_r, mid_c, point{row, col}, rad, max_val);
            }
        }
    }

    // step 4 and 5 - normalize and flatten
    for (auto row = 0; row != size; ++row) {
        for (auto col = 0; col != size; ++col) {
            terrain[row][col] /= max_val;
        }
    }

    return terrain;
}

}
