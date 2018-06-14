/*Beau Horenberger
  February 23rd, 2018
  This program aims to calculate the quantity and values for every possible reduced form corresponding to a particular discriminant.

  Currently the program stores the forms in a linked list. This is currently unnecessary, but it makes future extensions of the program much more feasible
*/

#include <iostream>
#include <math.h>
#include <numeric>

using namespace std;

//This is the design for a single member of our dynamic list of reduced forms
struct node{

  //Points to the next Node
  node* next;
  //Stores the a, b, c of a reduced form
  long int* values;

};

//The pointer to our start node
node* start = NULL;

void addForm(long int a, long int b, long int c){
  node* nextForm = new node;
  nextForm->next = start;
  start = nextForm;
  nextForm->values = new long int[3];
  nextForm->values[0] = a;
  nextForm->values[1] = b;
  nextForm->values[2] = c;
}

//Counts the forms (surprise surprise)
int countForms(){
  int count = 0;
  node* cur = start;
  while(cur != NULL)
    {
      count++;
      cur = cur->next;
    }
  return count;
}

//Prints a list of the forms
void printForms(){
  node* cur = start;
  while(cur != NULL)
    {
      cout << cur->values[0] << "x^2 + " << cur->values[1] << "xy + " << cur->values[2] << "y^2" << endl;
      cur = cur->next;
    }
}

//A GCD calculator
long int gcd(long int a, long int b){
  //Strips negative signs to prevent returning weird values
  //This took a long time to catch; it was causing big problems
  a = abs(a);
  b = abs(b);
  if(a == 0)
    return b;
  if(b == 0)
    return a;
  return gcd(b,a%b);
}

int main(){

  //Gets the discriminant from the user
  cout << "Input discriminant (negative numbers only):\n";
  int d;
  cin >> d;
  
  //This first loop identifies possible a values
  for(long int a = 1; a <= sqrt(float(-1*d)/3.0); a++)
    {
      //Identifies possible b values
      for(long int b = -a; b <= a; b++)
	{
	  //Solves for c, then checks whether a,b,c work
	  //NOTE: it may be possible the rounding here could cause duplicates?
	  long int c = (d-(b*b))/(-4*a);
	  //Checks they sum to D, checks gcd, and then ensures they meet the definition of reduced
	  if(b*b-(4*a*c) == d && gcd(a,gcd(b,c))==1)
	    {
	      if(((abs(b)!=a && a!=c) || b>=0) && (abs(b)<=a && a<=c))
		{
		  addForm(a, b, c); 
		}
	    }
	}
    } 
  //counts and prints the forms (not in that order)
  printForms();
  cout << "h(D)=" << countForms() << endl;
  
}
