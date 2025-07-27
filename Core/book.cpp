#include "book.h"

#include <stdexcept>

const std::unordered_set<std::string> Book::genres = {"SF", "Romance", "Drama", "Fantasy", "History"};

bool Book::validAuthor(std::string author) {
    if (author.empty()) return false;

    for (unsigned char c : author) {
        bool allowed = std::isalpha(c) || std::isspace(c) || c == '.' || c == '-' || c == '\'';
        if (!allowed) {
            //std::cerr << "Invalid char found: '" << c << "' (ASCII " << int(c) << ")\n";
            return false;
        }
    }
    return true;
}

bool Book::validGenre(std::string genre) {
    return genres.find(genre) != genres.end();
}

bool Book::validYear(int year) {
    return ((year > 0) && (year <= 2025));
}

Book::Book() {}

Book::Book(std::string title, std::string author, std::string genre, int year, int id) : title(title), id(id) {
    setAuthor(author);
    setGenre(genre);
    setYear(year);
}

void Book::setAuthor(std::string author) {
    if (!validAuthor(author) || author.empty()) throw std::invalid_argument("Invalid author");

    this->author = std::move(author);
}

void Book::setGenre(std::string genre) {
    if (!validGenre(genre) || genre.empty()) throw std::invalid_argument("Invalid genre");

    this->genre = std::move(genre);
}

void Book::setYear(int year) {
    if (!validYear(year)) throw std::invalid_argument("Invalid year");

    this->year = std::move(year);
}

QJsonObject Book::toJson() const {
    QJsonObject obj;

    obj["title"] = QString::fromStdString(title);
    obj["author"] = QString::fromStdString(author);
    obj["genre"] = QString::fromStdString(genre);
    obj["year"] = year;
    obj["id"] = id;

    return obj;
}

Book Book::fromJson(const QJsonObject& obj) {
    return Book{
        obj["title"].toString().toStdString(),
        obj["author"].toString().toStdString(),
        obj["genre"].toString().toStdString(),
        obj["year"].toInt(),
        obj["id"].toInt()
    };
}

