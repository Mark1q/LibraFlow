#ifndef FILTER_H
#define FILTER_H

#include "book.h"

class Filter {
public:
    virtual bool matches(const Book& book) const = 0;
    virtual ~Filter() = default;
    Filter();
};

class GenreFilter : public Filter {
    std::string genre;
public:
    explicit GenreFilter(std::string genre) : genre(std::move(genre)) {}
    bool matches(const Book& book) const override {
        return book.getGenre() == genre;
    }
};

class AuthorFilter : public Filter {
    std::string author;
public:
    explicit AuthorFilter(std::string author) : author(std::move(author)) {}
    bool matches(const Book& book) const override {
        return book.getAuthor() == author;
    }
};

class YearFilter : public Filter {
    int year;
public:
    explicit YearFilter(int year) : year(year) {}
    bool matches(const Book& book) const override {
        return book.getYear() == year;
    }
};

#endif // FILTER_H
