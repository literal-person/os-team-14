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
#include <errno.h>
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
    const char *keys[] = {"305", "306","307", "308","309", "310",};
    
    hashmap_set(map, &keys[0], "lspci");
    hashmap_set(map, &keys[1], "lsblk");
    hashmap_set(map, &keys[2], "lsmod");
    hashmap_set(map, &keys[3], "cat /proc/stat_gamepad");
    hashmap_set(map, &keys[4], "cd ..");
    return map;
}
int main(void) {
    hashmap *map = init_map();
    FILE *fp = fopen(PROC_PATH, "r");
    char buf[128]; 
    if (fp == NULL) {
        perror("fopen() error");
        return 1;
    }
    // fgets reads until \n or EOF
    while(1){
        while (fgets(buf, sizeof(buf), fp) != NULL) {
            buf[strcspn(buf, "\r\n")] = 0; //adds null terminator
            char *last_line = strrchr(buf, ' ');
            if (last_line) {
                last_line++; //go to actual id, past the space
                printf("Read ID: %s\n", last_line);
                printf("Command: %s\n", (char*)hashmap_get(map, &last_line));
                parse_id(last_line, map);
            }
        }
        if (feof(fp)) {//if end of file
            clearerr(fp);//clears end of file flag 
            sleep(1);//sleep until something else gets added
        } 
        else{
            perror("fgets error");
            break;
        }
    }
    hashmap_free(map);
    fclose(fp);
    return 0;
}
//TODO: use real procfile
//TODO: use real button id