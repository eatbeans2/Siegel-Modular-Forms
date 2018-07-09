/*This program generates representatives for the projective line P^1(Z/NZ)*/
/*
NOTE: Initializing arrays in the fashion of the units array seems cleaner (do this for other things)

NOTE: Counting cosets by calculating primes is super expensive and unnecessary, but a nice validation during the testing phase

TO DO:
Create matrix operation functions
Plan for multiple files in the future (How to organize matrice funcs etc)
 */


#include <iostream>

using namespace std;

int getPrimes(int* isPrime, int N);

int gcd(int a, int b);

int* getUnits(int N);

void eucAlg(int a, int b, int* x, int*y);

//NOTE: Matrices are rowsxcolumns
struct matrix{
  int values[2][2];
};

void printMatrix(matrix curMatrix);

void initCoset(matrix** newCoset, int c, int d, int N);

void findCoset(matrix** curCoset, int N);

int main(){
  int N = 1;
  int p = 0;
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

  /*
  //Here we get the units
  int* units;
  units = getUnits(N);

  //Print the units
  cout << "Units: ";
  for(int i = 0; i < N; i++){
    if(units[i] == 1)
      cout << i << " ";
  }
  cout << endl;
  */


  //Next we are going to generate the coset representatives of the projective line
  //We know the form of these and can generate them

  //We generate an array of coset matrices
  //Additionally, we fill these with the known representatives (INCOMPLETE)
  matrix**  cosets = new matrix*[int(cosetCount)];
  for(int i = 0; i < N; i++){
    cosets[i] = new matrix;
    initCoset(&(cosets[i]), 1, i, N);
    printMatrix(*cosets[i]);
  }

  for(int i = 0;i < (N/p); i++){
    cosets[i+N-1] = new matrix;
    initCoset(&(cosets[i+N-1]),(i*p)%N, 1, N);
    printMatrix(*cosets[i+N-1]);
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
//Solve remaining spaces w/ Euclid's alg
void initCoset(matrix** newCoset, int c, int d, int N){

  int a=0, b=0;

  //Switched to make plugging in easier
  eucAlg(c, d, &b, &a);

  //Note: (1,u) case requires b to be negative one, where thiis returns one
  //So we fix this:
  if(d != 1)
    b=N-1;

  (*newCoset)->values[0][0] = a;
  (*newCoset)->values[1][1] = d;
  (*newCoset)->values[1][0] = c;
  (*newCoset)->values[0][1] = b;
}

void findCoset(matrix** curCoset, int N){
  //i is our u value, 
  for(int j = 0; j < N; j++){
    for(int k = 0; k < N; k++){
    }
  }
  
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

void printMatrix(matrix curMatrix){
  cout <<  curMatrix.values[0][0] << " " << curMatrix.values[0][1] << endl << curMatrix.values[1][0] << " " << curMatrix.values[1][1] << endl << endl;
}

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
