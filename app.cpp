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

void pauseForUser() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
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
        double remainingWeight = 100.0;

        for (int i = 0; i < numAssessments; i++) {
            std::cout << "\nAssessment #" << (i + 1) << std::endl;
            std::cout << "Remaining weight: " << remainingWeight << "%" << std::endl;

            std::string name = getStringInput("Name: ");
            double weight = getInput<double>("Weight (%): ");

            while (weight > remainingWeight || weight <= 0) {
                std::cout << "Invalid weight. It must be between 0 and " << remainingWeight << "%.\n";
                weight = getInput<double>("Weight (%): ");
            }

            bool isTheory;
            if (isA5050Course) {
                isTheory = getInput<char>("Is this a theory assessment? (y/n for lab): ") == 'y';
            } else {
                isTheory = true;
            }

            bool isComplete = getInput<char>("Is this assessment complete? (y/n): ") == 'y';

            double grade = 0.0;
            if (isComplete) {
                grade = getInput<double>("Grade received (%): ");
            }
            
            Assessment newAssessment(name, weight, grade, isTheory, isComplete);
            newCourse.addAssessment(newAssessment);
            remainingWeight -= weight;
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
            std::cout << " - Completed -  Final Grade: " << course.calculateOverallGrade(true) << "%";
        } else {
            std::cout << " -  Pending  - Grade so far: " << course.calculateGradeSoFar(true) << "%";
        }
        std::cout << std::endl;
    }
}

void viewAssessmentsDetails(Course& chosenCourse, const std::vector<Assessment>& assessments, bool careForComplete) {
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
        double theoryGrade = chosenCourse.calculateSectionGradeSoFar(true, careForComplete);
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
            double grade = assessment.getGrade();
            
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
            if (grade != 0.0) {
                std::cout << " | " << std::setw(gradeWidth - 1) << std::right << std::fixed << std::setprecision(2) << grade << "%";
            } else {
                std::cout << " | " << std::setw(gradeWidth) << std::right << "N/A";
            }
            
            // Format Status column
            std::cout << " | " << std::setw(statusWidth) << std::left 
                    << (isComplete ? "Complete" : "Pending") << " |";
            
            std::cout << "\n";
        }
    }
        
        std::cout << horizontalLine << "\n";
        // Header row
        if (is5050Course) {
            double labGrade = chosenCourse.calculateSectionGradeSoFar(false, careForComplete);
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
            double grade = assessment.getGrade();
            
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
            if (grade != 0.0) {
                std::cout << " | " << std::setw(gradeWidth - 1) << std::right << std::fixed << std::setprecision(2) << grade << "%";
            } else {
                std::cout << " | " << std::setw(gradeWidth) << std::right << "N/A";
            }
            
            // Format Status column
            std::cout << " | " << std::setw(statusWidth) << std::left 
                    << (isComplete ? "Complete" : "Pending") << " |";
            
            std::cout << "\n";
        }
    }
    std::cout << horizontalLine << "\n";

}

