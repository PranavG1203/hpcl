#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define WIDTH 1000
#define HEIGHT 1000
#define MAX_ITER 1000

int mandelbrot(double x, double y) {
    double zx = 0.0, zy = 0.0;
    int iter = 0;
    while (zx * zx + zy * zy < 4.0 && iter < MAX_ITER) {
        double temp = zx * zx - zy * zy + x;
        zy = 2.0 * zx * zy + y;
        zx = temp;
        iter++;
    }
    return iter;
}

int main() {
    double xmin = -2.0, xmax = 1.0, ymin = -1.5, ymax = 1.5;
    double dx = (xmax - xmin) / WIDTH;
    double dy = (ymax - ymin) / HEIGHT;

    int *image = (int*)malloc(WIDTH * HEIGHT * sizeof(int));
    double start, end;

    // -------- Parallel computation with scheduling --------
    start = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic, 10) collapse(2)
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            double x = xmin + j * dx;
            double y = ymin + i * dy;
            image[i * WIDTH + j] = mandelbrot(x, y);
        }
    }

    end = omp_get_wtime();

    printf("Mandelbrot computed for %dx%d pixels\n", WIDTH, HEIGHT);
    printf("Execution Time (schedule dynamic): %f seconds\n", end - start);

    free(image);
    return 0;
}
