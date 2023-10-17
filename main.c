
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbooh.h>

#define CONVEYOR_BLUE 15
#define CONVEYOR_RED 10
#define BUFFER_SIZE 100

pthread_mutex_t lock;
pthread_cond_t blue_not_full, blue_not_empty;
pthread_cond_t red_not_full, red_not_empty;

int blue_buffer[CONVEYOR_BLUE]
int red_buffer[CONVEYOR_RED]
int b_front = 0, b_back = 0, b_count = 0;
int r_front = 0, r_back = 0, r_count = 0;
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;


int buffer[BUFFER_SIZE];
int sequence = 0;

FILE *railway_car;
FILE *blue_delivery;
FILE *red_delivery;

void Blueput (int part_number) {
    pthread_mutex_lock(&lock);
    
    while (b_count >= CONVEYOR_BLUE) {
        pthread_cond_wait(&blue_not_full, &lock);
    }
    
    sequence++;
    blue_buffer[b_back] = part_number;
    b_back = (b_back + 1) % CONVEYOR_BLUE;
    b_count++;

    pthread_cond_signal(&blue_not_empty);
    pthread_mutex_unlock(&lock);
}

void Redput (int part_number) {
    pthread_mutex_lock(&lock);
    
    while (r_count >= CONVEYOR_RED) {
        pthread_cond_wait(&red_not_full, &lock);
    }
    
    sequence++;
    red_buffer[r_back] = part_number;
    r_back = (r_back + 1) % CONVEYOR_BLUE;
    r_count++;

    pthread_cond_signal(&red_not_empty);
    pthread_mutex_unlock(&lock);
}

int Blueget() {
    int part_number;

    pthread_mutex_lock(&lock);

    while (b_count <= 0) {
        pthread_cond_wait(&blue_not_empty, &lock);
    }

    part_number = blue_buffer[b_front];
    b_front = (b_front + 1) % CONVEYOR_BLUE;
    b_count--;

    pthread_cond_signal(&blue_not_full);
    pthread_mutex_unlock(&lock);

    return part_number;
}

int Redget() {
    int part_number;

    pthread_mutex_lock(&lock);

    while (r_count <= 0) {
        pthread_cond_wait(&blue_not_empty, &lock);
    }

    part_number = blue_buffer[r_front];
    r_front = (r_front + 1) % CONVEYOR_RED;
    r_count--;

    pthread_cond_signal(&blue_not_full);
    pthread_mutex_unlock(&lock);

    return part_number;
}


void *threadL(void *arg) {


}

void threadR() {

}

void threadX() {

}

void threadY() {

}

int main(int argc, char *argv[]) {


return 0;

}

