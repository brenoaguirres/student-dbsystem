/*
**************************************************************************************************
Instituto Federal de Goiás - IFG   **** 24/11/2023.
Student: Breno Freitas Aguirres.
Semester: 2nd.
Teacher: Felipe Almeida Araújo.
Subject: Interdisciplinary Project.
**************************************************************************************************

IFG SYSTEM:

Project: IFG System is an interface for a database with information about IFG student's incomes
to see if they're eligible for receiving social benefits. The main goal is to optimize the work
of IFG's social workers.


**************************************************************************************************
About this file:
**************************************************************************************************
- main.c is the entry point for the program and acts as the center pillar for it. 
Used for centering procedures.
__________________________________________________________________________________________________
*/

// Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbconnector.c"
#include <libpq-fe.h>


// Declaring Structs
typedef struct Student {
    int id;
    char name[100];
    float income;
    int is_active;
} Student;


// Declaring Functions
void registerStudent(PGconn *connection, Student student);
void updateStudent(PGconn *connection, Student student);
int searchStudent(PGconn *connection, int s_id);


// main function
int main() {

    int buf_cleaner;
    PGconn *conn = connect_db();

    Student student1;
    Student student2;

    int option;
    do {
        printf(" ----------------------------------------------------------------------\n");
        printf("| _ _______ _______     ______ _     _  ______ _______ _______ _______ |\n");
        printf("|| (_______|_______)   / _____) |   | |/ _____|_______|_______|_______)|\n");
        printf("|| |_____   _   ___   ( (____ | |___| ( (____     _    _____   _  _  _ |\n");
        printf("|| |  ___) | | (_  |   \\____ \\|_____  |\\____ \\   | |  |  ___) | ||_|| ||\n");
        printf("|| | |     | |___) |   _____) )_____| |_____) )  | |  | |_____| |   | ||\n");
        printf("||_|_|      \\_____/   (______/(_______(______/   |_|  |_______)_|   |_||\n");
        printf("|                                                                      |\n");
        printf(" ----------------------------------------------------------------------\n");
        printf("| 1 - Register Student.                                                |\n");
        printf("| 2 - Update Student.                                                  |\n");
        printf("| 3 - View Student.                                                    |\n");
        printf("| 4 - Search Students.                                                 |\n");
        printf("| 5 - Exit Program.                                                    |\n");
        printf(" ----------------------------------------------------------------------\n");
        scanf("%d", &option);
        while ((buf_cleaner = getchar()) != '\n' && buf_cleaner != EOF);

        switch(option) {
            case 1:
                // Receive new student's data from user input

                //system("cls");
                printf(" ----------------------------------------------------------------------\n");
                printf("|Please inform the student id to register:                             |\n");
                printf(" ----------------------------------------------------------------------\n");
                scanf("%d", &student1.id);
                while ((buf_cleaner = getchar()) != '\n' && buf_cleaner != EOF);

                //system("cls");
                printf(" ----------------------------------------------------------------------\n");
                printf("|Please inform the student's name to register:                         |\n");
                printf(" ----------------------------------------------------------------------\n");
                fgets(student1.name, 100, stdin);

                //system("cls");
                printf(" ----------------------------------------------------------------------\n");
                printf("|Please inform the student's income to register:                       |\n");
                printf(" ----------------------------------------------------------------------\n");
                scanf("%f", &student1.income);
                while ((buf_cleaner = getchar()) != '\n' && buf_cleaner != EOF);

                student1.is_active = 1;

                // Register student on db
                registerStudent(conn, student1);
                break;
            case 2:

                // Receive existing student's data from user input to update

                //system("cls");
                printf(" ----------------------------------------------------------------------\n");
                printf("|Please inform the student id to update:                               |\n");
                printf(" ----------------------------------------------------------------------\n");
                scanf("%d", &student2.id);
                while ((buf_cleaner = getchar()) != '\n' && buf_cleaner != EOF);

                //system("cls");
                printf(" ----------------------------------------------------------------------\n");
                printf("|Please inform the student name to update:                             |\n");
                printf(" ----------------------------------------------------------------------\n");
                fgets(student2.name, 100, stdin);

                //system("cls");
                printf(" ----------------------------------------------------------------------\n");
                printf("|Please inform the student's income to update:                         |\n");
                printf(" ----------------------------------------------------------------------\n");
                scanf("%f", &student2.income);
                while ((buf_cleaner = getchar()) != '\n' && buf_cleaner != EOF);

                // Turn account Active/Inactive
                char active;
                do {
                    system("cls");
                    printf(" ----------------------------------------------------------------------\n");
                    printf("|Deactivate student account? (Y/N)                                     |\n");
                    printf("|       >> Please activate CAPS LOCK.                                  |\n");
                    printf(" ----------------------------------------------------------------------\n");
                    scanf("%c", &active);
                } while (active != 'Y' && active != 'N');
                if (active == 'Y') {
                    student2.is_active = 0;
                }
                else if (active == 'N'){
                    student2.is_active = 1;
                }

                // Update student on db
                updateStudent(conn, student2);

                break;
            case 3:
                int s_id;
                int valid_query = 0;
                do {
                    printf(" ----------------------------------------------------------------------\n");
                    printf("|Please inform the student's id to search:                             |\n");
                    printf(" ----------------------------------------------------------------------\n");
                    scanf("%d", &s_id);
                    while ((buf_cleaner = getchar()) != '\n' && buf_cleaner != EOF);
                    valid_query = searchStudent(conn, s_id);
                } while (valid_query == 0);

                break;
            case 4:
                break;
            default:
                break;
        }
    } while (option != 5);

    close_db(conn);

    printf("Press any key to continue...\n");
    getch();
    return 0;
}

