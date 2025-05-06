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

std::vector<Assessment> Course::calculateRequiredGrades(double goalGrade) const {

    const double threshold = 0.1; //how close is the projection to the goal
    const double interval = 0.5; // increment interval

    if (isTotalWeightValid()) {
        return getAllAssessments(); // returns same
    }

    double gradeSoFar = calculateGradeSoFar();
    std::vector<Assessment> assessmentsCopy = getAllAssessments();
    
    if (std::abs(goalGrade - gradeSoFar) < threshold) {
        return assessmentsCopy;
    }
    
    double increment;
    
    auto calculateProjectedGrade = [](const std::vector<Assessment>& assessments) -> double {
        double weightedSum = 0.0;
        double totalWeight = 0.0;
        
        for (const Assessment& assessment : assessments) {
            // Include all assessments in the calculation
            weightedSum += assessment.getGrade() * assessment.getWeight();
            totalWeight += assessment.getWeight();
        }
        
        if (totalWeight == 0.0) {
            return 0.0;
        }
        
        return std::round((weightedSum / 100) * 100) / 100; // Round to 2 decimal places
    };
    
    // Initial setup for incomplete assessments
    for (Assessment& assessment : assessmentsCopy) {
        if (!assessment.getIsComplete()) {
            // Start with a reasonable grade if currently 0
            if (assessment.getGrade() == 0.0) {
                assessment.setGrade(50.0);
            }
        }
    }
    
    double projectedGrade = calculateProjectedGrade(assessmentsCopy);
    
    int maxIterations = 9999; // Safety against infinite loops
    int iteration = 0;
    
    while (std::abs(goalGrade - projectedGrade) > threshold && iteration < maxIterations) {
        for (Assessment& assessment : assessmentsCopy) {
            if (!assessment.getIsComplete()) {
                // Update the grade
                increment = (goalGrade > projectedGrade) ? interval : -interval;
                double newGrade = assessment.getGrade() + increment;
                
                // Keep grades within reasonable bounds (0-100%)
                if (newGrade >= 0.0 && newGrade <= 100.0) {
                    assessment.setGrade(newGrade);
                }
            }
        }
        
        // Recalculate the projected grade
        projectedGrade = calculateProjectedGrade(assessmentsCopy);
        iteration++;
        std::cout << "Iteration: " << iteration << ", Projected Grade: " << projectedGrade << "%" << std::endl;
    }
    
    //set all to now completed for analysis printing
    for (Assessment& assessment : assessmentsCopy) {
        if (!assessment.getIsComplete()) {
            assessment.setIsComplete(true);
        }
    }

    if (iteration >= maxIterations) {
        return std::vector<Assessment>();
    }

    return assessmentsCopy;
}

std::vector<Assessment> Course::calculateWhatIf() const {

}