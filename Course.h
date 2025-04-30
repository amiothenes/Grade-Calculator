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
    Course(std::string courseCode, std::vector<Assessment> assessments, bool isA5050Course) {
        this->courseCode = courseCode;
        this->assessments = assessments;
        this->isA5050Course = isA5050Course;
    }

    //getter
    std::string getCourseCode();

    //setter
    void setCourseCode(std::string newCourseCode);

    //assessment management
    void addAssessment(const Assessment& assessment);
    void removeAssessment(int index);
    Assessment& getAssessment(int index);
    std::vector<Assessment&> getAllAssessment();
    int getTotalWeight() const;
    int getAssessmentCount() const;
    double calculateOverallGrade() const;

};

#endif