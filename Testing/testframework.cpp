#include "testframework.h"
#include <iostream>

TestFramework::TestFramework(const std::string& name) : frameworkName(name) {}

void TestFramework::runAllTests() {
    registerTests();
    std::cout << "=== Running " << frameworkName << " Tests ===\n";

    for (const auto& [name, test] : tests) {
        std::cout << "Running test: " << name << "... ";
        try {
            test();
            passedTests.push_back(name);
            std::cout << "PASSED\n";
        } catch (const std::exception& e) {
            failedTests.push_back(name);
            std::cout << "FAILED (" << e.what() << ")\n";
        }
    }

    std::cout << "=== Tests Complete ===\n\n";
}

void TestFramework::printResults() const {
    std::cout << "\n=== Test Results ===\n";
    std::cout << "Framework: " << frameworkName << "\n";
    std::cout << "Passed: " << passedTests.size() << "\n";
    std::cout << "Failed: " << failedTests.size() << "\n";

    if (!failedTests.empty()) {
        std::cout << "\nFailed tests:\n";
        for (const auto& test : failedTests) {
            std::cout << " - " << test << "\n";
        }
    }
}

void TestFramework::addTest(const std::string& name, std::function<void()> test) {
    tests.emplace_back(name, test);
}
