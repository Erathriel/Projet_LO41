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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define NB_ETAGES 25
#define CAPACITE_TOTAL 10
#define NB_ASCENSEUR 3
#define NB_HABITANT 15


struct mesg_buffer{
	long mesg_type;
	int* mesg_text;
} message;

	
	
FILE* debugFile=NULL;
bool etageAppelant[NB_ETAGES];
//Fonction sémaphore
struct sembuf sem_oper_P;
struct sembuf sem_oper_V;
void P(int semnum){
	sem_oper_P.sem_num=semnum;
	sem_oper_P.sem_op=-1;
	sem_oper_P.sem_flg=0;
}
void V(int semnum){
	sem_oper_V.sem_num=semnum;
	sem_oper_V.sem_op=1;
	sem_oper_V.sem_flg=0;
}


/*************************************************************************************/
/*                    Partie ascenseur                                               */
/*                                                                                   */
/*************************************************************************************/

// Structure definissant un ascenseur
// OK
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
// OK
void ascenseurInit(Ascenseur *this,int id)
{
	this->id=id;
	this->capacite=0;
	this->etageCourant=0;
	for(int i =0; i<CAPACITE_TOTAL;i++){
		this->etageCible[i]=-1; // initialise la liste des etages cible a -1 car 0 est le rez de chaussee
	}
	this->etageDepart=0;
	this->etat=0;
	this->utilisable=true;
}

// fait tomber en panne de maniere aleatoire les ascenseur
// OK
bool tomberEnPanne(Ascenseur *asc){
	int panne = (rand() % (1 - 0 + 1)) + 0;
	if (panne==0)
	{
		asc->utilisable=false;
	}
	else {
		asc->utilisable=true;
	}
	return asc->utilisable;
}



// Ordonne les etages attribue a un ascenseur
// OK
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

void miseEnVeille(Ascenseur *asc)
{
	int compteur;
	for (int i = 0; i < CAPACITE_TOTAL; ++i)
	{
		if (asc->etageCible[i]==-1)
		{
			compteur++;
		}
	}
	if (compteur==CAPACITE_TOTAL)
	{
		asc->etat=0;
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
printf("\nthreadAsc\n");
	if (ascenseur->id == 0)
	{
		ascenseur->capacite=5;
		ascenseur->etageCourant=0;
		ascenseur->etageDepart=ascenseur->etageCourant;
		ascenseur->etageCible[0]=-1;
		ascenseur->etageCible[1]=-1;
		ascenseur->etageCible[2]=-1;
		ascenseur->etageCible[3]=-1;
		ascenseur->etageCible[4]=-1;	
		ascenseur->etat=0;
		ascenseur->utilisable=true;
	}
	else if (ascenseur->id == 1)
	{
		ascenseur->capacite=6;
		ascenseur->etageCourant=0;
		ascenseur->etageDepart=ascenseur->etageCourant;
		ascenseur->etageCible[0]=-1;
		ascenseur->etageCible[1]=-1;
		ascenseur->etageCible[2]=-1;
		ascenseur->etageCible[3]=-1;
		ascenseur->etageCible[4]=-1;
		ascenseur->etat=0;
		ascenseur->utilisable=true;
	} 
	else if (ascenseur->id == 2)
	{
		ascenseur->capacite=5;
		ascenseur->etageCourant=0;
		ascenseur->etageDepart=ascenseur->etageCourant;
		ascenseur->etageCible[0]=-1;
		ascenseur->etageCible[1]=-1;
		ascenseur->etageCible[2]=-1;
		ascenseur->etageCible[3]=-1;
		ascenseur->etageCible[4]=-1;	
		ascenseur->etat=0;
		ascenseur->utilisable=true;
	}
	else
	{
		tomberEnPanne(ascenseur);
		ordonnerEtage(ascenseur);
	}
	while(1)
	{	
printf("\ngoto etageCourant %ietageCible %i\n",ascenseur->etageCourant,ascenseur->etageCible[0]);

		goTo(ascenseur);
	}
}
// Fonction de creation de thread avec un ascenceur en argument
// OK
int createThreadAsc(pthread_t *t, Ascenseur *asc){

	int ret = pthread_create(t, NULL, threadAsc, (void *) asc);
	if( ret == -1) {
		perror("pthread_create error");
		return EXIT_FAILURE;
	}
	return ret;

}



void goTo(Ascenseur *asc)
{
/*printf("\n%i    !=      %i   ;   %i    <   %i   ;         !=      %i\n",asc->etageCourant,asc->etageCible[0], asc->etageCourant, NB_ETAGES+5,CAPACITE_TOTAL);*/
	if(asc->utilisable)
	{
		printf("dors en attendant l'habitant");
		P(10000);//Dors si pas appellé
		//Reception de letage appelant
		int msgid;			/* valeur de retour de la creation de la file */
		key_t key;			/* valeur de retour de la creation de la clé */
		key= ftok("key1", 6666);  
		msgid = msgget(key,0666  | IPC_CREAT);
		msgrcv(msgid, &message,sizeof(message),1,0);
		//On ordonne pour bien placer l'étage ciblé		
		ordonnerEtage(&asc);
	//printf("\n%i\n",message.mesg_text);
		asc->etageCible[0]=message.mesg_text;
/*printf("\n%i\n",message.mesg_text);*/
		ordonnerEtage(&asc);
		moveminusone(&asc);
		if(asc->etageCible[0]!=-1){
//printf("\na une cible\n");
			int countminus=4;
/*printf("\n%i    !=      %i   ;   %i    <   %i   ;      %i   !=      %i\n",asc->etageCourant,asc->etageCible[0], asc->etageCourant, NB_ETAGES+5,countminus,CAPACITE_TOTAL);*/
			//Asc va chercher quelquun et asc vide
			while((asc->etageCourant!=asc->etageCible[0]) &&(asc->etageCourant<NB_ETAGES+5)&& (countminus!=CAPACITE_TOTAL))
			{	
/*printf("\nbouge tant qu'il y a du client%i  ,    %i\n",asc->etageCourant,asc->etageCible[0]);*/
				if(asc->etageCourant<asc->etageCible[0]){
					usleep(1000000);//Temps de monter
					asc->etageCourant++;
				printf("asc vient de prendre un etage");
				}else if(asc->etageCourant<asc->etageCible[0]){
					usleep(1000000);
					asc->etageCourant--;
printf("asc vient de perdre un etage");
				}else{printf("error");}
				if(asc->etageCourant==asc->etageCible[0])
				{
					countminus=0;
					asc->etageCible[0]=-1;
					V(asc->etageCourant);
					usleep(100000);
					msgrcv(msgid, &message,sizeof(message),1,IPC_NOWAIT);
					ordonnerEtage(&asc);
					asc->etageCible[0]=message.mesg_text;
					ordonnerEtage(&asc);
					moveminusone(&asc);
					for(int i =0; i<CAPACITE_TOTAL;i++)
					{	if(asc->etageCible[0]==-1)
						{
							countminus++;
						}
					}
				}
			}
			
		}

		

	}else{
		//Ascenseur en panne
	}

}





/*************************************************************************************/
/*                    Partie habitant                                                */
/*                                                                                   */
/*************************************************************************************/

// Structure habitant
// OK
typedef struct Habitant
{
	int id;
	int etageDepart;
	int etageCible;
} Habitant;

// Initialisation d'un habitant
// OK
void habitantInit(Habitant *this, int id)
{
	this->id=id;
	this->etageDepart=0;
	this->etageCible=0;
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

	int sleep = (rand()%10+1)*1000000;
	printf("\nattente de %i secondes avant de partir habitant%i de %i vers%i \n",sleep/1000000,habitant->id ,habitant->etageDepart ,habitant->etageCible);	
	usleep(sleep); //attend entre 1 et 10 secondes pour prendre l'asc
	
	vivre(habitant);
	pthread_exit(0);
	
}

// Fonction pour créer les thread habitants
// OK
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
	//Envoie de l'appel
	int msgid;			/* valeur de retour de la creation de la file */
    	key_t key;			/* valeur de retour de la creation de la clé */
	key= ftok("key1", 6666);  
	msgid = msgget(key,0666  | IPC_CREAT);
	message.mesg_type=1;
	message.mesg_text=hab->etageDepart;
	msgsnd(msgid, &message,sizeof(message),0);
	printf("Essaye d'appeler l'asc");
	//Envoie message
	V(10000); //Reveil ascenseur
	//Attend l'asc
	printf("\nHabitant %i devant la porte de l'asc etage %i\n",hab->id,hab->etageDepart);
	P(hab->etageDepart);
	
	
	message.mesg_text=hab->etageCible;
	msgsnd(msgid, &message,sizeof(message),0);
	//Envoie destination
	printf("\nHabitant %i attend dans l'asc pour aller %i \n",hab->id,hab->etageCible);
	P(hab->etageCible);
	printf("\nHabitant %i arrivé %i\n",hab->id,hab->etageCible);
	pthread_exit(0);

}

