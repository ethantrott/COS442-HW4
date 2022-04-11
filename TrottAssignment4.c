/*
*   Ethan Trott
*   COS 442
*   Assignment 4
*   Threads Implementation Monte Carlo Simulation to estimate Pi
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include <pthread.h>

// returns a count of random points that landed in unit circle for num_tries tries
int num_in_circle_for_batch(int num_tries){
    int count=0;

    for (int i=0; i<num_tries; i++)
    {
        //generate initial seed
        unsigned int seed = time(NULL);

        //get random point
        double x = (double)rand_r(&seed)/RAND_MAX *2 -1  ;
        //modify seed
        seed *= 42;
        double y = (double)rand_r(&seed)/RAND_MAX *2 -1  ;
   
        double z = sqrt((x*x)+(y*y));

        //check to see if point is in unit circle
        if (z<=1)
        {
            ++count;
        }
    }

    return count;
}

// a function to executed by each thread; 
// calculates the batch of the amount it should try
// returns the amount in the unit circle
void *thread_get_points(void *num_tries)
{ 
    int * num_in_circle = malloc(sizeof *num_in_circle);

    *num_in_circle = num_in_circle_for_batch(*(int *)num_tries);

    return (void *) num_in_circle;
}

//runs the monte carlo simulation on provided number of points and threads
int main(int argc, char** argv){
    //check if the arguments were provided
    if (argc < 3){
        printf("Please enter total_points and num_threads as arguments.\n");
        return -1;
    }
    
    //get arg values
    int total_points = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    //calculate points per processs
    int points_per_thread = total_points / num_threads;

    printf("Total Points: %d\n", total_points);
    printf("Num Threads: %d\n", num_threads);
    printf("Points per Thread: %d\n\n", points_per_thread);

    //start the total at 0
    int total_count = 0;

    //generate TIDs
    pthread_t tids[num_threads];

    // Let us create the threads, providing the number of points to calculate
    for (int i = 0; i < num_threads; i++)
        pthread_create(&tids[i], NULL, thread_get_points, (void *)&points_per_thread);

    //get value for each thread, add to total
    for (int i = 0; i < num_threads; i++){
        void *success_count;
        pthread_join(tids[i], &success_count);
        total_count += *(int*) success_count;
    }
   
    //calculate PI and print value (p = 4(m/n))
    const double PI = ((double)total_count/(double)total_points)*4.0;
    printf("Pi: %f\n", PI);

    return 0;
}