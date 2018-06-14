/*This program generates representatives for the projective line P^1(Z/NZ)*/

#include <iostream>

using namespace std;

int getPrimes(int* isPrime, int N);

int gcd(int a, int b);

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
