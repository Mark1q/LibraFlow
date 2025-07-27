#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "repository.h"
#include "commands.h"

#include <stack>
#include <memory>
#include <vector>

class Controller
{
public:
    Controller();
    explicit Controller(std::unique_ptr<Repository> repo);

    void addBook(const Book& book);
    void removeBook(int id);
    void updateBook(const Book& book);

    std::vector<Book> getAllBooks() const;
    std::unique_ptr<Book> findBook(int id) const;

    // Undo/Redo
    void undo();
    void redo();

    // Filtering
    std::vector<Book> filterBooks(const std::function<bool(const Book&)>& filterFn) const;
private:
    std::unique_ptr<Repository> repo;

    std::stack<std::unique_ptr<Commands>> undoStack;
    std::stack<std::unique_ptr<Commands>> redoStack;
};

#endif // CONTROLLER_H
