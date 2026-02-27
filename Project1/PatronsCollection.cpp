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

// Forward helper input functions (moved above usages to avoid identifier-not-found errors)
string getStringInput(const string& prompt);
int getIntInput(const string& prompt);

// Definition of static member declared in header
int PatronsCollection::nextPatronID = 0;

// Helper input functions

// Edit a patron's details
void PatronsCollection::EditPatron() {
    cout << "\n--- Edit Patron ---\n";
    Patron* patron = PromptForSearchMechanism();
    if (!patron) {
        cout << "Patron not found.\n";
        return;
    }
    string newName = getStringInput("Enter new full name (leave blank to keep current): ");
    if (!newName.empty()) {
        patron->setName(newName);
    }
    // Optionally edit fines
    cout << "Current fine balance: $" << patron->getFineBalance() << "\n";
    cout << "Enter new fine balance (or press Enter to keep current): ";
    string line;
    getline(cin, line);
    if (!line.empty()) {
        try {
            float f = stof(line);
            patron->setFineBalance(f);
        } catch (...) {
            cout << "Invalid fine value entered; keeping current.\n";
        }
    }
    cout << "Patron updated.\n";
}

// Delete a patron
void PatronsCollection::DeletePatron() {
    cout << "\n--- Delete Patron ---\n";
    Patron* patron = PromptForSearchMechanism();
    if (!patron) {
        cout << "Patron not found.\n";
        return;
    }
    auto it = std::find(patronsList.begin(), patronsList.end(), patron);
    if (it != patronsList.end()) {
        delete *it;
        patronsList.erase(it);
        cout << "Patron deleted.\n";
    }
}

// Print a single patron's details
void PatronsCollection::PrintPatron() {
    Patron* patron = PromptForSearchMechanism();
    if (!patron) {
        cout << "Patron not found.\n";
        return;
    }
    cout << "ID: " << patron->getPatronID() << ", Name: " << patron->getName()
         << ", Fines: $" << patron->getFineBalance() << ", Books Checked Out: " << patron->getNumBooks() << "\n";
}

// Pay fine for a patron
void PatronsCollection::PayFine() {
    Patron* patron = PromptForSearchMechanism();
    if (!patron) {
        cout << "Patron not found.\n";
        return;
    }
    cout << "Current fine balance: $" << patron->getFineBalance() << "\n";
    float amount = getNumericInput<float>("Enter payment amount: ");
    float newBal = patron->getFineBalance() - amount;
    if (newBal < 0) newBal = 0;
    patron->setFineBalance(newBal);
    cout << "Payment applied. New balance: $" << patron->getFineBalance() << "\n";
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

    // Assign a unique incremental ID and create the patron
    int ID = nextPatronID++;
    string fullName = firstName + " " + lastName;

    Patron* patron = new Patron(fullName, ID);
    patronsList.push_back(patron);

    // Print first name, last name and assigned ID, then confirmation message
    cout << "First Name: " << firstName << ", Last Name: " << lastName << ", Assigned ID: " << ID << "\n";
    cout << "Patron added successfully." << "\n";
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
    if (patronsList.empty()) {
        cout << "There are no patrons." << endl;
        return;
    }

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
            cout << "You can only have 3 books checked Out." << endl;
        }
    }
    else {
        cout << "Patron not found.\n";
    }
}


// NEW RETURN FUNCTION

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