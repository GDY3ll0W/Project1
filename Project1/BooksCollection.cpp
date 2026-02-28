#include "BooksCollection.h"
#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <cctype>
#include <sstream>

// helpers (file-local)
static std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) ++start;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) --end;
    return s.substr(start, end - start);
}

static std::string toLower(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (unsigned char c : s) out.push_back(static_cast<char>(std::tolower(c)));
    return out;
}

BooksCollection::BooksCollection() {}
BooksCollection::~BooksCollection() {
    for (Books* b : booksList) delete b;
    booksList.clear();
}

void BooksCollection::AddBook() {
    // Use getline with std::ws to skip any leftover whitespace/newline so the user
    // doesn't have to press Enter twice.
    std::string author, title, line;
    std::string isbn;
    int libraryID = 0;
    float cost = 0.0f;
    int statusChoice = 0;

    // Author: allow letters and spaces only
    auto isAlphaSpaceOnly = [](const std::string& s) {
        if (s.empty()) return false;
        bool hasLetter = false;
        for (unsigned char ch : s) {
            if (std::isalpha(ch)) hasLetter = true;
            if (!(std::isalpha(ch) || std::isspace(ch))) return false;
        }
        return hasLetter;
    };

    while (true) {
        std::cout << "Enter author: ";
        if (!std::getline(std::cin >> std::ws, author)) return;
        author = trim(author);
        if (!isAlphaSpaceOnly(author)) {
            std::cout << "Please enter letters and spaces only for the author." << std::endl;
            continue;
        }
        break;
    }

    // Title: allow letters and spaces only
    while (true) {
        std::cout << "Enter title: ";
        if (!std::getline(std::cin >> std::ws, title)) return;
        title = trim(title);
        if (!isAlphaSpaceOnly(title)) {
            std::cout << "Please enter letters and spaces only for the title." << std::endl;
            continue;
        }
        break;
    }

    // Read integers/floats from full lines to avoid mixing operator>> and getline.
    // Read ISBN as a 10-digit numeric string (exactly 10 digits required)
    while (true) {
        std::cout << "Enter ISBN (10 digits): ";
        if (!std::getline(std::cin, line)) return;
        line = trim(line);
        if (line.empty()) continue;
        // Validate exactly 10 digits
        bool ok = (line.size() == 10);
        for (char c : line) if (!std::isdigit(static_cast<unsigned char>(c))) { ok = false; break; }
        if (!ok) {
            std::cout << "Please write 10 numbers" << std::endl;
            continue;
        }
        isbn = line;
        break;
    }

    while (true) {
        std::cout << "Enter Library ID (positive integer): ";
        if (!std::getline(std::cin, line)) return;
        line = trim(line);
        if (line.empty()) continue;
        // Validate digits only
        bool ok = true;
        for (char c : line) if (!std::isdigit(static_cast<unsigned char>(c))) { ok = false; break; }
        if (!ok) { std::cout << "Please enter a positive integer for Library ID" << std::endl; continue; }
        try {
            libraryID = std::stoi(line);
            if (libraryID <= 0) { std::cout << "Please enter a positive integer for Library ID" << std::endl; continue; }
            break;
        } catch (...) {
            std::cout << "Please enter a positive integer for Library ID" << std::endl;
        }
    }

    while (true) {
        std::cout << "Enter cost (positive number): ";
        if (!std::getline(std::cin, line)) return;
        line = trim(line);
        if (line.empty()) continue;
        // Validate positive decimal number with optional one dot
        bool ok = false;
        int dots = 0;
        int digits = 0;
        for (unsigned char c : line) {
            if (std::isdigit(c)) { ok = true; ++digits; }
            else if (c == '.') { ++dots; if (dots > 1) { ok = false; break; } }
            else { ok = false; break; }
        }
        if (!ok || digits == 0) { std::cout << "Please enter a non-negative number" << std::endl; continue; }
        try {
            cost = std::stof(line);
            if (cost < 0.0f) { std::cout << "Please enter a non-negative number" << std::endl; continue; }
            break;
        } catch (...) {
            std::cout << "Please enter a non-negative number" << std::endl;
        }
    }

    while (true) {
        std::cout << "Select status (0 = IN, 1 = OUT, 2 = LOST): ";
        if (!std::getline(std::cin, line)) return;
        line = trim(line);
        if (line.empty()) continue;
        try {
            statusChoice = std::stoi(line);
            if (statusChoice >= 0 && statusChoice <= 2) break;
        } catch (...) {}
        std::cout << "Invalid choice. Enter 0, 1, or 2.\n";
    }

    Books::BookStatus status = static_cast<Books::BookStatus>(statusChoice);

    Books* newBook = new Books(author, title, isbn, libraryID, cost, status);
    booksList.push_back(newBook);

    std::cout << "Book added: \"" << title << "\" by " << author << '\n';
}

