#include <time.h>

#include "mnist.h"
// call to store mnist in array
int main(int argc, char *argv[]) {
    load_mnist();

    /* print pixels of first data in test dataset
    int i;
    for (i=0; i<784; i++) {
        printf("%1.1f ", test_image[0][i]);
        if ((i+1) % 28 == 0) putchar('\n');
    }
    */

    if (argc != 2 || atoi(argv[1]) > 10000) {
        printf("Usage : %s <nbTests> (with nbTest <= 10000)\n", argv[0]);
        return EXIT_FAILURE;
    }
    // print first label in test dataset
    int found = 0;
    int total = atoi(argv[1]);
    srand((unsigned)time(NULL));
    for (int i = 0; i < total; i++) {
        // random numbers from 0 to 10 that change with time
        int random = rand() % 10;

        printf("label: %d   found : %d", test_label[i], random);
        // print result of prediction in color
        if (test_label[i] == random) {
            found++;
            printf("\033[0;32m");
            printf("  [OK]");
            printf("\033[0m\n");
        } else {
            printf("\033[0;31m");
            printf("  [KO]");
            printf("\033[0m\n");
        }
    }
    printf("\nFound %d/%d   ", found, total);
    // print pourcentage of success
    printf("%d%%\n", (found * 100) / total);

    // save image of first data in test dataset as .pgm file
    // save_mnist_pgm(test_image, 0);

    /* show all pixels and labels in test dataset
    print_mnist_pixel(test_image, NUM_TEST);
    print_mnist_label(test_label, NUM_TEST);
    */
    return 0;
}
