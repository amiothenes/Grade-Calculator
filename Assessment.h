#ifndef ASSESSMENT_H
#define ASSESSMENT_H

#include <string>

class Assessment {
private:
    std::string name;
    double weight;
    double grade;
    bool isTheory; //theory vs lab
    bool isComplete;

public:
    // Constructor declaration
    Assessment(std::string name, double weight, double grade = 0.0, bool isTheory = true, bool isComplete = false);
    
    // Getters
    std::string getName() const;
    double getWeight() const;
    double getGrade() const;
    bool getIsTheory() const;
    bool getIsComplete() const;

    // Setters
    void setName(std::string newName);
    void setWeight(double newWeight);
    void setGrade(double newGrade);
    void setIsTheory(bool newIsTheory);
    void setIsComplete(bool newStatus);
};

#endif // ASSESSMENT_H