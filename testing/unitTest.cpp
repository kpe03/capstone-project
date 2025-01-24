/* Unit Test for C++
Kazi Priom
Ticket 2
Date: 1/23/2024
*/

#include <iostream>
#include <unordered_map>
#include <string>
#include <cmath>
// #include<catch2 / catch.hpp>

int main()
{
    std::unordered_map<std::string, int> mock_values;
    mock_values.insert(std::make_pair("temperature", 21));
    mock_values.insert(std::make_pair("humidity", 40));
    mock_values.insert(std::make_pair("energy", 100));
}

void checkPriorities(std::unordered_map<std::string, int> actual_metrics)
{

    std::string priorities[] = {"temperature", "humidity", "energy"};
    std::unordered_map<std::string, int> desired_metrics;
    desired_metrics.insert(std::make_pair("temperature", 20));
    desired_metrics.insert(std::make_pair("humidity", 35));
    desired_metrics.insert(std::make_pair("energy", 70));

    std::unordered_map<std::string, double> difference;

    for (int i = 0; i < sizeof(priorities) / sizeof(priorities[0]); i++)
    {
        std::string current_property = priorities[i];
        int actual_value = actual_metrics[current_property];
        int desired_value = desired_metrics[current_property];

        double diff = abs(actual_value - desired_value) / (double)desired_value * 100;
        std::cout << diff;
        difference[current_property] = diff;
    }
};
