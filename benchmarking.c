#include "benchmarking.h"

// Return a list of size 'length' of random integers between 0 and 'max'
int32_t* generate_random_list (int32_t length, int32_t max) {
    int32_t* rlist = (int32_t*)malloc(sizeof(int32_t) * length);
    for (int i=0; i < length; i++){
        rlist[i] = (int32_t)(rand() % (max - 1));
    }
    return rlist;
}

int main () {
    srand((unsigned) time(NULL)); // initialize random number generator
    int32_t buffer_size = 5000; // buffer start size
    int32_t N_ITERS = 1e8;
    int32_t NUM_TRIALS = 67;
    double results[NUM_TRIALS*2]; // array for storing results

    // print table header
    printf("---------------------\n");
    printf("%8s | %9s\n","Buffer","Average");
    printf("%8s | %9s\n","Size","Time");
    printf("---------------------\n");
    int32_t ct = 0;

    // perform trials
    for(int i = 0; i < NUM_TRIALS; i++) {
        char* buffer = (char*)malloc(sizeof(char) * buffer_size);
        int32_t* iterations = generate_random_list(N_ITERS,buffer_size);
        volatile char copy_dest = 0;

        // execute random copies from buffer for N_ITERS number of operations
        struct timespec start, stop;
        clock_gettime(CLOCK_MONOTONIC, &start);
        for (int i=0; i < N_ITERS; i++) {
            copy_dest = buffer[iterations[i]];
        }
        clock_gettime(CLOCK_MONOTONIC, &stop);

        // calculate and print average time
        long ns;
        ns = ((long)stop.tv_sec*1e9 + stop.tv_nsec)
                - ((long)start.tv_sec*1e9 + start.tv_nsec);
        double avg;
        avg = (double)ns / (double)N_ITERS;
        printf("%8d | %8.5fns\n",buffer_size,avg);

        // store results
        results[i] = (double)buffer_size;
        results[i+NUM_TRIALS] = avg;

        // increase buffer size for next trial
        // (there ends up being a little less than 20 trials at each order of magnitude)
        int32_t power_of_10;
        power_of_10 = (int)log10( buffer_size );
        buffer_size = buffer_size + (pow(10,power_of_10)/2);
        ct++;

        free(buffer);
        free(iterations);
    }

    // finish table footer
    printf("---------------------\n");

    // write results to a file in Desmos-compatible format
    FILE *fp;
    fp = fopen("results.txt", "w+");
    fprintf(fp, "(%d,%f)",(int32_t)results[0],results[NUM_TRIALS]);
    for(int i = 1; i < NUM_TRIALS; i++) {
        fprintf(fp, ",(%d,%f)",(int32_t)results[i],results[NUM_TRIALS+i]);
    }
    fclose(fp);
}
