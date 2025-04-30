#include "Course.h"

Course::Course(std::string courseCode) {
    this->courseCode = courseCode;
}

std::string Course::getCourseCode() {
    return courseCode;
}

void Course::setCourseCode(std::string newCourseCode) {
    courseCode = newCourseCode;
}

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

double Course::calculateOverallGrade() const {
    double totalWeight = 0.0;
    double weightedSum = 0.0;

    for (const Assessment& assessment : assessments) {
        if (assessment.getIsComplete()) {
            totalWeight += assessment.getWeight();
            weightedSum += assessment.getGrade() * assessment.getWeight();
        }
    }

    return totalWeight > 0 && totalWeight <=100 ? weightedSum / totalWeight : 0.0;
}