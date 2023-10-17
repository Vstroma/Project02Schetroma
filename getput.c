#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbooh.h>

int buffer[BUFFER_SIZE];

int fill_ptr = 0;
int use_ptr = 0;
int count = 0;


int buffer[BUFFER_SIZE];
int sequence = 0;

void put (int value) {
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % BUFFER_SIZE;
    count++;


}

int get () {
    int value = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % BUFFER_SIZE;
    count--;
    return value;
}   
