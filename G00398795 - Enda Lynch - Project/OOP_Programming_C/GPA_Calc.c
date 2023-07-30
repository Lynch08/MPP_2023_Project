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

    // Skip the first line (header) in the CSV file
    fgets(line, sizeof(line), file);

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

        // Format the highest and lowest scoring module names
        char formatted_highest[16]; // Max 15 characters + null terminator
        char formatted_lowest[16];  // Max 15 characters + null terminator
        strncpy(formatted_highest, module_columns[highest_index], 15);
        strncpy(formatted_lowest, module_columns[lowest_index], 15);
        formatted_highest[15] = '\0'; // Ensure null termination
        formatted_lowest[15] = '\0';  // Ensure null termination

        // Add "..." if the module names are longer than 15 characters
        if (strlen(module_columns[highest_index]) > 15) {
            formatted_highest[12] = '\0'; // Truncate to 12 characters
            strcat(formatted_highest, "...");
        }
        if (strlen(module_columns[lowest_index]) > 15) {
            formatted_lowest[12] = '\0'; // Truncate to 12 characters
            strcat(formatted_lowest, "...");
        }

        printf("%-25s%-10.2f%-25s%-25s%-20.2f%-15.2f%-25.2f\n", data[i].student_name, gpa,
               formatted_highest, formatted_lowest, standard_deviation, mean, 4.2 - gpa);
    }
}

void run_live_mode(const char* module_columns[], int num_modules, const char* csv_file) {
    printf("\n--- Live Mode: GPA Calculator ---\n");

    while (true) {
        char input[50];
        printf("Enter student name or 'm' for menu: ");
        scanf(" %49[^\n]", input);

        if (strcmp(input, "m") == 0) {
            // Display the menu options
            printf("\nMenu Options:\n");
            printf("Do you want to add another new student, display updated data, or process data again? (add/display/process/quit):\n");
            printf("Enter your choice: ");

            char choice[20];
            scanf(" %19s", choice);

            if (strcmp(choice, "add") == 0) {
                FILE* append_file = fopen(csv_file, "a");
                if (append_file == NULL) {
                    printf("Error: Unable to open file '%s'\n", csv_file);
                    return;
                }

                // Code to add a student with the entered name and scores
                float marks[MAX_MODULES];
                printf("Enter student name: ");
                scanf(" %49[^\n]", input);

                for (int i = 0; i < num_modules; i++) {
                    while (true) {
                        printf("Enter the mark for %s: ", module_columns[i]);
                        if (scanf("%f", &marks[i]) != 1) {
                            // Clear input buffer if invalid input is provided
                            while (getchar() != '\n');
                            printf("Invalid input. Please enter a valid score between 0 and 100.\n");
                        } else if (marks[i] < 0 || marks[i] > 100) {
                            printf("Invalid mark. Please enter a value between 0 and 100.\n");
                        } else {
                            break;
                        }
                    }
                }

                float gpa = calculate_gpa(marks, num_modules);
                printf("GPA for %s: %.2f\n\n", input, gpa);

                fprintf(append_file, "%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", input,
                        marks[0], marks[1], marks[2], marks[3], marks[4], marks[5]);
                fclose(append_file);
            } else if (strcmp(choice, "display") == 0) {
                FILE* file = fopen(csv_file, "r");
                if (file == NULL) {
                    printf("Error: Unable to open file '%s'\n", csv_file);
                    return;
                }

                printf("\n--- Data in the CSV file ---\n");
                char line[200];
                while (fgets(line, sizeof(line), file)) {
                    printf("%s", line);
                }
                fclose(file);
            } else if (strcmp(choice, "process") == 0) {
                process_data(module_columns, num_modules, csv_file);
            } else if (strcmp(choice, "quit") == 0) {
                printf("Quitting the program.\n");
                break;
            } else {
                printf("Invalid menu option. Please try again.\n\n");
            }
        } else {
            // Code to handle student name entry (same as before)
            FILE* file = fopen(csv_file, "r");
            if (file == NULL) {
                printf("Error: Unable to open file '%s'\n", csv_file);
                return;
            }

            bool student_exists = false;
            char line[200];

            while (fgets(line, sizeof(line), file)) {
                char name[50];
                sscanf(line, "%[^,],", name);
                if (strcmp(name, input) == 0) {
                    student_exists = true;
                    break;
                }
            }

            fclose(file);

            if (student_exists) {
                printf("Data for '%s' already exists in the CSV. Skipping...\n\n", input);
                continue;
            }

            float marks[MAX_MODULES];
            for (int i = 0; i < num_modules; i++) {
                while (true) {
                    printf("Enter the mark for %s: ", module_columns[i]);
                    if (scanf("%f", &marks[i]) != 1) {
                        // Clear input buffer if invalid input is provided
                        while (getchar() != '\n');
                        printf("Invalid input. Please enter a valid score between 0 and 100.\n");
                    } else if (marks[i] < 0 || marks[i] > 100) {
                        printf("Invalid mark. Please enter a value between 0 and 100.\n");
                    } else {
                        break;
                    }
                }
            }

            float gpa = calculate_gpa(marks, num_modules);
            printf("GPA for %s: %.2f\n\n", input, gpa);

            FILE* append_file = fopen(csv_file, "a");
            if (append_file == NULL) {
                printf("Error: Unable to open file '%s'\n", csv_file);
                return;
            }

            fprintf(append_file, "%s,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", input,
                    marks[0], marks[1], marks[2], marks[3], marks[4], marks[5]);
            fclose(append_file);
        }
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