// Functions definition
void registerStudent(PGconn *connection, Student student) {
    char query[200];
    PGresult *res;

    sprintf(query, "INSERT INTO students(s_id, s_name, income, is_active) VALUES(%d, '%s', %.2f, %d);", student.id, student.name, student.income, student.is_active);
    res = PQexec(connection, query);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Data insertion failed: %s", PQerrorMessage(connection));
        PQclear(res);
        PQfinish(connection);
        getch();
        exit(1);
    }
    else {
        printf("Data inserted successfully!\n");
        PQclear(res);
        getch();
    }
}

void updateStudent(PGconn *connection, Student student) {
    char query[200];
    PGresult *res;

    sprintf(query, "UPDATE students SET s_name = '%s', income = %.2f, is_active = %d WHERE s_id = %d;", student.name, student.income, student.is_active, student.id);
    res = PQexec(connection, query);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Data insertion failed: %s", PQerrorMessage(connection));
        PQclear(res);
        PQfinish(connection);
        getch();
        exit(1);
    }
    else {
        printf("Data inserted successfully!\n");
        PQclear(res);
        getch();
    }
}

int searchStudent(PGconn *connection, int s_id) {
    char query[200];
    PGresult *res;

    sprintf(query, "SELECT * FROM students WHERE s_id = '%d';", s_id);
    res = PQexec(connection, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Data retrieval failed: %s", PQerrorMessage(connection));
        PQclear(res);
        PQfinish(connection);
        exit(1);
    }

    int rows = PQntuples(res);
    if (rows == 0) {
        printf("No student found with the following id: %d\n", s_id);
        PQclear(res);
        getch();
        return 0;
    }
    else {
        for (int i = 0; i < rows; ++i) {
            int id = atoi(PQgetvalue(res, i, 0));
            const char * name = PQgetvalue(res, i, 1);
            float income = atof(PQgetvalue(res, i, 2));
            int is_active = atoi(PQgetvalue(res, i, 3));

            printf(" ----------------------------------------------------------------------\n");
            printf("|Student register                                                      |\n");
            printf(" ----------------------------------------------------------------------\n");
            printf("|ID: %d\n", id);
            printf("|Name: %s\n", name);
            printf("|Income: %.2f\n", income);
            if (is_active == 1) {
                printf("|Active Status: Active\n");
            }
            else {
                printf("|Active Status: Inactive\n");
            }
            printf(" ----------------------------------------------------------------------\n");
            break;
        }

        PQclear(res);
        getch();
        return 1;
    }
}