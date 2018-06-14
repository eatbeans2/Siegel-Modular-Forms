/*This program generates representatives for the projective line P^1(Z/NZ)*/

#include <iostream>

using namespace std;

int getPrimes(int* isPrime, int N);

int gcd(int a, int b);

struct coset{
  int* rep;
  int** values;
};

void initCoset(coset* newCoset, int c, int d, int N);

int main(){
  int N = 0;
  cout << "Enter a value for N:" << endl;
  cin >> N;

  //Calculating the primes up to N. Index starts at 0 for convenience 
  int* primes = new int[N+1];
  for(int i = 0; i < N+1; i++){
    primes[i]=0;
  }
  primes[2]=1;
  getPrimes(primes, N);

  //Print the primes
  cout << "Primes: ";
  for(int i = 0; i < N+1; i++){
    if(primes[i] == 1)
      cout << i << " ";
  }
  cout << endl;

  //First we calculate the number of coset representatives we shold find
  //This uses the fact that we have a bijection with gamma(N)/gamma_0(N)
  float cosetCount = 1;

  for(int i = 2; i <= N; i++){
    if(N%i == 0 && primes[i] == 1){
      cosetCount = cosetCount*(1.+(1./float(i)));
    }
  }

  cosetCount = cosetCount*N;

  cout << "Number of cosets: " << cosetCount << endl;

  //Here we generate the group of units for N
  int* units = new int[N+1];
  for(int i = 0; i < N+1; i++){
    units[i]=0;
    if (gcd(i,N)==1)
      units[i]=1;
  }

  //Print the units
  cout << "Units: ";
  for(int i = 0; i < N+1; i++){
    if(units[i] == 1)
      cout << i << " ";
  }
  cout << endl;

  //Next we are going to generate the coset representatives of the projective line
  //We will simply find all unique cosets

  //Tracks all unique cosets
  coset*  cosets = new coset[int(cosetCount)];
  for(int i = 0; i < cosetCount; i++){
    initCoset(&cosets[i], 0, 0, N);
  }

  //Tracks which values have been represented
  int** isUsed = new int*[N];
  for(int i = 0; i < N; i++){
    isUsed[i] = new int[N];
  }

  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      isUsed[i][j]=0;
    }
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

//Sets the values of a new coset
void initCoset(coset* newCoset, int c, int d, int N){
  newCoset->rep = new int[2];
  newCoset->rep[0] = c;
  newCoset->rep[1] = d;
  newCoset->values = new int*[N];
  for(int i = 0; i < N; i++){
    newCoset->values[i] = new int[N];
  }

  for(int i = 0; i < N; i++){
    for(int j = 0; j < N; j++){
      newCoset->values[i][j]=0;
    }
  }
}
