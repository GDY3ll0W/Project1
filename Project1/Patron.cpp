#include "Patron.h"
#include <iostream>

// Maximum books allowed per patron
const int Patron::MAX_BOOKS = 5;

// Constructs a Patron with a name and ID.
Patron::Patron(std::string nm, int ID)
    : name(nm), patronID(ID), fineBalance(0.0), numBooks(0) {}

// Default constructor
Patron::Patron()
    : name(""), patronID(0), fineBalance(0.0), numBooks(0) {}

// Getters
std::string Patron::getName() const {
    return name;
}

int Patron::getPatronID() const {
    return patronID;
}

float Patron::getFineBalance() const {
    return fineBalance;
}

int Patron::getNumBooks() const {
    return numBooks;
}

// Setters
void Patron::setName(std::string nm) {
    name = nm;
}

void Patron::setPatronID(int ID) {
    patronID = ID;
}

void Patron::setFineBalance(float bal) {
    fineBalance = bal;
}

void Patron::setNumBooks(int num) {
    numBooks = num;
}

// NEW LIMIT FUNCTIONS


bool Patron::canCheckout() const {
    return numBooks < MAX_BOOKS;
}

bool Patron::checkoutBook() {
    if (canCheckout()) {
        numBooks++;
        return true;
    }
    return false;
}

void Patron::returnBook() {
    if (numBooks > 0) {
        numBooks--;
    }
}