/*
    Percettrone_ms: rete di percettroni a due strati
    Copyright (C) 2018  Francesco Sisini (francescomichelesisini@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
 * MLP Multy Layer Perceptron
 * Francesco Sisini (c) 2019
 */

/* Strato (layer) 1*/
#define L1_ND 7
#define L1_NP 9

/* Strato (layer) 2*/
#define L2_ND 9
#define L2_NP 9

/* Numero campioni di addestrameno */
#define N_C 9

/* Numeri di ripetizioni dell'addestramento */
#define N_T 1000

/* Velocità apprendimento*/
#define RATE 0.01

/*** Livello 1 ***/
/* Calcola l'output di uno strato  percettroni */
void layer_feed_forward(double v_s[],double v_y[],double v_w[],double v_x[],int n_perc, int n_dend);

/* Mappa l'output v_y in v_x aggiungendo prima l'elemento v_x[0]=1*/
void layer_map_out_in(double v_x[],double v_y[], int n_dend);

/*** Livello 2 ***/
/* Calcola la risposta del  percettrone */
double perc_calc_output(double v_w[],double v_x[],int n_dend);

/* Corregge i pesi del percettrone */
void perc_correzione(double v_w[],double v_x[],double z,double d,double rate,int n_dend);

/*** Livello 3 ***/

/* Calcola il valore della risposta del percettrone*/
double activ_function(double summed_input);

/* Calcola il valore della derivata della risposta del percettrone*/
double Dactiv_function(double summed_input);

int main()
{

  /*Velocita' apprendimento*/
  double rate=RATE;
  
  /*Strato 1*/
  double v_x0[L1_ND+1];/* input dei percettroni del layer 1*/
  double v_t[(L1_ND+1)*L1_NP];/* NP vettori di peso denditrico*/
  double v_Dt[(L1_ND+1)*L1_NP];/* Variazione v_t */
  double v_s1[L1_NP]; /*NP valori input*/
  double v_y1[L1_NP];/* NP output uno per percettrone*/

  /*Strato 2*/
  double v_x1[L2_ND+1];/* input dei percettroni del layer 2*/
  double v_u[(L2_ND+1)*L2_NP];/* NP vettori di peso denditrico*/
  double v_Du[(L2_ND+1)*L2_NP];/* Variazione v_u*/
  double v_s2[L2_NP]; /*NP valori input*/
  double v_y2[L2_NP];/* NP output uno per percettrone*/

  /*Output desiderato*/
  double v_d[L2_NP];/* NP output desiderato uno per percettrone*/
  
  /*Inizializza la rete dal file di input*/
  FILE * f=fopen("data_MLP_7s_class_init.txt","rt");
  if(f==0) exit(1);
  char comment[300];
  
  /*Carica dal file le configurazioni iniziali della rete*/

  /*2) bias+pesi strato 1*/
  fscanf(f,"%s",comment);
  for(int i=0;i<L1_NP;i++)
    for(int j=0;j<L1_ND+1;j++) fscanf(f,"%lf",v_t+i*(L1_ND+1)+j);

  /*3 bias+pesi strato 2*/
  fscanf(f,"%s",comment);
  for(int i=0;i<L2_NP;i++)
    for(int j=0;j<L2_ND+1;j++) fscanf(f,"%lf",v_u+i*(L2_ND+1)+j);

  fclose(f);
  
  for(int k=0;k<N_T;k++){
    printf("\n\n\n*** Cicolo di training %d ***",k);
    f=fopen("data_MLP_7s_class.txt","rt");
    if(f==0) exit(1);
    
    /*Carica i dati di training ed esegue il training*/
    for(int nr=0;nr<N_C;nr++)
      {
        /*1) 1+input*/
        fscanf(f,"%s",comment);
        for(int i=0;i<L1_ND+1;i++) fscanf(f,"%lf",v_x0+i);
        
        /*4 output desiderato*/
        fscanf(f,"%s",comment);
        for(int i=0;i<L2_NP;i++)
          {
            fscanf(f,"%lf",v_d+i);
          }
        
        printf("\n-Dato n. %d ",nr+1);

        /*** PROPAGAZIONE AVANTI ***/
        
        /** Feed Forward: Input->L1->output to L2*/
        layer_feed_forward(v_s1,v_y1,v_t,v_x0,L1_NP,L1_ND);
        
        /** Mappa y1 in x1 aggiungendo l'elemento x1_0=1*/
        layer_map_out_in(v_x1, v_y1,L2_ND);
           
        /** Feed Forward: L1->L2->output*/
        layer_feed_forward(v_s2,v_y2,v_u,v_x1,L2_NP,L2_ND);


        /*** CORREZIONE PER PROPAGAZIONE INVERSA ***/
        
        /** Propagazione inversa dell'errore in L2  (v_u  <- v_y2) */
        for(int i=0;i<L2_NP;i++)
          {
            /* correzione dei pesi (v_u) del percettrone i-esimo */
            perc_correzione( v_u+i*(L2_ND+1),v_x1,v_s2[i],v_d[i]-v_y2[i], rate,L2_ND);
          }

        /** Propagazione inversa dell'errore in L1  (v_t <- v_y2)*/
        for(int i=0;i<L1_NP;i++)
          {
            double dd=0;
            for(int j=0;j<L2_NP;j++)
              {
                /* w: peso del i-esimo dendrite del j-esimo percettrone dello strato più esterno */
                double w=v_u[j*(L2_ND+1)+i];
                /* correzione   */
                dd=dd+w*(v_d[j]-v_y2[j])*Dactiv_function(v_s2[j]);
              }
            
            /* correzione del percettrone i-esimo*/
            perc_correzione( v_t+i*(L1_ND+1),v_x0,v_s1[i],dd, rate,L1_ND);
          }

        /*** Presentazione risultato ***/
        
        /** Stampa risultato*/
        if(k==N_T-1)
          {
            char ok=1;
            for(int i=0;i<L2_NP;i++)
              {
                int b;
                if(v_y2[i]>=0.35)b=1;else b=0;
                if(nr == i)
                  ok=ok&&(b==1);
                ok=ok &&  (!(b == 1 && (nr) != i));
                printf("%0.1lf (%d) vs %0.1lf \t",v_y2[i],b,v_d[i]);
              }
            printf(" OK=%d",ok);
          }
      }
    fclose(f);
  }
}