void editCourse(Course& chosenCourse, CourseManager& manager) {
    int choice;
    do {
        clearScreen();
        std::cout << "==== Edit Course: " << chosenCourse.getCourseCode() << " ====\n";
        std::cout << "1. Rename course\n";
        std::cout << "2. Add assessment\n";
        std::cout << "3. Edit existing assessment\n";
        std::cout << "4. Delete assessment\n";
        std::cout << "5. Toggle 50/50 course type\n";
        std::cout << "6. Back to course menu\n";
        std::cout << "=============================\n";
        
        choice = getInput<int>("Enter your choice: ");
        switch (choice) {
            case 1: {
                // Rename course
                std::string newCode = getStringInput("Enter new course code: ");
                chosenCourse.setCourseCode(newCode);
                std::cout << "Course code updated successfully!\n";
                manager.saveToFile();
                pauseForUser();
                break;
            }
                
            case 2: {
                // Add assessment
                clearScreen();
                std::cout << "==== Add New Assessment ====\n";
                std::string name = getStringInput("Name: ");
                double weight = getInput<double>("Weight (%): ");
                bool isTheory = getInput<char>("Is this a theory assessment? (y/n for lab): ") == 'y';
                bool isComplete = getInput<char>("Is this assessment complete? (y/n): ") == 'y';
                
                double grade = 0.0;
                if (isComplete) {
                    grade = getInput<double>("Grade received (%): ");
                }
                
                Assessment newAssessment(name, weight, grade, isTheory, isComplete);
                chosenCourse.addAssessment(newAssessment);
                std::cout << "Assessment added successfully!\n";
                manager.saveToFile();
                pauseForUser();
                break;
            }
                
            case 3: {
                // Edit existing assessment
                clearScreen();
                std::cout << "==== Edit Assessment ====\n";
                
                // Display assessments
                const std::vector<Assessment>& assessments = chosenCourse.getAllAssessments();
                if (assessments.empty()) {
                    std::cout << "No assessments to edit.\n";
                    pauseForUser();
                    break;
                }
                
                // List assessments
                std::cout << "Current assessments:\n";
                for (size_t i = 0; i < assessments.size(); i++) {
                    std::cout << (i + 1) << ". " << assessments[i].getName() 
                              << " (Weight: " << assessments[i].getWeight() << "%, "
                              << (assessments[i].getIsComplete() ? "Complete" : "Pending") << ")\n";
                }
                
                // Get assessment to edit
                int assessmentIndex = getInput<int>("\nEnter assessment number to edit (1-" + 
                                                   std::to_string(assessments.size()) + "): ");
                
                if (assessmentIndex < 1 || assessmentIndex > static_cast<int>(assessments.size())) {
                    std::cout << "Invalid assessment number.\n";
                    pauseForUser();
                    break;
                }
                
                assessmentIndex--;
                
                // Display editable properties
                clearScreen();
                std::cout << "==== Edit Assessment: " << assessments[assessmentIndex].getName() << " ====\n";
                std::cout << "1. Name (currently: " << assessments[assessmentIndex].getName() << ")\n";
                std::cout << "2. Weight (currently: " << assessments[assessmentIndex].getWeight() << "%)\n";
                std::cout << "3. Assessment type (currently: " << (assessments[assessmentIndex].getIsTheory() ? "Theory" : "Lab") << ")\n";
                std::cout << "4. Completion status (currently: " << (assessments[assessmentIndex].getIsComplete() ? "Complete" : "Pending") << ")\n";
                std::cout << "5. Grade (currently: " << std::fixed << std::setprecision(2) << assessments[assessmentIndex].getGrade() << "%)\n";
                std::cout << "6. Back\n";
                
                int propertyChoice = getInput<int>("What would you like to edit? ");
                
                switch (propertyChoice) {
                    case 1: {
                        std::string newName = getStringInput("Enter new name: ");
                        chosenCourse.updateAssessmentName(assessmentIndex, newName);
                        break;
                    }
                    case 2: {
                        double newWeight = getInput<double>("Enter new weight (%): ");
                        chosenCourse.updateAssessmentWeight(assessmentIndex, newWeight);
                        break;
                    }
                    case 3: {
                        bool newIsTheory = getInput<char>("Is this a theory assessment? (y/n for lab): ") == 'y';
                        chosenCourse.updateAssessmentType(assessmentIndex, newIsTheory);
                        break;
                    }
                    case 4: {
                        bool newIsComplete = getInput<char>("Is this assessment complete? (y/n): ") == 'y';
                        chosenCourse.updateAssessmentCompletionStatus(assessmentIndex, newIsComplete);
                        
                        // If marked as complete, ask for grade
                        if (newIsComplete && !assessments[assessmentIndex].getIsComplete()) {
                            double newGrade = getInput<double>("Enter grade received (%): ");
                            chosenCourse.updateAssessmentGrade(assessmentIndex, newGrade);
                        }
                        break;
                    }
                    case 5: {
                        if (!assessments[assessmentIndex].getIsComplete()) {
                            std::cout << "Cannot set grade for incomplete assessment.\n";
                        } else {
                            double newGrade = getInput<double>("Enter new grade (%): ");
                            chosenCourse.updateAssessmentGrade(assessmentIndex, newGrade);
                        }
                        break;
                    }
                    case 6:
                        break;
                    default:
                        std::cout << "Invalid choice.\n";
                }
                
                std::cout << "Assessment updated successfully!\n";
                manager.saveToFile();
                pauseForUser();
                break;
            }
                
            case 4: {
                // Delete assessment
                clearScreen();
                std::cout << "==== Delete Assessment ====\n";
                
                const std::vector<Assessment>& assessments = chosenCourse.getAllAssessments();
                if (assessments.empty()) {
                    std::cout << "No assessments to delete.\n";
                    pauseForUser();
                    break;
                }
                
                // List assessments
                std::cout << "Current assessments:\n";
                for (size_t i = 0; i < assessments.size(); i++) {
                    std::cout << (i + 1) << ". " << assessments[i].getName() << "\n";
                }
                
                // Get assessment to delete
                int assessmentIndex = getInput<int>("\nEnter assessment number to delete (1-" + 
                                                   std::to_string(assessments.size()) + "): ");
                
                if (assessmentIndex < 1 || assessmentIndex > static_cast<int>(assessments.size())) {
                    std::cout << "Invalid assessment number.\n";
                    pauseForUser();
                    break;
                }
                
                // Confirm deletion
                char confirm = getInput<char>("Are you sure you want to delete this assessment? (y/n): ");
                if (confirm == 'y') {
                    chosenCourse.removeAssessment(assessmentIndex - 1);
                    std::cout << "Assessment deleted successfully!\n";
                    manager.saveToFile(); // Save changes to file
                } else {
                    std::cout << "Deletion cancelled.\n";
                }
                
                pauseForUser();
                break;
            }
                
            case 5: {
                // Toggle 50/50 course type
                bool isA5050Course = chosenCourse.getIsA5050Course();
                char confirm = getInput<char>("This course is currently " + 
                                             std::string(isA5050Course ? "a 50/50" : "NOT a 50/50") +
                                             " course. Change? (y/n): ");
                
                if (confirm == 'y') {
                    chosenCourse.setIsA5050Course(!isA5050Course);
                    std::cout << "Course type updated successfully!\n";
                    manager.saveToFile();
                }
                
                pauseForUser();
                break;
            }
                
            case 6:
                // Return to course menu
                break;
                
            default:
                std::cout << "Invalid choice. Please try again.\n";
                pauseForUser();
        }
    } while (choice != 6);
}

