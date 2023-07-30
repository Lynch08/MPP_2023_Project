#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h> // Include math.h for the sqrt function

#define MAX_MODULES 6
#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50


struct GPAData {
    char student_name[MAX_NAME_LENGTH];
    float marks[MAX_MODULES];
};

struct GPAScale {
    char grade[3];
    float gpa_value;
};

struct GPAScale gpa_scale[] = {
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

struct Student {
    char student_name[MAX_NAME_LENGTH];
    float marks[MAX_MODULES];
    int num_modules;
};

// Function to calculate the GPA for a student
float calculate_gpa(const float marks[], int num_modules) {
    float gpa = 0.0;
    for (int i = 0; i < num_modules; i++) {
        char letter_grade = calculate_letter_grade(marks[i]);
        for (int j = 0; j < sizeof(gpa_scale) / sizeof(gpa_scale[0]); j++) {
            if (gpa_scale[j].grade[0] == letter_grade) {
                gpa += gpa_scale[j].gpa_value;
                break;
            }
        }
    }
    return gpa / num_modules;
}


// Function to calculate the letter grade based on the mark
char calculate_letter_grade(float mark) {
    if (mark < 50)
        return 'F';
    else if (mark < 60)
        return 'D';
    else if (mark < 70)
        return 'C';
    else if (mark < 80)
        return 'B';
    else if (mark < 90)
        return 'A';
    else
        return 'A';
}

// Function to read student data from the CSV file
int read_students_from_csv(struct Student students[]) {
    FILE* file = fopen("MPPSample.csv", "r");
    if (file == NULL) {
        printf("Error: Unable to open the CSV file.\n");
        return 0;
    }

    int num_students = 0;
    char line[200];

    // Skip the first line (header) in the CSV file
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%f,%f,%f,%f,%f,%f", students[num_students].student_name,
               &students[num_students].marks[0], &students[num_students].marks[1],
               &students[num_students].marks[2], &students[num_students].marks[3],
               &students[num_students].marks[4], &students[num_students].marks[5]);
        num_students++;
    }

    fclose(file);
    return num_students;
}

// Function to save student data to the CSV file
void save_student_to_csv(const struct Student* student) {
    FILE* file = fopen("MPPSample.csv", "a");
    if (file == NULL) {
        printf("Error: Unable to open the CSV file.\n");
        return;
    }

    fprintf(file, "%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", student->student_name,
            student->marks[0], student->marks[1], student->marks[2],
            student->marks[3], student->marks[4], student->marks[5]);

    fclose(file);
}

// Function to get module marks from the user and add them to the Student instance
void add_marks_to_student(struct Student* student, const char* module_columns[]) {
    for (int i = 0; i < MAX_MODULES; i++) {
        printf("Enter the mark for %s: ", module_columns[i]);
        scanf("%f", &student->marks[i]);
    }
    student->num_modules = MAX_MODULES;
}

