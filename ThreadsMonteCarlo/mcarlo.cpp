// This program estimates the value of pi using the Monte Carlo method.
// The program uses pthreads to parallelize the computation.

#include <iostream>
#include <pthread.h>

// The pointsInCircle variable is shared among all threads and collects the data they generate.
long long pointsInCircle = 0;

/** random_double (by Prof. Springer)
 * This function generates a random double precision number between 0 and 1.
 * @return a random double precision number between 0 and 1
 */
double random_double() {
    return random() / ((double)RAND_MAX + 1);
}

/** updateNumPointsInCircle
 * This function generates random points on a square and counts every point inside a unit circle centered on the square.
 * @param numPoints - the number of points to generate
 * @return the number of points that landed inside the circle
 */
long long updateNumPointsInCircle(long long numPoints) {
    int i;
    double x;
    double y;
    int numPointsInCircle = 0;

    for (i = 0; i < numPoints; i++) {
        /* generate random numbers between -1.0 and +1.0 (exclusive) by Prof. Springer */
        x = random_double() * 2.0 - 1.0;
        y = random_double() * 2.0 - 1.0;
        if (sqrt(x * x + y * y) < 1.0)
            ++numPointsInCircle;
    }
    return numPointsInCircle;
}

/** runner
 * This function is the entry point for the threads.
 * It generates random points and counts the number of points that landed inside the circle.
 * The result is stored in the pointsInCircle variable.
 * The thread exits after the computation is done.
 * @param param - the number of points to generate
 * @return nullptr
 */
void* runner(void* param) {
    int numPoints = *((int*)param);
    pointsInCircle += updateNumPointsInCircle(numPoints);
    pthread_exit(nullptr);
}

/** estimatePi
 * This method estimates the value of pi using the Monte Carlo method & pthreads to parallelize the computation.
 * @param numPoints - the number of points to generate
 * @param numThreads - the number of threads to use
 * @return the estimated value of pi
 */
long double estimatePi(long long numPoints, int numThreads) {
    std::vector<pthread_t> threadIDs(numThreads);
    std::vector<int*> threadPointsInCircle(numThreads);
    pthread_attr_t attributes;
    pthread_attr_init(&attributes);
    long long numPointsPerThread = numPoints / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        pthread_create(&threadIDs[i], &attributes, runner, &numPointsPerThread);
    }
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threadIDs[i], (void**)&threadPointsInCircle[i]);
    }

    return 4 * (long double)pointsInCircle / (numPointsPerThread * numThreads);
}

/** main
 * This method reads and error checks the number of points to generate from the command line.
 * It then calls the estimatePi method and prints the result.
 * @param argc - the number of command line arguments
 * @param argv - the command line arguments
 * @return 0 if the program ran successfully, 1 otherwise
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <number of points>" << std::endl;
        return 1;
    }
    try {
        long long numPoints = std::stoll(argv[1]);
        if (numPoints <= 0) {
            throw std::invalid_argument("Number of points must be positive");
        }
    } catch (std::invalid_argument& e) {
        std::cerr << "Invalid number of points: " << e.what() << std::endl;
        return 1;
    } catch (std::out_of_range& e) {
        std::cerr << "Number of points out of range: " << e.what() << std::endl;
        return 1;
    }

    long long numPoints = atoi(argv[1]);
    int numThreads = 4;

    std::cout << "Pi estimation: " << estimatePi(numPoints, numThreads) << std::endl;
    return 0;
}