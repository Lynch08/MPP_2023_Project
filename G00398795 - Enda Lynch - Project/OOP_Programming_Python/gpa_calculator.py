import statistics
from gpa_scale import GPAScale  # Importing the GPAScale class from the gpa_scale module
from student import Student  # Importing the Student class from the student module
from csv_manager import CSVManager  # Importing the CSVManager class from the csv_manager module
import pandas as pd

class GPACalculator:
    def __init__(self, module_columns, csv_file):
        self.module_columns = module_columns
        self.csv_manager = CSVManager(csv_file)  # Creating an instance of CSVManager with the provided csv_file
        self.gpa_scale = GPAScale()  # Creating an instance of GPAScale to access the GPA scale dictionary

    def process_data(self):
        df = self.csv_manager.read_data()  # Reading data from the CSV using the CSVManager instance

        # Calculate letter grades for each module and add new columns to the DF
        for module in self.module_columns:
            df[module + ' - Letter Grade'] = df[module].apply(self.gpa_scale.calculate_letter_grade)

        # Calculate the GPA for each student and add new columns to the DF
        df['GPA'] = df[[module + ' - Letter Grade' for module in self.module_columns]].applymap(lambda x: self.gpa_scale.gpa_scale[x]).mean(axis=1)

        # Find the highest scoring and lowest scoring modules for each student
        df['Highest Scoring Module'] = df[self.module_columns].idxmax(axis=1)
        df['Lowest Scoring Module'] = df[self.module_columns].idxmin(axis=1)

        # Calculate the standard deviation and median values for each student's module scores
        df['Standard Deviation'] = df[self.module_columns].apply(lambda row: statistics.stdev(row), axis=1)
        df['Median Value'] = df[self.module_columns].apply(lambda row: statistics.median(row), axis=1)

        # Calculate the difference between each student's GPA and the maximum possible GPA (4.2)
        df['GPA Difference from 4.2'] = 4.2 - df['GPA']

        return df[['Student Name', 'GPA', 'Highest Scoring Module', 'Lowest Scoring Module', 'Standard Deviation',
                   'Median Value', 'GPA Difference from 4.2']]

    def run_live_mode(self):
        print('\n--- Live Mode: GPA Calculator ---')
        while True:
            student_name = input("Enter student name (or 'm' for Menu): ")
            if student_name.lower() == 'm':
                break

            df = self.csv_manager.read_data()  # Reading data from the CSV using the CSVManager instance
            if student_name in df['Student Name'].values:
                print(f"Data for '{student_name}' already exists in the CSV. Skipping...\n")
                continue

            student = Student(student_name, self.module_columns)  # Creating a new Student instance

            # Get module marks from the user and add them to the Student instance
            for module in self.module_columns:
                mark = float(input(f"Enter the mark for {module}: "))
                student.add_mark(module, mark)
            
            # Calculate the GPA for the student and print the result
            gpa = student.calculate_gpa(self.gpa_scale)
            print(f'GPA for {student_name}: {gpa:.2f}\n')

            # Construct a DF from a dictionary containing the new student's data
            data_input = {"Student Name": [student_name]}
            for module, mark in student.marks.items():
                data_input[module] = [mark]
            new_student_df = pd.DataFrame(data_input)

            # Concatenate the new student's data with the existing DF and save it to the CSV
            df = pd.concat([df, new_student_df], ignore_index=True)
            self.csv_manager.save_data(df)

        # Continue in live mode until the user chooses to quit
        while True:
            choice = input("Do you want to add another new student, display updated data, or process data again? (add/display/process/quit): ")
            if choice.lower() == 'quit':
                break
            elif choice.lower() == 'display':
                print('\n--- Updated CSV Data ---')
                df = self.csv_manager.read_data()  # Reading data from the CSV using the CSVManager instance
                print(df)
            elif choice.lower() == 'add':
                student_name = input("Enter student name: ")
                if student_name.lower() == 'q':
                    break

                df = self.csv_manager.read_data()  # Reading data from the CSV using the CSVManager instance
                if student_name in df['Student Name'].values:
                    print(f"Data for '{student_name}' already exists in the CSV. Skipping...\n")
                    continue

                student = Student(student_name, self.module_columns)  # Creating a new Student instance

                # Get module marks from the user and add them to the Student instance
                for module in self.module_columns:
                    mark = float(input(f"Enter the mark for {module}: "))
                    student.add_mark(module, mark)

                # Calculate the GPA for the student and print the result
                gpa = student.calculate_gpa(self.gpa_scale)
                print(f'GPA for {student_name}: {gpa:.2f}\n')

                # Create DF from a dictionary containing the new student's data
                data_input = {"Student Name": [student_name]}
                for module, mark in student.marks.items():
                    data_input[module] = [mark]
                new_student_df = pd.DataFrame(data_input)

                # Concatenate the new student's data with the existing DF and save it to the CSV
                df = pd.concat([df, new_student_df], ignore_index=True)
                self.csv_manager.save_data(df)
            elif choice.lower() == 'process':
                df = self.process_data()  # Process the data and get the results 
                print('\n--- Processed Data ---')
                print(df)
            else:
                print("Invalid choice. Please enter 'add', 'display', 'process', or 'quit'.")
                continue