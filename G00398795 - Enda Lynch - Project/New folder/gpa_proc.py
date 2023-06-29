import pandas as pd
import statistics

# Define the GPA scale
gpa_scale = {
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

# Read the CSV file
df = pd.read_csv('MPPSample.csv')

# Calculate the letter grade for each module
module_columns = ['Intro to Programming', 'Databases', 'Computer Architecture', 'Ethics in Computer Science',
                  'Advanced Programming', 'Puzzles and Problem Solving']
for module in module_columns:
    df[module + ' - Letter Grade'] = df[module].apply(lambda x: 'F' if x < 50 else 'D' if x < 60 else 'C' if x < 70
                                                     else 'B' if x < 80 else 'A-' if x < 90 else 'A')

# Calculate the GPA for each student
df['GPA'] = df[[module + ' - Letter Grade' for module in module_columns]].applymap(lambda x: gpa_scale[x]).mean(axis=1)

# Calculate additional statistics for each student
df['Highest Scoring Module'] = df[module_columns].idxmax(axis=1)
df['Lowest Scoring Module'] = df[module_columns].idxmin(axis=1)
df['Standard Deviation'] = df[module_columns].apply(lambda row: statistics.stdev(row), axis=1)
df['Median Value'] = df[module_columns].apply(lambda row: statistics.median(row), axis=1)
df['GPA Difference from 4.2'] = 4.2 - df['GPA']

# Display the results
print(df[['Student Name', 'GPA', 'Highest Scoring Module', 'Lowest Scoring Module', 'Standard Deviation',
          'Median Value', 'GPA Difference from 4.2']])

# Live mode for calculating GPA
def calculate_letter_grade(mark):
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

print('\n--- Live Mode ---')
while True:
    marks = []
    for module in module_columns:
        mark = input(f"Enter the mark for {module}: ")
        marks.append(float(mark))
    letter_grades = [calculate_letter_grade(mark) for mark in marks]
    gpa = sum(gpa_scale[letter_grade] for letter_grade in letter_grades) / len(module_columns)
    print(f'GPA: {gpa}\n')

    choice = input('Do you want to calculate another GPA? (y/n): ')
    if choice.lower() != 'y':
        break






