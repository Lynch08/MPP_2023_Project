import pandas as pd
import statistics

class GPA_Calculator:
    def __init__(self, module_columns):
        self.module_columns = module_columns
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
        gpa = sum(self.gpa_scale[letter_grade] for letter_grade in letter_grades) / len(self.module_columns)
        return gpa

    def process_data(self, filename):
        df = pd.read_csv(filename)

        for module in self.module_columns:
            df[module + ' - Letter Grade'] = df[module].apply(self.calculate_letter_grade)

        df['GPA'] = df[[module + ' - Letter Grade' for module in self.module_columns]].applymap(lambda x: self.gpa_scale[x]).mean(axis=1)
        df['Highest Scoring Module'] = df[self.module_columns].idxmax(axis=1)
        df['Lowest Scoring Module'] = df[self.module_columns].idxmin(axis=1)
        df['Standard Deviation'] = df[self.module_columns].apply(lambda row: statistics.stdev(row), axis=1)
        df['Median Value'] = df[self.module_columns].apply(lambda row: statistics.median(row), axis=1)
        df['GPA Difference from 4.2'] = 4.2 - df['GPA']

        return df[['Student Name', 'GPA', 'Highest Scoring Module', 'Lowest Scoring Module', 'Standard Deviation',
                    'Median Value', 'GPA Difference from 4.2']]

    def run_live_mode(self):
        print('\n--- Live Mode: GPA Calculator ---')
        while True:
            marks = []
            for module in self.module_columns:
                mark = input(f"Enter the mark for {module}: ")
                marks.append(float(mark))
            
            gpa = self.calculate_gpa(marks)
            print(f'GPA: {gpa}\n')

            choice = input('Do you want to calculate another GPA? (y/n): ')
            if choice.lower() != 'y':
                break

# Define the module columns
module_columns = ['Intro to Programming', 'Databases', 'Computer Architecture', 'Ethics in Computer Science',
                  'Advanced Programming', 'Puzzles and Problem Solving']

calculator = GPA_Calculator(module_columns)

# Process the data from the CSV file
df = calculator.process_data('MPPSample.csv')

# Display the results
print(df)

# Run live mode for calculating GPA
calculator.run_live_mode()