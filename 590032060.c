#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define FILE_NAME "student_records.dat"

// Structure to store student information

struct Student
{
    long long sapID;
    int rollNo;

    char name[50];
    char phone[15];
    char email[60];

    int year;
    int semester;

    float sgpa[8];
};

// Global variables

struct Student students[MAX_STUDENTS];

int studentCount = 0;


// Function declarations

void loadRecords();
void saveRecords();

void addStudent();
void searchStudent();
void displayAllStudents();

float calculateCGPA(struct Student s);

void calculateYearAverage();

void deleteStudent();

void sortBySAP();
void sortByName();

void academicAdvisor();

void displayStudent(struct Student s);


// Function to load records from file

void loadRecords()
{
    FILE *fp;

    fp = fopen(FILE_NAME, "rb");

    if (fp == NULL)
    {
        studentCount = 0;
        return;
    }

    fread(&studentCount, sizeof(int), 1, fp);

    if (studentCount < 0 || studentCount > MAX_STUDENTS)
    {
        studentCount = 0;
        fclose(fp);
        return;
    }

    fread(students, sizeof(struct Student),
          studentCount, fp);

    fclose(fp);
}


// Function to save records in file

void saveRecords()
{
    FILE *fp;

    fp = fopen(FILE_NAME, "wb");

    if (fp == NULL)
    {
        printf("\nError saving records!\n");
        return;
    }

    fwrite(&studentCount, sizeof(int), 1, fp);

    fwrite(students, sizeof(struct Student),
           studentCount, fp);

    fclose(fp);
}


// Function to calculate CGPA

float calculateCGPA(struct Student s)
{
    float total = 0;
    int count = 0;
    int i;

    for (i = 0; i < 8; i++)
    {
        if (s.sgpa[i] > 0)
        {
            total = total + s.sgpa[i];
            count++;
        }
    }

    if (count == 0)
    {
        return 0;
    }

    return total / count;
}


// Function to display student details

void displayStudent(struct Student s)
{
    int i;

    printf("\n------------------------------------\n");

    printf("SAP ID: %lld\n", s.sapID);

    printf("Roll Number: %d\n", s.rollNo);

    printf("Name: %s\n", s.name);

    printf("Phone Number: %s\n", s.phone);

    printf("Email ID: %s\n", s.email);

    printf("Year: %d\n", s.year);

    printf("Current Semester: %d\n", s.semester);

    printf("\nSemester-wise SGPA:\n");

    for (i = 0; i < 8; i++)
    {
        if (s.sgpa[i] > 0)
        {
            printf("Semester %d: %.2f\n",
                   i + 1, s.sgpa[i]);
        }
        else
        {
            printf("Semester %d: Not Available\n",
                   i + 1);
        }
    }

    printf("\nCGPA: %.2f\n", calculateCGPA(s));

    printf("------------------------------------\n");
}

// Function to add a new student

void addStudent()
{
    struct Student s;

    int i;
    int found = 0;

    if (studentCount >= MAX_STUDENTS)
    {
        printf("\nStudent storage is full!\n");
        return;
    }

    printf("\n========== ADD STUDENT ==========\n");

    printf("Enter SAP ID: ");
    scanf("%lld", &s.sapID);

    // Check duplicate SAP ID

    for (i = 0; i < studentCount; i++)
    {
        if (students[i].sapID == s.sapID)
        {
            found = 1;
            break;
        }
    }

    if (found)
    {
        printf("\nStudent with this SAP ID already exists!\n");
        return;
    }

    printf("Enter Roll Number: ");
    scanf("%d", &s.rollNo);

    getchar();

    printf("Enter Student Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter Phone Number: ");
    scanf("%14s", s.phone);

    printf("Enter Email ID: ");
    scanf("%59s", s.email);

    printf("Enter Year (1-4): ");
    scanf("%d", &s.year);

    if (s.year < 1 || s.year > 4)
    {
        printf("\nInvalid year!\n");
        return;
    }

    printf("Enter Current Semester (1-8): ");
    scanf("%d", &s.semester);

    if (s.semester < 1 || s.semester > 8)
    {
        printf("\nInvalid semester!\n");
        return;
    }

    printf("\nEnter SGPA for all 8 semesters.\n");
    printf("Enter 0 if result is not available.\n");

    for (i = 0; i < 8; i++)
    {
        printf("Semester %d SGPA: ", i + 1);

        scanf("%f", &s.sgpa[i]);

        if (s.sgpa[i] < 0 || s.sgpa[i] > 10)
        {
            printf("\nInvalid SGPA! Enter a value between 0 and 10.\n");
            i--;
        }
    }

    students[studentCount] = s;

    studentCount++;

    saveRecords();

    printf("\nStudent added successfully!\n");
}


// Function to search student

void searchStudent()
{
    long long id;
    int i;

    printf("\nEnter SAP ID to search: ");
    scanf("%lld", &id);

    for (i = 0; i < studentCount; i++)
    {
        if (students[i].sapID == id)
        {
            printf("\nStudent Record Found!\n");

            displayStudent(students[i]);

            return;
        }
    }

    printf("\nStudent not found!\n");
}


// Function to display all students

void displayAllStudents()
{
    int i;

    if (studentCount == 0)
    {
        printf("\nNo student records available!\n");
        return;
    }

    printf("\n========== ALL STUDENT RECORDS ==========\n");

    for (i = 0; i < studentCount; i++)
    {
        printf("\nStudent %d", i + 1);

        displayStudent(students[i]);
    }
}

