#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <string>
#include <functional>

class TestFramework {
public:
    TestFramework(const std::string& name);
    virtual ~TestFramework() = default;

    void runAllTests();
    void printResults() const;

protected:
    virtual void registerTests() = 0;
    void addTest(const std::string& name, std::function<void()> test);

private:
    std::string frameworkName;
    std::vector<std::pair<std::string, std::function<void()>>> tests;
    std::vector<std::string> passedTests;
    std::vector<std::string> failedTests;
};

#endif // TEST_FRAMEWORK_H
