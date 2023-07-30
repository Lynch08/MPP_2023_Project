from gpa_calculator import GPACalculator


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