void BooksCollection::EditBook() {
    Books* book = PromptForSearchMechanism();
    if (!book) {
        std::cout << "Book not found.\n";
        return;
    }

    std::cout << "Editing Book: " << book->getTitle() << "\n";
    std::cout << "Select the attribute to edit: \n";
    std::cout << "1. Title\n";
    std::cout << "2. Author\n";
    std::cout << "3. ISBN\n";
    std::cout << "4. Cost\n";
    std::cout << "Enter choice: ";
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the newline character from the buffer

    switch (choice) {
        case 1: {
            // Validate title: letters and spaces only
            auto isAlphaSpaceOnly = [](const std::string& s) {
                if (s.empty()) return false;
                bool hasLetter = false;
                for (unsigned char ch : s) {
                    if (std::isalpha(ch)) hasLetter = true;
                    if (!(std::isalpha(ch) || std::isspace(ch))) return false;
                }
                return hasLetter;
            };

            std::string newTitle;
            while (true) {
                std::cout << "Enter new title: ";
                if (!std::getline(std::cin, newTitle)) { std::cout << "Invalid title. No change made.\n"; break; }
                newTitle = trim(newTitle);
                if (!isAlphaSpaceOnly(newTitle)) {
                    std::cout << "Please enter letters and spaces only for the title." << std::endl;
                    continue;
                }
                book->setTitle(newTitle);
                break;
            }
            break;
        }
        case 2: {
            // Validate author: letters and spaces only
            auto isAlphaSpaceOnly = [](const std::string& s) {
                if (s.empty()) return false;
                bool hasLetter = false;
                for (unsigned char ch : s) {
                    if (std::isalpha(ch)) hasLetter = true;
                    if (!(std::isalpha(ch) || std::isspace(ch))) return false;
                }
                return hasLetter;
            };

            std::string newAuthor;
            while (true) {
                std::cout << "Enter new author: ";
                if (!std::getline(std::cin, newAuthor)) { std::cout << "Invalid author. No change made.\n"; break; }
                newAuthor = trim(newAuthor);
                if (!isAlphaSpaceOnly(newAuthor)) {
                    std::cout << "Please enter letters and spaces only for the author." << std::endl;
                    continue;
                }
                book->setAuthor(newAuthor);
                break;
            }
            break;
        }
        case 3: {
            std::string newISBN;
            std::cout << "Enter new ISBN (10 digits): ";
            if (!std::getline(std::cin, newISBN)) { std::cout << "Invalid ISBN. No change made.\n"; break; }
            newISBN = trim(newISBN);
            bool ok = (newISBN.size() == 10);
            for (char c : newISBN) if (!std::isdigit(static_cast<unsigned char>(c))) { ok = false; break; }
            if (!ok) {
                std::cout << "Please write 10 numbers" << std::endl;
            } else {
                book->setISBN(newISBN);
            }
            break;
        }
        case 4: {
            // Read cost as a positive decimal number (no letters, no negatives)
            std::string costLine;
            while (true) {
                std::cout << "Enter new cost: ";
                if (!std::getline(std::cin, costLine)) { std::cout << "Invalid cost. No change made.\n"; break; }
                costLine = trim(costLine);
                if (costLine.empty()) { std::cout << "Please enter a positive number" << std::endl; continue; }
                bool ok = false;
                int dots = 0;
                int digits = 0;
                for (unsigned char c : costLine) {
                    if (std::isdigit(c)) { ok = true; ++digits; }
                    else if (c == '.') { ++dots; if (dots > 1) { ok = false; break; } }
                    else { ok = false; break; }
                }
                if (!ok || digits == 0) { std::cout << "Please enter a non-negative number" << std::endl; continue; }
                try {
                    float newCost = std::stof(costLine);
                    if (newCost < 0.0f) { std::cout << "Please enter a non-negative number" << std::endl; continue; }
                    book->setCost(newCost);
                } catch (...) {
                    std::cout << "Please enter a non-negative number" << std::endl;
                    continue;
                }
                break;
            }
            break;
        }
        default:
            std::cout << "Invalid choice. Returning to main menu.\n";
    }

    std::cout << "Book updated successfully.\n";
}


