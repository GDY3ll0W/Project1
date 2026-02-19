#include<iostream>
#include<string>
#include<vector>
#include <limits>
#include <algorithm>
#include <cctype>
#include "PatronsCollection.h"
#include "Patron.h"

using namespace std;

// Static unique ID initialization
int PatronsCollection::nextPatronID = 1;

// This is included as a reference if you wish to use this type of template style coding for hw4 or beyond.
template<typename T>
T getNumericInput(const string& prompt) {
    T input;
    cout << prompt;
    while (!(cin >> input)) {
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard the line
        cout << "Invalid input. Please enter a valid number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard any extra input to prepare for next input
    return input;
}

// Function to get a string input from the user with a prompt
string getStringInput(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin >> ws, input); // Properly handles leading whitespace
    return input;
}

// Function to get integer input from the user with a prompt
int getIntInput(const string& prompt) {
    int input;
    cout << prompt;
    while(!(cin >> input)) {
        cout << "Invalid input, please enter a number: ";
        cin.clear(); // clear the error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard input
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard any remaining input
    return input;
}

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

// Adds a new patron to the collection
void PatronsCollection::AddPatron() {
    cout << "\n--- Add a New Patron ---\n";
    string firstName = getStringInput("Enter patron's first name: ");
    string lastName = getStringInput("Enter patron's last name: ");
    int ID = nextPatronID++; // Use unique incremental ID

    string fullName = firstName + " " + lastName;
    auto* patron = new Patron(fullName, ID);
    patronsList.push_back(patron);
    cout << "Patron added successfully. Assigned ID: " << ID << "\n";
}

// Prompts the user to choose a search mechanism and returns the corresponding Patron
Patron* PatronsCollection::PromptForSearchMechanism() {
    while (true) {
        string method = getStringInput("Search by name or ID? (name/id): ");
        method = toLower(trim(method));
        if (method == "name" || method == "n") {
            string name = getStringInput("Enter the patron's full name (or partial name): ");
            return FindPatronByName(name);
        } else if (method == "id" || method == "i") {
            int id = getIntInput("Enter the patron's ID: ");
            return FindPatronByID(id);
        } else {
            cout << "Invalid option. Please type 'name' or 'id'.\n";
        }
    }
}

// Finds a patron by name (case-insensitive, allows partial match and resolves multiple matches)
Patron* PatronsCollection::FindPatronByName(string name) {
    string key = toLower(trim(name));
    if (key.empty()) return nullptr;

    vector<Patron*> exactMatches;
    vector<Patron*> partialMatches;

    for (auto* patron : patronsList) {
        string pname = toLower(trim(patron->getName()));
        if (pname == key) {
            exactMatches.push_back(patron);
        } else if (pname.find(key) != string::npos) {
            partialMatches.push_back(patron);
        }
    }

    // If exactly one exact match, return it
    if (exactMatches.size() == 1) return exactMatches.front();

    // If multiple exact matches, ask the user to disambiguate
    if (exactMatches.size() > 1) {
        cout << "Multiple exact matches found:\n";
        for (auto* p : exactMatches) {
            cout << "ID: " << p->getPatronID() << "  Name: " << p->getName() << "\n";
        }
        int chosen = getIntInput("Enter the ID of the correct patron: ");
        return FindPatronByID(chosen);
    }

    // No exact matches -> handle partial matches
    if (partialMatches.empty()) {
        // nothing found
        return nullptr;
    }

    if (partialMatches.size() == 1) {
        return partialMatches.front();
    }

    // multiple partial matches -> disambiguate
    cout << "Multiple partial matches found:\n";
    for (auto* p : partialMatches) {
        cout << "ID: " << p->getPatronID() << "  Name: " << p->getName() << "\n";
    }
    int chosen = getIntInput("Enter the ID of the correct patron: ");
    return FindPatronByID(chosen);
}

// Finds a patron by ID
Patron* PatronsCollection::FindPatronByID(int id) {
    for (auto* patron : patronsList) {
        if (patron->getPatronID() == id) {
            return patron;
        }
    }
    return nullptr; // Patron not found
}

// Prints all patrons in the collection
void PatronsCollection::PrintAllPatrons() const {
    cout << "\n--- List of All Patrons ---\n";
    for (const auto* patron : patronsList) {
        cout << "ID: " << patron->getPatronID() << ", Name: " << patron->getName() << ", Fines: $" << patron->getFineBalance() << ", Books Checked Out: " << patron->getNumBooks() << endl;
    }
}

// Edits an existing patron's details
void PatronsCollection::EditPatron() {
    cout << "\n--- Edit a Patron ---\n";
    Patron* patron = PromptForSearchMechanism();
    if (patron != nullptr) {
        string firstName = getStringInput("Enter patron's new first name: ");
        string lastName = getStringInput("Enter patron's new last name: ");
        patron->setName(firstName + " " + lastName);
        cout << "Patron updated successfully.\n";
    } else {
        cout << "Patron not found.\n";
    }
}

// Deletes a patron from the collection
void PatronsCollection::DeletePatron() {
    cout << "\n--- Delete a Patron ---\n";
    Patron* patron = PromptForSearchMechanism();
    if (patron != nullptr) {
        auto it = find(patronsList.begin(), patronsList.end(), patron);
        if (it != patronsList.end()) {
            delete *it; // Free the memory
            patronsList.erase(it); // Remove from the list
            cout << "Patron deleted successfully.\n";
        }
    } else {
        cout << "Patron not found.\n";
    }
}

// Prints details of a specific patron
void PatronsCollection::PrintPatron() {
    cout << "\n--- Print a Patron's Details ---\n";
    Patron* patron = PromptForSearchMechanism();
    if (patron != nullptr) {
        cout << "ID: " << patron->getPatronID() << ", Name: " << patron->getName() << ", Fines: $" << patron->getFineBalance() << ", Books Checked Out: " << patron->getNumBooks() << endl;
    } else {
        cout << "Patron not found.\n";
    }
}

// Handles fine payment for a patron
void PatronsCollection::PayFine() {
    cout << "\n--- Pay a Patron's Fine ---\n";
    Patron* patron = PromptForSearchMechanism();
    if (patron != nullptr) {
        cout << "Current Fine: $" << patron->getFineBalance() << endl;
        float amount = getNumericInput<float>("Enter payment amount: $");
        if (amount > 0) {
            float newBalance = max(0.0f, patron->getFineBalance() - amount);
            patron->setFineBalance(newBalance);
            cout << "New Fine Balance: $" << newBalance << endl;
        } else {
            cout << "Invalid payment amount.\n";
        }
    } else {
        cout << "Patron not found.\n";
    }
}
