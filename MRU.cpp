#include "MRU.h"
#include <fstream>
#include <algorithm>

MRU::MRU()
{
    std::ifstream file("mru.txt");
    if (file.bad() || file.eof())
        return;
    std::string line;
    while (std::getline(file, line))
        files.push_back(line);
}

void MRU::open(const std::string fileName)
{
    auto file = std::find(files.begin(), files.end(), fileName);
    if (file != files.end())
        files.erase(file);
    files.push_front(fileName);
    while (files.size() > 10)
        files.remove(files.back());

    std::ofstream outfile("mru.txt");
    for (const auto& fn : files)
        outfile << fn << std::endl;
}