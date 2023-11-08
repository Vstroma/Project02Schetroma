
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#define CONVEYOR_BLUE 15
#define CONVEYOR_RED 10
#define PARTS 25

void Blueput(int part_number);
void Redput (int part_number);
int Blueget();
int Redget();
void *threadLF(void *arg);
void *threadRF(void *arg);
void *threadXF(void *arg);
void *threadYF(void *arg);
void writePart(char filename[], int part_number, int sequence);

int r_count = 0;
int b_count = 0;
int sequence = 0;


int blue_belt[CONVEYOR_BLUE];
int red_belt[CONVEYOR_RED];


//int fill_ptr = 0;
//int use_ptr = 0;

// pthread_mutex_t lock;
//pthread_cond_t blue_not_full, blue_not_empty;
//pthread_cond_t red_not_full, red_not_empty;

pthread_mutex_t blueLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t blueNotEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t blueNotFull = PTHREAD_COND_INITIALIZER;
pthread_mutex_t redLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t redNotEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t redNotFull = PTHREAD_COND_INITIALIZER;



FILE *blue_delivery;
FILE *red_delivery;

int main() {

    //initialize threads
    pthread_t threadL, threadR, threadX, threadY;

    // creates threadL, threadR, threadX and threadY
    pthread_create(&threadL, NULL, threadLF, NULL);     
    pthread_create(&threadR, NULL, threadRF, NULL);
    pthread_create(&threadX, NULL, threadXF, NULL);
    pthread_create(&threadY, NULL, threadYF, NULL);

    //open files for writing
    blue_delivery = fopen("BLUE_DELIVERY.txt", "w");
    red_delivery = fopen("RED_DELIVERY.txt", "w");

    // waits for threads to finish
    pthread_join(threadL, NULL);            
    pthread_join(threadR, NULL);
    pthread_join(threadX, NULL);
    pthread_join(threadY, NULL);

    //closes the files
    fclose(blue_delivery);
    fclose(red_delivery);

return 0;

}


void Blueput(int part_number) {
     // set lock
    pthread_mutex_lock(&blueLock);         
    
    // if the blue conveyor is full wait
    while (b_count == CONVEYOR_BLUE) {             
        pthread_cond_wait(&blueNotFull, &blueLock);
    }
    
    //adds a part to the blue conveyor and increments the sequence
    blue_belt[b_count] = part_number;
    b_count++;   
    sequence++;             
    printf("Part added to Blue belt.\n");             

    //signal that the blue conveyor is not empty and release the lock
    pthread_cond_signal(&blueNotEmpty);       
    pthread_mutex_unlock(&blueLock);              
}

void Redput (int part_number) {
    // set lock
    pthread_mutex_lock(&redLock);
    
    //if the red conveyor is full wait
    while (r_count == CONVEYOR_RED) {
        pthread_cond_wait(&redNotFull, &redLock);
    }
    
    //adds a part to the red conveyor and increments the sequence
    red_belt[r_count] = part_number;
    r_count++;  
    sequence++;  
    printf("Part added to Red belt.\n");                      

    //signal that the red conveyor is not empty and release the lock
    pthread_cond_signal(&redNotEmpty);     
    pthread_mutex_unlock(&redLock);     
}

int Blueget() {
     // initialize part number
    int part_number;         

    //sets lock
    pthread_mutex_lock(&blueLock); 

    // if blue count is empty, wait
    while (b_count == 0) {             
        pthread_cond_wait(&blueNotEmpty, &blueLock);
    }

    // get a part from the blue conveyor and update the count
    part_number = blue_belt[b_count - 1];       
    b_count--;
    printf("Retrieved part from Blue belt.\n");

    // signal the blue conveyor is not full and release the lock
    pthread_cond_signal(&blueNotFull);      
    pthread_mutex_unlock(&blueLock);     

    return part_number;
}

int Redget() {
    // initialize part number
    int part_number;  

    // set lock
    pthread_mutex_lock(&redLock);

    // if red cound is empty, wait
    while (r_count == 0) {  
        pthread_cond_wait(&redNotEmpty, &redLock);
    }

    // get a part from the red conveyor and update the count
    part_number = red_belt[r_count - 1];   
    r_count--;
    printf("Retrieved part from Red belt.\n");

    // signal the red conveyor is not full and release the lock
    pthread_cond_signal(&redNotFull);   
    pthread_mutex_unlock(&redLock);  

    return part_number;
}

// adding parts to the blue belt
void *threadLF(void *arg) {

    // add blue parts to the blue belt
    for ( int i = 0; i < PARTS; i++) {
        Blueput(i + 1);
        usleep(250000); // sleep .25
    }

    // -1 to indicate the end of blue parts
    Blueput(-1);
    return NULL;
}


//adding parts to the red belt
void *threadRF(void *arg) {
  
  // add red parts to the red belt
    for (int i = 0; i < PARTS; i++) {
        Redput(i + 1);
        usleep(500000); // sleep .5
    }

    //-1 to indicate the end of red parts
    Redput(-1);
    return NULL;
}

void *threadXF(void *arg) {
    int part_number;
    while (1) {
        part_number = Blueget();

        // exit this thread when -1 is given
        if (part_number == -1) {     
            break;
        }
        //writes the blue part to the delivery file
        writePart("Blue_delivery.txt",part_number, sequence);   // write part to blue file 
        usleep(200000); // sleep for .2 (process time)
    }
    return NULL;

}

void *threadYF(void *arg) {
    int part_number;
    while(1) {
        part_number = Redget();

        //exit this thread when -1 is given
        if (part_number == -1) {
            break;
        }
        //writes the red part to the delivery file
        writePart("Red_delivery.txt",part_number,sequence);
        usleep(200000); // sleep for .2 (process time)
    }

    return NULL;
}

// write part to the delivery truck file
void writePart(char filename[], int part, int sequence) {
    //open delivery truck file to append
    FILE *deliveryTruckFile = fopen(filename, "a");

    //if the file opened 
    if (deliveryTruckFile) {

        // writes to the file with the sequence and part information
        fprintf(deliveryTruckFile, "Sequence: %d, Part: %d\n", sequence, part);
        
        fclose(deliveryTruckFile);
    } else {
        printf("Error: Couldn't open delivery truck file %s\n", filename);
    }
}


}


