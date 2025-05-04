#include "CourseManager.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

CourseManager::CourseManager(const std::string& filePath) : dataFilePath(filePath) {
    //load saved courses when called
    loadFromFile();
}

void CourseManager::addCourse(const Course& course) {
    courses.push_back(course);
    saveToFile();
}

void CourseManager::removeCourse(int index) {
    if (index >= 0 && index < courses.size()) {
        courses.erase(courses.begin() + index);
        saveToFile();
    }
}

Course& CourseManager::getCourse(int index) {
    return courses[index];
}

int CourseManager::getCourseCount() const {
    return courses.size();
}

const std::vector<Course>& CourseManager::getAllCourses() const {
    return courses;
}

//file management
bool CourseManager::loadFromFile() {
    try {
        std::ifstream file(dataFilePath);
        if (!file.is_open()) {
            // File doesn't exist, create an empty one
            std::ofstream newFile(dataFilePath);
            if (!newFile.is_open()) {
                std::cerr << "Error: Could not create a new data file at " << dataFilePath << std::endl;
                return false;
            }
            
            // Write an empty courses array structure
            json emptyData;
            emptyData["courses"] = json::array();
            newFile << emptyData.dump(4);
            newFile.close();
            
            // Courses vector is already empty by default
            return true;
        }
        
        json jsonData;
        file >> jsonData;
        
        courses.clear();
        for (const auto& courseJson : jsonData["courses"]) {
            std::string courseCode = courseJson["courseCode"];
            bool isA5050Course = courseJson["isA5050Course"];
            
            std::vector<Assessment> assessments;
            for (const auto& assessmentJson : courseJson["assessments"]) {
                std::string name = assessmentJson["name"];
                double weight = assessmentJson["weight"];
                double grade = assessmentJson["grade"];
                bool isComplete = assessmentJson["isComplete"];
                
                Assessment assessment(name, weight, grade, isComplete);
                assessments.push_back(assessment);
            }
            
            Course course(courseCode, assessments, isA5050Course);
            courses.push_back(course);
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading courses: " << e.what() << std::endl;
        return false;
    }
}

bool CourseManager::saveToFile() const {
    try {
        json jsonData;
        jsonData["courses"] = json::array();
        
        for (const Course& course : courses) {
            json courseJson;
            courseJson["courseCode"] = course.getCourseCode();
            courseJson["isA5050Course"] = course.getIsA5050Course();
            
            courseJson["assessments"] = json::array();
            for (const Assessment& assessment : course.getAllAssessments()) {
                json assessmentJson;
                assessmentJson["name"] = assessment.getName();
                assessmentJson["weight"] = assessment.getWeight();
                assessmentJson["grade"] = assessment.getGrade();
                assessmentJson["isTheory"] = assessment.getIsTheory();
                assessmentJson["isComplete"] = assessment.getIsComplete();
                
                courseJson["assessments"].push_back(assessmentJson);
            }
            
            jsonData["courses"].push_back(courseJson);
        }
        
        std::ofstream file(dataFilePath);
        file << jsonData.dump(4); // 4 spaces indentation for pretty printing
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving courses: " << e.what() << std::endl;
        return false;
    }
}