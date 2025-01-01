#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Globally declared so that these variables can be used anywhere throughout the program.
// Declared as constant so that these cannot be updated throughout the program.
/* As the address of the first element of an array is the exact address of that array,
   a character pointer is used to declare the file names which points to the first character (Ex-'a') of the string (Ex-'attendance_records.txt').*/

const char *ATTENDANCE = "attendance_records.txt";
const char *NAMES = "student_names.txt";
const char *ATTENDANCE_DETAILS = "attendance_details.txt";
const int TOTAL_STUDENT = 10;

// Structure that stores the students' details
struct Student
{
    int serialNumber;         // Student Serial Number
    char name[50];            // Name of Student
    int attendanceStatus;     // 1 for present and 0 for absent in a day
    int totalClasses;         // Total number of classes taken by the teacher
    int totalPresent;         // Total classes that student has attended.
    int subjectAttendance[5]; // Track attendance for different subjects (5 subjects in total)
};

// All function declarations
void saveStudentNames(struct Student students[]);
void loadStudentNames(struct Student students[]);
void takeAttendance(struct Student students[]);
void saveStudentAttendance(struct Student students[]);
void viewRecords();
void viewStudentAttendance();
int authenticateTeacher();

// This function is used to take the names of students and save them in the student_names.txt file.
// This function is executed only during the first run of the program or also executed if there is any problem in loading the students' names.
void saveStudentNames(struct Student students[])
{
    FILE *ptr = fopen(NAMES, "w"); // NAMES is already declared globally to be 'student_names.txt'
    if (ptr == NULL)
    {
        printf("Error opening file for writing student names\n");
        return;
    } // If the file is not present it shows error
    for (int i = 0; i < TOTAL_STUDENT; i++)
    {
        printf("Enter name for Student %d: ", i + 1);
        fgets(students[i].name, sizeof(students[i].name), stdin); // stdin means we are taking input from the keyboard
        students[i].name[strcspn(students[i].name, "\n")] = '\0'; // Changes the new line character of null character.
        students[i].serialNumber = i + 1;                         // Initializing student serial number with name
        students[i].totalClasses = 0;                             // Initially no classes taken
        students[i].totalPresent = 0;                             // Initially no presence
        fprintf(ptr, "%s\n", students[i].name);                   // Saves student names to the mentioned file
    }

    fclose(ptr);
}

// This function gets the student's name whenever the teacher is taking the attendance.
void loadStudentNames(struct Student students[])
{
    FILE *ptr = fopen(NAMES, "r");
    if (ptr == NULL)
    {
        printf("No previous student names found. Please enter names.\n");
        saveStudentNames(students);
        return;
    } // If file doesn't exist, it tells the user to input all student's names.

    // To check if the file is empty
    char buffer[50];
    if (fgets(buffer, sizeof(buffer), ptr) == NULL)
    {
        fclose(ptr);
        printf("Student names file is empty. Please enter names.\n");
        saveStudentNames(students); // If file is empty, tells user to input all student's names.
        return;
    }
    rewind(ptr); // It resets the FILE pointer to the beginning of the file for processing the next codes from the beginning.

    // Read the names of students from the file and assign them to an array (Ex-students[i].name).
    for (int i = 0; i < TOTAL_STUDENT; i++)
    {
        char *result = fgets(students[i].name, sizeof(students[i].name), ptr); // Read all names from ptr one by one due to loop.
        if (result != NULL)
        {
            students[i].name[strcspn(students[i].name, "\n")] = '\0'; // Removes new line as a character
            students[i].serialNumber = i + 1;                         // Assigns serial number
            students[i].totalClasses = 0;                             // Initialize total classes to 0
            students[i].totalPresent = 0;                             // Initialize total present to 0
        }
        else
        {
            printf("Error reading names from file.\n");
            break;
        }
    }

    fclose(ptr);
}

// This function works when the teacher is taking attendance.
void takeAttendance(struct Student students[])
{
    printf("Enter Date (dd-mm-yyyy): "); // Asks for the date of attendance
    char date[15];
    scanf("%s", date);

    printf("Taking Attendance for %s.\n", date);
    // For taking total attendance of students one by one.
    for (int i = 0; i < TOTAL_STUDENT; i++)
    {
        int check;
        printf("%d. %s: ", students[i].serialNumber, students[i].name);
        scanf("%d", &check); // Enter 1 for present, 0 for absent
        if (check != 0 && check != 1)
        {
            printf("Invalid Input. Please Enter 1 (Present) or 0 (Absent).\n");
            i--; // Decrements index to re-enter invalid entry. If it is not used, the program skips the invalid entered student's attendance.
        }
        else
        {
            students[i].attendanceStatus = check; // Stores present or absent to student's attendance status.
            if (check == 1)
            {
                students[i].totalPresent++; // Increment present count if student is present, to calculate total presence.
            }
        }
    }

    FILE *ptr = fopen(ATTENDANCE, "a"); // Append is used not to delete the previous details in file.
    if (ptr == NULL)
    {
        printf("Error opening file for writing attendance\n");
        return;
    }

    // Saves attendance details in 'attendance_records.txt'.
    fprintf(ptr, "Attendance for %s:\n", date);
    for (int i = 0; i < TOTAL_STUDENT; i++)
    {
        fprintf(ptr, "%d. %s: %s\n", students[i].serialNumber, students[i].name, students[i].attendanceStatus ? "Present" : "Absent"); // Ternary operator, present if condition is true(1), absent if false(0).
    }
    fprintf(ptr, "\n");
    fclose(ptr);
    // Increment the total classes held directly after storing the student details.
    for (int i = 0; i < TOTAL_STUDENT; i++)
    {
        students[i].totalClasses++;
    }

    printf("Attendance saved successfully for %s.\n", date);

    saveStudentAttendance(students); // Direct jump to another function to save the same details in different format for student view.
}

