#ifndef COURSE_MANAGER_H
#define COURSE_MANAGER_H

#include <vector>
#include <string>
#include "Course.h"

class CourseManager {
private:
    std::vector<Course> courses;
    std::string dataFilePath;

public:
    //constructor
    CourseManager(const std::string& filePath = "courses.json");

    //course management
    void addCourse(const Course& course);
    void removeCourse(int index);
    Course& getCourse(int index);
    int getCourseCount() const;

    //file op
    bool loadFromFile();
    bool saveToFile() const;

    const std::vector<Course>& getAllCourses() const;

};
#endif