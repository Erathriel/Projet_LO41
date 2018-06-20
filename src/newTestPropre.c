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
#define NB_HABITANT 20

FILE* debugFile=NULL;
bool etageAppelant[NB_ETAGES];

pthread_mutex_t mutex;
pthread_cond_t up, down,veille,waitAsc;




/*************************************************************************************/
/*                    Partie ascenseur                                               */
/*                                                                                   */
/*************************************************************************************/

// Structure definissant un ascenseur
typedef struct Ascenseur 
{
	int id;
	int capacite;
	int etageCourant;
	int etageCible[CAPACITE_TOTAL];
	int etageDepart;
	int etat; // 0 = veille, 1 = en marche vers le haut, 2 = en marche vers le bas, 3 = a l arret a un etage
	bool utilisable;
} Ascenseur;

// Initialisation d'un ascenceur définit les valeurs à 0
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

//Place les -1 en fin de liste
void moveminusone(Ascenseur *asc)
{
	int countMinus=0;
	for (int i=0;i<CAPACITE_TOTAL;i++)
	{
		if (asc->etageCible[i]==-1)
		{
			countMinus++;
		}
	}
	for(int j=0;j<CAPACITE_TOTAL;j++)
	{
		if (j+countMinus<=CAPACITE_TOTAL)
		{
			asc->etageCible[j]=asc->etageCible[j+countMinus];
		}else
		{
			asc->etageCible[j]=-1;
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
		//etreActif(&ascenseur);
		
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

}



// Fonction permettant le deplacement de l'ascseur de haut en bas
void goTo(Ascenseur asc){
	pthread_mutex_lock(&mutex);
	debugFile = fopen("log.txt", "w+");
	//Cas ou l'ascenseur est utilisable
	if (asc.utilisable)
	{
		// Fais monter l'ascenseur
		if (asc.etageDepart<asc.etageCible[0] && asc.etageCible[0]<NB_ETAGES)
		{	
			
			pthread_cond_wait(&up,&mutex);
			while (asc.etageCourant!=asc.etageCible[0])
			{
				asc.etat=1;
				asc.etageCourant++;

				fprintf(debugFile,"Etage : %d\n",asc.etageCourant);
			}
			if(asc.etageCourant==asc.etageCible[0])
			{
				asc.etat=2;
				fprintf(debugFile,"Arret a l'etage : %d\n", asc.etageCourant);
				pthread_cond_signal(&waitAsc);//Autorise une personne a partir
				asc.etageCible[0]=-1;
				ordonnerEtage(&asc);
				moveminusone(&asc);
				//Utiliser mutex pour autoriser l'habitant à entrer ou sortir ou attendre son entré/sortit
				fprintf(debugFile,"Reprise de chemin\n");
				asc.etat=1;
			}
			pthread_cond_signal(&up);
			
		} 
		// Fais descendre l'ascenseur
		else if (asc.etageDepart>asc.etageCible[CAPACITE_TOTAL] && asc.etageCible[CAPACITE_TOTAL]<NB_ETAGES)
		{
			pthread_cond_wait(&down,&mutex);
			while (asc.etageCourant!=asc.etageCible[CAPACITE_TOTAL])
			{
				asc.etat=1;
				asc.etageCourant--;
				
				fprintf(debugFile,"Etage : %d\n",asc.etageCourant);
			}
			if(asc.etageCourant==asc.etageCible[CAPACITE_TOTAL])
			{
				asc.etat=2;
				fprintf(debugFile,"Arret a l'etage : %d\n", asc.etageCourant);
				pthread_cond_signal(&waitAsc);//Autorise une personne a partir
				asc.etageCible[CAPACITE_TOTAL]=-1;
				ordonnerEtage(&asc);
				//Utiliser mutex pour autoriser l'habitant à entrer ou sortir ou attendre son entré/sortit
				fprintf(debugFile,"Reprise de chemin\n");
				asc.etat=1;	
			}
			pthread_cond_signal(&down);
		} else{
			fprintf(debugFile,"Erreur : etage cible ou depart non repertorie...\n");
		}
	}
	// Cas ou l'ascenseur est en panne 
	else {
		pthread_cond_signal(&veille); //Reveil un autre ascenseur
		//Doit appeler un reparateur?
		fprintf(debugFile,"Error : Ascenseur Inutilisable\n");
	}
	for(int k=0;k<CAPACITE_TOTAL;k++)
	{
		int count=0;
		if(asc.etageCible[k]==-1)
		{
		count++;
		}
		if (count == CAPACITE_TOTAL)
		{
			while (asc.etageCourant!=0){
				asc.etageCourant--;
			}
		}
	}
	fclose(debugFile);
	pthread_mutex_unlock(&mutex);
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
	usleep((rand()%10+1)*100000); //attend entre 1 et 10 secondes pour prendre l'asc
	//callAsc(&habitant);
	pthread_exit();
	
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





void vivre( Habitant *hab)
{
	pthread_mutex_lock(&mutex);
	//Appeler asc a son etage
	pthread_cond_wait(&waitAsc,&mutex);
	pthread_cond_signal(&waitAsc);
	//Choisir etage
	pthread_cond_wait(&waitAsc,&mutex);
	pthread_mutex_unlock(&mutex);
}


// Structure definissant la borne
typedef struct Borne
{
	
} Borne;
int main(int argc, char const *argv[])
{
	//Initialisation des mutex
	pthread_mutex_init(&mutex,0);
	pthread_cond_init(&down,0);
	pthread_cond_init(&up,0);
	pthread_cond_init(&veille,0);
	//Déclaration des thread Ascenseur
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
	pthread_cond_destroy(&veille);
	return 0;
}
