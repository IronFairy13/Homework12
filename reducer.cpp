#include "stats.hpp"

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>

int main()
{
    double total_sum = 0.0;
    double total_sumsq = 0.0;
    std::uint64_t total_count = 0;

    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line.empty())
        {
            continue;
        }

        double value = 0.0;
        double squared = 0.0;
        std::uint64_t count = 0;
        if (!stats::ParseMapperOutputLine(line, value, squared, count))
        {
            continue;
        }

        total_sum += value;
        total_sumsq += squared;
        total_count += count;
    }

    if (total_count == 0)
    {
        return 0;
    }

    const double mean = stats::ComputeMean(total_sum, total_count);
    const double variance = stats::ComputeVariance(total_sum, total_sumsq, total_count);

    std::cout.setf(std::ios::fixed);
    std::cout << std::setprecision(6);
    std::cout << "mean\t" << mean << '\n';
    std::cout << "variance\t" << variance << '\n';

    return 0;
}
