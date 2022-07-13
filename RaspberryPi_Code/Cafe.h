#ifndef __CAFE_H__
#define __CAFE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <pthread.h>
#include <mysql/mysql.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define Tall 5
#define Grande 10
#define Venti 15

#define noshot 0
#define oneshot 5
#define twoshot 10

#define BUF_SIZE 100
//#define MAX_CLNT 256
#define PORT     5070

extern char* host;
extern char* user;
extern char* pass;
extern char* dbname;

//int conn_socks[MAX_CLNT];

extern socklen_t addrlen;
extern int nport;

extern char ser_buff[BUF_SIZE + 1];

extern MYSQL* conn;
extern int sql_index;
extern char in_sql[200];
extern int res;

extern int flag;
extern char menu[20], water_temp[5], drink_size[10], syrup[10];
extern char* pToken;

extern char* material_BEAN[];
extern char* material_TEA[];
extern char* material_CHOCO[];
extern char* material[];
extern char* temp_sell[];
extern char* size_sell[];
extern int size_sell_int[];
extern char* syrup_sell[];
extern int syrup_sell_int[];

void data_parsing();
void dbupdate(char* menu_f, int size_f, char* temp_f, char* syrup);
char* menu_select(char* menu_f);
int size_select(char* size_f);
int syrup_select(char* syrup_f);
int temp_select(char* temp_f);
int milk_select(char* menu_f);
void* clnt_handle(void* arg);

#endif //!__CAFE_H__
