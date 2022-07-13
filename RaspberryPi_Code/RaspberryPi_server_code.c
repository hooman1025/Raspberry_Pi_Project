#include "Cafe.h"

char* host = "localhost";
char* user = "root";
char* pass = "kcci";
char* dbname = "Cafe";

//int conn_socks[MAX_CLNT];

socklen_t addrlen;
int nport;

char ser_buff[BUF_SIZE + 1] = { 0 };

MYSQL* conn;
int sql_index;
char in_sql[200] = { 0 };
int res = 0;

int flag = 0;
char menu[20] = { '\0' }, water_temp[5] = { '\0' }, drink_size[10] = { '\0' }, syrup[10] = { '\0' };
char* pToken;

char* material_BEAN[] = { "Americano" , "Espresso" , "Caffelatte" };
char* material_TEA[] = { "Icetea" , "Herbtea" };
char* material_CHOCO[] = { "Chocolatte" };
char* material[] = { "BEAN" , "TEA" , "CHOCO", "MILK" , "SYRUP", "ICE" };
char* temp_sell[] = { "Ice", "Hot" };
char* size_sell[] = { "Tall", "Venti", "Grande" };
int size_sell_int[] = { Tall, Grande, Venti };
char* syrup_sell[] = { "Noshot", "Oneshot", "Twoshot" };
int syrup_sell_int[] = { noshot, oneshot, twoshot };

int main(int argc, char* argv[]) {
    
    int serv_sock, conn_sock;
    struct sockaddr_in serv_addr, conn_addr;
    conn = mysql_init(NULL);
    flag = 0;
    pthread_t t_id;
    
    if (!(mysql_real_connect(conn, host, user, pass, dbname, 0, NULL, 0))) {
        fprintf(stderr, "error : %s[%d]\n", mysql_error(conn), mysql_errno(conn));
        exit(1);
    }
	//printf("mysql connected\n");
        
    fflush(stdout);

    if (argc == 1) {
        nport = PORT;
    }
    else if (argc == 2) {
        nport = atoi(argv[1]);
    }
    else {
        printf("Usage: %s <port>\n", argv[0]);
        exit(0);
    }
	//printf("port connected\n");

    serv_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serv_sock == -1) {
        perror("socket() error!\n");
        exit(0);
    }
	//printf("socket connected\n");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(nport);
    //printf("memset connected\n");

	int opt = 1;
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind() error\n");
        exit(0);
    }
	//printf("bind connected\n");

    if (listen(serv_sock, 5) == -1) {
        perror("listen() error\n");
        exit(0);
    }
	//printf("listen connected\n"); 
    
    while (1) {
		addrlen = sizeof(conn_sock);
		conn_sock = accept(serv_sock, (struct sockaddr*)&conn_addr, &addrlen);
		if (conn_sock == -1) {
			perror("accept() error\n");
			exit(0);
		}
		//printf("accept connected\n");
		
		//printf("pthread create\n");
        pthread_create(&t_id, NULL, clnt_handle, (void*)&conn_sock);
		//printf("pthread create complete\n");
        pthread_detach(t_id);
    }

    close(conn_sock);
    close(serv_sock);
    
    return 0;
}
