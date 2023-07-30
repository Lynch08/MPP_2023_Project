#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_MODULES 6

struct GPAData {
    char student_name[50];
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

float calculate_gpa(float marks[], int num_modules) {
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

void process_data(const char* module_columns[], int num_modules, const char* csv_file) {
    FILE* file = fopen(csv_file, "r");
    if (file == NULL) {
        printf("Error: Unable to open file '%s'\n", csv_file);
        return;
    }

    char line[200];
    struct GPAData data[100];
    int num_students = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%f,%f,%f,%f,%f,%f", data[num_students].student_name,
               &data[num_students].marks[0], &data[num_students].marks[1],
               &data[num_students].marks[2], &data[num_students].marks[3],
               &data[num_students].marks[4], &data[num_students].marks[5]);
        num_students++;
    }

    fclose(file);

    // Calculate GPA and other metrics
    printf("\n--- Processed Data ---\n");
    printf("%-25s%-10s%-25s%-25s%-20s%-15s%-25s\n", "Student Name", "GPA", "Highest Scoring Module", "Lowest Scoring Module", "Standard Deviation", "Median Value", "GPA Difference from 4.2");

    for (int i = 0; i < num_students; i++) {
        float gpa = calculate_gpa(data[i].marks, num_modules);
        float highest_score = data[i].marks[0];
        float lowest_score = data[i].marks[0];
        int highest_index = 0;
        int lowest_index = 0;
        float sum_scores = data[i].marks[0];
        float squared_sum_scores = data[i].marks[0] * data[i].marks[0];

        for (int j = 1; j < num_modules; j++) {
            if (data[i].marks[j] > highest_score) {
                highest_score = data[i].marks[j];
                highest_index = j;
            }
            if (data[i].marks[j] < lowest_score) {
                lowest_score = data[i].marks[j];
                lowest_index = j;
            }
            sum_scores += data[i].marks[j];
            squared_sum_scores += data[i].marks[j] * data[i].marks[j];
        }

        float mean = sum_scores / num_modules;
        float variance = (squared_sum_scores - (sum_scores * sum_scores) / num_modules) / num_modules;
        float standard_deviation = sqrt(variance);

        printf("%-25s%-10.2f%-25s%-25s%-20.2f%-15.2f%-25.2f\n", data[i].student_name, gpa,
               module_columns[highest_index], module_columns[lowest_index], standard_deviation, mean, 4.2 - gpa);
    }
}

void run_live_mode(const char* module_columns[], int num_modules, const char* csv_file) {
    printf("\n--- Live Mode: GPA Calculator ---\n");

    while (true) {
        char student_name[100]; // Increase the buffer size to accommodate forename and surname
        printf("Enter student name (or 'q' to quit): ");
        fgets(student_name, sizeof(student_name), stdin);

        // Remove the newline character at the end of the input
        student_name[strcspn(student_name, "\n")] = '\0';

        if (strcmp(student_name, "q") == 0)
            break;

        FILE* file = fopen(csv_file, "r");
        if (file == NULL) {
            printf("Error: Unable to open file '%s'\n", csv_file);
            return;
        }

        bool student_exists = false;
        char line[200];

        while (fgets(line, sizeof(line), file)) {
            char name[100]; // Increase the buffer size to accommodate forename and surname
            sscanf(line, "%[^,],", name);
            if (strcmp(name, student_name) == 0) {
                student_exists = true;
                break;
            }
        }

        fclose(file);

        if (student_exists) {
            printf("Data for '%s' already exists in the CSV. Skipping...\n\n", student_name);
            continue;
        }

        float marks[MAX_MODULES];
        for (int i = 0; i < num_modules; i++) {
            printf("Enter the mark for %s: ", module_columns[i]);
            scanf("%f", &marks[i]);
        }

        float gpa = calculate_gpa(marks, num_modules);
        printf("GPA for %s: %.2f\n\n", student_name, gpa);

        FILE* append_file = fopen(csv_file, "a");
        if (append_file == NULL) {
            printf("Error: Unable to open file '%s'\n", csv_file);
            return;
        }

        fprintf(append_file, "%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", student_name,
                marks[0], marks[1], marks[2], marks[3], marks[4], marks[5]);
        fclose(append_file);
    }
}


int main() {
    // Define the module columns
    const char* module_columns[MAX_MODULES] = {
        "Intro to Programming", "Databases", "Computer Architecture",
        "Ethics in Computer Science", "Advanced Programming", "Puzzles and Problem Solving"
    };

    const char* csv_file = "MPPSample.csv";

    // Process the data from the CSV file and display the results
    process_data(module_columns, MAX_MODULES, csv_file);

    // Run live mode for calculating GPA
    run_live_mode(module_columns, MAX_MODULES, csv_file);

    return 0;
}
