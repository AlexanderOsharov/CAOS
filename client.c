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
    printf("Client: starting cleanup...\n");
    
    if (shm_data != NULL && shm_data != MAP_FAILED) {
        shm_data->client_active = 0; // Notify server
        if (munmap(shm_data, SHM_SIZE) == -1) {
            perror("Client: munmap failed");
        } else {
            printf("Client: shared memory unmapped\n");
        }
    }
    
    if (shm_fd != -1) {
        if (close(shm_fd) == -1) {
            perror("Client: close shm_fd failed");
        }
    }
    
    printf("Client: cleanup completed\n");
}

void signal_handler(int sig) {
    printf("\nClient: received signal %d, shutting down...\n", sig);
    cleanup();
    exit(0);
}

int main() {
    printf("Client: starting POSIX shared memory client...\n");
    printf("Client: PID = %d\n", getpid());
    
    // Set up signal handlers
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Client: sigaction SIGINT failed");
        return 1;
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("Client: sigaction SIGTERM failed");
        return 1;
    }
    
    // Open existing shared memory object
    printf("Client: connecting to shared memory...\n");
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Client: shm_open failed");
        printf("Make sure server is running first!\n");
        return 1;
    }
    printf("Client: shared memory opened with fd = %d\n", shm_fd);
    
    // Map shared memory
    shm_data = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_data == MAP_FAILED) {
        perror("Client: mmap failed");
        close(shm_fd);
        return 1;
    }
    printf("Client: shared memory mapped at address %p\n", shm_data);
    
    // Verify server is active
    if (!shm_data->server_active) {
        printf("Client: server is not active\n");
        cleanup();
        return 1;
    }
    
    // Mark client as active
    shm_data->client_active = 1;
    printf("Client: successfully connected to server!\n");
    printf("Client: starting to generate random numbers (0-100)...\n");
    
    // Initialize random number generator
    srand(time(NULL));
    
    // Main loop - generate random numbers and write to shared memory
    int number_count = 0;
    int max_numbers = 20; // Generate 20 numbers then stop for testing
    
    while (shm_data->client_active && shm_data->server_active && number_count < max_numbers) {
        // Generate random number in range 0-100
        int random_num = rand() % 101;
        shm_data->number = random_num;
        number_count++;
        
        printf("Client: generated number = %d (#%d/%d)\n", random_num, number_count, max_numbers);
        
        // Check if server is still active
        if (!shm_data->server_active) {
            printf("Client: server disconnected\n");
            break;
        }
        
        sleep(1); // 1 second delay
    }
    
    printf("Client: finished generating numbers\n");
    cleanup();
    printf("Client: terminated normally\n");
    return 0;
}