void saveStudentAttendance(struct Student students[])
{
    FILE *ptr = fopen(ATTENDANCE_DETAILS, "w");
    if (ptr == NULL)
    {
        printf("Error opening file for writing student attendance\n");
        return;
    }
    // Saves all student's attendance percentage
    for (int i = 0; i < TOTAL_STUDENT; i++)
    {
        int percentage = (students[i].totalClasses > 0) ? (students[i].totalPresent * 100) / students[i].totalClasses : 0;
        fprintf(ptr, "%d. %s - %d%% (%d/%d)\n", students[i].serialNumber, students[i].name, percentage, students[i].totalPresent, students[i].totalClasses);
    }

    fclose(ptr);
}

// This function is used to show all the details saved in 'attendance_records.txt'.
void viewRecords()
{
    FILE *ptr = fopen(ATTENDANCE, "r");
    if (ptr == NULL)
    {
        printf("No attendance records found.\n");
        return;
    }

    printf("\nAttendance Records:\n");
    char ch = fgetc(ptr); // Assign ch to the first character of 'attendance_records.txt'.
    while (ch != EOF)     // Run a loop until the last character of FILE pointer i.e 'attendance_records.txt'.
    {
        putchar(ch);     // Print characters one by one in every iteration of the loop.
        ch = fgetc(ptr); // Re-assign ch to the next character after completing one iteration of the loop.
    }

    fclose(ptr);
}

// This function is used to show the attendance percentage to students.
void viewStudentAttendance()
{
    FILE *ptr = fopen(ATTENDANCE_DETAILS, "r");
    if (ptr == NULL)
    {
        printf("No attendance information found.\n");
        return;
    }

    printf("\nYour Attendance Information:\n");
    char ch = fgetc(ptr);
    while (ch != EOF)
    {
        putchar(ch);
        ch = fgetc(ptr);
    }

    fclose(ptr);
}

// This function ensures that the teacher is giving the correct password to enter the attendance program.
int authenticateTeacher()
{
    char enteredPassword[50];
    printf("Enter teacher password: ");
    scanf("%s", enteredPassword);

    if (strcmp(enteredPassword, "123") == 0) // Compares the two strings to be same.
    {
        return 1;
    }
    else
    {
        printf("Incorrect password. Access denied.\n");
        return 0;
    }
}

int main()
{
    struct Student students[TOTAL_STUDENT]; // Array of structure of the details of students.
    int enter;

    loadStudentNames(students); // Load the student names initially to use them.
    // Choose what you are
    printf("1. Teacher\n");
    printf("2. Student\n");
    printf("Press 1 or 2 to enter: \n");
    scanf("%d", &enter);

    if (enter == 1)
    {
        if (authenticateTeacher()) // If teacher enters correct password
        {
            while (1) // While loop runs continuously until it reaches a return or break statement.
            {
                // Teacher-Menu
                printf("Welcome to Attendance System\n");
                printf("1. Take Attendance\n");
                printf("2. View Attendance Records\n");
                printf("3. Exit\n");
                printf("Enter your choice: ");
                int choice;
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    takeAttendance(students); // Function Call to Take attendance for students
                    break;
                case 2:
                    viewRecords(); // Function Call to View all attendance records
                    break;
                case 3:
                    printf("Exiting the program.\n");
                    return 0; // Exits the program
                default:
                    printf("Invalid input. Please try again.\n");
                    break;
                }
            }
        }
        else
        {
            printf("Exiting the program.\n");
            return 0; // Exits the program if the password is incorrect
        }
    }
    else if (enter == 2)
    {
        while (1)
        {
            // Student Menu
            printf("Welcome to Attendance System\n");
            printf("1. View Attendance\n");
            printf("2. Exit\n");
            printf("Enter your choice: ");
            int choice;
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                viewStudentAttendance(); // View total attendance with percentage
                break;
            case 2:
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid input. Please try again.\n");
                break;
            }
        }
    }
    else
    {
        printf("Invalid input. Exiting the program.\n"); // If any other option is chosen despite 1 (Teacher) or 2(Student).
    }

    return 0;
}