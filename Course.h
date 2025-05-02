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
    std::string getCourseCode();
    std::vector<Assessment> getAllAssessments();
    bool getIsA5050Course();

    //setter
    void setCourseCode(std::string newCourseCode);
    void setAssessments(std::vector<Assessment> newAssessments);
    void setIsA5050Course(bool newIsA5050Course);

    //assessment management
    void addAssessment(const Assessment& assessment);
    void removeAssessment(int index);
    Assessment& getAssessment(int index);

    double getTotalWeight() const;
    int getAssessmentCount() const;
    bool isTotalWeightValid() const;
    double calculateOverallGrade() const;

};

#endif