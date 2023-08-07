# This Class will manage the functions that set the scale for the data calculations
class GPAScale:
    def __init__(self):
        self.gpa_scale = {
            'A+': 4.2,
            'A': 4.0,
            'A-': 3.8,
            'B+': 3.3,
            'B': 3.0,
            'B-': 2.7,
            'C+': 2.3,
            'C': 2.0,
            'C-': 1.7,
            'D+': 1.3,
            'D': 1.0,
            'F': 0.0
        }

    def calculate_letter_grade(self, mark):
        if mark < 50:
            return 'F'
        elif mark < 60:
            return 'D'
        elif mark < 70:
            return 'C'
        elif mark < 80:
            return 'B'
        elif mark < 90:
            return 'A-'
        else:
            return 'A'

    def calculate_gpa(self, marks):
        letter_grades = [self.calculate_letter_grade(mark) for mark in marks]
        gpa = sum(self.gpa_scale[letter_grade] for letter_grade in letter_grades) / len(marks)
        return gpa