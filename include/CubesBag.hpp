#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <Reader.hpp>

class CubesBag
{
public:
    CubesBag();

    bool possible(uint8_t redCubes = 0, uint8_t greenCubes = 0, uint8_t blueCubes = 0);

    int analyzeGames(std::vector<std::string> games);

private:
    bool analyzeGame(std::string game);

    const uint8_t kRedCubes;
    const uint8_t kGreenCubes;
    const uint8_t kBlueCubes;
    std::string const kDigits;
};