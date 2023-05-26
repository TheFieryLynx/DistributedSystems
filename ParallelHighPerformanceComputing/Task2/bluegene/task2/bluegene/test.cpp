#include <iostream>
#include <ctime>
 
int main()
{
    int low = 1;
    int high = 10;
 
    srand(time(NULL));
 
    for (int i = 0; i < 10; i++) {
        float r = low + static_cast<float>(rand()) * static_cast<float>(high - low) / RAND_MAX;
        std::cout << r << std::endl;
    }
 
    return 0;
}
