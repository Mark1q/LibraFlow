#include "mainwindow.h"
#include "librarytests.h"

#include <vector>
#include <memory>

#include <QApplication>

void runTests() {
    std::vector<std::unique_ptr<TestFramework>> testSuites;
    testSuites.emplace_back(std::make_unique<BookTests>());
    testSuites.emplace_back(std::make_unique<CSVRepositoryTests>());
    testSuites.emplace_back(std::make_unique<JSONRepositoryTests>());
    testSuites.emplace_back(std::make_unique<ControllerTests>());
    testSuites.emplace_back(std::make_unique<FilterTests>());

    for (auto& suite : testSuites) {
        suite->runAllTests();
        suite->printResults();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    runTests();
    w.show();
    return a.exec();
}
