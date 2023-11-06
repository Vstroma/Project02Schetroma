
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define CONVEYOR_BLUE 15
#define CONVEYOR_RED 10
#define PARTS 25



int blue_belt[CONVEYOR_BLUE]
int red_belt[CONVEYOR_RED]
int b_count = 0;
int r_count = 0;
//int fill_ptr = 0;
//int use_ptr = 0;
int count = 0;

//pthread_mutex_t lock;
pthread_cond_t blue_not_full, blue_not_empty;
pthread_cond_t red_not_full, red_not_empty;


pthread_mutex_t blueLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t blueNotEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t blueNotFull = PTHREAD_COND_INITIALIZER;
pthread_mutex_t redLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t redNotEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t redNotFull = PTHREAD_COND_INITIALIZER;

int blueBelt{CONVEYOR_BLUE};
int redBelt{CONVEYOR_RED};

int sequence = 0;

FILE *blue_delivery;
FILE *red_delivery;

void Blueput(int part_number);
void Redput (int part_number);
int Blueget();
int Redget();
void *threadL(void *arg);
void *threadR(void *arg);
void threadX(void *arg);
void threadY(void *arg);
void writePart(char filename[], int part, int sequence);


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
                usleep(250000);         // delay of.25 secomnds
            } else {                    // moves to conveyor Red
                Redput(part_number);
                usleep(250000);         // delay of.25 secomnds
            }
        //usleep(250000);         // delay of.25 secomnds
    

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
            usleep(250000);         // delay of.25 secomnds
        } else {                // moves to conveyor Blue

            Blueput(part_number);
            usleep(250000);         // delay of.25 secomnds
        }
        //usleep(500000);     // delay of .50 seconds
    }

    return NULL;
}

void threadX(void *arg) {
    int part_number;
    while (1) {
        part_number = Blueget();
        if (part_number == -1) {        // exit when -1
            break;
        }
        usleep(200000); // process time
    }
    return NULL;

}

void threadY() {
    int part_number;
    while(1) {
        part_number = Redget();
        it (part == -1) {           // exit when -1
            break;
        }
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


int main(int argc, char *argv[]) {

    pthread_t threadL, threadR, threadX, threadY;

    pthread_create(&threadL, NULL, &threadL, NULL);     // create blue and red threads
    pthread_create(&threadR, NULL, &threadR, NULL);
    pthread_create(&threadX, NULL, &threadX, NULL);
    pthread_create(&threadY, NULL, &threadY, NULL);

    blueDelivery = fopen("BLUE_DELIVERY.txt", "w");
    redDelivery = fopen("RED_DELIVERY.txt", "w");

    pthread_join(threadL, NULL);            // wait for threads to finish
    pthread_join(threadR, NULL);
    pthread_join(threadX, NULL);
    pthread_join(threadY, NULL);

    fclose(blueDelivery);
    fclose(redDelivery);

return 0;

}

