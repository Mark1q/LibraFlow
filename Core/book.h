#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <unordered_set>
#include <cctype>

#include <QJsonObject>

class Book
{
public:
    Book();
    Book(std::string title, std::string author, std::string genre, int year, int id);

    void setTitle(std::string title) { this->title = std::move(title); }
    void setAuthor(std::string author);
    void setGenre(std::string genre);
    void setYear(int year);
    void setId(int id) { this->id = std::move(id); }

    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getGenre() const { return genre; }
    int getYear() const { return year; }
    int getId() const { return id; }

    QJsonObject toJson() const;
    static Book fromJson(const QJsonObject& obj);
private:
    std::string title, author, genre;
    int year, id;

    static const std::unordered_set<std::string> genres;

    static bool validAuthor(std::string author);
    static bool validGenre(std::string genre);
    static bool validYear(int year);
};

#endif // BOOK_H
