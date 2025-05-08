#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <string>
#include <vector>
#include "Assessment.h"

class Course {
private:

    std::string courseCode;
    std::vector<Assessment> assessments;
    bool isA5050Course;

public:

    //constructor
    Course(std::string courseCode, std::vector<Assessment> assessments, bool isA5050Course);

    //getter
    std::string getCourseCode() const;
    std::vector<Assessment> getAllAssessments() const;
    bool getIsA5050Course() const;

    //setter
    void setCourseCode(std::string newCourseCode);
    void setAssessments(std::vector<Assessment> newAssessments);
    void setIsA5050Course(bool newIsA5050Course);

    //assessment management
    void addAssessment(const Assessment& assessment);
    void removeAssessment(int index);
    Assessment& getAssessment(int index);

    void updateAssessmentName(int index, const std::string& newName);
    void updateAssessmentWeight(int index, double newWeight);
    void updateAssessmentType(int index, bool isTheory);
    void updateAssessmentCompletionStatus(int index, bool isComplete);
    void updateAssessmentGrade(int index, double newGrade);

    double getTotalWeight() const;
    int getAssessmentCount() const;
    int getIncompleteAssessmentCount() const;
    bool isTotalWeightValid() const;
    double calculateOverallGrade(bool careForComplete) const;

    double calculateGradeSoFar(bool careForComplete) const;
    double calculateSectionGradeSoFar(bool isTheory, bool careForComplete) const;

    std::vector<Assessment> calculateRequiredGrades(double goal) const;
    std::vector<Assessment> calculateWhatIf() const;

};

#endif