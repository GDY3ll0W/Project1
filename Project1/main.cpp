#include <iostream>
#include <limits>
#include <string>
#include <cctype>
#include <algorithm>
#include "PatronsCollection.h"
#include "BooksCollection.h"
#include "LoansCollection.h"

void patronOptions(PatronsCollection& patrons) {
    int choice = -1;
    do {
        std::cout << "\n--- Patron Options ---\n";
        std::cout << "1. Add Patron\n";
        std::cout << "2. Edit Patron\n";
        std::cout << "3. Delete Patron\n";
        std::cout << "4. Print All Patrons\n";
        std::cout << "5. Return to Main Menu\n";
        std::cout << "Enter choice: ";
        // Read and validate input as a line to avoid infinite loop on bad input
        std::string line;
        std::getline(std::cin, line);
        // Trim
        auto start = line.find_first_not_of(" \t\r\n");
        auto end = line.find_last_not_of(" \t\r\n");
        if (start == std::string::npos) {
            std::cout << "Please type one of the numbers shown" << std::endl;
            continue;
        }
        std::string token = line.substr(start, end - start + 1);
        // Reject negative numbers and non-digits
        if (token.size() > 0 && token[0] == '-') {
            std::cout << "Please type one of the numbers shown" << std::endl;
            continue;
        }
        bool valid = true;
        size_t idx = 0;
        if (token.size() > 0 && (token[0] == '+' )) idx = 1;
        for (; idx < token.size(); ++idx) {
            if (!std::isdigit(static_cast<unsigned char>(token[idx]))) { valid = false; break; }
        }
        if (!valid) {
            std::cout << "Please type one of the numbers shown" << std::endl;
            continue;
        }
        try {
            choice = std::stoi(token);
        } catch (...) {
            std::cout << "Please type one of the numbers shown" << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                patrons.AddPatron();
                break;
            case 2:
                patrons.EditPatron();
                break;
            case 3:
                patrons.DeletePatron();
                break;
            case 4:
                patrons.PrintAllPatrons();
                break;
            case 5:
                std::cout << "Returning to Main Menu...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}

void bookOptions(BooksCollection& books) {
    int choice = -1;
    do {
        std::cout << "\n--- Book Options ---\n";
        std::cout << "1. Add Book\n";
        std::cout << "2. Edit Book\n";
        std::cout << "3. Delete Book\n";
        std::cout << "4. Print All Books\n";
        std::cout << "5. Return to Main Menu\n";
        std::cout << "Enter choice: ";
        std::string line;
        std::getline(std::cin, line);
        auto start = line.find_first_not_of(" \t\r\n");
        auto end = line.find_last_not_of(" \t\r\n");
        if (start == std::string::npos) {
            std::cout << "Please type one of the numbers shown" << std::endl;
            continue;
        }
        std::string token = line.substr(start, end - start + 1);
        if (token.size() > 0 && token[0] == '-') {
            std::cout << "Please type one of the numbers shown" << std::endl;
            continue;
        }
        bool valid = true;
        size_t idx = 0;
        if (token.size() > 0 && token[0] == '+') idx = 1;
        for (; idx < token.size(); ++idx) {
            if (!std::isdigit(static_cast<unsigned char>(token[idx]))) { valid = false; break; }
        }
        if (!valid) { std::cout << "Please type one of the numbers shown" << std::endl; continue; }
        try { choice = std::stoi(token); } catch (...) { std::cout << "Please type one of the numbers shown" << std::endl; continue; }
        //error occurs around this line?
        switch (choice) {
            case 1:
                books.AddBook();
                break;
            case 2:
                books.EditBook();
                break;
            case 3:
                books.DeleteBook();
                break;
            case 4:
                books.PrintAllBooks();
                break;
            case 5:
                std::cout << "Returning to Main Menu...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}

void loanOptions(LoansCollection& loans, PatronsCollection& patrons, BooksCollection& books) {
    int choice = -1;
    do {
        std::cout << "\n--- Loan Options ---\n";
        std::cout << "1. Check Out Book\n";
        std::cout << "2. Check In Book\n";
        std::cout << "3. List All Overdue Books\n";
        std::cout << "4. List All Checked Out Books\n";
        std::cout << "5. Return to Main Menu\n";
        std::cout << "Enter choice: ";
        std::string line;
        std::getline(std::cin, line);
        auto start = line.find_first_not_of(" \t\r\n");
        auto end = line.find_last_not_of(" \t\r\n");
        if (start == std::string::npos) {
            std::cout << "Please type one of the numbers shown" << std::endl;
            continue;
        }
        std::string token = line.substr(start, end - start + 1);
        if (token.size() > 0 && token[0] == '-') {
            std::cout << "Please type one of the numbers shown" << std::endl;
            continue;
        }
        bool valid = true;
        size_t idx = 0;
        if (token.size() > 0 && token[0] == '+') idx = 1;
        for (; idx < token.size(); ++idx) {
            if (!std::isdigit(static_cast<unsigned char>(token[idx]))) { valid = false; break; }
        }
        if (!valid) { std::cout << "Please type one of the numbers shown" << std::endl; continue; }
        try { choice = std::stoi(token); } catch (...) { std::cout << "Please type one of the numbers shown" << std::endl; continue; }

        switch (choice) {
            case 1:
                loans.CheckOutBook(patrons, books);
                break;
            case 2:
                loans.CheckInBook(patrons, books);
                break;
            case 3:
                loans.ListAllOverdueBooks();
                break;
            case 4:
                loans.ListAllCheckedOutBooks(books);
                break;
            case 5:
                std::cout << "Returning to Main Menu...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}

int main() {
    PatronsCollection patrons;
    BooksCollection books;
    LoansCollection loans;

    int choice = -1;
    do {
        std::cout << "\n--- Library Management System ---\n";
        std::cout << "1. Patron Options\n";
        std::cout << "2. Book Options\n";
        std::cout << "3. Loan Options\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter choice: ";
        std::string line;
        std::getline(std::cin, line);
        auto start = line.find_first_not_of(" \t\r\n");
        auto end = line.find_last_not_of(" \t\r\n");
        if (start == std::string::npos) {
            std::cout << "Please type one of the numbers shown" << std::endl;
            continue;
        }
        std::string token = line.substr(start, end - start + 1);
        if (token.size() > 0 && token[0] == '-') {
            std::cout << "Please type one of the numbers shown" << std::endl;
            continue;
        }
        bool valid = true;
        size_t idx = 0;
        if (token.size() > 0 && token[0] == '+') idx = 1;
        for (; idx < token.size(); ++idx) {
            if (!std::isdigit(static_cast<unsigned char>(token[idx]))) { valid = false; break; }
        }
        if (!valid) { std::cout << "Please type one of the numbers shown" << std::endl; continue; }
        try { choice = std::stoi(token); } catch (...) { std::cout << "Please type one of the numbers shown" << std::endl; continue; }

        switch (choice) {
            case 1:
                patronOptions(patrons);
                break;
            case 2:
                bookOptions(books);
                break;
            case 3:
                loanOptions(loans, patrons, books);
                break;
            case 4:
                std::cout << "Exiting Library Management System. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}
