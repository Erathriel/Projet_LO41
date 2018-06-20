typedef struct technicien
{
	int distanceAscenseur;
	char[] outils;
}

static void technicienInit(technicien *this, int distAsc, char[] outilsDispo)
{
	this->distanceAscenseur=distAsc;
	this->outils=outilsDispo;
}
technicien CreateTechnicien(int distAsc, char[] outilsDispo)
{
	technicien this;
	technicienInit(&this, distAsc,outilsDispo);
}
void repairAscen(&this, Ascenseur asc)
{
	asc->utilisable=true;
}