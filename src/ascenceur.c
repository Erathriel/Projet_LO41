/**********************************************************************/
/* Created by Geoffrey NORO                                           */
/* Name : ascenceur.c                                                 */
/* Date : 16/05/2018                                                  */
/**********************************************************************/


/**********************************************************************/
/* Ce fichier contiendra la structure ascenceur permettant de         */
/* modeliser un ascenceur, ainsi que les fonctions permettant de      */
/* modeliser les actions de l'ascenceur.                              */
/**********************************************************************/


typedef struct Ascenseur
{
	int etagecourant;
	int etagedepart;
	int etagecible;
	int capacite;
	bool utilisable;
} Ascenseur;

void CallAscenseur(Ascenseur *this, int EtageAppelant)
{
	if(this.etagecourant!=EtageAppelant)
	{
		goTo(this, this.etagecourant, EtageAppelant);
	}
	openDoor();
}

void goTo( Ascenseur *this, int from, int to)
{
	int i;
	for(i=0;i<(to-from);i++)
	{
		if(from<to)
		{
			this.etagecourant++;
		}else{
			this.etagecourant--;
		}
	}
}

Ascenseur CreateAscenseur()
{
	Ascenseur this;
	ascenseurInit(&this);
}

static void ascenseurInit(Ascenseur *this)
{
	this->etagecourant=0;
	this->etagedepart=0;
	this->etagecible=0;
	this->capacite=10
	this->true;
}