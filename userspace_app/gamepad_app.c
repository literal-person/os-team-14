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
#define PROC_PATH "/home/literal-person/Documents/mark-project/os-team-14/userspace_app/tempproc.txt"
int parse_id(char *button_id, hashmap *map){
    const char *key = button_id;
    char *command = (char*)hashmap_get(map, &key);
    
    if(!command){
        printf("Invalid Command for ID: %s\n", button_id);
        return 1;
    }
    else{
        system(command);
    }
    return 0;
    //read file, match button id to hashmap
    //read from temporary proc file first
}//id is an unsigned char
hashmap *init_map(){
    hashmap *map = hashmap_new(
        sizeof(char*),
        sizeof(char*),
        0,
        hash_string,
        compare_string,
        NULL,
        NULL);
    const char *key1 = "305";
    const char *key2 = "306";

    hashmap_set(map, &key1, "lspci");
    hashmap_set(map, &key2, "lsblk");
    return map;
}
int main(void) {
    hashmap *map = init_map();
    FILE *fp = fopen(PROC_PATH, "r");
    char buf[64]; 
    if (fp == NULL) {
        perror("fopen() error");
        return 1;
    }
    // fgets reads until \n or EOF
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        buf[strcspn(buf, "\r\n")] = 0; //adds null terminator
        char *last_line = strrchr(buf, ' ');
        if (last_line) {
            last_line++; // Move past the space to the actual ID
            printf("Read ID: '%s'\n", last_line);
            parse_id(last_line, map);
        }
    }
    hashmap_free(map);
    fclose(fp);
    return 0;
}
//TODO: use real procfile
//TODO: use real button id