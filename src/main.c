/* 
 * File:   main.c
 * Author: Luis Calado <sguergachi at gmail.com>
 *
 * Created on 2 de Outubro de 2014, 14:25
 */

#include <stdio.h>
#include <stdlib.h>
#include "../deps/sqlite3.h"
#include <string.h>

int main(int argc, char** argv) {
    
    setvbuf(stdout, NULL, _IONBF, 0);
    
    char nome[255];
    printf("Insira nome do utilizador: ");
    scanf("%s", nome);
    
    //CONEXAO
    sqlite3 *db;
    char googleChrome[255];
    char googleChromeXP[255];

    strcpy(googleChrome, "C:\\Users\\");
    strcat(googleChrome, nome);
    strcat(googleChrome, "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\History");

    strcpy(googleChromeXP, "C:\\users\\");
    strcat(googleChromeXP, nome);
    strcat(googleChromeXP, "\\Local Settings\\Application Data\\Google\\Chrome\\User Data\\Default\\History");

    if (sqlite3_open(googleChrome, &db) != SQLITE_OK) {
        if (sqlite3_open(googleChromeXP, &db) != SQLITE_OK) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            exit(0);
        }
    }
    printf("Opened database successfully\n");

    //PREPARACAO
    const char *query = "SELECT lower_term, term FROM keyword_search_terms";
    int len = strlen(query) + 1;
    sqlite3_stmt *stmHandle;

    if (sqlite3_prepare(db, query, len, &stmHandle, NULL) != SQLITE_OK) {
        fprintf(stderr, "Não foi possivel fazer a query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(0);
    }
    printf("Query realizada com sucesso.\n");

    char *lowerterm = (char *) malloc(255);
    char *term = (char *) malloc(255);
    int hasRow;

    do {

        hasRow = sqlite3_step(stmHandle);

        if (hasRow == SQLITE_ROW) {
            strcpy(lowerterm, (char *) sqlite3_column_text(stmHandle, 0));
            strcpy(term, (char *) sqlite3_column_text(stmHandle, 1));
        }

        printf("\tLower Termo: %s -::- Termo: %s \n", lowerterm, term);

    } while (hasRow == SQLITE_ROW);

    free(lowerterm);
    free(term);
    sqlite3_close(db);

    printf("Prima <enter> para sair.\n");
    
    int ch;
    while((ch = getchar()) != '\n');
    getchar();
    
    return (EXIT_SUCCESS);
}

