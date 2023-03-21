#include "AdptArray.h"
#include <stdlib.h>
#include <stdio.h>


/// ----- Define the struct AdptArray_ -----
typedef struct AdptArray_ {
    int size;
    DEL_FUNC del_func;
    COPY_FUNC copy_func;
    PRINT_FUNC print_func;
    PElement *arr;
}
AdptArray;


/// ----- Create new array of PElements in a specified size -----
PElement create_new_arr(int size) {

    // ----- Validate that the memory allocation was successful -----
    PElement arr = (PElement) calloc(size, sizeof(PElement)) ;
    if(arr == NULL){
        return NULL ;
    }
    return arr ;
}


/// ----- Delete array, release its elements memory and release the pointer memory -----
void delete_arr(PElement * arr, DEL_FUNC del_func, int size){
    PElement current_element = NULL ;
    for(int i = 0 ; i < size ; i++){
        current_element = arr[i] ;
        // ----- If an element is exists (not NULL), delete the element and release its memory -----
        if(current_element != NULL) {
            del_func(current_element) ;
        }
    }

    // ----- Release the array pointer -----
    free(arr) ;
}


/// ----- Replace a current array with a bigger one that holding all of the existing elements -----
void extend_arr(PAdptArray adpt_arr_p, int index){
    // ----- The new array size will be the index supplied + 1, so that if the client will request the 5th index,
    // ----- and the array size is < 6 the new array size will be 6 -----
    int new_size = index + 1 ;

    // ----- Create new empty array at the wanted size -----
    PElement * new_arr = create_new_arr(new_size) ;

    if(adpt_arr_p->arr != NULL) {
        PElement *current_arr = adpt_arr_p->arr;

        // ----- If the old array is not empty, copy each element from it into the new array -----
        for (int i = 0; i < adpt_arr_p->size; i++) {
            new_arr[i] = current_arr[i];
        }

        // ----- Free the old array memory -----
        free(current_arr);
    }

    // ----- Set the array pointer of the adpt_arr to the new one and update the size field to the new size -----
    adpt_arr_p->arr = new_arr ;
    adpt_arr_p->size = new_size ;
}

/// ----- Constructopr for a Adpt Array structure -----
PAdptArray CreateAdptArray(COPY_FUNC copy_func, DEL_FUNC del_func, PRINT_FUNC print_func) {

    // ----- Allocate memory for the AdptArray object and validate that the allocation succeed -----
    PAdptArray adpt_arr_p = (PAdptArray) malloc(sizeof(AdptArray));
    if (adpt_arr_p == NULL) {
        printf("Failed to allocate memory for the adpt array.") ;
        return NULL ;
    }

    // ----- Initialize the struct attributes -----
    adpt_arr_p->size = 0 ;
    adpt_arr_p->arr = create_new_arr(0) ;
    adpt_arr_p->del_func = del_func;
    adpt_arr_p->copy_func = copy_func;
    adpt_arr_p->print_func = print_func;

    return adpt_arr_p;
}


/// ----- Delete the Adpt array, release the memory of all the elements inside of it -----
void DeleteAdptArray(PAdptArray adpt_arr_p){
    delete_arr(adpt_arr_p->arr, adpt_arr_p->del_func, adpt_arr_p->size) ;
    free(adpt_arr_p) ;
}


/// ----- Set a given element into a given index at the array, if the array is too small, it will be extended to
/// ----- the wanted index -----
Result SetAdptArrayAt(PAdptArray adpt_arr_p, int index, PElement p_element){

    // ----- Check if the array needs to be extended and extend it if it is-----
    if(index >= adpt_arr_p->size){
        extend_arr(adpt_arr_p, index) ;
        if(adpt_arr_p->arr == NULL){
            return FAIL ;
        }
    }

    // ----- Check if an element is already exists in the specified index, if there is an element, delete it -----
    PElement existing_element = adpt_arr_p->arr[index] ;
    if(existing_element != NULL){
        adpt_arr_p->del_func(existing_element) ;
    }

    // ----- Make a copy of the given element to insert and assign it to its place inside the array -----
    PElement new_element = adpt_arr_p->copy_func(p_element) ;
    adpt_arr_p->arr[index] = new_element ;

    return SUCCESS ;
}


/// ----- Get an element from the array from a given index -----
PElement GetAdptArrayAt(PAdptArray adpt_arr_p, int index){

    // ----- If the given index is bigger then the array size, return NULL -----
    if(index >= adpt_arr_p->size){
            return NULL ;
    }

    // ----- If the wanted element is NULL, return NULL, otherwise copy it and return its copy -----
    PElement p_element = adpt_arr_p->arr[index] ;
    if(p_element == NULL){
        return NULL ;
    }
    return adpt_arr_p->copy_func(p_element) ;
}

/// ----- Get the current adpt_arr size -----
int GetAdptArraySize(PAdptArray adpt_arr_p){
    if(adpt_arr_p == NULL){
        return -1 ;
    }
    return adpt_arr_p->size ;
}


/// ----- Print the adpt_arr elements -----
void PrintDB(PAdptArray adpt_arr_p){
    PElement current_element = NULL ;
    for(int i = 0 ; i < adpt_arr_p->size ; i++){
        current_element = adpt_arr_p->arr[i] ;
        if(current_element != NULL){
            adpt_arr_p->print_func(current_element) ;
        }
    }
}