void showCourseOptions(Course& chosenCourse, CourseManager& manager) {
    int choice;
do {
    clearScreen();
    std::cout << "==== Select a Choice ====\n";
    std::cout << "1. Edit Course\n";
    std::cout << "2. Calculate Minimum Grades Needed for Target Final Grade\n";
    std::cout << "3. Simulate Final Grade Based on Hypothetical Scores\n";
    std::cout << "4. Back to Main Menu\n";
    std::cout << "=========================\n";
    
    choice = getInput<int>("Enter your choice: ");
    
    switch (choice) {
        case 1:
            editCourse(chosenCourse, manager);
            break;
            
        case 2:
            clearScreen();
            {
                std::cout << "My grade so far: " << std::fixed << std::setprecision(2) 
                          << chosenCourse.calculateGradeSoFar(true) << "%" << std::endl;
                std::cout << "Number of incomplete assessments: " << chosenCourse.getIncompleteAssessmentCount() << std::endl;
                double goal = getInput<double>("What's your goal final grade (%): ");
                std::vector<Assessment> resultingAssessments = chosenCourse.calculateRequiredGrades(goal);
        
                // Check if the resulting assessments vector is empty
                if (resultingAssessments.empty()) {
                    std::cout << "\n==== Target Grade Analysis ====\n";
                    std::cout << "Achieving a grade of " << std::fixed << std::setprecision(2) 
                              << goal << "% is impossible with the current assessment structure.\n";
                } else {
                    Course tempCourse(chosenCourse.getCourseCode(), 
                                     resultingAssessments, 
                                     chosenCourse.getIsA5050Course());
        
                    viewAssessmentsDetails(tempCourse, resultingAssessments, false);
        
                    const int idWidth = 3;
                    const int nameWidth = 25;
                    const int weightWidth = 8;
                    const int gradeWidth = 8;
                    const int statusWidth = 10;
                
                    // Calculate total width for horizontal line
                    const int totalWidth = idWidth + 1 + nameWidth + 1 + weightWidth + 1 + gradeWidth + 1 + statusWidth + 12;
                
                    std::string horizontalLine(totalWidth, '-');
        
                    // Summary row 
                    std::stringstream overallGradeStream;
                    std::string overallGradeText;
        
                    overallGradeStream << std::fixed << std::setprecision(2) << tempCourse.calculateOverallGrade(false);
                    overallGradeText = "Final Grade would be: " + overallGradeStream.str() + "%";
                    
                    // Check if spaces needed is negative to prevent string length error
                    int spacesNeeded = totalWidth - overallGradeText.length() - 4;
                    if (spacesNeeded < 0) spacesNeeded = 0;
                    
                    std::cout << "| " << std::left << overallGradeText
                            << std::string(spacesNeeded, ' ') << " |\n";
                            
                    std::cout << horizontalLine << "\n";
                }
            }
            pauseForUser();
            break;
            
        case 3:
            {
                std::cout << "==== What If Grade Simulation ====\n\n";
                
                // Get copy of assessments for simulation
                std::vector<Assessment> simulationAssessments = chosenCourse.getAllAssessments();
                bool madeChanges = false;
                
                // Show current grade status
                std::cout << "Your current grade: " << std::fixed 
                          << std::setprecision(2) << chosenCourse.calculateGradeSoFar(true) 
                          << "% (based on " << chosenCourse.getTotalWeight() << "% of course weight)\n\n";
                
                // Ask for hypothetical grades for incomplete assessments
                std::cout << "Enter hypothetical grades for incomplete assessments:\n";
                for (size_t i = 0; i < simulationAssessments.size(); i++) {
                    Assessment& assessment = simulationAssessments[i];
                    
                    if (!assessment.getIsComplete()) {
                        std::cout << "\n" << assessment.getName() 
                                  << " (Weight: " << assessment.getWeight() << "%)"
                                  << " - Currently not completed\n";
                                  
                        double hypotheticalGrade = getInput<double>("Enter hypothetical grade (%): ");
                        
                        // Keep the grade within reasonable bounds
                        if (hypotheticalGrade < 0.0) hypotheticalGrade = 0.0;
                        if (hypotheticalGrade > 100.0) hypotheticalGrade = 100.0;
                        
                        assessment.setGrade(hypotheticalGrade);
                        // Don't change isComplete status - we're just simulating
                        
                        madeChanges = true;
                    }
                }
                
                // Calculate and show the hypothetical final grade
                if (madeChanges) {
                    // Create a temporary course with our modified assessments
                    Course tempCourse(chosenCourse.getCourseCode(), 
                                     simulationAssessments, 
                                     chosenCourse.getIsA5050Course());
                                     
                    // Calculate and display the hypothetical grade
                    double hypotheticalGrade = tempCourse.calculateGradeSoFar(false);
                    
                    std::cout << "\n==== Simulation Results ====\n";
                    std::cout << "With your hypothetical grades, your final grade would be: "
                              << std::fixed << std::setprecision(2) << hypotheticalGrade << "%\n";
                              
                    // Show the detailed breakdown
                    std::cout << "\nDetailed breakdown:\n";
                    viewAssessmentsDetails(tempCourse, simulationAssessments, false);

                    const int idWidth = 3;
                    const int nameWidth = 25;
                    const int weightWidth = 8;
                    const int gradeWidth = 8;
                    const int statusWidth = 10;
                
                    // Calculate total width for horizontal line
                    const int totalWidth = idWidth + 1 + nameWidth + 1 + weightWidth + 1 + gradeWidth + 1 + statusWidth + 12;
                
                    std::string horizontalLine(totalWidth, '-');

                    // Summary row 
                    std::stringstream gradeStream;
                    std::stringstream weightStream;
                    
                    std::stringstream overallGradeStream;
                    std::string overallGradeText;

                    overallGradeStream << std::fixed << std::setprecision(2) << tempCourse.calculateOverallGrade(false);
                    overallGradeText = "Final Grade would be: " + overallGradeStream.str() + "%";
                    std::cout << "| " << std::left << overallGradeText
                            << std::string(totalWidth - overallGradeText.length() - 4, ' ') << " |\n";
                            
                    std::cout << horizontalLine << "\n";
                } else {
                    std::cout << "\nNo incomplete assessments found to simulate grades for.\n";
                }
            }
            pauseForUser();
            break;
        case 4:
            break;
            
        default:
            std::cout << "Invalid choice. Please try again.\n";
            pauseForUser();
    }
} while (choice != 4);
}

