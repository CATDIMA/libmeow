#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include "meow.h"

std::random_device dev;
std::mt19937 rng(dev());

uint16_t randomInt(uint16_t min, uint16_t max)
{
    std::uniform_int_distribution<std::mt19937::result_type> intDist(min, max);
    return intDist(rng);
}

int main(int argc, char** argv)
{
    char arr[100];

    while(true)
    {
        int meows = randomInt(1, 6);
        memset(arr, 0, sizeof(arr));

        for(int i = 0; i < meows; i++)
        {
            generateMeowString(arr, randomInt);
        }

        std::cout << arr <<std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(randomInt(50, 300)));
    }
    return 0;
}
