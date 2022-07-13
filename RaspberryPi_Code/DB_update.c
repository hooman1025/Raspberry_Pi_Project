#include "Cafe.h"

void dbupdate(char* menu_f, int size_f, char* temp_f, char* syrup_f) {

    char* material_f = { menu_select(menu_f) };
    int syrup_value = syrup_select(syrup_f);
    int res = 0;

    sprintf(in_sql, "update menu set %s = %s-%d", material_f, material_f, size_f);
    printf("%s, %d\n", in_sql, mysql_query(conn, in_sql));
    

    if (temp_select(temp_f) == 1) {
        sprintf(in_sql, "update menu set %s = %s-%d", material[5], material[5], size_f);
        printf("%s, %d\n", in_sql, mysql_query(conn, in_sql));
    }
    

    if (milk_select(menu_f) == 1) {
        sprintf(in_sql, "update menu set %s = %s-%d", material[3], material[3], size_f);
        printf("%s, %d\n", in_sql, mysql_query(conn, in_sql));
    }
    

    if (syrup_value > 0) {
        sprintf(in_sql, "update menu set %s = %s-%d", material[4], material[4], syrup_value);
        printf("%s, %d\n", in_sql, mysql_query(conn, in_sql));
    }
    
    /*
    if (!res) {
        printf("inserted %lu rows\n", (unsigned long)mysql_affected_rows(conn));
    }
    else {
        fprintf(stderr, "error : %s[%d]\n", mysql_error(conn), mysql_errno(conn));
        exit(1);
    }
    */
	memset(menu, 0x0, sizeof(menu));
	memset(water_temp, 0x0, sizeof(water_temp));
	memset(drink_size, 0x0, sizeof(drink_size));
	memset(syrup, 0x0, sizeof(syrup));
}
