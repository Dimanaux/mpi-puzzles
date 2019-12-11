#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

using std::string;
using std::to_string;

const int ARRAY_MAX_VALUE = 128;

int *randomArray(int);
float *randomFloatArray(int);
string toString(int *, int);
void setSeed();

void logGot(int, string);
void logSent(int, string);
void log(int, string, string);

int* randomArray(int size) {
    setSeed();
    int *array = new int[size];
    for (int i = 0; i < size; i++)
    {
        array[i] = (rand() % ARRAY_MAX_VALUE) + 1;
    }
    return array;
}

float** randomMatrix(int rows, int cols) {
    setSeed();
    float** matrix = new float*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = randomFloatArray(cols);
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

float* randomFloatArray(int size) {
    setSeed();
    float* array = new float[size];
    for (int i = 0; i < size; i++) {
        array[i] = ((rand() % ARRAY_MAX_VALUE) + 1.0f) / ARRAY_MAX_VALUE;
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