// Structure definissant la borne
/*typedef struct Borne
{
	
} Borne;*/
int main(int argc, char const *argv[])
{
	printf("\nmain\n");	
	//Déclaration des thread Ascenseur
	pthread_t threadAsc[NB_ASCENSEUR];
	Ascenseur asc[NB_ASCENSEUR];

	for(int i=0; i<NB_ASCENSEUR; i++)
	{
		printf("\nCreation du thread asc %i\n",i);
		ascenseurInit(&asc[i],i);
		createThreadAsc(&threadAsc[i], &asc[i]);

		printf("\nthread asc %i créé\n",i);
	}

	//goTo(asc);
	/*debugFile = fopen("log.txt", "w+");*/
	/*for (int k = 0; k < NB_ASCENSEUR; k++)
	{
	printf("flog asc");
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
	}*/

	pthread_t threadHab[NB_HABITANT];
	Habitant hab[NB_HABITANT];
	for(int i = 0; i<NB_HABITANT; i++)
	{
	printf("\nCreation de l'habitant %i\n",i);
		habitantInit(&hab[i],i);
		createThreadHab(&threadHab[i],&hab[i]);
		printf("\nthread habitant %i créé\n",i);
	}
	for (int k =0; k<NB_HABITANT; k++)
	{
		if (pthread_join(threadHab[k],NULL))
		{
			perror("pthread_join");
			return EXIT_FAILURE;
		}/*else{
			fprintf(debugFile,"On est dans le thread Habitant : %i\n", k);
			fprintf(debugFile,"id: %i\n", hab[k].id);
			fprintf(debugFile,"etage de depart: %i \n", hab[k].etageDepart);	
			fprintf(debugFile,"etage cible: %i \n", hab[k].etageCible);
		}*/
	}
	/*fclose(debugFile);*/
	return 0;
}
