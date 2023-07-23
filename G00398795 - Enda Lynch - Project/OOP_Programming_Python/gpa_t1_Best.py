import pandas as pd
import statistics

class GPAHelper:
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

class Student:
    def __init__(self, name, module_columns):
        self.name = name
        self.marks = {module: None for module in module_columns}

    def add_mark(self, module, mark):
        self.marks[module] = mark

    def calculate_gpa(self, gpa_helper):
        marks = list(self.marks.values())
        return gpa_helper.calculate_gpa(marks)

class CSVManager:
    def __init__(self, csv_file):
        self.csv_file = csv_file

    def read_data(self):
        return pd.read_csv(self.csv_file)

    def save_data(self, df):
        df.to_csv(self.csv_file, index=False)

class GPACalculator:
    def __init__(self, module_columns, csv_file):
        self.module_columns = module_columns
        self.csv_manager = CSVManager(csv_file)
        self.gpa_helper = GPAHelper()

    def process_data(self):
        df = self.csv_manager.read_data()

        for module in self.module_columns:
            df[module + ' - Letter Grade'] = df[module].apply(self.gpa_helper.calculate_letter_grade)

        df['GPA'] = df[[module + ' - Letter Grade' for module in self.module_columns]].applymap(lambda x: self.gpa_helper.gpa_scale[x]).mean(axis=1)
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
            student_name = input("Enter student name (or 'm' for Menu): ")
            if student_name.lower() == 'm':
                break

            df = self.csv_manager.read_data()
            if student_name in df['Student Name'].values:
                print(f"Data for '{student_name}' already exists in the CSV. Skipping...\n")
                continue

            student = Student(student_name, self.module_columns)

            for module in self.module_columns:
                mark = float(input(f"Enter the mark for {module}: "))
                student.add_mark(module, mark)
            
            gpa = student.calculate_gpa(self.gpa_helper)
            print(f'GPA for {student_name}: {gpa:.2f}\n')

            # Construct a DataFrame from a dictionary containing the new student's data
            data_input = {"Student Name": [student_name]}
            for module, mark in student.marks.items():
                data_input[module] = [mark]
            new_student_df = pd.DataFrame(data_input)

            df = pd.concat([df, new_student_df], ignore_index=True)
            self.csv_manager.save_data(df)

        while True:
            choice = input("Do you want to add another new student, display updated data, or process data again? (add/display/process/quit): ")
            if choice.lower() == 'quit':
                break
            elif choice.lower() == 'display':
                print('\n--- Updated CSV Data ---')
                df = self.csv_manager.read_data()
                print(df)
            elif choice.lower() == 'add':
                student_name = input("Enter student name: ")
                if student_name.lower() == 'q':
                    break

                df = self.csv_manager.read_data()
                if student_name in df['Student Name'].values:
                    print(f"Data for '{student_name}' already exists in the CSV. Skipping...\n")
                    continue

                student = Student(student_name, self.module_columns)

                for module in self.module_columns:
                    mark = float(input(f"Enter the mark for {module}: "))
                    student.add_mark(module, mark)

                gpa = student.calculate_gpa(self.gpa_helper)
                print(f'GPA for {student_name}: {gpa:.2f}\n')

                # Construct a DataFrame from a dictionary containing the new student's data
                data_input = {"Student Name": [student_name]}
                for module, mark in student.marks.items():
                    data_input[module] = [mark]
                new_student_df = pd.DataFrame(data_input)

                df = pd.concat([df, new_student_df], ignore_index=True)
                self.csv_manager.save_data(df)
            elif choice.lower() == 'process':
                df = self.process_data()
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

    calculator = GPACalculator(module_columns, csv_file)

    # Process the data from the CSV file and display the results
    df = calculator.process_data()
    print(df)

    # Run live mode for calculating GPA
    calculator.run_live_mode()
