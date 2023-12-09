#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include "constants.c"


PGconn* connect_db() {
    PGconn *conn;

    char conn_str[100];
    snprintf(conn_str, sizeof(conn_str), "dbname=%s user=%s password=%s host=%s port=%s", DBNAME, USER, PASSWORD, HOST, PORT);
    //const char *conn_str = sprintf("dbname=%s user=%s password=%s host=%s port=%s", DBNAME, USER, PASSWORD, HOST, PORT);

    conn = PQconnectdb(conn_str);

    if(PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", (char *)PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    printf("Connected to the database\n");

    return conn;
}

void close_db(PGconn* conn) {
    PQfinish(conn);
    printf("Connection closed\n");
}
