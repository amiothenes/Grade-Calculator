#include "Assessment.h"

// Constructor implementation
Assessment::Assessment(std::string name, double weight, double grade, bool isTheory, bool isComplete) {
    this->name = name;
    this->weight = weight;
    this->grade = grade;
    this->isTheory = isTheory;
    this->isComplete = isComplete;
}

// Getters implementations
std::string Assessment::getName() const { return name; }
double Assessment::getWeight() const { return weight; }
double Assessment::getGrade() const { return grade; }
bool Assessment::getIsTheory() const { return isTheory; }
bool Assessment::getIsComplete() const { return isComplete; }

// Setters implementations
void Assessment::setName(std::string newName) { name = newName; }
void Assessment::setWeight(double newWeight) { weight = newWeight; }
void Assessment::setGrade(double newGrade) { grade = newGrade; }
void Assessment::setIsTheory(bool newIsTheory) { isTheory = newIsTheory; };
void Assessment::setIsComplete(bool newStatus) { isComplete = newStatus; }