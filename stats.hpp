#pragma once

#include <cstdint>
#include <string>

namespace stats
{
bool ExtractPriceFromCsvLine(const std::string &line, double &price);

bool ParseMapperOutputLine(const std::string &line, double &sum, double &sumsq, std::uint64_t &count);

double ComputeMean(double sum, std::uint64_t count);

double ComputeVariance(double sum, double sumsq, std::uint64_t count);
} 
