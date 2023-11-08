
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void Blueput(int part_number);
void Redput (int part_number);
int Blueget();
int Redget();
void *threadL(void *arg);
void *threadR(void *arg);
void *threadX(void *arg);
void *threadY(void *arg);
void writePart(char filename[], int part, int sequence);

#define CONVEYOR_BLUE 15
#define CONVEYOR_RED 10
#define PARTS 25

int blue_belt[CONVEYOR_BLUE];
int red_belt[CONVEYOR_RED];

int r_count = 0;
int b_count = 0;
//int fill_ptr = 0;
//int use_ptr = 0;

// pthread_mutex_t lock;
pthread_cond_t blue_not_full, blue_not_empty;
pthread_cond_t red_not_full, red_not_empty;

pthread_mutex_t blueLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t blueNotEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t blueNotFull = PTHREAD_COND_INITIALIZER;
pthread_mutex_t redLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t redNotEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t redNotFull = PTHREAD_COND_INITIALIZER;

int blueBelt[CONVEYOR_BLUE];
int redBelt[CONVEYOR_RED];

int sequence = 0;

FILE *blue_delivery;
FILE *red_delivery;





int main(int argc, char *argv[]) {

    pthread_t threadL, threadR, threadX, threadY;

    pthread_create(&threadL, NULL, threadL, NULL);     // create blue and red threads
    pthread_create(&threadR, NULL, threadR, NULL);
    pthread_create(&threadX, NULL, threadX, NULL);
    pthread_create(&threadY, NULL, threadY, NULL);

    blue_delivery = fopen("BLUE_DELIVERY.txt", "w");
    red_delivery = fopen("RED_DELIVERY.txt", "w");

    pthread_join(threadL, NULL);            // wait for threads to finish
    pthread_join(threadR, NULL);
    pthread_join(threadX, NULL);
    pthread_join(threadY, NULL);

    fclose(blue_delivery);
    fclose(red_delivery);

return 0;

}












void Blueput(int part_number) {
    pthread_mutex_lock(&blueLock);          // set lock
    
    while (b_count == CONVEYOR_BLUE) {              // if conveyor is full wait
        pthread_cond_wait(&blue_not_full, &blueLock);
    }
    
    blueBelt[b_count] = part_number;
    b_count++;   
    sequence++;             
    printf("Part added to Blue belt.\n");             

    pthread_cond_signal(&blue_not_empty);       // signal not empty
    pthread_mutex_unlock(&blueLock);              // release lock
}

void Redput (int part_number) {
    pthread_mutex_lock(&redLock);          // set lock
    
    while (r_count == CONVEYOR_RED) {               // if conveyor is full wait
        pthread_cond_wait(&red_not_full, &redLock);
    }
    
    redBelt[r_count] = part_number;
    r_count++;  
    sequence++;  
    printf("Part added to Red belt.\n");                      


    pthread_cond_signal(&red_not_empty);        // signal not empty
    pthread_mutex_unlock(&redLock);        // release lock
}

int Blueget() {
    int part_number;                // initialize part number

    pthread_mutex_lock(&blueLock);              // set lock

    while (b_count == 0) {                  // if conveyor is empty wait
        pthread_cond_wait(&blue_not_empty, &blueLock);
    }

    part_number = blueBelt[b_count - 1];            // retrieve part number from buffer
    b_count--;
    printf("Retrieved part from Blue belt.\n");

    pthread_cond_signal(&blue_not_full);        // signal not full
    pthread_mutex_unlock(&blueLock);        // release lock

    return part_number;
}

int Redget() {
    int part_number;            // initialize part number

    pthread_mutex_lock(&redLock);      // set lock

    while (r_count == 0) {                  // if conveyor is empty wait
        pthread_cond_wait(&red_not_empty, &redLock);
    }

    part_number = redBelt[r_count - 1];         // retrieve part number from buffer
    r_count--;
    printf("Retrieved part from Red belt.\n");


    pthread_cond_signal(&red_not_full);            // signal not full
    pthread_mutex_unlock(&redLock);                // release lock

    return part_number;
}

// adding parts to the blue belt
void *threadL(void *arg) {

        // adding parts to the blue belt
    for ( int i = 0; i < PARTS; i++) {
        Blueput(i + 1);
        usleep(250000); // sleep .25
    }

    Blueput(-1);
    return NULL;
}


//adding parts to the red belt
void *threadR(void *arg) {
  
    for (int i = 0; i < PARTS; i++) {
        Redput(i + 1);
        usleep(500000); // sleep .5
    }

    Redput(-1);
    return NULL;
}

void *threadX(void *arg) {
    int part_number;
    while (1) {
        part_number = Blueget();
        if (part_number == -1) {        // exit when -1
            break;
        }
        writePart("Blue_delivery.txt",part_number, sequence);   // write part to blue file 
        usleep(200000); // process time
    }
    return NULL;

}

void *threadY(void *arg) {
    int part_number;
    while(1) {
        part_number = Redget();
        if (part_number == -1) {           // exit when -1
            break;
        }
        writePart("Red_delivery.txt",part_number,sequence);
        usleep(200000); // process time
    }

    return NULL;
}

// write part to the delivery truck file
void writePart(char filename[], int part, int sequence) {
    FILE *deliveryTruckFile = fopen(filename, "a");
    if (deliveryTruckFile) {
        fprintf(deliveryTruckFile, "Sequence: %d, Part: %d\n", sequence, part);
        fclose(deliveryTruckFile);
    } else {
        printf("Error: Couldn't open delivery truck file %s\n", filename);
    }
}



}