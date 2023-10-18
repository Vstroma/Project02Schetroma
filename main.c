
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
//FILE *blue_delivery;
//FILE *red_delivery;

void Blueput (int part_number) {
    pthread_mutex_lock(&lock);          // set lock
    
    while (b_count >= CONVEYOR_BLUE) {              // if conveyor is full wait
        pthread_cond_wait(&blue_not_full, &lock);
    }
    
    sequence++;             
    blue_buffer[b_back] = part_number;          // store part number in buffer
    b_back = (b_back + 1) % CONVEYOR_BLUE;      // increment back pointer
    b_count++;                

    pthread_cond_signal(&blue_not_empty);       // signal not empty
    pthread_mutex_unlock(&lock);              // release lock
}

void Redput (int part_number) {
    pthread_mutex_lock(&lock);          // set lock
    
    while (r_count >= CONVEYOR_RED) {               // if conveyor is full wait
        pthread_cond_wait(&red_not_full, &lock);
    }
    
    sequence++;                        
    red_buffer[r_back] = part_number;       // store part number in buffer
    r_back = (r_back + 1) % CONVEYOR_BLUE;          // increment back pointer
    r_count++;  

    pthread_cond_signal(&red_not_empty);        // signal not empty
    pthread_mutex_unlock(&lock);        // release lock
}

int Blueget() {
    int part_number;                // initialize part number

    pthread_mutex_lock(&lock);              // set lock

    while (b_count <= 0) {                  // if conveyor is empty wait
        pthread_cond_wait(&blue_not_empty, &lock);
    }

    part_number = blue_buffer[b_front];            // retrieve part number from buffer
    b_front = (b_front + 1) % CONVEYOR_BLUE;        // increment front pointer
    b_count--;

    pthread_cond_signal(&blue_not_full);        // signal not full
    pthread_mutex_unlock(&lock);        // release lock

    return part_number;
}

int Redget() {
    int part_number;            // initialize part number

    pthread_mutex_lock(&lock);      // set lock

    while (r_count <= 0) {                  // if conveyor is empty wait
        pthread_cond_wait(&blue_not_empty, &lock);
    }

    part_number = blue_buffer[r_front];         // retrieve part number from buffer
    r_front = (r_front + 1) % CONVEYOR_RED;         // increment front pointer
    r_count--;

    pthread_cond_signal(&blue_not_full);            // signal not full
    pthread_mutex_unlock(&lock);                // release lock

    return part_number;
}


void *threadL(void *arg) {
    int part_number;

        while (fscanf(railway_car, "%d", &part_number) == 1) {
            if (part_number == -1) {        // if it is EOF
                Redput(-1);
                Blueput(-1);
                break;
            }
        
            if (part_number >= 1 && part_number <= 12)  // move to conveyor Blue

                Blueput(part_number);
            } else {                    // moves to conveyor Red
                Redput(part_number);
            }
        usleep(250000);         // delay of.25 secomnds
    }

    return NULL;
}

void *threadR(void *arg) {
    int part_number;

    while (fscanf(railway_car, "%d", &part_number) == 1) {
        if (part_number == -1) {        // if it is EOF
            Blueput(-1);
            Redput(-1);     
            break;
        }
        
        if (part_number >= 13 && part_number <= 25) {       // moves to conveyor Red
          
            Redput(part_number);
        } else {                // moves to conveyor Blue

            Blueput(part_number);
        }
        usleep(500000);     // delay of .50 seconds
    }

    return NULL;
}

void threadX() {

}

void threadY() {

}

int main(int argc, char *argv[]) {

    pthread_t threadL, threadR, threadX, threadY;

    pthread_create(&threadL, NULL, &threadL, NULL);
    pthread_create(&threadR, NULL, &threadR, NULL);
    pthread_create(&threadX, NULL, &threadX, NULL);
    pthread_create(&threadY, NULL, &threadY, NULL);

return 0;

}

