/* Unit Test for C++
Kazi Priom
Ticket 2
Date: 1/23/2024
*/

#include <iostream>
#include <unordered_map>
#include <string>
#include <cmath>
class unitTest
{

public:
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
            std::cout << diff << std::endl;
            difference[current_property] = diff;
        }

        for (int i = 0; i < sizeof(priorities) / sizeof(priorities[1]) - 1; i++)
        {
            std::string key_high = priorities[i];
            std::string key_low = priorities[i + 1];
            if (difference[key_high] > difference[key_low])
            {
                std::cout << "Priority order is incorrect: " << key_high << " should have a lower or equal difference than " << key_low << std::endl;
                std::cout << "\n";
            }
            else
            {
                std::cout << "Priority order is correct: " << key_high << " has a lower or equal difference than " << key_low << std::endl;
                std::cout << "\n";
            }
        }
    };
};

int main()
{
    std::unordered_map<std::string, int> mock_values;
    mock_values.insert(std::make_pair("temperature", 21));
    mock_values.insert(std::make_pair("humidity", 40));
    mock_values.insert(std::make_pair("energy", 100));

    unitTest test;

    test.checkPriorities(mock_values);
}