// Function to calculate average CGPA year-wise

void calculateYearAverage()
{
    int year, i;

    float total;
    int count;

    printf("\n========== YEAR-WISE CGPA ==========\n");

    for (year = 1; year <= 4; year++)
    {
        total = 0;
        count = 0;

        for (i = 0; i < studentCount; i++)
        {
            if (students[i].year == year)
            {
                float cgpa = calculateCGPA(students[i]);

                if (cgpa > 0)
                {
                    total = total + cgpa;
                    count++;
                }
            }
        }

        if (count > 0)
        {
            printf("\nYear %d Average CGPA: %.2f",
                   year, total / count);
        }
        else
        {
            printf("\nYear %d: No student records available",
                   year);
        }
    }

    printf("\n");
}


// Function to delete a student

void deleteStudent()
{
    long long id;

    int i, j;

    printf("\nEnter SAP ID to delete: ");
    scanf("%lld", &id);

    for (i = 0; i < studentCount; i++)
    {
        if (students[i].sapID == id)
        {
            for (j = i; j < studentCount - 1; j++)
            {
                students[j] = students[j + 1];
            }

            studentCount--;

            saveRecords();

            printf("\nStudent record deleted successfully!\n");

            return;
        }
    }

    printf("\nStudent not found!\n");
}


// Function to sort students by SAP ID

void sortBySAP()
{
    int i, j;

    struct Student temp;

    if (studentCount == 0)
    {
        printf("\nNo records available!\n");
        return;
    }

    for (i = 0; i < studentCount - 1; i++)
    {
        for (j = 0; j < studentCount - i - 1; j++)
        {
            if (students[j].sapID > students[j + 1].sapID)
            {
                temp = students[j];

                students[j] = students[j + 1];

                students[j + 1] = temp;
            }
        }
    }

    saveRecords();

    printf("\nStudents sorted by SAP ID successfully!\n");

    displayAllStudents();
}


// Function to sort students by name

void sortByName()
{
    int i, j;

    struct Student temp;

    if (studentCount == 0)
    {
        printf("\nNo records available!\n");
        return;
    }

    for (i = 0; i < studentCount - 1; i++)
    {
        for (j = 0; j < studentCount - i - 1; j++)
        {
            if (strcmp(students[j].name,
                       students[j + 1].name) > 0)
            {
                temp = students[j];

                students[j] = students[j + 1];

                students[j + 1] = temp;
            }
        }
    }

    saveRecords();

    printf("\nStudents sorted alphabetically by name!\n");

    displayAllStudents();
}

// Function for AI-assisted academic advisor

void academicAdvisor()
{
    long long id;

    int i;

    float cgpa;

    printf("\n========== ACADEMIC ADVISOR ==========\n");

    printf("Enter Student SAP ID: ");

    scanf("%lld", &id);

    for (i = 0; i < studentCount; i++)
    {
        if (students[i].sapID == id)
        {
            cgpa = calculateCGPA(students[i]);

            printf("\nStudent Name: %s", students[i].name);

            printf("\nCurrent CGPA: %.2f\n", cgpa);

            if (cgpa == 0)
            {
                printf("\nNo academic results available yet.\n");
            }

            else if (cgpa >= 9.0)
            {
                printf("\nCategory: Excellent Performance\n");

                printf("Recommendation: Maintain your current study pattern.\n");
            }

            else if (cgpa >= 7.5)
            {
                printf("\nCategory: Good Performance\n");

                printf("Recommendation: Continue working consistently and aim higher.\n");
            }

            else if (cgpa >= 6.0)
            {
                printf("\nCategory: Needs Improvement\n");

                printf("Recommendation: Focus more on subjects with lower SGPA.\n");
            }

            else
            {
                printf("\nCategory: Requires Immediate Attention\n");

                printf("Recommendation: Improve study habits and seek academic guidance.\n");
            }

            return;
        }
    }

    printf("\nStudent not found!\n");
}


// Main function

int main()
{
    int choice;

    loadRecords();

    printf("\n============================================\n");

    printf("  AI-ASSISTED STUDENT RECORD MANAGEMENT\n");

    printf("============================================\n");

    do
    {
        printf("\n\n============== MAIN MENU ==============\n");

        printf("1. Add Student Details\n");

        printf("2. Search Student Record\n");

        printf("3. Display All Students\n");

        printf("4. Calculate Year-wise Average CGPA\n");

        printf("5. Delete Student Record\n");

        printf("6. Sort Students by SAP ID\n");

        printf("7. Sort Students by Name\n");

        printf("8. AI-Assisted Academic Advisor\n");

        printf("9. Exit\n");

        printf("\nEnter your choice: ");

        scanf("%d", &choice);

        switch (choice)
        {
            case 1:

                addStudent();

                break;


            case 2:

                searchStudent();

                break;


            case 3:

                displayAllStudents();

                break;


            case 4:

                calculateYearAverage();

                break;


            case 5:

                deleteStudent();

                break;


            case 6:

                sortBySAP();

                break;


            case 7:

                sortByName();

                break;


            case 8:

                academicAdvisor();

                break;


            case 9:

                saveRecords();

                printf("\nThank you for using the Student Management System!\n");

                break;


            default:

                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 9);

    return 0;
}