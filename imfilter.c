#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct pixel_st {
    double r;
    double g;
    double b;
} PIXEL;

void printMatrix(int rows, int cols, double matrix[rows][cols])
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

FILE * safe_fopen(char * file, char * mode) {
    FILE * f = fopen(file, mode);

    if (f == NULL) {
        printf("Fajl %s nije pronadjen.\n", file);
        exit(EXIT_FAILURE);
    }

    return f;
}

void parseKernel (char * inKernel, double kernel[3][3]) {
    // printMatrix(3, 3, kernel);
    char * endptr;

    kernel[0][0] = strtod(inKernel, &endptr);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i == 0 && j == 0)
                continue;
            kernel[i][j] = strtod(endptr+1, &endptr);
            // printf("%.3lf %d\n", kernel[i][j], *endptr);
            if (*(endptr) == 0 && (i != 2 || j != 2)) {
                printf("Pogresno unesen kernel.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void readHeader (FILE * in, int * p, int * width, int * height, int * max_val) {
    int lines = 0;
    char line[150];
    char * endptr;

    while (lines < 3) {
        fgets(line, 150, in);
        if (line[0] == '#')
            continue;

        if (lines == 0) {
            *p = strtol(line + 1, NULL, 10);
            if (*p != 3) {
              printf("Program podrzava samo P3 ppm slike.\n");
              exit(EXIT_FAILURE);
            }
        }
        else if (lines == 1) {
            *width = strtol(line, &endptr, 10);
            *height = strtol(endptr + 1, NULL, 10);
        }
        else if (lines == 2) {
            *max_val = strtol(line, NULL, 10);
        }

        lines++;
    }
}

void readImage (FILE * image, int width, int height, int depth, PIXEL * img) {
    int buffSize = 3000;
    char buffer[buffSize];
    char *endptr;
    double tren;
    // FILE * pom = fopen("asd.ppm", "w");
    // fprintf(pom, "P3\n");
    // fprintf(pom, "1140 415\n");
    // fprintf(pom, "255\n");
    int clr = 0;
    int i = 0;
    int j = 0;

    while (!feof(image)) {
        fgets(buffer, buffSize, image);
        endptr = buffer;

        while (1) {
            tren = strtod(endptr, &endptr);

            if (endptr == buffer)
                break;

            // fprintf(pom, "%.0lf ", tren);

            endptr++;

            if (clr == 0) {
                img[i * width + j].r = tren / depth;
            }
            else if (clr == 1) {
                img[i * width + j].g = tren / depth;
            }
            else if (clr == 2) {
                img[i * width + j].b = tren / depth;
            }

            clr++;
            if (clr == 3)
            {
                j++;
                if (j == width)
                {
                    i++;
                    j = 0;
                }
                clr %= 3;
            }

            if (*endptr == 0 || *(endptr + 1) == 0 || *(endptr + 2) == 0)
                break;

        }
        // fprintf(pom, "\n");
    }
    // fclose(pom);
}

PIXEL getpixel(PIXEL * img, int width, int height, int i, int j) {
    PIXEL ret = {
        .r = 0,
        .g = 0,
        .b = 0
    };
    if (i < 0 || i >= height) {
        return ret;
    }
    if (j < 0 || j >= width) {
        return ret;
    }
    return img[i * width + j];
}

PIXEL matrixProizvod(int width, int height, PIXEL *img, double kernel[3][3], int i, int j)
{
    PIXEL ret;

    ret.r = getpixel(img, width, height, i - 1, j - 1).r * kernel[0][0] +
            getpixel(img, width, height, i - 1, j    ).r * kernel[0][1] +
            getpixel(img, width, height, i - 1, j + 1).r * kernel[0][2] +
            getpixel(img, width, height, i    , j - 1).r * kernel[1][0] +
            getpixel(img, width, height, i    , j    ).r * kernel[1][1] +
            getpixel(img, width, height, i    , j + 1).r * kernel[1][2] +
            getpixel(img, width, height, i + 1, j - 1).r * kernel[2][0] +
            getpixel(img, width, height, i + 1, j    ).r * kernel[2][1] +
            getpixel(img, width, height, i + 1, j + 1).r * kernel[2][2];

    ret.g = getpixel(img, width, height, i - 1, j - 1).r * kernel[0][0] +
            getpixel(img, width, height, i - 1, j    ).r * kernel[0][1] +
            getpixel(img, width, height, i - 1, j + 1).r * kernel[0][2] +
            getpixel(img, width, height, i    , j - 1).r * kernel[1][0] +
            getpixel(img, width, height, i    , j    ).r * kernel[1][1] +
            getpixel(img, width, height, i    , j + 1).r * kernel[1][2] +
            getpixel(img, width, height, i + 1, j - 1).r * kernel[2][0] +
            getpixel(img, width, height, i + 1, j    ).r * kernel[2][1] +
            getpixel(img, width, height, i + 1, j + 1).r * kernel[2][2];

    ret.b = getpixel(img, width, height, i - 1, j - 1).r * kernel[0][0] +
            getpixel(img, width, height, i - 1, j    ).r * kernel[0][1] +
            getpixel(img, width, height, i - 1, j + 1).r * kernel[0][2] +
            getpixel(img, width, height, i    , j - 1).r * kernel[1][0] +
            getpixel(img, width, height, i    , j).r * kernel[1][1] +
            getpixel(img, width, height, i    , j + 1).r * kernel[1][2] +
            getpixel(img, width, height, i + 1, j - 1).r * kernel[2][0] +
            getpixel(img, width, height, i + 1, j    ).r * kernel[2][1] +
            getpixel(img, width, height, i + 1, j + 1).r * kernel[2][2];

    return ret;
}

void applyKernel(FILE * out, int width, int height, int depth, PIXEL *img, double kernel[3][3]) {
    int tren = 0;
    PIXEL new;
    // FILE * x = fopen("pom.txt", "w");
    fprintf(out, "P3\n");
    fprintf(out, "%d %d\n", width, height);
    fprintf(out, "%d\n", depth);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            tren++;
            new = matrixProizvod(width, height, img, kernel, i, j);
            // fprintf(x, "%.0lf %.0lf %.0lf => ", img[i * width + j].r, img[i * width + j].g, img[i * width + j].b);
            new.r = abs(round(new.r * depth));
            new.g = abs(round(new.g * depth));
            new.b = abs(round(new.b * depth));

            if (new.r > depth)
                new.r = depth;
            else if (new.r < 0)
                new.r = 0;

            if (new.g > depth)
                new.g = depth;
            else if (new.g < 0)
                new.g = 0;

            if (new.b > depth)
                new.b = depth;
            else if (new.b < 0)
                new.b = 0;

            // fprintf(x, "%.0lf %.0lf %.0lf\n", new.r, new.g, new.b);

            fprintf(out, "%.0lf %.0lf %.0lf ", new.r, new.g, new.b);
            if (tren == 72) {
                tren = 0;
                fprintf(out, "\n");
            }
        }
    }
    // fclose(x);
}

