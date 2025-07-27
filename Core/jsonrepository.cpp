#include "jsonrepository.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <stdexcept>
#include <algorithm>

JSONRepository::JSONRepository() {}

JSONRepository::JSONRepository(const QString& fileName)
    : fileName(fileName) {
    loadFromFile();
}

void JSONRepository::add(const Book& book) {
    books.push_back(book);
    saveToFile();
}

void JSONRepository::remove(int id) {
    auto it = std::remove_if(books.begin(), books.end(),
                             [id](const Book& b) { return b.getId() == id; });

    if (it == books.end())
        throw std::out_of_range("Book with ID not found");

    books.erase(it, books.end());
    saveToFile();
}

std::vector<Book> JSONRepository::getAll() const {
    return books;
}

std::unique_ptr<Book> JSONRepository::findById(int id) const {
    for (const auto& book : books) {
        if (book.getId() == id)
            return std::make_unique<Book>(book);
    }
    return nullptr;
}

void JSONRepository::loadFromFile() {
    books.clear();

    QFile file(fileName);
    if (!file.exists()) return; // Silent if no file yet (valid case)
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Failed to open JSON file for reading.");
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        throw std::runtime_error("JSON parsing failed: " + parseError.errorString().toStdString());
    }

    if (!doc.isArray()) {
        throw std::runtime_error("Invalid JSON structure: expected an array.");
    }

    QJsonArray array = doc.array();
    for (const QJsonValue& val : array) {
        if (!val.isObject()) continue;

        QJsonObject obj = val.toObject();
        Book book;

        if (!obj.contains("id") || !obj.contains("title") || !obj.contains("author") ||
            !obj.contains("genre") || !obj.contains("year")) {
            continue; // skip invalid record
        }

        book.setId(obj["id"].toInt());
        book.setTitle(obj["title"].toString().toStdString());
        book.setAuthor(obj["author"].toString().toStdString());
        book.setGenre(obj["genre"].toString().toStdString());
        book.setYear(obj["year"].toInt());

        books.push_back(book);
    }
}

void JSONRepository::saveToFile() const {
    QJsonArray array;
    for (const auto& book : books) {
        QJsonObject obj;
        obj["id"] = book.getId();
        obj["title"] = QString::fromStdString(book.getTitle());
        obj["author"] = QString::fromStdString(book.getAuthor());
        obj["genre"] = QString::fromStdString(book.getGenre());
        obj["year"] = book.getYear();
        array.append(obj);
    }

    QJsonDocument doc(array);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        throw std::runtime_error("Failed to open JSON file for writing.");
    }

    file.write(doc.toJson());
    file.close();
}
