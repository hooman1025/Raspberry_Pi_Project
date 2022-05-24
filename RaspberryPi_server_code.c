#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <wiringPi.h>
#include <wiringSerial.h>
#include <mysql/mysql.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAXLINE    100
#define PORT    23

static char *host = "localhost";
static char *user = "root";
static char *pass = "kcci";
static char *dbname = "Cafe";

int serv_sock;
int conn_sock;

struct sockaddr_in    serv_addr;
struct sockaddr_in    conn_addr;

int addrlen, datalen;

static char ser_buff[MAXLINE +1] = {0};
int nbytes;
int nport;

pid_t pid;

static MYSQL *conn;
static int sql_index, flag;
static char in_sql[200] = {0};

static int str_len, menu_len, water_temp_len, drink_size_len, syrup_len;
static char menu[20] = { '\0' }, water_temp[5] = { '\0' }, drink_size[10] = { '\0' }, syrup[10] = { '\0' };
static char *ptr_menu = NULL, *ptr_water_temp = NULL, *ptr_drink_size = NULL, *ptr_syrup = NULL;
static char *pArray[4] = {0};
static char *pToken;
static int res = 0;

void dbupdate(char row[], int value);

int main(int argc, char* argv[]){
    
    //printf("%d, %u\n", argc, *argv);
    
    conn = mysql_init(NULL);
	int index = 0;
	flag = 0;

	if(!(mysql_real_connect(conn, host, user, pass, dbname, 0, NULL, 0))) {
		fprintf(stderr, "error : %s[%d]\n", mysql_error(conn), mysql_errno(conn));
		exit(1);
	}
    
    printf("Raspberry Startup\n");
	fflush(stdout);
	
	if(wiringPiSetup() == -1) {
		return -1;
	}
	
    if (argc == 1){
        nport = PORT;    
    } else if (argc == 2) {
        nport = atoi(argv[1]);
    } else {
        printf("Usage: %s <port>\n", argv[0]);
        exit(0);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1){
        perror("socket() error!\n");
        exit(0);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;            
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);     
    serv_addr.sin_port = htons(nport);        

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind() error\n");
        exit(0);
    }

    if(listen(serv_sock, 1) == -1) {
        perror("listen() error\n");
        exit(0);
    }

    addrlen = sizeof(conn_sock);    
    conn_sock = accept(serv_sock, (struct sockaddr *)&conn_addr, &addrlen); 
    if(conn_sock == -1){
        perror("accept() error\n");
        exit(0);
    }

    if((pid = fork()) == -1){
        close(conn_sock);
        perror("fork() error\n");
    } else if(pid ==0) {    
        while(1){
            fgets(ser_buff, sizeof(ser_buff), stdin);    
            nbytes = strlen(ser_buff);
            write(conn_sock, ser_buff, MAXLINE);    
            if(strncmp(ser_buff, "exit", 4) ==0) {        
                puts("exit program");    
                exit(0);       
            }
        }
    } else if (pid > 0){    
        while(1){
            if((nbytes = read(conn_sock, ser_buff, MAXLINE)) < 0){    
                perror("read() error\n");
                exit(0);
            }
            printf("%s\n", ser_buff); 
			flag = 1;
			str_len = strlen(ser_buff);
			printf("ser_buff = %s\n", ser_buff);
			pToken = strtok(ser_buff, "#");
			int i = 0;
			while(pToken != NULL) {
				pArray[i] = pToken;
				if(++i > 5)
					break;
				pToken = strtok(NULL, "#");
			}
			
			ptr_menu = pArray[0];
			ptr_water_temp = pArray[1];
			ptr_drink_size = pArray[2];
			ptr_syrup = pArray[3];
			
			menu_len = strlen(ptr_menu);
			water_temp_len = strlen(ptr_water_temp);
			drink_size_len = strlen(ptr_drink_size);
			syrup_len = strlen(ptr_syrup);
			
			for(int i = 0; i <= menu_len; i++)
				menu[i] = *(ptr_menu + i);
			
			for(int i = 0; i <= water_temp_len; i++)
				water_temp[i] = *(ptr_water_temp + i);
				
			for(int i = 0; i <= drink_size_len; i++)
				drink_size[i] = *(ptr_drink_size + i);
				
			for(int i = 0; i <= syrup_len; i++)
				syrup[i] = *(ptr_syrup + i);
			
			printf("menu = %s, water_temp = %s, drink_size = %s, syrup = %s\n", menu, water_temp, drink_size, syrup);
			for(int i = 0; i <= str_len; i++)
				ser_buff[i] = 0;
			index = 0;
            
            if(flag == 1) {
				if (strcmp("Americano", menu) == 0) {
					if (strcmp("Ice", water_temp) == 0) {
						if (strcmp("Tall", drink_size) == 0) {
							if (strcmp("Noshot", syrup) == 0) {
								dbupdate("BEAN", 5);
								dbupdate("ICE", 5);
							}
							else if (strcmp("Oneshot", syrup) == 0) {
								dbupdate("BEAN", 5);
								dbupdate("ICE", 5);
								dbupdate("SYRUP", 5);
							}
							else if (strcmp("Twoshot", syrup) == 0) {
								dbupdate("BEAN", 5);
								dbupdate("ICE", 5);
								dbupdate("SYRUP", 10);
							}
						}
						else if (strcmp("Grande", drink_size) == 0) {
							if (strcmp("Noshot", syrup) == 0) {
								dbupdate("BEAN", 10);
								dbupdate("ICE", 10);
							}
							else if (strcmp("Oneshot", syrup) == 0) {
								dbupdate("BEAN", 10);
								dbupdate("ICE", 10);
								dbupdate("SYRUP", 5);
							}
							else if (strcmp("Twoshot", syrup) == 0) {
								dbupdate("BEAN", 10);
								dbupdate("ICE", 10);
								dbupdate("SYRUP", 10);
							}
						}
						else if (strcmp("Venti", drink_size) == 0) {
							if (strcmp("Noshot", syrup) == 0) {
								dbupdate("BEAN", 15);
								dbupdate("ICE", 15);
							}
							else if (strcmp("Oneshot", syrup) == 0) {
								dbupdate("BEAN", 15);
								dbupdate("ICE", 15);
								dbupdate("SYRUP", 5);
							}
							else if (strcmp("Twoshot", syrup) == 0) {
								dbupdate("BEAN", 15);
								dbupdate("ICE", 15);
								dbupdate("SYRUP", 10);
							}
						}
					}
					else if (strcmp("hot", water_temp) == 0) {
						if (strcmp("Tall", drink_size) == 0) {
							if (strcmp("Noshot", syrup) == 0) {
								dbupdate("BEAN", 5);
							}
							else if (strcmp("Oneshot", syrup) == 0) {
								dbupdate("BEAN", 5);
								dbupdate("SYRUP", 5);
							}
							else if (strcmp("Twoshot", syrup) == 0) {
								dbupdate("BEAN", 5);
								dbupdate("SYRUP", 10);
							}
						}
						else if (strcmp("Grande", drink_size) == 0) {
							if (strcmp("Noshot", syrup) == 0) {
								dbupdate("BEAN", 10);
							}
							else if (strcmp("Oneshot", syrup) == 0) {
								dbupdate("BEAN", 10);
								dbupdate("SYRUP", 5);
							}
							else if (strcmp("Twoshot", syrup) == 0) {
								dbupdate("BEAN", 10);
								dbupdate("SYRUP", 10);
							}
						}
						else if (strcmp("Venti", drink_size) == 0) {
							if (strcmp("Noshot", syrup) == 0) {
								dbupdate("BEAN", 15);
							}
							else if (strcmp("Oneshot", syrup) == 0) {
								dbupdate("BEAN", 15);
								dbupdate("SYRUP", 5);
							}
							else if (strcmp("Twoshot", syrup) == 0) {
								dbupdate("BEAN", 15);
								dbupdate("SYRUP", 10);
							}
						}
					}
				}
			
				else if (strcmp("Espresso", menu) == 0) {

					if (strcmp("Tall", drink_size) == 0) {
						dbupdate("BEAN", 5);
					}
					else if (strcmp("Grande", drink_size) == 0) {
						dbupdate("BEAN", 10);
					}
					else if (strcmp("Venti", drink_size) == 0) {
						dbupdate("BEAN", 15);
					}
				}
				else if (strcmp("Caffelatte", menu) == 0) {
					if (strcmp("Ice", water_temp) == 0) {
						if (strcmp("Tall", drink_size) == 0) {
							dbupdate("BEAN", 5);
							dbupdate("MILK", 5);
							dbupdate("ICE", 5);
						}
						else if (strcmp("Grande", drink_size) == 0) {
							dbupdate("BEAN", 10);
							dbupdate("MILK", 10);
							dbupdate("ICE", 10);
						}
						else if (strcmp("Venti", drink_size) == 0) {
							dbupdate("BEAN", 15);
							dbupdate("MILK", 15);
							dbupdate("ICE", 15);
						}
					}
					else if (strcmp("hot", water_temp) == 0) {
						if (strcmp("Tall", drink_size) == 0) {
							dbupdate("BEAN", 5);
							dbupdate("MILK", 5);
						}
						else if (strcmp("Grande", drink_size) == 0) {
							dbupdate("BEAN", 10);
							dbupdate("MILK", 10);
						}
						else if (strcmp("Venti", drink_size) == 0) {
							dbupdate("BEAN", 15);
							dbupdate("MILK", 15);
						}
					}
				}
				else if (strcmp("Icetea", menu) == 0) {
	
					if (strcmp("Tall", drink_size) == 0) {
						dbupdate("TEA", 5);
						dbupdate("ICE", 5);
					}
					else if (strcmp("Grande", drink_size) == 0) {
						dbupdate("TEA", 10);
						dbupdate("ICE", 10);
					}
					else if (strcmp("Venti", drink_size) == 0) {
						dbupdate("TEA", 15);
						dbupdate("ICE", 15);
					}
				}
				else if (strcmp("Herbtea", menu) == 0) {

					if (strcmp("Tall", drink_size) == 0) {
						dbupdate("TEA", 5);
					}
					else if (strcmp("Grande", drink_size) == 0) {
						dbupdate("TEA", 10);
					}
					else if (strcmp("Venti", drink_size) == 0) {
						dbupdate("TEA", 15);
					}
				}
				else if (strcmp("Chocolatte", menu) == 0) {
					if (strcmp("Ice", water_temp) == 0) {
						if (strcmp("Tall", drink_size) == 0) {
							dbupdate("MILK", 5);
							dbupdate("CHOCO", 5);
							dbupdate("ICE", 5);
						}
						else if (strcmp("Grande", drink_size) == 0) {
							dbupdate("MILK", 10);
							dbupdate("CHOCO", 10);
							dbupdate("ICE", 10);
						}
						else if (strcmp("Venti", drink_size) == 0) {
							dbupdate("MILK", 15);
							dbupdate("CHOCO", 15);
							dbupdate("ICE", 15);
						}
					}
					else if (strcmp("hot", water_temp) == 0) {
						if (strcmp("Tall", drink_size) == 0) {
							dbupdate("MILK", 5);
							dbupdate("CHOCO", 5);
						}
						else if (strcmp("Grande", drink_size) == 0) {
							dbupdate("MILK", 10);
							dbupdate("CHOCO", 10);
						}
						else if (strcmp("Venti", drink_size) == 0) {
							dbupdate("MILK", 15);
							dbupdate("CHOCO", 15);
						}
					}
				}
			flag = 0;
		}
        if(strncmp(ser_buff, "exit",4) == 0)
            exit(0);
        }
    }

    close(conn_sock);
    close(serv_sock);
    return 0;
} 

void dbupdate(char row[], int value) {
	sprintf(in_sql, "update menu set %s = %s-%d", row, row, value);
	res = mysql_query(conn, in_sql);
	//printf("res = %d\n", res);
	if (!res) {
		//printf("inserted %lu rows\n", (unsigned long)mysql_affected_rows(conn));
	}
	else {
		fprintf(stderr, "error : %s[%d]\n", mysql_error(conn), mysql_errno(conn));
		exit(1);
	}
}
