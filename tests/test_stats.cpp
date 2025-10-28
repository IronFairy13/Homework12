#include "stats.hpp"
#include "tests/gtestlite.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace
{
std::vector<std::string> sample_lines()
{
    return {
        "2539,Clean & quiet apt home by the park,2787,John,Brooklyn,Kensington,40.64749,-73.97237,Private room,149,1,9,2018-10-19,0.21,6,365",
        "2595,Skylit Midtown Castle,2845,Jennifer,Manhattan,Midtown,40.75362,-73.98377,Entire home/apt,225,1,45,2019-05-21,0.38,2,355",
        "5803,\"Lovely Room 1; Garden; Best Area; Legal rental\",9744,Laurie,Brooklyn,South Slope,40.66829,-73.98779,Private room,89,4,167,2019-06-24,1.34,3,314",
        "8490,\"MAISON DES SIRENES1;bohemian apartment\",25183,Nathalie,Brooklyn,Bedford-Stuyvesant,40.68371,-73.94028,Entire home/apt,120,2,88,2019-06-19,0.73,2,233",
        "9518,\"SPACIOUS; LOVELY FURNISHED MANHATTAN BEDROOM\",31374,Shon,Manhattan,Inwood,40.86482,-73.92106,Private room,44,3,108,2019-06-15,1.11,3,311",
        "9657,Modern 1 BR / NYC / EAST VILLAGE,21904,Dana,Manhattan,East Village,40.7292,-73.98542,Entire home/apt,180,14,29,2019-04-19,0.24,1,67"};
}
} 
TEST(Mapper, ExtractsPriceFromSimpleLine)
{
    double price = 0.0;
    ASSERT_TRUE(stats::ExtractPriceFromCsvLine(sample_lines()[0], price));
    EXPECT_DOUBLE_EQ(price, 149.0);
}

TEST(Mapper, HandlesQuotedFields)
{
    double price = 0.0;
    ASSERT_TRUE(stats::ExtractPriceFromCsvLine(sample_lines()[2], price));
    EXPECT_DOUBLE_EQ(price, 89.0);
}

TEST(Mapper, RejectsInvalidPrice)
{
    const std::string invalid_line = "1004,Invalid price example,2004,Dana,Bronx,Melrose,40.40000,-73.40000,Shared room,abc,1,1,2019-01-04,0.10,1,400";
    double price = 0.0;
    EXPECT_FALSE(stats::ExtractPriceFromCsvLine(invalid_line, price));
}

TEST(Reducer, ParsesMapperOutputLine)
{
    double sum = 0.0;
    double sumsq = 0.0;
    std::uint64_t count = 0;
    ASSERT_TRUE(stats::ParseMapperOutputLine("stats\t10\t100\t2", sum, sumsq, count));
    EXPECT_DOUBLE_EQ(sum, 10.0);
    EXPECT_DOUBLE_EQ(sumsq, 100.0);
    EXPECT_EQ(count, static_cast<std::uint64_t>(2));
}

TEST(Reducer, RejectsBrokenMapperOutputLine)
{
    double sum = 0.0;
    double sumsq = 0.0;
    std::uint64_t count = 0;
    EXPECT_FALSE(stats::ParseMapperOutputLine("stats\tNaN\t25\t1", sum, sumsq, count));
    EXPECT_FALSE(stats::ParseMapperOutputLine("stats\t10\t25\tbad", sum, sumsq, count));
}

TEST(Stats, ComputesMeanAndVarianceForSample)
{
    double total_sum = 0.0;
    double total_sumsq = 0.0;
    std::uint64_t total_count = 0;

    for (const auto &line : sample_lines())
    {
        double price = 0.0;
        ASSERT_TRUE(stats::ExtractPriceFromCsvLine(line, price));
        total_sum += price;
        total_sumsq += price * price;
        ++total_count;
    }

    ASSERT_EQ(total_count, static_cast<std::uint64_t>(6));
    EXPECT_DOUBLE_EQ(stats::ComputeMean(total_sum, total_count), 134.5);
    EXPECT_NEAR(stats::ComputeVariance(total_sum, total_sumsq, total_count), 3490.25, 1e-6);
}

int main()
{
    return testing::RunAllTests();
}
