#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include "PatronsCollection.h"
#include "Patron.h"

using namespace std;

template<typename T>
T getNumericInput(const string& prompt) {
    T input;
    cout << prompt;
    while (!(cin >> input)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return input;
}

string getStringInput(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin >> ws, input);
    return input;
}

int getIntInput(const string& prompt) {
    int input;
    cout << prompt;
    while (!(cin >> input)) {
        cout << "Invalid input, please enter a number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return input;
}

// Add Patron
void PatronsCollection::AddPatron() {
    cout << "\n--- Add a New Patron ---\n";
    string firstName = getStringInput("Enter patron's first name: ");
    string lastName = getStringInput("Enter patron's last name: ");

    int ID = patronsList.size();
    string fullName = firstName + " " + lastName;

    Patron* patron = new Patron(fullName, ID);
    patronsList.push_back(patron);

    cout << "Patron added successfully.\n";
}

// Search Options
Patron* PatronsCollection::PromptForSearchMechanism() {
    while (true) {
        string method = getStringInput("Search by name or ID? (name/id): ");
        if (method == "name") {
            string name = getStringInput("Enter the patron's full name: ");
            return FindPatronByName(name);
        }
        else if (method == "id") {
            int id = getIntInput("Enter the patron's ID: ");
            return FindPatronByID(id);
        }
        else {
            cout << "Invalid option. Please type 'name' or 'id'.\n";
        }
    }
}

Patron* PatronsCollection::FindPatronByName(string name) {
    for (auto* patron : patronsList) {
        if (patron->getName() == name)
            return patron;
    }
    return nullptr;
}

Patron* PatronsCollection::FindPatronByID(int id) {
    for (auto* patron : patronsList) {
        if (patron->getPatronID() == id)
            return patron;
    }
    return nullptr;
}

// Print All
void PatronsCollection::PrintAllPatrons() const {
    cout << "\n--- List of All Patrons ---\n";
    for (const auto* patron : patronsList) {
        cout << "ID: " << patron->getPatronID()
            << ", Name: " << patron->getName()
            << ", Fines: $" << patron->getFineBalance()
            << ", Books Checked Out: " << patron->getNumBooks()
            << endl;
    }
}

// -----------------------------
// NEW CHECKOUT FUNCTION
// -----------------------------
void PatronsCollection::CheckoutBook() {
    cout << "\n--- Checkout a Book ---\n";
    Patron* patron = PromptForSearchMechanism();

    if (patron != nullptr) {
        if (patron->checkoutBook()) {
            cout << "Book checked out successfully.\n";
        }
        else {
            cout << "Checkout limit reached (5 books max).\n";
        }
    }
    else {
        cout << "Patron not found.\n";
    }
}

// -----------------------------
// NEW RETURN FUNCTION
// -----------------------------
void PatronsCollection::ReturnBook() {
    cout << "\n--- Return a Book ---\n";
    Patron* patron = PromptForSearchMechanism();

    if (patron != nullptr) {
        patron->returnBook();
        cout << "Book returned successfully.\n";
    }
    else {
        cout << "Patron not found.\n";
    }
}