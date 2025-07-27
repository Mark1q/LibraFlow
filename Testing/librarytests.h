#ifndef LIBRARY_TESTS_H
#define LIBRARY_TESTS_H

#include "testframework.h"

class BookTests : public TestFramework {
public:
    BookTests();
    void registerTests() override;
};

class CSVRepositoryTests : public TestFramework {
public:
    CSVRepositoryTests();
    void registerTests() override;
};

class JSONRepositoryTests : public TestFramework {
public:
    JSONRepositoryTests();
    void registerTests() override;
};

class ControllerTests : public TestFramework {
public:
    ControllerTests();
    void registerTests() override;
};

class FilterTests : public TestFramework {
public:
    FilterTests();
    void registerTests() override;
};

#endif // LIBRARY_TESTS_H
