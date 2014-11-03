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
#include <windef.h>
#include <winsock2.h>
#include "winbase.h"
#include "wincrypt.h"

#define QUERY "SELECT password_value, username_value, origin_url FROM logins"
#define PATH1 "C:\\Users\\"
#define PATH2 "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Login Data"

void getPath(char* str, const char* user, const char* part1, const char* part2);
void printData(sqlite3_stmt* stmHandle);
void setPath(char* path);
void wQuit();
void cutPage(char* str, const char* value);

int main(int argc, char** argv) {

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    char googleChrome[255];
    setPath(googleChrome);

    sqlite3 *db;
    sqlite3_stmt *stmt;
    int res = 0;

    res = sqlite3_open(googleChrome, &db);
    if (res != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    res = sqlite3_prepare(db, QUERY, strlen(QUERY) + 1, &stmt, NULL);
    if (res != SQLITE_OK) {
        fprintf(stderr, "Couldn't execute query: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    int hasRow;
    do {
        hasRow = sqlite3_step(stmt);

        if (hasRow == SQLITE_ROW) {
            printData(stmt);
        }

    } while (hasRow == SQLITE_ROW);
    sqlite3_close(db);

    wQuit();

    return (EXIT_SUCCESS);
}

void wQuit() {
    printf("\nPress <enter> to Quit.\n");
    int ch;
    while ((ch = getchar()) != '\n');
    getchar();
}

void setPath(char* path) {
    char name[255];

    printf("Insert user name: ");
    scanf("%s", name);

    getPath(path, name, PATH1, PATH2);
}

void printData(sqlite3_stmt* stmHandle) {

    char page[255];
    char username[255];
    DATA_BLOB pass;
    DATA_BLOB blob;

    blob.cbData = sqlite3_column_bytes(stmHandle, 0);
    blob.pbData = (BYTE*) sqlite3_column_blob(stmHandle, 0);
    CryptUnprotectData(&blob, NULL, NULL, NULL, NULL, 0, &pass);

    char passStr[pass.cbData];
    memcpy(passStr, pass.pbData, pass.cbData);

    cutPage(page, (char *) sqlite3_column_text(stmHandle, 2));
    strcpy(username, (char *) sqlite3_column_text(stmHandle, 1));
    
    printf("%s;%s;%s \n", username, passStr, page);

}

void getPath(char* str, const char* user, const char* part1, const char* part2) {
    strcpy(str, part1);
    strcat(str, user);
    strcat(str, part2);
}

void cutPage(char* str, const char* value) {
    int slashs = 0, i = 0;
    memset(str, '\0', sizeof(str) * strlen(str));
    str[strlen(value)];
    for (i = 0; i < strlen(value); i++) {
        if(slashs > 2) break;
        if(value[i] == '/') slashs++;
        str[i] = value[i];
    }
}