int main() {
    const char* module_columns[MAX_MODULES] = {
        "Intro to Programming", "Databases", "Computer Architecture",
        "Ethics in Computer Science", "Advanced Programming", "Puzzles and Problem Solving"
    };

    struct Student students[MAX_STUDENTS];
    int num_students = read_students_from_csv(students);

    printf("\n--- Live Mode: GPA Calculator ---\n");

    while (true) {
        char student_name[MAX_NAME_LENGTH];
        printf("Enter student name (or 'm' for Menu): ");
        scanf("%s", student_name);

        if (strcmp(student_name, "m") == 0) {
            // Continue in live mode until the user chooses to quit
            while (true) {
                char choice[10];
                printf("Do you want to add another new student, display updated data, or process data again? (add/display/process/quit): ");
                scanf("%s", choice);

                if (strcmp(choice, "quit") == 0) {
                    return 0;
                } else if (strcmp(choice, "display") == 0) {
                    printf("\n--- Updated CSV Data ---\n");
                    for (int i = 0; i < num_students; i++) {
                        printf("%-25s", students[i].student_name);
                        for (int j = 0; j < MAX_MODULES; j++) {
                            printf("%.2f ", students[i].marks[j]);
                        }
                        printf("\n");
                    }
                } else if (strcmp(choice, "add") == 0) {
                    printf("Enter student name: ");
                    scanf("%s", student_name);

                    bool student_exists = false;
                    for (int i = 0; i < num_students; i++) {
                        if (strcmp(student_name, students[i].student_name) == 0) {
                            student_exists = true;
                            break;
                        }
                    }

                    if (student_exists) {
                        printf("Data for '%s' already exists in the CSV. Skipping...\n\n", student_name);
                        continue;
                    }

                    struct Student new_student;
                    strcpy(new_student.student_name, student_name);
                    add_marks_to_student(&new_student, module_columns);
                    float gpa = calculate_gpa(new_student.marks, new_student.num_modules);
                    printf("GPA for %s: %.2f\n\n", new_student.student_name, gpa);

                    save_student_to_csv(&new_student);
                    students[num_students++] = new_student;
                } else if (strcmp(choice, "process") == 0) {
                    printf("\n--- Processed Data ---\n");
                    printf("%-25s%-10s%-25s%-25s%-20s%-15s%-25s\n", "Student Name", "GPA", "Highest Scoring Module", "Lowest Scoring Module", "Standard Deviation", "Median Value", "GPA Difference from 4.2");
                    for (int i = 0; i < num_students; i++) {
                        float gpa = calculate_gpa(students[i].marks, students[i].num_modules);
                        // ... Implement the rest of the processed data display here as done previously ...
for (int i = 0; i < num_students; i++) {
    float gpa = calculate_gpa(students[i].marks, students[i].num_modules);
    float highest_score = students[i].marks[0];
    float lowest_score = students[i].marks[0];
    int highest_index = 0;
    int lowest_index = 0;
    float sum_scores = students[i].marks[0];
    float squared_sum_scores = students[i].marks[0] * students[i].marks[0];

    for (int j = 1; j < students[i].num_modules; j++) {
        if (students[i].marks[j] > highest_score) {
            highest_score = students[i].marks[j];
            highest_index = j;
        }
        if (students[i].marks[j] < lowest_score) {
            lowest_score = students[i].marks[j];
            lowest_index = j;
        }
        sum_scores += students[i].marks[j];
        squared_sum_scores += students[i].marks[j] * students[i].marks[j];
    }

    float mean = sum_scores / students[i].num_modules;
    float variance = (squared_sum_scores - (sum_scores * sum_scores) / students[i].num_modules) / students[i].num_modules;
    float standard_deviation = sqrt(variance);

    char formatted_highest[16];
    char formatted_lowest[16];
    strncpy(formatted_highest, module_columns[highest_index], 15);
    strncpy(formatted_lowest, module_columns[lowest_index], 15);
    formatted_highest[15] = '\0';
    formatted_lowest[15] = '\0';

    if (strlen(module_columns[highest_index]) > 15) {
        formatted_highest[12] = '\0';
        strcat(formatted_highest, "...");
    }
    if (strlen(module_columns[lowest_index]) > 15) {
        formatted_lowest[12] = '\0';
        strcat(formatted_lowest, "...");
    }

    printf("%-25s%-10.2f%-25s%-25s%-20.2f%-15.2f%-25.2f\n", students[i].student_name, gpa,
           formatted_highest, formatted_lowest, standard_deviation, mean, 4.2 - gpa);
}

                    }
                } else {
                    printf("Invalid choice. Please enter 'add', 'display', 'process', or 'quit'.\n");
                }
            }
        } else {
            bool student_exists = false;
            for (int i = 0; i < num_students; i++) {
                if (strcmp(student_name, students[i].student_name) == 0) {
                    student_exists = true;
                    break;
                }
            }

            if (student_exists) {
                printf("Data for '%s' already exists in the CSV. Skipping...\n\n", student_name);
                continue;
            }

            struct Student new_student;
            strcpy(new_student.student_name, student_name);
            add_marks_to_student(&new_student, module_columns);
            float gpa = calculate_gpa(new_student.marks, new_student.num_modules);
            printf("GPA for %s: %.2f\n\n", new_student.student_name, gpa);

            save_student_to_csv(&new_student);
            students[num_students++] = new_student;
        }
    }

    return 0;
}

