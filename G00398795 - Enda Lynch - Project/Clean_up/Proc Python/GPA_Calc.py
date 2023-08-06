import pandas as pd
import statistics

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

def calculate_gpa(marks):
    letter_grades = [calculate_letter_grade(mark) for mark in marks]
    gpa = sum(gpa_scale[letter_grade] for letter_grade in letter_grades) / len(marks)
    return gpa

def process_data(module_columns, csv_file):
    df = pd.read_csv(csv_file)

    for module in module_columns:
        df[module + ' - Letter Grade'] = df[module].apply(calculate_letter_grade)

    df['GPA'] = df[[module + ' - Letter Grade' for module in module_columns]].applymap(lambda x: gpa_scale[x]).mean(axis=1)
    df['Highest Scoring Module'] = df[module_columns].idxmax(axis=1)
    df['Lowest Scoring Module'] = df[module_columns].idxmin(axis=1)
    df['Standard Deviation'] = df[module_columns].apply(lambda row: statistics.stdev(row), axis=1)
    df['Median Value'] = df[module_columns].apply(lambda row: statistics.median(row), axis=1)
    df['GPA Difference from 4.2'] = 4.2 - df['GPA']

    return df[['Student Name', 'GPA', 'Highest Scoring Module', 'Lowest Scoring Module', 'Standard Deviation',
               'Median Value', 'GPA Difference from 4.2']]

def run_live_mode(module_columns, csv_file):
    print('\n--- Live Mode: GPA Calculator ---')
    while True:
        student_name = input("Enter student name (or 'm' for menu): ")
        if student_name.lower() == 'm':
            break

        df = pd.read_csv(csv_file)
        if student_name in df['Student Name'].values:
            print(f"Data for '{student_name}' already exists in the CSV. Skipping...\n")
            continue

        marks = []
        for module in module_columns:
            mark = float(input(f"Enter the mark for {module}: "))
            marks.append(mark)
        
        gpa = calculate_gpa(marks)
        print(f'GPA for {student_name}: {gpa:.2f}\n')

        # Construct a DataFrame from a dictionary containing the new student's data
        data_input = {"Student Name": [student_name]}
        for i, module in enumerate(module_columns):
            data_input[module] = [marks[i]]
        new_student_df = pd.DataFrame(data_input)

        df = pd.concat([df, new_student_df], ignore_index=True)
        df.to_csv(csv_file, index=False)

    while True:
        choice = input("Do you want to add another new student, display updated data, or process data again? (add/display/process/quit): ")
        if choice.lower() == 'quit':
            break
        elif choice.lower() == 'display':
            print('\n--- Updated CSV Data ---')
            df = pd.read_csv(csv_file)
            print(df)
        elif choice.lower() == 'add':
            student_name = input("Enter student name: ")
            if student_name.lower() == 'q':
                break

            df = pd.read_csv(csv_file)
            if student_name in df['Student Name'].values:
                print(f"Data for '{student_name}' already exists in the CSV. Skipping...\n")
                continue

            marks = []
            for module in module_columns:
                mark = float(input(f"Enter the mark for {module}: "))
                marks.append(mark)
            
            gpa = calculate_gpa(marks)
            print(f'GPA for {student_name}: {gpa:.2f}\n')

            # Construct a DataFrame from a dictionary containing the new student's data
            data_input = {"Student Name": [student_name]}
            for i, module in enumerate(module_columns):
                data_input[module] = [marks[i]]
            new_student_df = pd.DataFrame(data_input)

            df = pd.concat([df, new_student_df], ignore_index=True)
            df.to_csv(csv_file, index=False)
        elif choice.lower() == 'process':
            df = process_data(module_columns, csv_file)
            print('\n--- Processed Data ---')
            print(df)
        else:
            print("Invalid choice. Please enter 'add', 'display', 'process', or 'quit'.")
            continue

if __name__ == "__main__":
    # Define the module columns
    module_columns = ['Intro to Programming', 'Databases', 'Computer Architecture', 'Ethics in Computer Science',
                      'Advanced Programming', 'Puzzles and Problem Solving']

    csv_file = 'MPPSample.csv'

    # Process the data from the CSV file and display the results
    df = process_data(module_columns, csv_file)
    print(df)

    # Run live mode for calculating GPA
    run_live_mode(module_columns, csv_file)