void BooksCollection::DeleteBook() {
    Books* book = PromptForSearchMechanism();
    if (!book) {
        std::cout << "Book not found.\n";
        return;
    }

    auto it = std::find_if(booksList.begin(), booksList.end(), [book](const Books* b) { return b == book; });
    if (it != booksList.end()) {
        delete *it;
        booksList.erase(it);
        std::cout << "Book deleted successfully.\n";
    } else {
        std::cout << "Error deleting the book.\n";
    }
}

Books* BooksCollection::PromptForSearchMechanism() {
    std::string line;
    int choice = 0;

    // Read choice as a full line and parse it
    while (true) {
        std::cout << "Search by (1) Title, (2) ISBN, or (3) ID? ";
        if (!std::getline(std::cin, line)) return nullptr;
        line = trim(line);
        if (line.empty()) continue;
        try {
            choice = std::stoi(line);
        } catch (...) {
            std::cout << "Invalid input. Please enter 1, 2 or 3.\n";
            continue;
        }
        if (choice >= 1 && choice <= 3) break;
        std::cout << "Please enter 1, 2 or 3.\n";
    }

    if (choice == 1) {
        std::string title;
        std::cout << "Enter title: ";
        if (!std::getline(std::cin, title)) return nullptr;
        title = trim(title);
        return FindBookByTitle(title);
    } else if (choice == 2) {
        std::string isbnInput;
        while (true) {
            std::cout << "Enter ISBN (10 digits): ";
            if (!std::getline(std::cin, isbnInput)) return nullptr;
            isbnInput = trim(isbnInput);
            if (isbnInput.empty()) continue;
            bool ok = (isbnInput.size() == 10);
            for (char c : isbnInput) if (!std::isdigit(static_cast<unsigned char>(c))) { ok = false; break; }
            if (!ok) {
                std::cout << "Please write 10 numbers" << std::endl;
                continue;
            }
            break;
        }
        return FindBookByISBN(isbnInput);
    } else { // choice == 3
        int id = 0;
        while (true) {
            std::cout << "Enter ID: ";
            if (!std::getline(std::cin, line)) return nullptr;
            line = trim(line);
            if (line.empty()) continue;
            try {
                id = std::stoi(line);
                break;
            } catch (...) {
                std::cout << "Invalid ID. Enter an integer.\n";
            }
        }
        return FindBookByID(id);
    }
}

Books* BooksCollection::FindBookByTitle(const std::string& title) {
    const std::string key = toLower(trim(title));
    for (auto* book : booksList) {
        if (toLower(trim(book->getTitle())) == key) return book;
    }
    return nullptr;
}

Books* BooksCollection::FindBookByISBN(const std::string& isbn) {
    for (auto* book : booksList) {
        if (book->getISBN() == isbn) return book;
    }
    return nullptr;
}

Books* BooksCollection::FindBookByID(int id) {
    for (auto* book : booksList) {
        if (book->getLibraryID() == id) return book;
    }
    return nullptr;
}

void BooksCollection::PrintAllBooks() const {
    if (booksList.empty()) {
        std::cout << "There are no books to print" << std::endl;
        return;
    }

    for (const auto* book : booksList) {
        std::cout << "ID: " << book->getLibraryID() << ", Title: " << book->getTitle() 
                  << ", Author: " << book->getAuthor() << ", ISBN: " << book->getISBN() 
                  << ", Cost: $" << book->getCost() << std::endl;
    }
}

void BooksCollection::PrintBook() {
    Books* book = PromptForSearchMechanism();
    if (book) {
        std::cout << "ID: " << book->getLibraryID() << ", Title: " << book->getTitle() 
                  << ", Author: " << book->getAuthor() << ", ISBN: " << book->getISBN() 
                  << ", Cost: $" << book->getCost() << std::endl;
    } else {
        std::cout << "Book not found.\n";
    }
}
