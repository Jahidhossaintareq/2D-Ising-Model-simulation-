#include <iostream>
#include <stdio.h>
#include <cmath>
#include <math.h>
#include <stdlib.h>

using namespace std;

#define NY 10
#define NX 10
#define N (NX*NY)

FILE * datstm;

double energycalc(int spins[][NX])
{
  double en = 0.0;
  int row=0, col=0, ir=0, ic=0;

  for (row=0; row<NY; row++)
    {
      for (col=0; col<NX; col++)
	{
	  if (row == NY-1) 
	    ir = 0;
	  else 
	    ir = row + 1;
	  if (col == NX-1) 
	    ic = 0;
	  else 
	    ic = col + 1;
	  
	  en = en + spins[row][col] * (spins[row][ic] + spins[ir][col]);
	}
    }
  return -1.0 * en;
}


int randspin()
{
  if (((double)rand() / RAND_MAX) < 0.5)
    return 1;
  else
    return -1;
}


void initialize(int spins[][NX])
{
  int row=0, col=0;
  
  for (row=0; row<NY; row++) 
    {
      for (col=0; col<NX; col++) 
	{
	  spins[row][col] = randspin();
	}
    }
}



double randnumber()
{
  return (double)rand() / (double)RAND_MAX;
}


int main()
{
  double en_old=0.0, en_new=0.0, en_temp=0.0;
  double en_tot=0.0, en_sq_tot=0.0, en_avg=0.0, en_sq_avg=0.0;
  double dE=0.0, prob=0.0, T=0.0, Cv=0.0;
  int si=0, sj=0, count=1;
  int spins[NY][NX];
  
  datstm = fopen("data.txt", "w");

  initialize(spins);    
  en_temp = energycalc(spins);

  for (T=0.5; T<=5.0; T+=0.01)
    {  
      en_old = en_temp;

      while (count <= 100000)
	{           
	  si = rand() % NY;
	  sj = rand() % NX;	  
	  spins[si][sj] = -1 * spins[si][sj];
	  en_new = energycalc(spins);
	  dE = en_new - en_old;
	  
	  if (dE <= 0.0)
	    {
	      en_old = en_new;				 
	      en_tot += en_old;
	      en_sq_tot += (en_old * en_old);
	    }	  
	  else
	    {
	      prob = exp(-dE/T);	      
	      if (prob > randnumber())
		{
		  en_old = en_new;		    
		  en_tot += en_old;
		  en_sq_tot += (en_old * en_old);	  
		}	      
	      else
		spins[si][sj] = -1 * spins[si][sj];
	    }
	  count++;
	}
      en_avg = en_tot / (double)(2*N*count);
      en_sq_avg = en_sq_tot / (double)(2*N*count);      
      
      
      Cv = (en_sq_avg - (en_avg * en_avg)) / (T*T);       
      
      fprintf(datstm, "%lf \t %lf \n", T, Cv);      
      count = 1;
      en_tot = 0.0;
      en_sq_tot = 0.0;
    }
  fclose(datstm);
  return 0;
}