void viewCourseDetails(CourseManager& manager) {
    clearScreen();
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
        viewAssessmentsDetails(chosenCourse, assessments, true);
    }
                
        // Summary row 
        std::stringstream gradeStream;
        std::stringstream weightStream;
        
        std::stringstream overallGradeStream;
        std::string overallGradeText;
        if (chosenCourse.isTotalWeightValid()) {
            overallGradeStream << std::fixed << std::setprecision(2) << chosenCourse.calculateOverallGrade(true);
            overallGradeText = "Overall Grade: " + overallGradeStream.str() + "%";
        } else {
            gradeStream << std::fixed << std::setprecision(2) << chosenCourse.calculateGradeSoFar(true);
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
        pauseForUser();
        showCourseOptions(chosenCourse, manager);
}

void deleteCourse(CourseManager& manager) {
    clearScreen();
    displayCourses(manager);
    int courseCount = manager.getCourseCount();
    
    if (courseCount == 0) {
        std::cout << "No courses to delete. Please add a course first.\n";
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

    // Confirm deletion
    char confirm = getInput<char>("Are you sure you want to delete the course '" + chosenCourse.getCourseCode() + "'? (y/n): ");
    if (confirm == 'y') {
        std::string courseCode = chosenCourse.getCourseCode();
        manager.removeCourse(choice);
        std::cout << "Course '" << courseCode << "' deleted successfully!\n";
    } else {
        std::cout << "Deletion cancelled.\n";
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
        std::cout << "==========================\n";
        
        choice = getInput<int>("Enter your choice: ");
        
        switch (choice) {
            case 1:
                clearScreen();
                addNewCourse(manager);
                pauseForUser();
                break;
                
            case 2:
                clearScreen();
                displayCourses(manager);
                pauseForUser();
                break;
                
            case 3:
                clearScreen();
                viewCourseDetails(manager);
                break;
                
            case 4:
                // Implement delete course
                clearScreen();
                deleteCourse(manager);
                pauseForUser();
                break;
                
            case 5:
                std::cout << "Goodbye!\n";
                break;
                
            default:
                std::cout << "Invalid choice. Please try again.\n";
                pauseForUser();
        }
    } while (choice != 5);
}

int main() {
    // Create course manager with default file path
    CourseManager manager("courses.json");
    
    showMainMenu(manager);
    
    return 0;
}