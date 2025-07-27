#ifndef COMMANDS_H
#define COMMANDS_H

#include "repository.h"

class Commands {
public:
    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual ~Commands() = default;
    Commands();
};

class AddCommand : public Commands {
    Repository* repo;
    Book book;
public:
    AddCommand(Repository* repo, const Book& book) : repo(repo), book(book) {}
    void undo() override { repo->remove(book.getId()); }
    void redo() override { repo->add(book); }
};

class RemoveCommand : public Commands {
    Repository* repo;
    Book book;
public:
    RemoveCommand(Repository* repo, const Book& book) : repo(repo), book(book) {}
    void undo() override { repo->add(book); }
    void redo() override { repo->remove(book.getId()); }
};

class UpdateCommand : public Commands {
    Repository* repo;
    Book oldBook, newBook;
public:
    UpdateCommand(Repository* repo, const Book& oldBook, const Book& newBook)
        : repo(repo), oldBook(oldBook), newBook(newBook) {}
    void undo() override {
        repo->remove(newBook.getId());
        repo->add(oldBook);
    }
    void redo() override {
        repo->remove(oldBook.getId());
        repo->add(newBook);
    }
};

#endif // COMMANDS_H
