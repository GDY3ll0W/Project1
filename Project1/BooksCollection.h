#ifndef BOOKSCOLLECTION_H
#define BOOKSCOLLECTION_H

#include <vector>
#include <string> // Include the string header for std::string (Forgot to add on for the BooksCollection.cpp)
#include "Books.h"

class BooksCollection {
public:
    BooksCollection();
    ~BooksCollection();

    void AddBook();
    void EditBook();
    void DeleteBook();
    Books* PromptForSearchMechanism();
    Books* FindBookByTitle(const std::string& title);
    Books* FindBookByISBN(const std::string& isbn);
    Books* FindBookByID(int id);
    void PrintAllBooks() const;
    void PrintBook();

private:
    std::vector<Books*> booksList;
};

#endif // BOOKSCOLLECTION_H
