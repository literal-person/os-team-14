//getting data from copy_to_user
//we receive the button id
//parse id/possibly against hashmap where the k/v pair is id to command/mouse movement
//this button id is stored in proc/read_gamepad
//if the id is an error, print error message
// system("lspci");, is the function
#include "hashmap.h"
#define _POSIX_SOURCE
#include <fcntl.h>
#include <unistd.h>
#undef _POSIX_SOURCE
#include <stdlib.h>
#define PROC_PATH "/home/literal-person/Documents/mark project/os-team-14/userspace_app/tempproc.txt"
int parse_id(unsigned char button_id){
    hashmap *map = hashmap_new(sizeof(unsigned char), sizeof(char*), 0, NULL, NULL, NULL, NULL);
    
    hashmap_set(map, &(unsigned char){0}, &(char*){"lspci"});
    hashmap_set(map, &(unsigned char){1}, &(char*){"lsblk"});
    char** command = (char**) hashmap_get(map, &(unsigned char){button_id});
    
    if(!command){
        printf("Invalid Command!\n");
        return 1;
    }
    else{
        system(*command);
    }
    return 0;
    //read file, match button id to hashmap
    //read from temporary proc file first
}//id is an unsigned char

int main(void) {
    FILE *fp = fopen(PROC_PATH, "r");
    char buf[64]; 
    if (fp == NULL) {
        perror("fopen() error");
        return 1;
    }
    // fgets reads until \n or EOF
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        int id = atoi(buf); //convert string to int
        printf("Read ID: %d\n", id);
        parse_id((unsigned char)id);
    }

    fclose(fp);
    return 0;
}
//TODO: use real procfile
//TODO: use real button id