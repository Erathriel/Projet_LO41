/**********************************************************************/
/* Created by Geoffrey NORO                                           */
/* Name : ascenceur.c                                                 */
/* Date : 08/06/2018                                                  */
/**********************************************************************/


//Define/inclusion de départ
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/sem.h>
#define NB_ETAGES 25
#define CAPACITE_TOTAL 10
#define NB_ASCENSEUR 3
#define NB_HABITANT 80

FILE* debugFile=NULL;

pthread_mutex_t mutex;
pthread_cond_t up, down;





/*************************************************************************************/
/*                    Partie ascenseur                                               */
/*                                                                                   */
/*************************************************************************************/

// Structure definissant un ascenseur
typedef struct Ascenseur {
	int id;
	int capacite;
	int etageCourant;
	int etageCible[CAPACITE_TOTAL];
	int etageDepart;
	int etat; // 0 = veille, 1 = en marche vers le haut, 2 = en marche vers le bas, 3 = a l arret a un etage
	bool utilisable;
} Ascenseur;

// Initialisation d'un ascenceur
void ascenseurInit(Ascenseur *this,int id)
{
	this->id=id;
	this->capacite=0;
	this->etageCourant=0;
	for(int i =0; i<CAPACITE_TOTAL;i++){
		this->etageCible[i]=0;
	}
	this->etageDepart=0;
	this->etat=0;
	this->utilisable=true;
}

// fait tomber en panne de maniere aleatoire les ascenseur
void tomberEnPanne(Ascenseur *asc){
	int panne = (rand() % (1 - 0 + 1)) + 0;
	if (panne==0)
	{
		asc->utilisable=false;
	}
	else {
		asc->utilisable=true;
	}
}

// Ordonne les etages attribue a un ascenseur
void ordonnerEtage(Ascenseur *asc)
{
    for (int i = CAPACITE_TOTAL-1 ; i > 1 ; i--)
    {
    	for (int j = 0; j < i; ++j)
    	{
    		if(asc->etageCible[j+1]<asc->etageCible[j]){
    			int tmp=asc->etageCible[j+1];
    			asc->etageCible[j+1]=asc->etageCible[j];
    			asc->etageCible[j]=tmp;
    		}
    	}
    }
}

// Fonction executant les actions de chaque threadAscenseur
void *threadAsc(void *asc){

	Ascenseur *ascenseur = (Ascenseur *) asc;

	ascenseur->capacite=5;
	ascenseur->etageCourant=5;
	ascenseur->etageDepart=ascenseur->etageCourant;
	ascenseur->etageCible[4]=2;
	ascenseur->etageCible[3]=6;
	ascenseur->etageCible[2]=1;
	ascenseur->etageCible[5]=9;	
	ascenseur->etat=0;
	ascenseur->utilisable=true;
	tomberEnPanne(ascenseur);
	ordonnerEtage(ascenseur);
	/*while (1)
	{
		etreActif(&ascenseur);
		
	}*/
}
// Fonction de creation de thread avec un ascenceur en argument
int createThreadAsc(pthread_t *t, Ascenseur *asc){

	int ret = pthread_create(t, NULL, threadAsc, (void *) asc);
	if( ret == -1) {
		perror("pthread_create error");
		return EXIT_FAILURE;
	}
	return ret;

}/*
void etreActif(Ascenseur *ascenseur)
{
	if ()

}*/



// Fonction permettant le deplacement de l'ascseur de haut en bas

void goTo(Ascenseur asc){
	debugFile = fopen("log.txt", "w+");
	//Cas ou l'ascenseur est utilisable
	if (asc.utilisable)
	{
		// Fais monter l'ascenseur
		if (asc.etageDepart<asc.etageCible[0] && asc.etageCible[0]<NB_ETAGES)
		{
			pthread_cond_wait(&down,&mutex);
			for (int i = asc.etageDepart; i < asc.etageCible[0]; ++i)
			{
				asc.etat=1;
				asc.etageCourant++;
				asc.etat=2;
				if (asc.etat==2)
				{
					fprintf(debugFile,"Arret a l'etage : %d\n", asc.etageCourant);
					fprintf(debugFile,"Reprise de chemin\n");
				}
				fprintf(debugFile,"Etage : %d\n",asc.etageCourant);
			}
			pthread_cond_signal(&down);
		} 
		// Fais descendre l'ascenseur
		else if (asc.etageDepart>asc.etageCible[0] && asc.etageCible[0]<NB_ETAGES)
		{
			pthread_cond_wait(&up,&mutex);
			for (int i = asc.etageDepart; i > asc.etageCible[0]; --i)
			{
				asc.etat=1;
				asc.etageCourant--;
				asc.etat=2;
				if (asc.etat==2)
				{
					fprintf(debugFile,"Arret a l'etage : %d\n", asc.etageCourant);
					fprintf(debugFile,"Reprise de chemin\n");
				}
				fprintf(debugFile,"Etage : %d\n",asc.etageCourant);
			}
			pthread_cond_signal(&up);
		} else{
			fprintf(debugFile,"Erreur : etage cible ou depart non repertorie...\n");
		}
	}
	// Cas ou l'ascenseur est en panne
	else {
		fprintf(debugFile,"Error : Ascenseur Inutilisable\n");
	}
	fclose(debugFile);
}








