#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Define the GPA scale
typedef struct {
    char grade[3];
    float value;
} GPA_Scale;

GPA_Scale gpa_scale[] = {
    {"A+", 4.2},
    {"A", 4.0},
    {"A-", 3.8},
    {"B+", 3.3},
    {"B", 3.0},
    {"B-", 2.7},
    {"C+", 2.3},
    {"C", 2.0},
    {"C-", 1.7},
    {"D+", 1.3},
    {"D", 1.0},
    {"F", 0.0}
};

// Define the Module structure
typedef struct {
    char name[100];
    int mark;
    char grade[3];
} Module;

// Define the Student structure
typedef struct {
    char name[100];
    Module modules[6];
    float gpa;
    char highest_module[100];
    char lowest_module[100];
    float standard_deviation;
    float median_value;
    float gpa_difference_from_4_2;
} Student;

// Function to calculate the letter grade based on the mark
void calculate_letter_grade(Module* module) {
    int mark = module->mark;
    if (mark < 50)
        strcpy(module->grade, "F");
    else if (mark < 60)
        strcpy(module->grade, "D");
    else if (mark < 70)
        strcpy(module->grade, "C");
    else if (mark < 80)
        strcpy(module->grade, "B");
    else if (mark < 90)
        strcpy(module->grade, "A-");
    else
        strcpy(module->grade, "A");
}

// Function to calculate the GPA for a student
void calculate_gpa(Student* student) {
    float total = 0.0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 12; j++) {
            if (strcmp(student->modules[i].grade, gpa_scale[j].grade) == 0) {
                total += gpa_scale[j].value;
                break;
            }
        }
    }
    student->gpa = total / 6.0;
}

// Function to calculate the highest scoring module for a student
void calculate_highest_scoring_module(Student* student) {
    int max_mark = -1;
    int max_module_index = -1;
    for (int i = 0; i < 6; i++) {
        if (student->modules[i].mark > max_mark) {
            max_mark = student->modules[i].mark;
            max_module_index = i;
        }
    }
    if (max_module_index >= 0) {
        strcpy(student->highest_module, student->modules[max_module_index].name);
    }
}

// Function to calculate the lowest scoring module for a student
void calculate_lowest_scoring_module(Student* student) {
    int min_mark = 101;
    int min_module_index = -1;
    for (int i = 0; i < 6; i++) {
        if (student->modules[i].mark < min_mark) {
            min_mark = student->modules[i].mark;
            min_module_index = i;
        }
    }
    if (min_module_index >= 0) {
        strcpy(student->lowest_module, student->modules[min_module_index].name);
    }
}

// Function to calculate the standard deviation of module marks for a student
void calculate_standard_deviation(Student* student) {
    float sum = 0;
    for (int i = 0; i < 6; i++) {
        sum += student->modules[i].mark;
    }
    float mean = sum / 6.0;
    
    float variance_sum = 0;
    for (int i = 0; i < 6; i++) {
        float diff = student->modules[i].mark - mean;
        variance_sum += diff * diff;
    }
    float variance = variance_sum / 6.0;
    
    student->standard_deviation = sqrt(variance);
}

// Function to calculate the median value of module marks for a student
void calculate_median_value(Student* student) {
    int sorted_marks[6];
    for (int i = 0; i < 6; i++) {
        sorted_marks[i] = student->modules[i].mark;
    }

    // Sort the marks in ascending order
    for (int i = 0; i < 5; i++) {
        for (int j = i + 1; j < 6; j++) {
            if (sorted_marks[j] < sorted_marks[i]) {
                int temp = sorted_marks[i];
                sorted_marks[i] = sorted_marks[j];
                sorted_marks[j] = temp;
            }
        }
    }

    if (6 % 2 == 0) {
        int mid1 = 6 / 2 - 1;
        int mid2 = 6 / 2;
        student->median_value = (sorted_marks[mid1] + sorted_marks[mid2]) / 2.0;
    } else {
        int mid = 6 / 2;
        student->median_value = sorted_marks[mid];
    }
}

// Function to calculate the difference between GPA and 4.2
void calculate_gpa_difference(Student* student) {
    student->gpa_difference_from_4_2 = 4.2 - student->gpa;
}

// Function to process the data and calculate statistics
void process_data(Student* students, int num_students) {
    FILE* file = fopen("MPPSample.csv", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), file);  // Read header line

    for (int i = 0; i < num_students; i++) {
        fgets(line, sizeof(line), file);
        char* token = strtok(line, ",");
        strcpy(students[i].name, token);

        for (int j = 0; j < 6; j++) {
            token = strtok(NULL, ",");
            students[i].modules[j].mark = atoi(token);
            calculate_letter_grade(&students[i].modules[j]);
        }

        calculate_highest_scoring_module(&students[i]);
        calculate_lowest_scoring_module(&students[i]);
        calculate_standard_deviation(&students[i]);
        calculate_median_value(&students[i]);
        calculate_gpa(&students[i]);
        calculate_gpa_difference(&students[i]);
    }

    fclose(file);
}

int main() {
    // Read the data from the CSV file and process it
    Student students[10];  // Assuming there are 10 students or less in the CSV file
    process_data(students, 10);

    // Display the results
    printf("Student Name\t\tGPA\tHighest Scoring Module\tLowest Scoring Module\tStandard Deviation\tMedian Value\tGPA Difference from 4.2\n");
    for (int i = 0; i < 10; i++) {
        printf("%s\t\t%.2f\t%s\t%s\t%.2f\t%.2f\t%.2f\n", students[i].name, students[i].gpa,
               students[i].highest_module, students[i].lowest_module, students[i].standard_deviation,
               students[i].median_value, students[i].gpa_difference_from_4_2);
    }

    // Run live mode for calculating GPA
    printf("\n--- Live Mode ---\n");
    char choice;
    do {
        int marks[6];
        for (int i = 0; i < 6; i++) {
            printf("Enter the mark for Module %d: ", i + 1);
            scanf("%d", &marks[i]);
        }

        Student live_student;
        strcpy(live_student.name, "Live Student");
        for (int i = 0; i < 6; i++) {
            live_student.modules[i].mark = marks[i];
            calculate_letter_grade(&live_student.modules[i]);
        }
        calculate_gpa(&live_student);
        calculate_highest_scoring_module(&live_student);
        calculate_lowest_scoring_module(&live_student);

        printf("GPA: %.2f\n", live_student.gpa);


        printf("Do you want to calculate another GPA? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');

    return 0;
}
