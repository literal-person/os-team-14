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
#include <pthread.h>

#define PROC_PATH "/home/literal-person/Documents/mark-project/os-team-14/userspace_app/tempproc.txt"

typedef struct {
	hashmap *map;
	int commands_run;
	int running;
	pthread_mutex_t lock;
}shared_data

int parse_id(char *button_id, shared_data *shared){
    pthread_mutex_lock(&shared->lock); // lock cause were accessing shared data
 
    const char *key = button_id;
    char *command = (char*)hashmap_get(map, &key);

    if(!command){
        printf("Invalid Command for ID: %s\n", button_id);
        return 1;
    }

    else{
	shared->commands_run++;
	printf("[reader] Running command: %s\n", command);
	pthread_mutex_unlock(&shared->lock);//unlock before system() as it takes time
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
    hashmap_set(map, &keys[5], "echo 'button 310: unassigned'");
    return map;
}

//Thread 1 reads button ids and runs commands
void *reader_thread(void *arg){
	shared_data *shared = (shared_data*)arg;
	File *fp = fopen(PROC_PATH, "r");
	char buf[128];

	if(fp == NULL){
		perror("fopen() error");
		return NULL;
	}

	printf("[reader] Waiting for button press...\n");

	while (1){
       	  while (fgets(buf, sizeof(buf), fp) != NULL) {
  		 buf[strcspn(buf, "\r\n")] = 0; //adds null terminator
           	 char *last_line = strrchr(buf, ' ');

            	 if (last_line) {
               		last_line++; //go to actual id, past the space
               	 	printf("[reader] Read ID: %s\n", last_line);
           		parse_id(last_line, map);
            	}
       	 }

        	if (feof(fp)) {
            	  clearerr(fp);//clears end of file flag so fgets will try again nxt time 
               	  sleep(1);//sleep until something else gets added
        	}
		else{
            	  perror("fgets error");
        	  break;
	        }
       }
    fclose(fp);
    shared->running = 0;
    return NULL;
}

//Thread 2 prints count of commands ran periodically
void* monitor_thread(void *arg){
      shared_data *shared = (shared_data*)arg;

	   while(1){
		sleep(5);
		pthread_mutex_lock(&shared->lock);
		int still_running = shared->running; 
		printf("[monitor] Commands count: %d\n",shared->commands_run);
		pthread_mutex_unlock(&shared->lock);

		if(!still_running){
		   break;
		}
	   }
	return NULL:
}


int main(void) {
     shared_data shared;
     shared.map = init_map();
     shared.commands_run = 0;
     shared.running = 1;

     pthread_mutex_init(&shared.lock, NULL);
     pthread_t reader;
     pthread_t monitor;

     pthread_create(&reader, NULL, reader_thread, &shared);
     pthread_create(&monitor, NULL, monitor_thread, &shared);

     pthread_join(reader, NULL);
     pthread_join(monitor, NULL);

     hashmap_free(shared.map);
     pthread_mutex_destroy(&shared.lock); 

    return 0;
}
//TODO: use real procfile
//TODO: use real button id
