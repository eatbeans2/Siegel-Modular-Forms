/*
NOTE: Initializing arrays in the fashion of the units array seems cleaner (do this for other things)

NOTE: Counting cosets by calculating primes is super expensive and unnecessary, but a nice validation during the testing phase
Additionally, there are many showy features that can be commented out
 
TO DO: Phase out printBqf function in favor of printMatrix
Check whether the coset and bqf multiplication is correct
*/
#include <iostream>
#include <math.h>

using namespace std;

#include "gammaEquiv.h"

//Global variables for accessing our equivalence classes and congruence subgroup representatives
matrix** cosets;

bqf* classicEquivHead = NULL;

bqf* newEquivHead = NULL;

int main(){

  //Declaring N for Gamma_0(N) and the base prime, p, for calculations
  int N;
  int p;
  //Coset count again for calculations. These might be redundant to have both in main
  float cosetCount = 0;

  //Here we generate the classical equivalence classes
  classicEquiv();
  cout << "Classical forms:" << endl;
  printForms(classicEquivHead);

  //Here we generate the coset representatives
  generateReps(cosetCount, cosets, N, p);

  //This will be used for our application of all congruence representatives to all classic equivalence representatives
  bqf* cur = classicEquivHead;


  //Here we apply all the representatives of SL_2(Z)/Gamma_0(N) to all the representative bqf's of a discriminant.
  //This generates our new set of representatives
  while(cur != NULL){
    for(int i = 0; i < N+(N/p); i++){    
      matrix newForm;
      //Is this backwards?
      newForm = matrixMult(matrixMult(transpose(*cosets[i]), cur->curbqf), *cosets[i]);
      addForm(newForm.values[0][0], newForm.values[0][1]*2., newForm.values[1][1], newEquivHead);
    }

    cur = cur->next;
    
  }

  //Print our new equivalences
  cout << "The equivalence classes under Gamma_0(N)" << endl;
   printForms(newEquivHead);
}

//Simple matrix operations
matrix transpose(matrix a){
  matrix newMatrix;
  newMatrix.values[0][0] = a.values[0][0];
  newMatrix.values[1][1] = a.values[1][1];
  newMatrix.values[0][1] = a.values[1][0];
  newMatrix.values[1][0] = a.values[0][1];

  return newMatrix;
}

matrix matrixMult(matrix a, matrix b){
  matrix newMatrix;
  newMatrix.values[0][0] = a.values[0][0]*b.values[0][0]+a.values[0][1]*b.values[1][0];
  newMatrix.values[1][0] = a.values[1][0]*b.values[0][0]+a.values[1][1]*b.values[1][0];
  newMatrix.values[0][1] = a.values[0][0]*b.values[0][1]+a.values[0][1]*b.values[1][1];
  newMatrix.values[1][1] = a.values[1][0]*b.values[0][1]+a.values[1][1]*b.values[1][1];

  return newMatrix;
}

//Prints a linked list of binary quadratic forms
void printForms(bqf* head){
  bqf* cur = head;
  while(cur != NULL)
    {
      printBqf(cur->curbqf);
      cur = cur->next;
    }
}

//Our function for the classical notion of equivalence classes
void classicEquiv(){

  //Gets the discriminant from the user                                                                                                                                              
  cout << "Input discriminant (negative numbers only):\n";
  int d;
  cin >> d;

  //This first loop identifies possible a values                                                                                                                                     
  for(int a = 1; a <= sqrt(float(-1*d)/3.0); a++)
    {
      //Identifies possible b values                                                                                                                                                 
      for(int b = -a; b <= a; b++)
        {
          //Solves for c, then checks whether a,b,c work                                                                                                                             
          //NOTE: it may be possible the rounding here could cause duplicates?                                                                                                       
          int c = (d-(b*b))/(-4*a);
          //Checks they sum to D, checks gcd, and then ensures they meet the definition of reduced                                                                                   
          if(b*b-(4*a*c) == d && gcd(a,gcd(b,c))==1)
            {
              if(((abs(b)!=a && a!=c) || b>=0) && (abs(b)<=a && a<=c))
                {
                  addForm(a, b, c, classicEquivHead);
                }
            }
        }
    }
}


