#include "Cafe.h"

void data_parsing() {
    int str_len, menu_len, water_temp_len, drink_size_len, syrup_len;
    char* ptr_menu = NULL, * ptr_water_temp = NULL, * ptr_drink_size = NULL, * ptr_syrup = NULL;
    char* pArray[4] = { 0 };

    //printf("parsing func in\n");
    str_len = strlen(ser_buff);
    ser_buff[str_len] = 0;
    //printf("ser_buff = %s\n", ser_buff);
    pToken = strtok(ser_buff, "#");
    int i = 0;
    while (pToken != NULL) {
        pArray[i] = pToken;
        if (++i > 5)
            break;
        pToken = strtok(NULL, "#");
    }
    //printf("parsing pArray\n");
    ptr_menu = pArray[0];
    ptr_water_temp = pArray[1];
    ptr_drink_size = pArray[2];
    ptr_syrup = pArray[3];
    
    //printf("parsing strlen\n");
    menu_len = strlen(ptr_menu);
    water_temp_len = strlen(ptr_water_temp);
    drink_size_len = strlen(ptr_drink_size);
    syrup_len = strlen(ptr_syrup);
 
    //printf("parsing for\n");
    
    if(menu_len > 0) {
    for (int i = 0; i <= menu_len; i++)
        menu[i] = *(ptr_menu + i);
    }
    if(water_temp_len > 0) {
    for (int i = 0; i <= water_temp_len; i++)
        water_temp[i] = *(ptr_water_temp + i);
    }
    if(drink_size_len > 0) {
    for (int i = 0; i <= drink_size_len; i++)
        drink_size[i] = *(ptr_drink_size + i);
    }
    if(syrup_len > 0) {
    for (int i = 0; i <= syrup_len; i++)
        syrup[i] = *(ptr_syrup + i);
    }
	
	printf("menu = %s, water_temp = %s, drink_size = %s, syrup = %s\n", menu, water_temp, drink_size, syrup);
    
	/*
    for (int i = 0; i <= str_len; i++)
		ser_buff[i] = 0;
    */

    flag = 1;
}