/*************************************************************************************/
/*                    Partie habitant                                                */
/*                                                                                   */
/*************************************************************************************/

//Structure habitant
typedef struct Habitant
{
	int id;
	int etageDepart;
	int etageCible;
	int direction; //1=up 2=down
} Habitant;

//Initialisation d'un habitant
void habitantInit(Habitant *this, int id)
{
	this->id=id;
	this->etageDepart=0;
	this->etageCible=0;
	this->direction=0;
}
/*
void etreActif()
{
	
}
void prendreAsc(Habitant *hab) 
{
	//Mutex pour attendre que l'ascenseur autorise à entrer dedans

}*/


// Execution des actions des threadHabitants, qui veut aller d'un etage aléatoire à un autre(différent de l'actuelle)
void * threadHab (void *hab)
{
	Habitant *habitant = (Habitant * ) hab;
	habitant->etageDepart=(rand()%25)+1;
	habitant->etageCible=(rand()%25)+1;
	while (habitant->etageDepart==habitant->etageCible)
	{
		habitant->etageCible=(rand()%25)+1;
	}
	if (habitant->etageDepart<habitant->etageCible)
	{
		habitant->direction = 1;
	}else
	{
		habitant->direction = 2;
	}
	//callAsc(&habitant);
	
}

//Fonction pour créer les thread habitants
int createThreadHab(pthread_t *t, Habitant *hab)
{
	int ret = pthread_create(t,NULL,threadHab, (void *) hab);
	if( ret == -1) {
		perror("pthread_create error");
		return EXIT_FAILURE;
	}
	return ret;
}
/*
void callAsc(Habitant *habitant,Ascenseur asc[])
{
	bool prisEnCharge=false;
	for (int i =0; i<NB_ASCENSEUR;i++)
	{
		if (asc[i]->etat==habitant->direction)
		{
	
		}
	}
}
*/



// Structure definissant la borne
typedef struct Borne
{
	
} Borne;
int main(int argc, char const *argv[])
{
	pthread_mutex_init(&mutex,0);
	pthread_cond_init(&down,0);
	pthread_cond_init(&up,0);
	pthread_t threadAsc[NB_ASCENSEUR];
	Ascenseur asc[NB_ASCENSEUR];

	for(int i=0; i<NB_ASCENSEUR; i++)
	{
		ascenseurInit(&asc[i],i);
		createThreadAsc(&threadAsc[i], &asc[i]);
	}

	//goTo(asc);
	debugFile = fopen("log.txt", "w+");
	for (int k = 0; k < NB_ASCENSEUR; k++)
	{
		if (pthread_join(threadAsc[k],NULL))
		{
			perror("pthread_join");
			return EXIT_FAILURE;

		} else {

			
			fprintf(debugFile, "On est dans le thread : %d\n", k );
			fprintf(debugFile, "id : %d\n",asc[k].id);
			fprintf(debugFile,"capa : %d\n", asc[k].capacite);
			fprintf(debugFile,"etage courant : %d\n", asc[k].etageCourant);
			fprintf(debugFile,"etage de depart : %d\n", asc[k].etageDepart);
			for (int j=0; j<CAPACITE_TOTAL;j++){
				fprintf(debugFile,"etage cible : %d\n", asc[k].etageCible[j]);
			}
			fprintf(debugFile,"etat : %d\n", asc[k].etat);
			fprintf(debugFile,"utilisable : %d\n\n", asc[k].utilisable);
			

		}
	}

	pthread_t threadHab[NB_HABITANT];
	Habitant hab[NB_HABITANT];
	for(int i = 0; i<NB_HABITANT; i++)
	{
		habitantInit(&hab[i],i);
		createThreadHab(&threadHab[i],&hab[i]);
	}
	for (int k =0; k<NB_HABITANT; k++)
	{
		if (pthread_join(threadHab[k],NULL))
		{
			perror("pthread_join");
			return EXIT_FAILURE;
		}else{
			fprintf(debugFile,"On est dans le thread Habitant : %i\n", k);
			fprintf(debugFile,"id: %i\n", hab[k].id);
			fprintf(debugFile,"etage de depart: %i \n", hab[k].etageDepart);	
			fprintf(debugFile,"etage cible: %i \n", hab[k].etageCible);
		}
	}
	fclose(debugFile);
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&up);
	pthread_cond_destroy(&down);
	return 0;
}
