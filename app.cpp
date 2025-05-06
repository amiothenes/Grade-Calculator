#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
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
            bool isTheory = getInput<char>("Is this a theory assessment? (y/n for lab): ") == 'y';

            bool isComplete = getInput<char>("Is this assessment complete? (y/n): ") == 'y';

            double grade = 0.0;
            if (isComplete) {
                grade = getInput<double>("Grade received (%): ");
            }
            
            Assessment newAssessment(name, weight, grade, isTheory, isComplete);
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
        if (course.getTotalWeight() == 100) {
            std::cout << " - Current grade: " << course.calculateOverallGrade() << "%";
        } else {
            std::cout << " - My grade so far: " << course.calculateGradeSoFar() << "%";
        }
        std::cout << std::endl;
    }
}

void viewAssessmentsDetails(Course chosenCourse, std::vector<Assessment> assessments) {
    bool is5050Course = chosenCourse.getIsA5050Course();

    const int idWidth = 3;
    const int nameWidth = 25;
    const int weightWidth = 8;
    const int gradeWidth = 8;
    const int statusWidth = 10;

    // Calculate total width for horizontal line
    const int totalWidth = idWidth + 1 + nameWidth + 1 + weightWidth + 1 + gradeWidth + 1 + statusWidth + 12;

    std::string horizontalLine(totalWidth, '-');

    std::cout << "\nAssessments:\n";
    std::cout << horizontalLine << "\n";

    // Header row
    if (is5050Course) {
        // Display with section grade for 50/50 courses
        double theoryGrade = chosenCourse.calculateSectionGradeSoFar(true);
        std::cout << "| " << std::setw(idWidth) << std::left << "#"
                  << " | " << std::setw(nameWidth-8) << std::left << "Theory" 
                  << "(" << std::fixed << std::setprecision(2) << theoryGrade << "%)"
                  << " | " << std::setw(weightWidth) << std::right << "Weight"
                  << " | " << std::setw(gradeWidth) << std::right << "Grade"
                  << " | " << std::setw(statusWidth) << std::left << "Status" << " |"
                  << "\n";
    } else {
        // Display without section grade for regular courses
        std::cout << "| " << std::setw(idWidth) << std::left << "#"
                  << " | " << std::setw(nameWidth) << std::left << "Theory" 
                  << " | " << std::setw(weightWidth) << std::right << "Weight"
                  << " | " << std::setw(gradeWidth) << std::right << "Grade"
                  << " | " << std::setw(statusWidth) << std::left << "Status" << " |"
                  << "\n";
    }
    
    std::cout << horizontalLine << "\n";
    
    // Data rows for theory section
    for (int i = 0; i < chosenCourse.getAssessmentCount(); i++) {
        const Assessment& assessment = assessments[i];

        if (assessment.getIsTheory()) {

            bool isComplete = assessment.getIsComplete();
            
            // Format ID column
            std::cout << "| " << std::setw(idWidth) << std::left << (i + 1);
            
            // Format Name column
            std::string name = assessment.getName();
            if (name.length() > nameWidth) {
                name = name.substr(0, nameWidth - 3) + "...";
            }
            std::cout << " | " << std::setw(nameWidth) << std::left << name;
            
            // Format Weight column
            std::cout << " | " << std::setw(weightWidth - 1) << std::right << assessment.getWeight() << "%";
            
            // Format Grade column
            if (isComplete) {
                std::cout << " | " << std::setw(gradeWidth - 1) << std::right << assessment.getGrade() << "%";
            } else {
                std::cout << " | " << std::setw(gradeWidth) << std::right << "N/A";
            }
            
            // Format Status column
            std::cout << " | " << std::setw(statusWidth) << std::left 
                    << (isComplete ? "Complete" : "Pending") << " |";
            
            std::cout << "\n";
        }
    }

        // Header row
        if (is5050Course) {
            double labGrade = chosenCourse.calculateSectionGradeSoFar(false);
            std::cout << "| " << std::setw(idWidth) << std::left << "#"
                      << " | " << std::setw(nameWidth-8) << std::left << "Lab" 
                      << "(" << std::fixed << std::setprecision(2) << labGrade << "%)"
                      << " | " << std::setw(weightWidth) << std::right << "Weight"
                      << " | " << std::setw(gradeWidth) << std::right << "Grade"
                      << " | " << std::setw(statusWidth) << std::left << "Status" << " |"
                      << "\n";
        } else {
            std::cout << "| " << std::setw(idWidth) << std::left << "#"
                      << " | " << std::setw(nameWidth) << std::left << "Lab" 
                      << " | " << std::setw(weightWidth) << std::right << "Weight"
                      << " | " << std::setw(gradeWidth) << std::right << "Grade"
                      << " | " << std::setw(statusWidth) << std::left << "Status" << " |"
                      << "\n";
        }

        std::cout << horizontalLine << "\n";
    
    // Data rows for lab section
    for (int i = 0; i < chosenCourse.getAssessmentCount(); i++) {
        const Assessment& assessment = assessments[i];

        if (!assessment.getIsTheory()) {

            bool isComplete = assessment.getIsComplete();
            
            // Format ID column
            std::cout << "| " << std::setw(idWidth) << std::left << (i + 1);
            
            // Format Name column
            std::string name = assessment.getName();
            if (name.length() > nameWidth) {
                name = name.substr(0, nameWidth - 3) + "...";
            }
            std::cout << " | " << std::setw(nameWidth) << std::left << name;
            
            // Format Weight column
            std::cout << " | " << std::setw(weightWidth - 1) << std::right << assessment.getWeight() << "%";
            
            // Format Grade column
            if (isComplete) {
                std::cout << " | " << std::setw(gradeWidth - 1) << std::right << assessment.getGrade() << "%";
            } else {
                std::cout << " | " << std::setw(gradeWidth) << std::right << "N/A";
            }
            
            // Format Status column
            std::cout << " | " << std::setw(statusWidth) << std::left 
                    << (isComplete ? "Complete" : "Pending") << " |";
            
            std::cout << "\n";
        }
    }
}

