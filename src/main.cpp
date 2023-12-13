#include <iostream>
#include <memory>
#include <Reader.hpp>
#include <Trebuchet.hpp>
#include <Scratchcard.hpp>
#include <Garden.hpp>
#include <Race.hpp>
#include <CamelCard.hpp>
#include <Wasteland.hpp>
#include <CosmicExpansion.hpp>
#include <HotSprings.hpp>

void sumCalibration(std::vector<std::string> lines)
{
    Trebuchet trebuchet;
    int sum = 0;
    for(auto line : lines)
    {
        sum += trebuchet.readCalibration(line);
    }


    std::cout << "Sum of calibration is: " << sum << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Expected filename of input data!\n";
        return -1;
    }

    std::string inputName = argv[1];

    std::string filename = "../config/" + inputName;

    Reader reader(filename);

    auto lines = reader.readLines();

    std::unique_ptr<HotSprings> springs = std::make_unique<HotSprings>();

    uint64_t result = springs->calculateSpringArrangements(lines);

    std::cout << "Result: " << result << std::endl;

    return 0;
}