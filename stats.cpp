#include "stats.hpp"

#include <cctype>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

namespace
{
constexpr std::size_t PRICE_INDEX = 9; 

bool parse_double(const std::string &token, double &value)
{
    if (token.empty())
    {
        return false;
    }

    char *end_ptr = nullptr;
    value = std::strtod(token.c_str(), &end_ptr);
    if (end_ptr == token.c_str())
    {
        return false;
    }
    while (end_ptr && *end_ptr != '\0' && std::isspace(static_cast<unsigned char>(*end_ptr)))
    {
        ++end_ptr;
    }
    if (end_ptr && *end_ptr != '\0')
    {
        return false;
    }
    if (!std::isfinite(value))
    {
        return false;
    }
    return true;
}

bool parse_uint64(const std::string &token, std::uint64_t &value)
{
    if (token.empty())
    {
        return false;
    }

    char *end_ptr = nullptr;
    unsigned long long raw = std::strtoull(token.c_str(), &end_ptr, 10);
    if (end_ptr == token.c_str())
    {
        return false;
    }
    while (end_ptr && *end_ptr != '\0' && std::isspace(static_cast<unsigned char>(*end_ptr)))
    {
        ++end_ptr;
    }
    if (end_ptr && *end_ptr != '\0')
    {
        return false;
    }
    value = static_cast<std::uint64_t>(raw);
    return true;
}
} 

namespace stats
{
bool ExtractPriceFromCsvLine(const std::string &line, double &price)
{
    std::vector<std::string> fields;
    fields.reserve(16);

    std::string field;
    bool in_quotes = false;

    for (std::size_t i = 0; i < line.size(); ++i)
    {
        char ch = line[i];
        if (in_quotes)
        {
            if (ch == '"')
            {
                if (i + 1 < line.size() && line[i + 1] == '"')
                {
                    field.push_back('"');
                    ++i;
                }
                else
                {
                    in_quotes = false;
                }
            }
            else
            {
                field.push_back(ch);
            }
        }
        else
        {
            if (ch == '"')
            {
                in_quotes = true;
            }
            else if (ch == ',')
            {
                fields.push_back(field);
                field.clear();
            }
            else
            {
                field.push_back(ch);
            }
        }
    }
    fields.push_back(field);

    if (fields.size() <= PRICE_INDEX)
    {
        return false;
    }

    return parse_double(fields[PRICE_INDEX], price);
}

bool ParseMapperOutputLine(const std::string &line, double &sum, double &sumsq, std::uint64_t &count)
{
    if (line.empty())
    {
        return false;
    }

    std::size_t pos = line.find('\t');
    if (pos == std::string::npos)
    {
        return false;
    }
    pos += 1; 

    std::size_t next = line.find('\t', pos);
    if (next == std::string::npos)
    {
        return false;
    }
    std::string sum_str = line.substr(pos, next - pos);
    pos = next + 1;

    next = line.find('\t', pos);
    if (next == std::string::npos)
    {
        return false;
    }
    std::string sumsq_str = line.substr(pos, next - pos);
    pos = next + 1;

    std::string count_str = line.substr(pos);

    if (!parse_double(sum_str, sum))
    {
        return false;
    }
    if (!parse_double(sumsq_str, sumsq))
    {
        return false;
    }
    if (!parse_uint64(count_str, count))
    {
        return false;
    }

    return true;
}

double ComputeMean(double sum, std::uint64_t count)
{
    if (count == 0)
    {
        return 0.0;
    }
    return sum / static_cast<double>(count);
}

double ComputeVariance(double sum, double sumsq, std::uint64_t count)
{
    if (count == 0)
    {
        return 0.0;
    }
    const double mean = ComputeMean(sum, count);
    double variance = (sumsq / static_cast<double>(count)) - mean * mean;
    if (variance < 0.0 && std::fabs(variance) < 1e-9)
    {
        variance = 0.0;
    }
    return variance;
}
} 
