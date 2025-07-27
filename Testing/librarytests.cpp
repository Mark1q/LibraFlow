#include "librarytests.h"
#include "book.h"
#include "csvrepository.h"
#include "jsonrepository.h"
#include "controller.h"
#include <fstream>
#include <memory>
#include <cstdio> // For std::remove

// ========== Book Tests ==========
BookTests::BookTests() : TestFramework("Book") {}

void BookTests::registerTests() {
    addTest("Constructor and Getters", [] {
        Book book("1984", "George Orwell", "SF", 1949, 1);
        if (book.getTitle() != "1984") throw std::runtime_error("Title mismatch");
        if (book.getAuthor() != "George Orwell") throw std::runtime_error("Author mismatch");
    });

    addTest("Validation - Invalid Author", [] {
        Book book;
        try {
            book.setAuthor("Author123");
            throw std::runtime_error("Invalid author accepted");
        } catch (const std::invalid_argument& e) {
            if (std::string(e.what()).find("Invalid author") == std::string::npos)
                throw std::runtime_error("Unexpected error message");
        }
    });

    addTest("Validation - Empty Author", [] {
        Book book;
        try {
            book.setAuthor("");
            throw std::runtime_error("Empty author accepted");
        } catch (const std::invalid_argument&) {}
    });

    addTest("Validation - Valid Author", [] {
        Book book;
        try {
            book.setAuthor("J.K. Rowling");
        } catch (...) {
            throw std::runtime_error("Valid author rejected");
        }
    });
}

// ========== CSV Repository Tests ==========
CSVRepositoryTests::CSVRepositoryTests() : TestFramework("CSV Repository") {}

void CSVRepositoryTests::registerTests() {
    addTest("File Operations", [] {
        const std::string filename = "test_books.csv";

        // Setup test file
        std::ofstream out(filename);
        out << "1,1984,George Orwell,SF,1949\n";
        out.close();

        CSVRepository repo(filename);
        if (repo.getAll().size() != 1) throw std::runtime_error("Initial load failed");

        Book newBook("Brave New World", "Aldous Huxley", "SF", 1932, 2);
        repo.add(newBook);
        if (repo.getAll().size() != 2) throw std::runtime_error("Add failed");

        repo.remove(1);
        if (repo.getAll().size() != 1) throw std::runtime_error("Remove failed");

        std::remove(filename.c_str()); // Clean up
    });

    addTest("Remove Non-existent Book", [] {
        const std::string filename = "test_remove_nonexistent.csv";
        std::ofstream(filename).close(); // Create empty file

        CSVRepository repo(filename);
        try {
            repo.remove(999); // Book ID not in repo
            throw std::runtime_error("Removing nonexistent book did not throw");
        } catch (const std::out_of_range&) {
            // Expected
        }

        std::remove(filename.c_str());
    });
}

JSONRepositoryTests::JSONRepositoryTests() : TestFramework("JSON Repository") {}

void JSONRepositoryTests::registerTests() {
    addTest("JSON Operations", [] {
        const std::string filename = "test_books.json";
        {
            JSONRepository repo(QString::fromStdString((filename)));
            Book book("Dune", "Frank Herbert", "SF", 1965, 1);
            repo.add(book);
        }

        JSONRepository repo(QString::fromStdString((filename)));
        auto loaded = repo.getAll();
        if (loaded.empty()) throw std::runtime_error("Add/load failed");
        if (loaded[0].getTitle() != "Dune") throw std::runtime_error("Data corruption");

        std::remove(filename.c_str()); // Clean up
    });
}

ControllerTests::ControllerTests() : TestFramework("Controller") {}

void ControllerTests::registerTests() {
    addTest("CRUD Operations", [] {
        const std::string filename = "test_controller.csv";
        std::ofstream(filename).close(); // Create empty file

        auto repo = std::make_unique<CSVRepository>(filename);
        Controller controller(std::move(repo));

        Book book("The Great Gatsby", "F. Scott Fitzgerald", "Drama", 1925, 1);
        controller.addBook(book);
        if (controller.getAllBooks().size() != 1) throw std::runtime_error("Add failed");

        controller.removeBook(1);
        if (!controller.getAllBooks().empty()) throw std::runtime_error("Remove failed");

        std::remove(filename.c_str());
    });

    addTest("Undo/Redo", [] {
        std::string filename = "test_undo_redo.csv";
        auto repo = std::make_unique<CSVRepository>(filename);
        Controller controller(std::move(repo));

        Book book("Test Book", "Test Author", "SF", 2000, 1);
        controller.addBook(book);
        controller.undo();
        if (!controller.getAllBooks().empty()) throw std::runtime_error("Undo failed");

        controller.redo();
        if (controller.getAllBooks().size() != 1) throw std::runtime_error("Redo failed");

        std::remove(filename.c_str());
    });

    addTest("Undo with No History", [] {
        Controller controller(std::make_unique<CSVRepository>());
        try {
            controller.undo();
            throw std::runtime_error("Undo with no history did not throw");
        } catch (...) {}
    });

    addTest("Redo without Undo", [] {
        Controller controller(std::make_unique<CSVRepository>());
        try {
            controller.redo();
            throw std::runtime_error("Redo with no prior undo did not throw");
        } catch (...) {}
    });
}

FilterTests::FilterTests() : TestFramework("Filter") {}

void FilterTests::registerTests() {
    addTest("Filter by Author", [] {
        std::string filename = "test_filtering.csv";
        auto repo = std::make_unique<CSVRepository>(filename);
        Controller controller(std::move(repo));

        controller.addBook(Book("Book1", "Author One", "SF", 2000, 1));
        controller.addBook(Book("Book2", "Author Two", "Fantasy", 2010, 2));

        auto filtered = controller.filterBooks([](const Book& b) {
            return b.getAuthor() == "Author One";
        });
        if (filtered.size() != 1) throw std::runtime_error("Filter failed");

        std::remove(filename.c_str());
    });

    addTest("Filter with No Match", [] {
        std::string filename = "test_filtering.csv";
        auto repo = std::make_unique<CSVRepository>(filename);
        Controller controller(std::move(repo));

        controller.addBook(Book("Book1", "Author A", "SF", 2000, 1));
        auto filtered = controller.filterBooks([](const Book& b) {
            return b.getAuthor() == "Nonexistent Author";
        });
        if (!filtered.empty()) throw std::runtime_error("Filter should return no results");

        std::remove(filename.c_str());
    });

    addTest("Filter Multiple Matches", [] {
        std::string filename = "test_filtering.csv";
        auto repo = std::make_unique<CSVRepository>(filename);
        Controller controller(std::move(repo));

        controller.addBook(Book("Book1", "Same Author", "SF", 2000, 1));
        controller.addBook(Book("Book2", "Same Author", "Drama", 2015, 2));

        auto filtered = controller.filterBooks([](const Book& b) {
            return b.getAuthor() == "Same Author";
        });
        if (filtered.size() != 2) throw std::runtime_error("Filter multiple match failed");

        std::remove(filename.c_str());
    });
}
