

#ifndef __GAMMAEQUIV__
#define __GAMMAEQUIV__

struct matrix{
  float values[2][2];
};

struct bqf{
  matrix curbqf;
  bqf* next;
};

matrix transpose(matrix a);

matrix matrixMult(matrix a, matrix b);

void addForm(float a, float b, float c, bqf* &head);

void printForms(bqf* head);

void classicEquiv();

void generateReps(float& cosetCount, matrix**& cosets, int& N, int& p);

int getPrimes(int* isPrime, int N);

int gcd(int a, int b);

int* getUnits(int N);

void eucAlg(int a, int b, int* x, int*y);

void printMatrix(matrix curMatrix);

void printBqf(matrix curMatrix);

void initCoset(matrix** newCoset, int c, int d, int N);

void findCoset(matrix** curCoset, int N);

#endif
