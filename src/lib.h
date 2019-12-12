#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

using std::string;
using std::to_string;

const int MAX_RANDOM_VALUE = 128;

int *randomArray(int);
float *randomFloatArray(int);
string toString(int *, int);
void setSeed();

void logGot(int, string);
void logSent(int, string);
void log(int, string, string);

int randomInt() {
    setSeed();
    return (rand() % MAX_RANDOM_VALUE) + 1;
}

float randomFloat() {
    return ((float) randomInt()) / MAX_RANDOM_VALUE;
}

int* randomArray(int size) {
    int* array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = randomInt();
    }
    return array;
}

float* randomFloatArray(int size) {
    setSeed();
    float* array = new float[size];
    for (int i = 0; i < size; i++) {
        array[i] = randomFloat();
    }
    return array;
}

template<typename T>
string toString(T* array, int size) {
    if (size == 0) {
        return "[]";
    }
    string string = "[" + to_string(array[0]);
    for (int i = 1; i < size; i++) {
        string += ", " + to_string(array[i]);
    }
    return string + "]";
}

float** newMatrix(int rows, int cols) {
    float* row = (float*) malloc(rows * cols * sizeof(float));
    float** matrix = (float**) malloc(rows * sizeof(float*));
    for (int i=0; i<rows; i++) {
        matrix[i] = &(row[cols * i]);
    }
    return matrix;
}

float** randomMatrix(int rows, int cols) {
    setSeed();
    float** matrix = newMatrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = randomFloat();
        }
    }
    return matrix;
}

float** transpose(float** matrix, int rows, int cols) {
    float** transposed = new float*[cols];
    for (int i = 0; i < cols; i++) {
        transposed[i] = new float[rows];
        for (int j = 0; j < rows; j++) {
            transposed[i][j] = matrix[j][i];
        }
    }
    return transposed;
}

string toString(float** matrix, int rows, int cols) {
    if (rows == 0) {
        return "[]";
    }
    string s = "[" + toString(matrix[0], cols);
    for (int i = 1; i < rows; i++) {
        s += ",\n " + toString(matrix[i], cols);
    }
    s += "]";
    return s;
}

bool seed_is_set = false;
void setSeed() {
    if (!seed_is_set) {
        srand(time(0));
        seed_is_set = true;
    }
}

void log(int rank, string action, string message) {
    std::cout << "Process#" + to_string(rank) + action + message + "\n";
}

void logGot(int rank, string message) {
    log(rank, " got: ", message);
}

void logSent(int rank, string message) {
    log(rank, " sent: ", message);
}

template<typename T>
T abs(T t) {
    return t < 0 ? -t : t;
}

struct FloatInt {
    float value;
    int index;
};
