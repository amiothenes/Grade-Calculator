CXX = g++
CXXFLAGS = -Wall -std=c++17 -I. -Inlohmann

SOURCES = app.cpp Assessment.cpp Course.cpp CourseManager.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = app

# Detect operating system
ifeq ($(OS),Windows_NT)
	RM = del /Q
	EXE = .exe
else
	RM = rm -f
	EXE = 
endif

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@$(EXE)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)$(EXE)

# mingw32-make clean
# mingw32-make
# .\app