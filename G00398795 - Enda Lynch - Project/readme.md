# GPA Calculator README

## Procdural Programming Python
This code is a GPA (Grade Point Average) calculator that takes in student marks and calculates their GPA based on a custom GPA scale. It provides two main functionalities: processing existing data from a CSV file and running live mode to input new student data and calculate their GPA.  

Prerequisites  
To run this code, you need the following installed:  

 - Python (version 3.6 or higher)  
 - pandas library  
 - statistics module  

You can install the required libraries using pip:

```pip install pandas```

### Functions

 - calculate_letter_grade(mark): This function takes a numerical mark as input and returns the corresponding letter grade based on the GPA scale.

 - calculate_gpa(marks): This function takes a list of numerical marks as input and calculates the GPA by converting each mark to a letter grade and then averaging the GPA values.

 - process_data(module_columns, csv_file): This function processes the data from the CSV file specified by csv_file, calculates the GPA for each student, and adds additional columns with relevant statistics (e.g., highest scoring module, lowest scoring module, standard deviation, median value, and GPA difference from 4.2). It returns a DataFrame with the processed data.

 - run_live_mode(module_columns, csv_file): This function runs the GPA calculator in live mode, allowing the user to enter new student data interactively. The live mode also allows displaying the updated data, adding new students, and reprocessing the data.

### Running the Code
 - Ensure you have installed the required libraries mentioned in the Prerequisites section.

 - Save the code to a Python file (e.g., gpa_calculator.py).

 - Prepare the input CSV file (e.g., MPPSample.csv) containing the student data. The CSV should have a column for each module's marks and a 'Student Name' column.

 - Define the module columns by updating the module_columns list with the relevant column names from the CSV file.

 - Run the code using the following command:

```python gpa_calculator.py```  

The code will process the existing data from the CSV file and display the calculated GPA for each student.  

After processing the data, the code will enter live mode. In live mode, you can choose to add new students, display the updated data, process the data again, or quit the program.  

Note: In live mode, you can add new student data interactively by providing the student name and marks for each module. The newly added data will be appended to the CSV file.

### Important Notes
The code handles common errors, such as file not found, empty CSV, and unexpected errors, displaying appropriate error messages.  

The code is designed to handle the input and processing of the GPA, making it easy to use for both existing data processing and live mode data entry.  

The GPA scale used in this code is customizable. If needed, you can update the GPA scale by modifying the gpa_scale dictionary.  

Before running the code, ensure that the CSV file (MPPSample.csv in this case) exists in the same directory as the Python script or provide the correct file path if the CSV is located elsewhere.  

The code is structured and modular, making it easy to understand, maintain, and expand for additional functionalities if needed.  

The code is equipped with user-friendly prompts and error handling to provide a smooth and intuitive user experience.  

If you encounter any issues or have questions about the code, feel free to reach out for support.  

## OOP Programming Python

This GPA calculator is an Object-Oriented implementation that utilizes four classes: GPAScale, Student, CSVManager, and GPACalculator. The code is organized into separate files, making it easier to maintain and extend for additional functionalities. The classes provide a structured approach for handling GPA calculations and student data management.

### Class: GPAScale
##### Functionality
The GPAScale class represents the GPA scale and provides methods to map letter grades to corresponding GPA values.

#### Usage
To use the GPAScale class, import the class from gpa_scale.py and create an instance. You can then call the get_gpa method to calculate the GPA based on the provided letter grades.

### Class: Student
##### Functionality
The Student class represents a student and stores their information, including name and marks for each module.

#### Usage
To use the Student class, import the class from gpa_calculator.py and create an instance. You can then set the student's name and marks for each module using the provided methods.

### Class: CSVManager
##### Functionality
The CSVManager class handles reading and writing data to CSV files. It provides methods to load existing student data from a CSV file and save new student data to the same file.

#### Usage
To use the CSVManager class, import the class from csv_manager.py and create an instance. You can then use the load_data method to load existing student data and the save_data method to add new student data to the CSV file.

### Class: GPACalculator
##### Functionality
The GPACalculator class provides methods for processing data, calculating GPA for a list of marks, and performing statistical calculations on the student data.

#### Usage
To use the GPACalculator class, import the class from gpa_calculator.py and create an instance. You can then call the provided methods to process the data and perform GPA calculations.


### Main File: main.py
The main.py file serves as the entry point to run the GPA calculator. It imports the required classes and modules and provides a user-friendly command-line interface to interact with the calculator. The main file handles the processing of existing data and allows for live mode data entry and GPA calculation.

#### Running the Code
 - Ensure you have installed Python (version 3.6 or higher) and the required libraries (pandas).

 - Save the provided classes and main file to the respective Python files in the same directory.

 - Prepare the input CSV file (e.g., MPPSample.csv) containing the existing student data.

 - Modify the module columns in the main.py file to match the columns present in the CSV file.

 - Run the code using the following command:

 ```python main.py```

The code will process the existing data from the CSV file and display the calculated GPA for each student.  
After processing the data, the code will enter live mode. In live mode, you can add new student data interactively, display the updated data, or quit the program.


## GPA Calculator in C (Compiled with GCC) 

This GPA calculator is a command-line application written in C and compiled using GCC (GNU Compiler Collection). The program allows you to process student data from a CSV file, calculate GPAs, and perform statistical calculations such as finding the median, variance, and standard deviation of student marks. Additionally, it features a live mode, enabling users to interactively input new student data and calculate their GPAs.

Prerequisites
To run this program, you need the following:

 - GCC (GNU Compiler Collection): Installed on your system to compile the C code.
 - A CSV file (MPPSample.csv in this case): Prepared with the student data in a specific format (details below).
 - A way to Compile and Run the Code (GCC)
 - Save the provided C code in a file named gpa_calculator.c.

Open a terminal or command prompt in the same directory as the gpa_calculator.c file.

Compile the C code using GCC:  
gcc GPA_Calc.c -o GPA

 - After successful compilation, an executable file named gpa_calculator will be generated.  

Run the program by typing:  
GPA.exe


### Important Notes
 - The program uses a GPA scale and letter grade mapping similar to the provided code. You can customize the GPA scale or letter grade mapping by modifying the struct GPAScale gpa_scale[] array.

 - The program can ONLY handle up to 100 student records in the CSV file. If you have more than 100 records, the data would need to be split into multiple csv files.

 - The code is designed to handle invalid input. It will display appropriate error messages if invalid marks are provided during live mode data entry.

 - The program calculates statistical metrics like variance, median, and standard deviation to provide comprehensive information about the student performance.

