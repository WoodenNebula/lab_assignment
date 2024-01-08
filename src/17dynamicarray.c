/* Make an array with dynamic memory allocation of integers and compute the sum using your own function */

/*  FOR PRINTING COLORED FONT IN TERMINAL
/033 = escape sequence
[<font_properties>;<more_props_separated_by_;>;...m
[ = starting of property, m denotes ending
0  to restore default color
1 = brighter colors  
4 = underlined text  
5 = flashing text    
------------------------
-------FOREGROUND-------
------------------------
30 = black 
31 = red    
32 = green 
33 = yellow (or brown)           
34 = blue 
35 = purple
36 = cyan   
37 = white (or gray)           
------------------------
-------BACKGROUND-------
────────────────────────
40 = black
41 = red  
42 = green
43 = yellow (or brown)
44 = blue 
45 = purple
46 = cyan  
47 = white (or gray)  
*/

#include <stdio.h>
#include <stdlib.h>

#define ERROR(msg) {printf("\033[1;31m[ERROR] line %d: %s\033[0m\n", __LINE__, msg); exit(EXIT_FAILURE);}

#ifndef _DEBUG
#define SUCCESS(msg) 0
#define NOTIFY(msg) 0

#else
#define SUCCESS(msg) printf("\033[1;32m[SUCCESS] : %s\033[0m\n", msg)
#define NOTIFY(msg) printf("\033[1;30m[NOTIF] :%s \033[0m\n", msg)
#endif

#define INITIAL_MAX_COUNT 10

typedef struct {
    unsigned int _count;
    unsigned int _max_count;
    int* _data;
} Array;


Array arr_construct(const int* arr_src, int arr_size);
void arr_delete(Array* arr);
void arr_empty(Array* arr);
void arr_print(Array* arr);
void arr_push_back(Array* arr, int data);
void arr_pop(Array* arr);
int arr_at(Array* arr, unsigned int index);
void arr_write_at(Array* arr, unsigned int index, int value);
void arr_fill(Array* arr, int fill_num);
Array arr_sum(Array* a, Array* b);


int main(int argc, char *argv[]) {
    int test[] = {1, 2};

    Array arr = arr_construct(test, sizeof(test)/sizeof(int));
    Array arrB = arr_construct(NULL, 0);

    for(int i = 0; i < 10; i++){
        arr_push_back(&arr, i + 3);
    }

    for(int i = 0; i < 15; i++){
        arr_push_back(&arrB, i);
    }

    printf("Array1 = ");
    arr_print(&arr);
    printf("\nArray2 = ");
    arr_print(&arrB);

    Array sum = arr_sum(&arr, &arrB);
    printf("\n\nSum = ");
    arr_print(&sum);

    arr_write_at(&sum, 14, arr_at(&sum, 14) + 4);
    arr_print(&sum);



    arr_delete(&arr);
    arr_delete(&arrB);
    arr_delete(&sum);
}



Array arr_construct(const int* src_arr, int src_arr_size) {
    Array arr;
    arr._max_count = src_arr_size + INITIAL_MAX_COUNT;
    arr._data = (int*)malloc(sizeof(int) * arr._max_count);
    arr._count = NULL;

    if(src_arr != NULL) {
        for(int i = 0; i < src_arr_size; i++) {
            arr._data[i] = src_arr[i];
        }
        arr._count = src_arr_size;
    }


    if(!arr._data){
        ERROR("ARRAY CONSTRUCTION FAILED");
    }

    SUCCESS("Constructed Array!");

    return arr;
}


void arr_delete(Array* arr) {
    free(arr->_data);
    arr->_max_count = NULL;
    arr->_count = NULL;
    SUCCESS("Freed array memory!");
}


void arr_empty(Array* arr) {
    for (int i = 0; i < arr->_count; i++) {
        arr->_data[i] = NULL;
    }
    arr->_count = 0;
    NOTIFY("Emptied array data!");
}


void arr_fill(Array* arr, int fill_num) {
    if(arr->_count == NULL || arr->_count == 0) {
        arr->_count = arr->_max_count;
    }

    for (int i = 0; i < arr->_count; i++) {
        arr->_data[i] = fill_num;
    }
    NOTIFY("Filled array data");
}


void arr_push_back(Array* arr, int data) {
    // Ensure the array isn't uninitialized
    if(arr->_data == NULL || arr->_max_count == NULL) {
        ERROR("WRITING TO UNINITIALIZED ARRAY!");
    } else {
        // if there is no space in the heap memory
        if(arr->_count == arr->_max_count) {
            arr->_max_count *= 1.5;
            arr->_data = (int *)realloc(arr->_data, (sizeof(int) * arr->_max_count));
        }

        arr->_data[arr->_count] = data;
        arr->_count++;
    }
    NOTIFY("Pushed back data");
}


void arr_pop(Array* arr) {
    if(arr->_data == NULL || arr->_max_count == NULL) {
        ERROR("WRITING TO UNINITIALIZED ARRAY!");
    } else  if (arr->_count <= 0) {
        ERROR("CAN'T POP ON EMPTY ARRAY");
    } else {
        arr->_data[arr->_count - 1] = NULL;
        arr->_count--;        
    }
    NOTIFY("Popped data");
}


int arr_at(Array* arr, unsigned int index) {
    if(index >= 0 && index < arr->_count) {
        return arr->_data[index];
    } else {
        ERROR("INDEX DOESN'T EXIST!");
    }
}


void arr_write_at(Array* arr, unsigned int index, int value) {
    if(index >= 0 && index < arr->_count) {
        arr->_data[index] = value;
    } else {
        ERROR("INDEX DOESN'T EXIST!");
    }

}


Array arr_sum(Array* a, Array* b) {
    Array result = arr_construct(NULL, 0);
    if (a->_count >= b->_count) {
        int i = 0;
        for(; i < b->_count; i++) {
            arr_push_back(&result, arr_at(a, i) + arr_at(b, i));
        }
        i = b->_count;
        for(; i < a->_count; i++) {
            arr_push_back(&result, arr_at(a, i));
        }
    } else if (b->_count > a->_count)
    {
        int i = 0;
        for(; i < a->_count; i++) {
            arr_push_back(&result, arr_at(a, i) + arr_at(b, i));
        }
        i = a->_count;
        for(; i < b->_count; i++) {
            arr_push_back(&result, arr_at(b, i));
        }
    }
    return result;
}


void arr_print(Array* arr) {
    if(arr->_count != NULL || arr->_count != 0) {
        putchar('[');
        // Not printing the last element within loop
        for (int i = 0; i < arr->_count - 1; i++) {
            printf("%d, ", arr->_data[i]);
        }
        printf("%d]\n", arr->_data[arr->_count - 1]);   

        printf("Size = %u\tMax_Size = %u\n",arr->_count, arr->_max_count);

    } else {
        printf("\033[1;30mEMPTY ARRAY!\033[0m\n");
    }
}