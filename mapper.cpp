#include "stats.hpp"

#include <iostream>
#include <string>

int main()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        double price = 0.0;
        if (!stats::ExtractPriceFromCsvLine(line, price))
        {
            continue;
        }

        const double squared = price * price;
        std::cout << "stats"
                  << '\t' << price
                  << '\t' << squared
                  << '\t' << 1
                  << '\n';
    }

    return 0;
}
