#include <iostream>
#include <string>
#include <vector>
#include "Course.h"
#include "Assessment.h"
#include "CourseManager.h"

// clear console screen on whatever
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Function to get user input with validation
template<typename T>
T getInput(const std::string& prompt) {
    T value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input. " << prompt;
    }
    std::cin.ignore(10000, '\n'); // Clear input buffer
    return value;
}

// Get string input (handles spaces)
std::string getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

// Function to add a new course
void addNewCourse(CourseManager& manager) {
    std::string courseCode = getStringInput("Enter course code: ");
    bool isA5050Course = getInput<char>("Is this a 50/50 course? (y/n): ") == 'y';
    
    // Create a new course with empty assessments list
    Course newCourse(courseCode, {}, isA5050Course);
    
    // Ask user if they want to add assessments now
    if (getInput<char>("Add assessments now? (y/n): ") == 'y') {
        int numAssessments = getInput<int>("How many assessments to add? ");
        
        for (int i = 0; i < numAssessments; i++) {
            std::cout << "\nAssessment #" << (i + 1) << std::endl;
            std::string name = getStringInput("Name: ");
            double weight = getInput<double>("Weight (%): ");
            
            char isComplete = getInput<char>("Is this assessment complete? (y/n): ");
            double grade = 0.0;
            if (isComplete == 'y') {
                grade = getInput<double>("Grade received (%): ");
            }
            
            Assessment newAssessment(name, weight, grade, isComplete == 'y');
            newCourse.addAssessment(newAssessment);
        }
    }
    
    manager.addCourse(newCourse);
    std::cout << "Course added successfully!" << std::endl;
}

// Display all courses
void displayCourses(const CourseManager& manager) {
    const auto& courses = manager.getAllCourses();
    if (courses.empty()) {
        std::cout << "No courses found." << std::endl;
        return;
    }
    
    std::cout << "\n=== Courses ===\n";
    for (int i = 0; i < courses.size(); i++) {
        const Course& course = courses[i];
        std::cout << i + 1 << ". " << course.getCourseCode()
                  << " (Assessments: " << course.getAssessmentCount() << ")";
                  
        // If there are completed assessments, show current grade
        if (course.getTotalWeight() > 0) {
            std::cout << " - Current grade: " << course.calculateOverallGrade() << "%";
        }
        std::cout << std::endl;
    }
}

// Main menu function
void showMainMenu(CourseManager& manager) {
    int choice;
    do {
        clearScreen();
        std::cout << "==== Grade Calculator ====\n";
        std::cout << "1. Add new course\n";
        std::cout << "2. View all courses\n";
        std::cout << "3. View/edit course details\n";
        std::cout << "4. Delete course\n";
        std::cout << "5. Exit\n";
        std::cout << "=======================\n";
        
        choice = getInput<int>("Enter your choice: ");
        
        switch (choice) {
            case 1:
                clearScreen();
                addNewCourse(manager);
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                break;
                
            case 2:
                clearScreen();
                displayCourses(manager);
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
                break;
                
            case 3:
                // Implement view/edit course
                break;
                
            case 4:
                // Implement delete course
                break;
                
            case 5:
                std::cout << "Goodbye!\n";
                break;
                
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::cout << "Press Enter to continue...";
                std::cin.get();
        }
    } while (choice != 5);
}

int main() {
    // Create course manager with default file path
    CourseManager manager("courses.json");
    
    showMainMenu(manager);
    
    return 0;
}