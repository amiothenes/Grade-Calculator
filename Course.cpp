#include "Course.h"
#include <cmath>

Course::Course(std::string courseCode, std::vector<Assessment> assessments, bool isA5050Course) {
    this->courseCode = courseCode;
    this->assessments = assessments;
    this->isA5050Course = isA5050Course;
}

std::string Course::getCourseCode() const {
    return courseCode;
}

std::vector<Assessment> Course::getAllAssessments() const {
    return assessments;
}

bool Course::getIsA5050Course() const {
    return isA5050Course;
}

void Course::setCourseCode(std::string newCourseCode) {
    courseCode = newCourseCode;
}

void Course::setAssessments(std::vector<Assessment> newAssessments) {
    assessments = newAssessments;
}

void Course::setIsA5050Course(bool newIsA5050Course) {
    isA5050Course = newIsA5050Course;
}

// Assessment Management
void Course::addAssessment(const Assessment& assessment) {
    assessments.push_back(assessment);
}

void Course::removeAssessment(int index) {
    if (index >= 0 && index < assessments.size()) {
        assessments.erase(assessments.begin() + index);
    }
}

Assessment& Course::getAssessment(int index) {
    return assessments[index];
}

int Course::getAssessmentCount() const {
    return assessments.size();
}

double Course::getTotalWeight() const {
    double totalWeight = 0.0;

    for (const Assessment& assessment : assessments) {
        if (assessment.getIsComplete()) {
            totalWeight += assessment.getWeight();
        }
    }

    return totalWeight;
}

bool Course::isTotalWeightValid() const {
    double totalWeight = getTotalWeight();
    return totalWeight == 100.0;
}

double Course::calculateOverallGrade() const {
    double myGradesWeighted = 0.0;
    double totalWeight = 0.0;

    for(const Assessment& assessment : assessments) {
        if (assessment.getIsComplete()) {
            myGradesWeighted += assessment.getGrade() * assessment.getWeight();
            totalWeight += assessment.getWeight();
        }
    }

    if (totalWeight == 0.0) {
        return 0.0;
    }

    double result = myGradesWeighted / 100; //100 is total

    return std::round(result * 100) / 100; //round two dec pts
}

double Course::calculateGradeSoFar() const {
    double myGradesWeighted = 0.0;
    double totalWeight = 0.0;

    for(const Assessment& assessment : assessments) {
        if (assessment.getIsComplete()) {
            myGradesWeighted += assessment.getGrade() * assessment.getWeight();
            totalWeight += assessment.getWeight();
        }
    }

    if (totalWeight == 0.0) {
        return 0.0;
    }

    double result = myGradesWeighted / totalWeight;

    return std::round(result * 100) / 100; //round two dec pts
}

double Course::calculateSectionGradeSoFar(bool isTheory) const {
    double myGradesWeighted = 0.0;
    double totalWeight = 0.0;

    for(const Assessment& assessment : assessments) {
        if (assessment.getIsComplete() && assessment.getIsTheory() == isTheory) {
            myGradesWeighted += assessment.getGrade() * assessment.getWeight();
            totalWeight += assessment.getWeight();
        }
    }

    if (totalWeight == 0.0) {
        return 0.0;
    }

    double result = myGradesWeighted / totalWeight;

    return std::round(result * 100) / 100; //round two dec pts
}