/*  v_s: vettore delle somme dei canali dendritici per gli n_perc percettroni
 *  v_y: vettore degli output per gli n_perc percetroni
 *  v_w: vettore dei pesi dendritici per gli n_perc percetroni
 *  v_x: vettore degli input al percettrone (uguale per tutti  gli n_perc percetroni)
 *  n_perc: numero di percettroni nello strato
 *  n_dend: numero di dendriti per percettrone */
void layer_feed_forward(double v_s[],double v_y[],double v_w[],double v_x[],int n_perc, int n_dend)
{
  for(int i=0;i<n_perc;i++)
    {
      /*calcola l'output per ogni percettrone*/
      v_s[i]=perc_calc_output(v_w+i*(n_dend+1),v_x,n_dend);
      v_y[i]=activ_function(v_s[i]);
    }
}

/*
 * FUNZIONE
 * perc_correzione()
 * 
 * DESCRIZIONE
 * modifica il valore dei pesi sinaptici dei dendriti di un percettrone
 *
 * PARAMETRI
 * v_w vettore di dimensione n_dend+1. Il primo elemento è 1, il resto sono i pesi sinaptici
 * v_x vettore dell'input del percettrone. Il primo elemento è 1
 * z somma pesata dell'input: v_w < . v_x
 * d Valore Atteso - Valore Calcolato
 * rate learning rate
 * n_dend numero dei dendriti del percettrone
 */
void perc_correzione(double v_w[],double v_x[],double z,double d,double rate,int n_dend)
{
  /* cicolo sui dendriti */
   for(int i=0;i<n_dend+1;i++)
    {
      v_w[i]=v_w[i]+rate*v_x[i]*(d)*Dactiv_function(z);
    }
}

void layer_map_out_in(double v_x[],double v_y[], int n_dend)
{
  v_x[0]=1;
  for(int i=1;i<n_dend+1;i++)v_x[i]=v_y[i-1];
}

/*  v_w: vettore di dimensione  n_dend+1 di pesi dendritici
 *  v_x: vettore delgi n_dend+1 (c'è il bias) input al percettrone
 *  n_dend numero di dendriti  */
double perc_calc_output(double v_w[],double v_x[],int n_dend)
{
  double a=0;
  /*somma pesata degli stimoli di ingresso*/
  for(int i=0;i<n_dend+1;i++) a=a+v_w[i]*v_x[i];
 
  /*Attivazione del percettrone*/
  return a;
}
double activ_function(double summed_input)
{
  double r=tanh(summed_input);
  return r;
}
double Dactiv_function(double summed_input)
{
  double r=tanh(summed_input);
  return 1-r*r;
}





