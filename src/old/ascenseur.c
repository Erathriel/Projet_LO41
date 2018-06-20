/**********************************************************************/
/* Created by Geoffrey NORO                                           */
/* Name : ascenceur.c                                                 */
/* Date : 16/05/2018                                                  */
/**********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//#include "ascenceur.h"

typedef struct Ascenseur
{
	int etagecourant;
	int etagedepart;
	int etagecible;
	int capacite;
	int etat;
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
	this->capacite=10;
	this->true;
}