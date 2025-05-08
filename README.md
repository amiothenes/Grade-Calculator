
# Grade Calculator

A simple C++ application that helps students track, simulate, and optimize their academic performance. Users can add courses along with customizable assessments, including weights and grades. Based on the input, the tool can perform analysis to:

- Calculate required grades on upcoming assessments to reach a desired final mark.
- Simulate "What if" scenarios (e.g., “What if I score 75% on the next test?”) to project potential final grades.


## Screenshots

<img src="https://i.imgur.com/KGxsoRS.png" alt="Main Menu" width="350"/>

<img src="https://i.imgur.com/WPc3jUH.png" alt="Course List" width="350"/>

<img src="https://i.imgur.com/Nj2yx1q.png" alt="Course Overview" width="350"/>

## Installation

Install Grade-Calculator

```bash
  git clone https://github.com/amiothenes/Grade-Calculator.git
  cd Grade-Calculator
  make
  ./app
```
    
## Features

- Course management (add/edit/delete)
- Assessment tracking with theory/lab distinction
- Special support for 50/50 courses
- "What-If" grade simulation
- Required grade calculations for target scores
- Persistent storage with JSON files

## Future Improvements

- GUI implementation
- Statistical analysis of performance trends
- GPA calculation across multiple courses
- Export to CSV/PDF for reports