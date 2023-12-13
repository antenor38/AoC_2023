#include <CosmicExpansion.hpp>

uint64_t CosmicExpansion::calculateShortestPaths(vector<string> lines)
{
    vector<Node> galaxies;
    vector<string> columns(lines.at(0).size());
    uint32_t galaxiesCount = 0;
    vector<uint16_t> emptyRows;

    for(size_t i = 0; i < lines.size(); i++)
    {
        string line = lines.at(i);
        size_t pos = string::npos;
        bool foundGalaxy = false;

        string searchLine = line;
        size_t distance = 0;

        while(string::npos != (pos = searchLine.find('#')))
        {
            distance += pos;
            searchLine = searchLine.substr(pos + 1);
            foundGalaxy = true;
            galaxiesCount++;
            Node galaxy;
            galaxy.xCoordinate = distance;
            galaxy.yCoordinate = i;
            galaxy.name = galaxiesCount;
            distance++;

            galaxies.push_back(galaxy);
        }

        if(false == foundGalaxy)
        {
            emptyRows.push_back(i);
        }

        for(size_t j = 0; j < line.size(); j++)
        {
            columns.at(j) += line.at(j);
        }
    }
    
    auto emptyColumns = findEmptyColumns(columns);
    cout << "Empty rows num: " << emptyRows.size() << ", empty columns num: " << emptyColumns.size() << endl;

    // for(auto galaxy : galaxies)
    // {
    //     cout << galaxy.name << "[" << galaxy.xCoordinate << "," << galaxy.yCoordinate << "], ";
    // }
    // cout << endl;

    for(auto& galaxy : galaxies)
    {
        uint64_t xIncrease = 0;
        uint64_t yIncrease = 0;
        for(auto emptyRow : emptyRows)
        {
            if(galaxy.yCoordinate > emptyRow)
            {
                yIncrease += 999999;
            }
        }
        for(auto emptyColumn : emptyColumns)
        {
            if(galaxy.xCoordinate > emptyColumn)
            {
                xIncrease += 999999;
            }
        }
        galaxy.xCoordinate += xIncrease;
        galaxy.yCoordinate += yIncrease;
    }

    // for(auto galaxy : galaxies)
    // {
    //     cout << galaxy.name << "[" << galaxy.xCoordinate << "," << galaxy.yCoordinate << "], ";
    // }
    // cout << endl;

    uint64_t sum = 0;
    for(size_t i = 0; i < galaxies.size(); i++)
    {
        for(size_t j = i + 1; j < galaxies.size(); j++)
        {
            auto shortestPath = calculateShortestPath(galaxies[i], galaxies[j]);
            sum += shortestPath;
            if(2 > i && 10 > j)
            {
                // cout << "Path between " << galaxies[i].name << "[" << galaxies[i].xCoordinate << "," << galaxies[i].yCoordinate << "]"
                // << " & " << galaxies[j].name << "[" << galaxies[j].xCoordinate << "," << galaxies[j].yCoordinate << "]"
                // << " = " << shortestPath << endl;
            }
        }
    }

    return sum;
}

vector<uint16_t> CosmicExpansion::findEmptyColumns(vector<string> columns)
{
    vector<uint16_t> emptyColumns;

    for(size_t i = 0; i < columns.size(); i++)
    {
        string column = columns.at(i);

        if (string::npos == column.find('#'))
        {
            emptyColumns.push_back(i);
        }
    }

    return emptyColumns;
}

uint64_t CosmicExpansion::calculateShortestPath(Node const& first, Node const& second)
{
    int64_t distanceX = second.xCoordinate - first.xCoordinate;
    int64_t distanceY = second.yCoordinate - first.yCoordinate;
    if( 0 > distanceX)
    {
        distanceX *= -1;
    }

    if( 0 > distanceY)
    {
        distanceY *= -1;
    }

    uint64_t distance = distanceX + distanceY;

    return distance;
}