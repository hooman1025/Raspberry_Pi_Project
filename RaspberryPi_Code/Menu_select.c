#include "Cafe.h"

char* menu_select(char* menu_f) {

	for (int i = 0; i < 3; i++) {
      	    if (strcmp(menu_f, material_BEAN[i]) == 0)
            	return material[0];
    	}
    	for (int i = 0; i < 2; i++) {
    	    if (strcmp(menu_f, material_TEA[i]) == 0)
            	return material[1];
    	}
    	for (int i = 0; i < 1; i++) {
    	    if (strcmp(menu_f, material_CHOCO[i]) == 0)
            	return material[2];
    	}
    return NULL;
}

int size_select(char* size_f) {
	for (int i = 0; i < 3; i++) {
		if (strcmp(size_f, size_sell[i]) == 0)
			return size_sell_int[i];
	}
	return -1;
}

int syrup_select(char* size_f) {
	for (int i = 0; i < 3; i++) {
		if (strcmp(size_f, syrup_sell[i]) == 0)
			return syrup_sell_int[i];
	}
	return -1;
}

int temp_select(char* temp_f) {
	if (strcmp(temp_sell[0], temp_f) == 0)
		return 1;
	else if (strcmp(temp_sell[1], temp_f) == 0)
		return 0;
	else
		return -1;
}

int milk_select(char* menu_f) {
	if (strcmp(material_BEAN[2], menu_f) == 0 || strcmp(material_CHOCO[0], menu_f) == 0)
		return 1;
	else
		return -1;
}
