#include "csvrepository.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

CSVRepository::CSVRepository() {}

CSVRepository::CSVRepository(std::string fileName) : fileName(std::move(fileName)) {
    loadFromFile();
}

void CSVRepository::add(const Book& book) {
    books.push_back(book);
    saveToFile();
}

void CSVRepository::remove(int id) {
    auto it = std::remove_if(books.begin(), books.end(),
                             [id](const Book& b) { return b.getId() == id; });

    if (it == books.end()) {
        throw std::out_of_range("Book with ID not found in CSV repository");
    }

    books.erase(it, books.end());
    saveToFile();
}

std::vector<Book> CSVRepository::getAll() const {
    return books;
}

std::unique_ptr<Book> CSVRepository::findById(int id) const {
    for (const auto& b : books) {
        if (b.getId() == id)
            return std::make_unique<Book>(b);
    }
    return nullptr;
}

void CSVRepository::loadFromFile() {
    books.clear();
    std::ifstream in{fileName};

    if (!in.is_open()) {
        // File might not exist yet — don't throw
        return;
    }

    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string item;
        Book book;

        try {
            std::getline(ss, item, ',');
            if (item.empty()) continue;
            book.setId(std::stoi(item));

            std::getline(ss, item, ',');
            book.setTitle(item);

            std::getline(ss, item, ',');
            book.setAuthor(item);

            std::getline(ss, item, ',');
            book.setGenre(item);

            std::getline(ss, item, ',');
            book.setYear(std::stoi(item));

            books.push_back(book);
        } catch (...) {
            // Skip malformed lines
            continue;
        }
    }
}

void CSVRepository::saveToFile() const {
    std::ofstream out{fileName};
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open CSV file for writing.");
    }

    for (const auto& b : books) {
        out << b.getId() << ","
            << b.getTitle() << ","
            << b.getAuthor() << ","
            << b.getGenre() << ","
            << b.getYear() << "\n";
    }
}
