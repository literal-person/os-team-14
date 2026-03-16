// getting data from copy_to_user
// we receive the button id
// parse id/possibly against hashmap where the k/v pair is id to command/mouse
// movement this button id is stored in proc/read_gamepad if the id is an error,
// print error message
//  system("lspci");, is the function
#include "hashmap.h"
#define _POSIX_SOURCE
#include <fcntl.h>
#include <unistd.h>
#undef _POSIX_SOURCE
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <poll.h>
#define PROC_PATH "/proc/stats_gamepad"

typedef struct {
  hashmap *map;
  int commands_run;
  int running;
  pthread_mutex_t lock;
} shared_data;

int parse_id(char *button_id, shared_data *shared) {
  pthread_mutex_lock(&shared->lock); // lock cause were accessing shared data
  const char *key = button_id;
  char **cmd_ptr = (char **)hashmap_get(shared->map, &key);
  if (!cmd_ptr) {
      printf("Invalid Command for ID: %s\n", button_id);
      pthread_mutex_unlock(&shared->lock);
      return 1;
  }
  char *command = *cmd_ptr;
  

  if (!command) {
    printf("Invalid Command for ID: %s\n", button_id);
    pthread_mutex_unlock(&shared->lock);
    return 1;
  }

  else {
    shared->commands_run++;
    printf("[reader] Running command: %s\n", command);
    pthread_mutex_unlock(
        &shared->lock); // unlock before system() as it takes time
    printf("==================================================================================================\n");
    system(command);
    printf("==================================================================================================\n");
  }
  return 0;
  // read file, match button id to hashmap
  // read from temporary proc file first
} // id is an unsigned char

hashmap *init_map() {
  hashmap *map = hashmap_new(sizeof(char *), sizeof(char *), 0, hash_string,
                             compare_string, NULL, NULL);
  const char *keys[] = {
      "48", "49", "51", "52", "309", "310",
  };
  const char *cmds[] = {
        "lspci | head -n 10",
        "lsblk | head -n 10",
        "lsmod | head -n 10",
        "cat /proc/stats_gamepad | head -n 10",
        "echo 'cd .. has no effect in a subprocess'",
        "echo 'button 310: unassigned'",
  };

  for (int i = 0; i < 6; i++) {
        char *cmd = strdup(cmds[i]);   // heap-allocated, stable pointer
        hashmap_set(map, &keys[i], &cmd);
    }
  return map;
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

  while (1) {
    sleep(5);
    pthread_mutex_lock(&shared->lock);
    int still_running = shared->running;
    printf("[monitor] Commands count: %d\n", shared->commands_run);
    pthread_mutex_unlock(&shared->lock);

    if (!still_running) {
      break;
    }
  }
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
// TODO: use real procfile
// TODO: use real button id
