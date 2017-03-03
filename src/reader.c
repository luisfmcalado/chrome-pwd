/*
 * File:   reader.c
 * Author: Luis Calado <luisfmcalado at gmail.com>
 *
 * Created on 1 de March de 2017, 14:30
 */

#include "reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__

#include "crypto.h"

typedef struct _BLOB {
	int cbData;
	unsigned char *pbData;
} DATA_BLOB;

#endif

void maketemp() {
	FILE *ftmp, *fsrc;

	char* srcpath;
	path(&srcpath);

#ifdef DEBUG
	printf("make tmp of: %s to %s\n", srcpath, TMP);
#endif

	fsrc = fopen(srcpath, "rb");
	if (fsrc == NULL) {
		printf("error opening logins file.\n");
		exit(1);
	}

	ftmp = fopen(TMP, "wb");
	if (ftmp == NULL) {
		printf("error opening temp file.\n");
		exit(1);
	}

	// Data to be read
	int l1, l2;
	unsigned char buf[1024 * 4];
	while ((l1 = fread(buf, 1, sizeof buf, fsrc)) > 0) {
		l2 = fwrite(buf, 1, l1, ftmp);
		if (l2 < l1) {
			if (ferror(ftmp)){
				printf("error writing to %s.\n", TMP);
				fclose(ftmp);
				fclose(fsrc);
				exit(1);
			}
		}
	}
	
	if (srcpath != 0) free(srcpath);
	fclose(ftmp);
	fclose(fsrc);
}

char* chrome_passwd() {
	FILE* fp;

	fp = popen("security find-generic-password -wa 'Chrome'", "r");
	if (fp == NULL) {
		printf("get chrome password failed\n");
		return NULL;
	}

	char* pwd = malloc(sizeof(char) * 256);
	memset(pwd, '\0', sizeof(char) * 256);
	if (fgets(pwd, 256, fp) == NULL) {
		printf("read chrome password is empty");
		return NULL;
	}
	pclose(fp);

	int i;
	for (i = 0; i < 255; i++)
		if (pwd[i] == '\n') pwd[i] = '\0';

#ifdef DEBUG
	printf("chrome pwd: %s\n", pwd);
#endif
	return pwd;
}

void path(char** path) {
	char name[255];
	printf("Insert user name: ");
	scanf("%[^\t\n]", name);

	*path = malloc(sizeof(char) * strlen(PATH) + strlen(name));
	sprintf(*path, PATH, name);
}

void rpad(unsigned char* t, int len) {
	char c;
	int i = 0, pad = 0;
	while ((c = t[i]) != '\0') i++;
	for (pad = t[i - 1]; pad > 0; pad--) t[i - pad] = '\0';
}

void printdata(sqlite3_stmt* stmHandle, char* pwd) {
	DATA_BLOB b_pass;
	DATA_BLOB b_blob;

	b_blob.cbData = sqlite3_column_bytes(stmHandle, 0);
	b_blob.pbData = (unsigned char*)sqlite3_column_blob(stmHandle, 0);

	unsigned char pass[255];
	memset(pass, '\0', sizeof(char) * 255);
	
#ifdef _WIN32
		
		CryptUnprotectData(&b_blob, NULL, NULL, NULL, NULL, 0, &b_pass);
		if(b_pass.cbData > 0) {
			memcpy(pass, &b_pass.pbData[0], b_pass.cbData);
		}
#elif __APPLE__

	unsigned char key[16];
	const char salt[] = "saltysalt";
	int res = pbkdf2_hmac_sha1(pwd, salt, 1003, 16, key);

	if (res && b_blob.cbData > 3 && strncmp(b_blob.pbData, "v10", 3) == 0) {
		char blob_s[b_blob.cbData - 3];
		memcpy(blob_s, &b_blob.pbData[3], b_blob.cbData - 2);

		unsigned char iv[16];
		memset(iv, ' ', sizeof(char) * 16);

		aes_cbc_block_decrypt((const unsigned char*)key, iv,
				      (const unsigned char*)blob_s, 16,
				      strlen(blob_s), pass);
		rpad(pass, 255);
	}
#endif
	printf("%s;%s;%s \n", (char*)sqlite3_column_text(stmHandle, 1), pass,
	       (char*)sqlite3_column_text(stmHandle, 2));
}

void opentable(sqlite3** db, sqlite3_stmt** stmt) {
	int res;

	maketemp();

	res = sqlite3_open(TMP, &(*db));
	if (res != SQLITE_OK) {
		fprintf(stderr, "Can't open database: %s\n",
			sqlite3_errmsg(*db));
		exit(1);
	}

	res = sqlite3_prepare(*db, QUERY, strlen(QUERY) + 1, &(*stmt), NULL);
	if (res != SQLITE_OK) {
		fprintf(stderr, "Couldn't execute query: %s\n",
			sqlite3_errmsg(*db));
		exit(1);
	}
}

int readdata() {
	sqlite3* db;
	sqlite3_stmt* stmt;

	opentable(&db, &stmt);

	char* pwd = NULL;
#ifdef __APPLE__
	pwd = chrome_passwd();
#endif

	int hasRow;
	do {
		hasRow = sqlite3_step(stmt);
		if (hasRow == SQLITE_ROW) {
			printdata(stmt, pwd);
		}
	} while (hasRow == SQLITE_ROW);

	if (pwd != 0) free(pwd);
	sqlite3_close(db);

	return 1;
}
