#pragma once

#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace testing
{
struct TestCase
{
    std::string suite;
    std::string name;
    void (*func)();
};

inline std::vector<TestCase> &registry()
{
    static std::vector<TestCase> tests;
    return tests;
}

inline bool &current_test_failed()
{
    static bool failed = false;
    return failed;
}

inline void report_failure(const std::string &expr, const std::string &message, const char *file, int line)
{
    std::cerr << file << ':' << line << ": Failure\n";
    if (!expr.empty())
    {
        std::cerr << "  Expected: " << message << '\n';
        std::cerr << "  Actual  : " << expr << '\n';
    }
    else
    {
        std::cerr << "  " << message << '\n';
    }
}

struct TestRegistrar
{
    TestRegistrar(const char *suite, const char *name, void (*func)())
    {
        registry().push_back(TestCase{suite, name, func});
    }
};

inline bool expect_true(bool condition, const char *expr, const char *file, int line, const char *message)
{
    if (!condition)
    {
        current_test_failed() = true;
        report_failure(expr, message, file, line);
        return false;
    }
    return true;
}

template <typename L, typename R>
inline bool expect_equal(const L &lhs, const R &rhs, const char *lexpr, const char *rexpr, const char *file, int line)
{
    if (!(lhs == rhs))
    {
        current_test_failed() = true;
        std::cerr << file << ':' << line << ": Failure\n";
        std::cerr << "  Expected equality of these values:\n";
        std::cerr << "    " << lexpr << '\n';
        std::cerr << "    " << rexpr << '\n';
        std::cerr << "  Which are: " << lhs << " and " << rhs << '\n';
        return false;
    }
    return true;
}

inline bool expect_near(double lhs, double rhs, double abs_error, const char *lexpr, const char *rexpr, const char *file, int line)
{
    if (std::fabs(lhs - rhs) > abs_error)
    {
        current_test_failed() = true;
        std::cerr << file << ':' << line << ": Failure\n";
        std::cerr << "  Expected |" << lexpr << " - " << rexpr << "| <= " << abs_error << '\n';
        std::cerr << "  Which is |" << lhs << " - " << rhs << "| = " << std::fabs(lhs - rhs) << '\n';
        return false;
    }
    return true;
}

inline int RunAllTests()
{
    auto &tests = registry();
    std::size_t failed_count = 0;
    std::cout << "[==========] Running " << tests.size() << " test(s)." << std::endl;
    for (const auto &test : tests)
    {
        current_test_failed() = false;
        std::cout << "[ RUN      ] " << test.suite << '.' << test.name << std::endl;
        test.func();
        if (current_test_failed())
        {
            ++failed_count;
            std::cout << "[  FAILED  ] " << test.suite << '.' << test.name << std::endl;
        }
        else
        {
            std::cout << "[       OK ] " << test.suite << '.' << test.name << std::endl;
        }
    }

    if (failed_count == 0U)
    {
        std::cout << "[==========] All tests passed." << std::endl;
        return 0;
    }

    std::cout << "[==========] " << failed_count << " test(s) failed." << std::endl;
    return 1;
}
} // namespace testing

#define TEST(suite, name)                                                                 \
    void suite##_##name();                                                                \
    static ::testing::TestRegistrar suite##_##name##_registrar(#suite, #name, suite##_##name); \
    void suite##_##name()

#define EXPECT_TRUE(cond) ::testing::expect_true((cond), #cond, __FILE__, __LINE__, "Value of: " #cond)
#define EXPECT_FALSE(cond) EXPECT_TRUE(!(cond))
#define ASSERT_TRUE(cond)                                                                 \
    if (!::testing::expect_true((cond), #cond, __FILE__, __LINE__, "Value of: " #cond))   \
    return
#define ASSERT_FALSE(cond)                                                                \
    if (!::testing::expect_true(!(cond), #cond, __FILE__, __LINE__, "Value of: !" #cond)) \
    return

#define EXPECT_EQ(lhs, rhs) ::testing::expect_equal((lhs), (rhs), #lhs, #rhs, __FILE__, __LINE__)
#define ASSERT_EQ(lhs, rhs)                                                               \
    if (!::testing::expect_equal((lhs), (rhs), #lhs, #rhs, __FILE__, __LINE__))            \
    return

#define EXPECT_NEAR(lhs, rhs, abs_error) ::testing::expect_near((lhs), (rhs), (abs_error), #lhs, #rhs, __FILE__, __LINE__)
#define EXPECT_DOUBLE_EQ(lhs, rhs) EXPECT_NEAR((lhs), (rhs), 1e-9)
