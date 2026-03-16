// getting data from copy_to_user
// we receive the button id
// parse id/possibly against hashmap where the k/v pair is id to command/mouse
// movement this button id is stored in proc/read_gamepad if the id is an error,
// print error message
//  system("lspci");, is the function

#include "defines.h"
#define _POSIX_SOURCE
#include <fcntl.h>
#include <unistd.h>
#undef _POSIX_SOURCE
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <stdio.h>
#define PROC_PATH "/proc/stats_gamepad"

typedef struct {
  int commands_run;
  int running;
  pthread_mutex_t lock;
} shared_data;

int parse_id(char *button_id, shared_data *shared) {
    pthread_mutex_lock(&shared->lock);

    int id = atoi(button_id);

    if (id < 48 || id > 60) {  
        printf("Invalid Command for ID: %d\n", id);
        pthread_mutex_unlock(&shared->lock);
        return 1;
    }

    const char *cmds[] = {
        "lspci | head -n 10",
        "lsblk | head -n 10",
        "lsmod | head -n 10",
        "cat /proc/stats_gamepad | head -n 10",
        "pwd | head -n 10",
        "echo 'Hello World!' | head -n 10",
        "echo 'top doesnt work' | head -n 10",
        "echo 'fortnite gaming' | head -n 10",
        "man man | head -n 10",
        "echo 'Ran out of command ideas' | head -n 10",
        "echo 'Hello Mark' | head -n 10"
    };

    const char *command = NULL;  //ids are 48,49,[50],51,52,[53],54,56,57,58,59,60
    printf("ID: %d\n",id);
    if(id < 50){
      command = cmds[id - 48];
    } else if (id <= 52){
      command = cmds[id - 49];
    } else if (id <= 60){
      command = cmds[id - 50];
    }
    shared->commands_run++;
    printf("[reader] Running command: %s\n", command);
    pthread_mutex_unlock(&shared->lock);  // unlock before slow system()

    printf("====================\n");
    system(command);
    printf("====================\n");

    return 0;
}


// Thread 1 reads button ids and runs commands
void *reader_thread(void *arg) {
    shared_data *shared = (shared_data *)arg;

    int fd = open(PROC_PATH, O_RDONLY);
    if (fd < 0) {
        perror("open() error");
        return NULL;
    }

    struct pollfd pfd = {
        .fd     = fd,
        .events = POLLIN,
    };

    printf("[reader] Waiting for button press...\n");

    while (shared->running) {
        // blocks here until kernel signals data is ready
        int ret = poll(&pfd, 1, -1);
        if (ret < 0) {
            perror("poll() error");
            break;
        }

        if (pfd.revents & POLLIN) {
            char buf[128];
            lseek(fd, 0, SEEK_SET); // rewind to start of proc file
            ssize_t n = read(fd, buf, sizeof(buf) - 1);
            if (n > 0) {
                buf[n] = '\0';
                buf[strcspn(buf, "\r\n")] = 0;
                // extract just the number after "Gamepad Status: "
                char *id_str = strrchr(buf, ' ');
                if (id_str) {
                    id_str++;
                    printf("[reader] Read ID: %s\n", id_str);
                    parse_id(id_str, shared);
                }
            }
        }
    }

    close(fd);
    shared->running = 0;
    return NULL;
}

// Thread 2 prints count of commands ran periodically
void *monitor_thread(void *arg) {
  shared_data *shared = (shared_data *)arg;
  int fd = open("/dev/gamepad", O_RDWR);
  if (fd < 0) {
      perror("open");
      return NULL;
  }

  int press_count = 0;
  while (1) {
    sleep(5);
    pthread_mutex_lock(&shared->lock);
    if (ioctl(fd, GAMEPAD_GET_PRESS_COUNT, &press_count) < 0) {
        perror("ioctl");
        close(fd);
    }
    int still_running = shared->running;
    printf("[monitor] Commands count: %d\n", shared->commands_run);
    printf("[monitor] Button Presses: %d\n", press_count);
    pthread_mutex_unlock(&shared->lock);

    if (!still_running) {
      break;
    }
  }
  close(fd);
  return NULL;
}

void print_welcome(void) {
  printf("=====================Team 14=====================\n");
  printf("Press any button to put a command in the console!\n");
  printf("=================================================\n");
}

int main(void) {
  print_welcome();
  shared_data shared;
  shared.commands_run = 0;
  shared.running = 1;

  pthread_mutex_init(&shared.lock, NULL);
  pthread_t reader;
  pthread_t monitor;

  pthread_create(&reader, NULL, reader_thread, &shared);
  pthread_create(&monitor, NULL, monitor_thread, &shared);

  pthread_join(reader, NULL);
  pthread_join(monitor, NULL);

  pthread_mutex_destroy(&shared.lock);

  return 0;
}