void editCourse(Course& chosenCourse) {
    std::cout << "Edit course functionality not implemented yet.\n";
}

void showCourseOptions(Course& chosenCourse) {
    int choice;
do {
    std::cout << "==== Select a Choice ====\n";
    std::cout << "1. Edit Course\n";
    std::cout << "2. Calculate Minimum Grades Needed for Target Final Grade\n";
    std::cout << "3. Simulate Final Grade Based on Hypothetical Scores\n";
    std::cout << "4. Back to Main Menu\n";
    std::cout << "=========================\n";
    
    choice = getInput<int>("Enter your choice: ");
    
    switch (choice) {
        case 1:
            clearScreen();
            editCourse(chosenCourse);
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            break;
            
        case 2:
            clearScreen();
            {
                std::cout << "My grade so far: " << chosenCourse.calculateGradeSoFar() << std::endl;
                double goal = getInput<double>("What's your goal final grade (%): ");
                std::vector<Assessment> resultingAssessments = chosenCourse.calculateRequiredGrades(goal);
                viewAssessmentsDetails(chosenCourse, resultingAssessments);
            }
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            break;
            
        case 3:
            clearScreen();
            chosenCourse.calculateWhatIf();
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            break;
        case 4:
            clearScreen();
            break;
            
        default:
            std::cout << "Invalid choice. Please try again.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
    }
} while (choice != 4);
}

void viewCourseDetails(CourseManager& manager) {
    displayCourses(manager);
    int courseCount = manager.getCourseCount();
    
    if (courseCount == 0) {
        std::cout << "No courses to view. Please add a course first.\n";
        return;
    }

    int choice;
    bool validInput = false;
    
    while (!validInput) {
        choice = getInput<int>("\nSelect course index number (1-" + std::to_string(courseCount) + "): ");
        
        if (choice >= 1 && choice <= courseCount) {
            validInput = true;
        } else {
            std::cout << "Invalid selection. Please enter a number between 1 and " 
                      << courseCount << ".\n";
        }
    }
    
    choice--;
    Course& chosenCourse = manager.getCourse(choice);
    const std::vector<Assessment> assessments = chosenCourse.getAllAssessments();
    
    std::cout << "\n === " << chosenCourse.getCourseCode() << " === \n";
    std::cout << "Type: " << (chosenCourse.getIsA5050Course() ? "50/50 Course" : "Regular Course") << "\n";
    std::cout << "Assessment Count: " << chosenCourse.getAssessmentCount() << "\n";

    const int idWidth = 3;
    const int nameWidth = 25;
    const int weightWidth = 8;
    const int gradeWidth = 8;
    const int statusWidth = 10;

    // Calculate total width for horizontal line
    const int totalWidth = idWidth + 1 + nameWidth + 1 + weightWidth + 1 + gradeWidth + 1 + statusWidth + 12;

    std::string horizontalLine(totalWidth, '-');
    
    if (chosenCourse.getAssessmentCount() > 0) {
        viewAssessmentsDetails(chosenCourse, assessments);
    }
        
        std::cout << horizontalLine << "\n";
        
        // Summary row 
        std::stringstream gradeStream;
        std::stringstream weightStream;
        
        std::stringstream overallGradeStream;
        std::string overallGradeText;
        if (chosenCourse.isTotalWeightValid()) {
            overallGradeStream << std::fixed << std::setprecision(2) << chosenCourse.calculateOverallGrade();
            overallGradeText = "Overall Grade: " + overallGradeStream.str() + "%";
        } else {
            gradeStream << std::fixed << std::setprecision(2) << chosenCourse.calculateGradeSoFar();
            std::string gradeText = "My grade so far: " + gradeStream.str() + "%";
    
            weightStream << std::fixed << std::setprecision(2) << chosenCourse.getTotalWeight();
            std::string weightText = " (based on " + weightStream.str() + "% of course weight)";
            
            std::cout << "| " << std::left << gradeText << weightText
                     << std::string(totalWidth - gradeText.length() - 39, ' ') << " |\n";
            overallGradeText = "Invalid Total Weight: Total weighting must equal to 100%";
        }
        
        std::cout << "| " << std::left << overallGradeText
                  << std::string(totalWidth - overallGradeText.length() - 4, ' ') << " |\n";
                  
        std::cout << horizontalLine << "\n";

        //course menu
        showCourseOptions(chosenCourse);
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
        std::cout << "==========================\n";
        
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
                clearScreen();
                viewCourseDetails(manager);
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
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