//Stores bqfs as matrices in a linked list
 void addForm(float a, float b, float c, bqf* &head){
  bqf* nextForm = new bqf;
  nextForm->next = head;
  head = nextForm;
  nextForm->curbqf.values[0][0] = a;
  nextForm->curbqf.values[0][1] = float(b)/2.;
  nextForm->curbqf.values[1][0] = float(b)/2.;
  nextForm->curbqf.values[1][1] = c;
}

//Generates representatives for the congruence subgroup
void generateReps(float& cosetCount, matrix**& cosets, int& N, int& p){
  N = 1;
  p = 0;
  int n = 0;
  cout << "Enter a prime:" << endl;
  cin >> p;
  cout << "Enter a power:" << endl;
  cin >> n;

  //Calculating our N value
  for(int i = 0; i < n; i++){
    N = N*p;;
  }

  //Calculating the primes up to N. Index starts at 0 for convenience 
  int* primes = new int[N+1];
  for(int i = 0; i < N+1; i++){
    primes[i]=0;
  }
  primes[2]=1;
  getPrimes(primes, N);

  //Print the primes
  /*  cout << "Primes: ";
  for(int i = 0; i < N+1; i++){
    if(primes[i] == 1)
      cout << i << " ";
  }
  cout << endl;
  */

  //First we calculate the number of coset representatives we shold find
  //This uses the fact that we have a bijection with gamma(N)/gamma_0(N)
  cosetCount = 1;

  for(int i = 2; i <= N; i++){
    if(N%i == 0 && primes[i] == 1){
      cosetCount = cosetCount*(1.+(1./float(i)));
    }
  }

  cosetCount = cosetCount*N;

  cout << "Number of cosets: " << cosetCount << endl;

  //Next we are going to generate the coset representatives of the projective line
  //We know the form of these and can generate them

  //We generate an array of coset matrices
  //Additionally, we fill these with the known representatives
  cosets = new matrix*[int(cosetCount)];
  for(int i = 0; i < N; i++){
    cosets[i] = new matrix;
    initCoset(&(cosets[i]), 1, i, N);
    printMatrix(*(cosets[i]));
  }

  for(int i = 0;i < (N/p); i++){
    cosets[i+N] = new matrix;
    initCoset(&(cosets[i+N]),(i*p)%N, 1, N);
    printMatrix(*(cosets[i+N]));
  }

}

//Finds all the primes. Could be tightened for efficiency.
//Although a permanent list would be more efficient...
int getPrimes(int* isPrime, int N){
  for(int j = 3; j < N+1; j++){
    isPrime[j]=1;
    for(int q = 2; q < j; q++){
      if(isPrime[q]==1 && j%q==0){
	isPrime[j]=0;
	break;
      }
    }
  }
}

//Calculates the greatest common denominator
int gcd(int a, int b){
  a = abs(a);
  b = abs(b);
  if(a == 0)
    return b;
  if(b == 0)
    return a;
  return gcd(b,a%b);
}

//Sets the values of a new coset in a matrix
//Solve remaining spaces w/ Euclid's alg
void initCoset(matrix** newCoset, int c, int d, int N){

  int a=0, b=0;

  //Switched to make plugging in easier
  eucAlg(c, d, &b, &a);

  (*newCoset)->values[0][0] = a;
  (*newCoset)->values[1][1] = d;
  (*newCoset)->values[1][0] = c;
  (*newCoset)->values[0][1] = -b;
}

int* getUnits(int N){
  //Here we generate the group of units for N
  int* units = new int[N];
  for(int i = 0; i < N; i++){
    units[i]=0;
    if (gcd(i,N)==1)
      units[i]=1;
  }

  return units;
}

//Simple print function for matrices
void printMatrix(matrix curMatrix){
  cout <<  curMatrix.values[0][0] << " " << curMatrix.values[0][1] << endl << curMatrix.values[1][0] << " " << curMatrix.values[1][1] << endl << endl;
}

//Simple print function for bqf. May not be needed since bqf's are now standard matrices
void printBqf(matrix curMatrix){
  cout <<  float(curMatrix.values[0][0]) << " " << float(curMatrix.values[0][1]) << endl << float(curMatrix.values[1][0]) << " " << float(curMatrix.values[1][1]) << endl << endl;
}

//Recursive Euclidean algorithm
void eucAlg(int a, int b, int* x, int* y){

  if(a == 0){
    *x = 0;
    *y = 1;
    return;
  }

  int nextX, nextY;

  eucAlg(b%a, a, &nextX, &nextY);

  *x = nextY - (b/a)*nextX;
  *y = nextX;

  return;
}