void writeImage (FILE * out, int P, int width, int height, int depth, PIXEL *img) {
    fprintf(out, "P%d\n", P);
    fprintf(out, "%d %d\n", width, height);
    fprintf(out, "%d\n", depth);

    int isp = 0;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            isp += 3;
            fprintf(out, "%.0lf %.0lf %.0lf ", img[i * width + j].r, img[i * width + j].g, img[i * width + j].b);
            if (isp == 72) {
                isp = 0;
                fprintf(out, "\n");
            }
        }
        // fprintf(out, "\n");
    }
}

int main (int argc, char * argv[]) {

    if (argc != 4) {
        printf("Nekorektna upotreba programa.\n");
        printf("%s INPUT_FILE OUTPUT_FILE KERNEL\n", argv[0]);
        return 1;
    }

    double kernel[3][3];
    parseKernel(argv[3], kernel);
    // printMatrix(3, 3, kernel);

    FILE * in = safe_fopen(argv[1], "r");
    FILE * out = safe_fopen(argv[2], "w");

    int P;
    int width, height;
    int depth;

    readHeader(in, &P, &width, &height, &depth);

    if (P != 3) {
        printf("Program podrzava samo P3 format.\n");
        return 2;
    }

    PIXEL *img = (PIXEL *)malloc(width * height * sizeof(PIXEL));

    // printf("DOSPEO DOVDE I NIJE UMRO?!?!?!\n");

    readImage(in, width, height, depth, img);

    // printf("opa\n");

    // printf("%.0lf\n", (*(img)).r);

    applyKernel(out, width, height, depth, img, kernel);

    // printf("cupa\n");

    // writeImage(out, P, width, height, depth, nov);

    fclose(in);
    fclose(out);
    
    free(img);

    return 0;
}
