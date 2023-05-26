#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cstring>

typedef std::vector<int> VINT; 

std::vector<std::pair<int, int>> comp;

void print(VINT v) {
    for (auto el : v) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

void print_comp() {
    for (auto el : comp) {
        std::cout << el.first << " " << el.second << std::endl;
    }
}

VINT generate_vector(size_t p) 
{
    VINT v;
    std::srand(std::time(0));
    for (auto i = 0; i < p; ++i) {
        v.push_back(rand() % 1000);
    }
    std::sort(v.begin(), v.end());
    return v;
}

VINT sort(VINT v) 
{
    for (auto el : comp) {
        if (v[el.first] > v[el.second]) {
            int tmp = v[el.first];
            v[el.first] = v[el.second];
            v[el.second] = tmp;
        }
    }
    return v;
}

void test_sort(VINT v)
{
    for (int i = 1; i < v.size(); ++i) {
        if (v[i - 1] > v[i]) {
            std::cout << "TEST FAILED" << std::endl;
            exit(0);
        }
    }
    std::cout << "TEST SUCCEDED" << std::endl;
}

void Network(int first, int second, int step, int p1, int p2) 
{
    if (p1 * p2 < 1) {
        return;
    }
    if (p1 == 1 && p2 == 1) {
        comp.push_back(std::make_pair(first, second));
        return; 
    }
    int p1_1 = p1 - p1 / 2; 
    int p2_1 = p2 - p2 / 2;
    Network(first, second, 2 * step, p1_1 , p2_1);
    Network(first + step, second + step, 2 * step, p1 - p1_1, p2 - p2_1);

    for(int i = 1; i < p1 - 1; i += 2) {
        comp.push_back(std::make_pair(first + step * i, first + step * (i + 1)));
    }
    int ind = 0;
    if (p1 % 2 == 0) {
        comp.push_back(std::make_pair(first + step *(p1 - 1), second));
        ind = 1;
    }
    
    for(int i = ind; i < p2 - 1; i += 2) {
        comp.push_back(std::make_pair(second + step * i, second + step *(i + 1)));
    }
}

void test_network()
{   
    for(int p1p2 = 1; p1p2 <= 24; ++p1p2) {
        for(int p1 = 1; p1 <= p1p2; ++p1) {
            int p2 = p1p2 - p1;
            std::cout << "================================================" << std::endl;
            std::cout << "p1: " << p1 << "; p2: " << p2 << "; p1 + p2 = " << p1 + p2 << std::endl;
            comp.clear();
            Network(0, p1, 1, p1, p2);
            for(int zero_p1 = 0; zero_p1 <= p1; ++zero_p1) {
                for(int zero_p2 = 0; zero_p2 <= p2; ++zero_p2) {
                    VINT v1(p1, 1), v2(p2, 1);
                    for(int i = 0; i < zero_p1; ++i) {
                        v1[i] = 0;
                    }
                    for(int i = 0; i < zero_p2; ++i) {
                        v2[i] = 0;
                    }
                    std::cout << "--------------------" << std::endl;
                    std::cout << "v1: ";
                    print(v1);
                    std::cout << "v2: ";
                    print(v2);
                    VINT v(v1);
                    std::move(v2.begin(), v2.end(), std::back_inserter(v));
                    std::cout << "v: ";
                    print(v);
                    VINT v_sorted = sort(v);
                    test_sort(v_sorted);
                }
            }
        }
    }
    std::cout << "All TESTS PASSED" << std::endl;
}

int tact_count(int n) {
    VINT v(n, 0);
    for (auto el : comp) {
        int cnt = std::max(v[el.first], v[el.second]);
        v[el.first] = cnt + 1;
        v[el.second] = cnt + 1;
    }
    auto res = std::max_element(v.begin(), v.end());
    return *res;
}

int main(int argc, const char* argv[]) 
{   
    if (argc < 3) {
        std::cout << "at startup, you need to pass two arguments:" << std::endl;
        std::cout << "./main p1 p2 -t(--test)" << std::endl;
        exit(0);
    }
    size_t p1 = atoi(argv[1]);
    size_t p2 = atoi(argv[2]);

    if (p1 < 1) {
        std::cout << "p1 must be >= 1" << std::endl;
        exit(0);
    }
    if (p2 < 0) {
        std::cout << "p2 must be >= 0" << std::endl;
        exit(0);
    }

    VINT v1 = generate_vector(p1);
    VINT v2 = generate_vector(p2);

    VINT v(v1);
	std::move(v2.begin(), v2.end(), std::back_inserter(v));

    Network(0, p1, 1, p1, p2);
    std::cout << p1 << " " << p2 << " " << 0 << std::endl;
    print_comp();
    std::cout << comp.size() << std::endl;
    std::cout << tact_count(p1 + p2) << std::endl;

    if (argc == 4) {
        if (!std::strcmp(argv[3], "-t") || !std::strcmp(argv[3], "--test")) {
            std::cout << std::endl;
            std::cout << "RUNNING TESTS:" << std::endl;
            VINT v_sorted = sort(v);
            test_sort(v_sorted);
            test_network();
        } else {
            std::cout << "Supported param is -t(--test)" << std::endl;
        }
    } 
}