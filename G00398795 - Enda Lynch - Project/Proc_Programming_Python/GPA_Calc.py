# Import libraries
import pandas as pd
import statistics

# Define the GPA scale mapping letter grades to GPA values
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

# Function to calculate letter grades based on numeric marks
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

# Function to calculate GPA from a list of marks
def calculate_gpa(marks):
    letter_grades = [calculate_letter_grade(mark) for mark in marks]
    gpa = sum(gpa_scale[letter_grade] for letter_grade in letter_grades) / len(marks)
    return gpa

# Function to process data from a CSV file 
def process_data(module_columns, csv_file):
    try:
        df = pd.read_csv(csv_file)

        # Calculate letter grades for each module
        for module in module_columns:
            df[module + ' - Letter Grade'] = df[module].apply(calculate_letter_grade)

        # Calculate statistics for each student
        df['GPA'] = df[[module + ' - Letter Grade' for module in module_columns]].applymap(lambda x: gpa_scale[x]).mean(axis=1)
        df['Highest Scoring Module'] = df[module_columns].idxmax(axis=1)
        df['Lowest Scoring Module'] = df[module_columns].idxmin(axis=1)
        df['Standard Deviation'] = df[module_columns].apply(lambda row: statistics.stdev(row), axis=1)
        df['Median Value'] = df[module_columns].apply(lambda row: statistics.median(row), axis=1)
        df['GPA Difference from 4.2'] = 4.2 - df['GPA']

        # Return the relevant columns of the DataFrame for display
        return df[['Student Name', 'GPA', 'Highest Scoring Module', 'Lowest Scoring Module', 'Standard Deviation',
                   'Median Value', 'GPA Difference from 4.2']]
    except FileNotFoundError:
        print(f"Error: Unable to open file '{csv_file}'")
        return None
    except pd.errors.EmptyDataError:
        print(f"Error: The CSV file '{csv_file}' is empty or contains no data.")
        return None
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
        return None
    

# Function to run the live mode GPA calculator
def run_live_mode(module_columns, csv_file):
    print('\n--- Live Mode: GPA Calculator ---')
    while True:
        student_name = input("Enter student name (or 'm' for menu): ")
        if student_name.lower() == 'm':
            break

        try:
            df = pd.read_csv(csv_file)
            if student_name in df['Student Name'].values:
                print(f"Data for '{student_name}' already exists in the CSV. Skipping...\n")
                continue
                
                # Input marks for each module and calculate GPA for the new student
            marks = []
            for module in module_columns:
                while True:
                    try:
                        mark = float(input(f"Enter the mark for {module}: "))
                        if 0 <= mark <= 100:
                            marks.append(mark)
                            break
                        else:
                            print("Invalid mark. Please enter a value between 0 and 100.")
                    except ValueError:
                        print("Invalid input. Please enter a valid score.")
        
            gpa = calculate_gpa(marks)
            print(f'GPA for {student_name}: {gpa:.2f}\n')

            # Construct a DataFrame from a dictionary containing the new student's data
            data_input = {"Student Name": [student_name]}
            for i, module in enumerate(module_columns):
                data_input[module] = [marks[i]]
            new_student_df = pd.DataFrame(data_input)

            # Concatenate the new student's data with the existing DataFrame and save to the CSV file
            df = pd.concat([df, new_student_df], ignore_index=True)
            df.to_csv(csv_file, index=False)
        except FileNotFoundError:
            print(f"Error: Unable to open file '{csv_file}'")
            return
        except Exception as e:
            print(f"An unexpected error occurred: {e}")
            return
        
    # Additional Operations
    while True:
        choice = input("Do you want to add another new student, display updated data, or process data again? (add/display/process/quit): ")
        if choice.lower() == 'quit':
            break
        elif choice.lower() == 'display':
            try:
                print('\n--- Updated CSV Data ---')
                df = pd.read_csv(csv_file)
                print(df)
            except FileNotFoundError:
                print(f"Error: Unable to open file '{csv_file}'")
        elif choice.lower() == 'add':
            student_name = input("Enter student name: ")
            if student_name.lower() == 'q':
                break

            try:
                df = pd.read_csv(csv_file)
                if student_name in df['Student Name'].values:
                    print(f"Data for '{student_name}' already exists in the CSV. Skipping...\n")
                    continue

                marks = []
                for module in module_columns:
                    while True:
                        try:
                            mark = float(input(f"Enter the mark for {module}: "))
                            if 0 <= mark <= 100:
                                marks.append(mark)
                                break
                            else:
                                print("Invalid mark. Please enter a value between 0 and 100.")
                        except ValueError:
                            print("Invalid input. Please enter a valid score.")
                
                gpa = calculate_gpa(marks)
                print(f'GPA for {student_name}: {gpa:.2f}\n')

                # Construct a DataFrame from a dictionary containing the new student's data
                data_input = {"Student Name": [student_name]}
                for i, module in enumerate(module_columns):
                    data_input[module] = [marks[i]]
                new_student_df = pd.DataFrame(data_input)

                df = pd.concat([df, new_student_df], ignore_index=True)
                df.to_csv(csv_file, index=False)
            except FileNotFoundError:
                print(f"Error: Unable to open file '{csv_file}'")
                return
            except Exception as e:
                print(f"An unexpected error occurred: {e}")
                return
        elif choice.lower() == 'process':
            try:
                df = process_data(module_columns, csv_file)
                if df is not None:
                    print('\n--- Processed Data ---')
                    print(df)
            except FileNotFoundError:
                print(f"Error: Unable to open file '{csv_file}'")
        else:
            print("Invalid choice. Please enter 'add', 'display', 'process', or 'quit'.")
            continue

if __name__ == "__main__":
    # Define the module columns
    module_columns = ['Intro to Programming', 'Databases', 'Computer Architecture', 'Ethics in Computer Science',
                      'Advanced Programming', 'Puzzles and Problem Solving']

    csv_file = 'MPPSample.csv'

    # Process the data from the CSV file and display the results
    try:
        df = process_data(module_columns, csv_file)
        if df is not None:
            print(df)
    except FileNotFoundError:
        print(f"Error: Unable to open file '{csv_file}'")

    # Run live mode for calculating GPA
    run_live_mode(module_columns, csv_file)
