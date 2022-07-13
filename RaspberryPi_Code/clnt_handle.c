#include "Cafe.h"

void* clnt_handle(void* arg) {
    int conn_sock = *((int*)arg);
    int nbytes = 0;
	
	//printf("func start\n");
    
	while (1) {
        memset(ser_buff, 0x0, sizeof(ser_buff));
        nbytes = read(conn_sock, ser_buff, sizeof(ser_buff));
        if (nbytes <= 0) {
            //printf("not msg\n");
            conn_sock = -1;
            break;
        }
        else if (strncmp(ser_buff, "exit", 4) == 0)
            exit(0);

        //printf("parsing func start\n");
        data_parsing();

        if (flag == 1) {
            dbupdate(menu, size_select(drink_size), water_temp, syrup);
	    flag = 0;
        }
    }
    return 0;
}
