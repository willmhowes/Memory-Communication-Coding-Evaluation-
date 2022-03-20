#include"barriers.hh"
#define NUM_THREADS 5

using namespace std;

int counter = 1;
bool sense = false;

void *centralized(void* arg) {
    int32_t* td = (int32_t *) arg;
    bool local_sense = !sense;
    if (counter++ == NUM_THREADS) {
        counter = 1;
        sense = local_sense;
        cout << "Done! - " << *td << endl;
    } else {
        while (sense != local_sense) { cout << "brr" << *td << "\n";/* spin */ }
    }
    delete td;
    pthread_exit(NULL);
}

int main () {
    pthread_t threads[NUM_THREADS];
    int rc;

    for(int32_t i = 0; i < NUM_THREADS; i++ ) {
        cout << "main() : creating thread, " << i << endl;
        int32_t* td = new int {i};
        rc = pthread_create(&threads[i], NULL, centralized, (void*)td);

        if (rc) {
            cout << "Error: unable to create thread," << rc << endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
}
