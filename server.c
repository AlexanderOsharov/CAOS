#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#define SHM_NAME "/random_numbers_shm"
#define SHM_SIZE sizeof(shared_data)

typedef struct {
    int number;
    int client_active;
    int server_active;
} shared_data;

shared_data *shm_data;
int shm_fd;

void cleanup() {
    printf("Server: starting cleanup...\n");
    
    if (shm_data != NULL && shm_data != MAP_FAILED) {
        if (munmap(shm_data, SHM_SIZE) == -1) {
            perror("Server: munmap failed");
        } else {
            printf("Server: shared memory unmapped\n");
        }
    }
    
    if (shm_fd != -1) {
        if (close(shm_fd) == -1) {
            perror("Server: close shm_fd failed");
        }
    }
    
    if (shm_unlink(SHM_NAME) == -1) {
        if (errno != ENOENT) {
            perror("Server: shm_unlink failed");
        }
    } else {
        printf("Server: shared memory object unlinked\n");
    }
    
    printf("Server: cleanup completed\n");
}

void signal_handler(int sig) {
    printf("\nServer: received signal %d, shutting down...\n", sig);
    if (shm_data != NULL && shm_data != MAP_FAILED) {
        shm_data->server_active = 0;
    }
    cleanup();
    exit(0);
}

int main() {
    printf("Server: starting POSIX shared memory server...\n");
    printf("Server: PID = %d\n", getpid());
    
    // Set up signal handlers
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Server: sigaction SIGINT failed");
        return 1;
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Server: sigaction SIGTERM failed");
        return 1;
    }
    
    // Create shared memory object
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Server: shm_open failed");
        return 1;
    }
    printf("Server: shared memory created with fd = %d\n", shm_fd);
    
    // Set size of shared memory
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("Server: ftruncate failed");
        close(shm_fd);
        return 1;
    }
    printf("Server: shared memory size set to %ld bytes\n", SHM_SIZE);
    
    // Map shared memory
    shm_data = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_data == MAP_FAILED) {
        perror("Server: mmap failed");
        close(shm_fd);
        return 1;
    }
    printf("Server: shared memory mapped at address %p\n", shm_data);
    
    // Initialize shared data
    shm_data->number = 0;
    shm_data->client_active = 0;
    shm_data->server_active = 1;
    
    printf("Server: shared data initialized\n");
    printf("Server: waiting for client connection...\n");
    
    // Wait for client to become active with timeout
    int timeout = 0;
    int max_wait_time = 30; // 30 seconds timeout
    
    while (!shm_data->client_active && timeout < max_wait_time) {
        printf("Server: waiting for client... (%d/%d seconds)\n", timeout, max_wait_time);
        sleep(1);
        timeout++;
    }
    
    if (!shm_data->client_active) {
        printf("Server: Timeout waiting for client after %d seconds\n", max_wait_time);
        cleanup();
        return 1;
    }
    
    printf("Server: client connected! Starting to read numbers...\n");
    
    // Main loop - read numbers from shared memory
    int last_number = -1;
    int message_count = 0;
    
    while (shm_data->server_active) {
        if (shm_data->number != last_number) {
            printf("Server: received number = %d (message #%d)\n", shm_data->number, ++message_count);
            last_number = shm_data->number;
        }
        
        // Check if client is still active
        if (!shm_data->client_active) {
            printf("Server: client disconnected\n");
            break;
        }
        
        usleep(500000); // 500ms
    }
    
    printf("Server: shutting down normally\n");
    cleanup();
    return 0;
}