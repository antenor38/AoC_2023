#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

class Trebuchet
{
public:
    Trebuchet() {}

    int readCalibration(std::string line, bool firstRecursion = true);
};