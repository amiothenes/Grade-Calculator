#include <iostream>

class Course {
private:

    std::string courseCode;
    //array of a dictionary of assignments with their weights and actual grades

public:

    //constructor
    Course(std::string courseCode) {
        this->courseCode = courseCode;
    }

    //getter
    std::string getCourseCode() {return courseCode;}

    //setter
    void setCourseCode(std::string newCourseCode) {courseCode = newCourseCode;}

};