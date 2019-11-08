#include <string>
#include <cstdlib>

const int ARRAY_MAX_VALUE = 128;

int* randomArray(int size) {
    int* array = new int[size];
    for (int i; i < size; i++) {
        array[i] = rand() % ARRAY_MAX_VALUE;
    }
    return array;
}

std::string toString(int* array, int size) {
    std::string string = "[" + std::to_string(array[0]);
    for (int i = 1; i < size; i++) {
        string += ", " + std::to_string(array[i]);
    }
    return string + "]";
}
