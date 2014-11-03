/* 
 * File:   main.c
 * Author: Luis Calado <luisfmcalado at gmail.com>
 *
 * Created on 2 de Outubro de 2014, 14:25
 */

#include <stdio.h>
#include <stdlib.h>
#include "../deps/sqlite3.h"
#include <string.h>

#ifdef _WIN32
#define OS "Windows"
#define PATH1 "C:\\Users\\"
#define PATH2 "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\history"
#define PATH1XP " C:\\Documents and Settings\\"
#define PATH2XP "\\Local Settings\\Application Data\\Google\\Chrome\\User Data\\Default"
#define QUERY "SELECT lower_term, term FROM keyword_search_terms"
#elif __APPLE__
#elif __linux
#endif

void getPath(char* str, const char* user, const char* part1, const char* part2) {
    strcpy(str, part1);
    strcat(str, user);
    strcat(str, part2);
}

int main(int argc, char** argv) {

    setvbuf(stdout, NULL, _IONBF, 0);

    sqlite3 *db;
    sqlite3_stmt *stmHandle;
    char nome[255];
    char googleChrome[255];
    char lowerterm[255];
    char term[255];
    int len = strlen(QUERY) + 1;
    int hasRow;

    printf("Insert user name: ");
    scanf("%s", nome);

    getPath(googleChrome, nome, PATH1, PATH2);

    if (sqlite3_open(googleChrome, &db) != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }

    if (sqlite3_prepare(db, QUERY, len, &stmHandle, NULL) != SQLITE_OK) {
        fprintf(stderr, "Couldn't execute query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(0);
    }
    
    do {
        hasRow = sqlite3_step(stmHandle);

        if (hasRow == SQLITE_ROW) {
            strcpy(lowerterm, (char *) sqlite3_column_text(stmHandle, 0));
            strcpy(term, (char *) sqlite3_column_text(stmHandle, 1));
        }

        printf("Lower Termo: %s -::- Termo: %s \n", lowerterm, term);

    } while (hasRow == SQLITE_ROW);
    sqlite3_close(db);

    printf("Prima <enter> para sair.\n");
    int ch;
    while ((ch = getchar()) != '\n');
    getchar();

    return (EXIT_SUCCESS);
}

