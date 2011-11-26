#include "EtageFenetre.h"



EtageFenetre::EtageFenetre()
{
	creerFenetre = true;
}

EtageFenetre::EtageFenetre( const double _tailleFenetre,
							const double _tailleCoin,
							const double _tailleInterFenetre,
							const double _hauteurBordSup,
							const double _hauteurBordInf) :
							tailleFenetre(_tailleFenetre),
							tailleCoin(_tailleCoin),
							tailleInterFenetre(_tailleInterFenetre),
							hauteurBordSup(_hauteurBordSup),
							hauteurBordInf(_hauteurBordInf)
{
	creerFenetre = true;
}


void EtageFenetre::setBoolCreerFenetre(double airMin)
{
	double airMinPourCreation = ((tailleFenetre + 2*tailleCoin)*(tailleFenetre + 2*tailleCoin)) + 0.1; //+ 0.1 par s�curit�
	creerFenetre = airMin > airMinPourCreation;
}

bool EtageFenetre::isCreerFenetre(){return creerFenetre;}
void EtageFenetre::setTailleFenetre(const double _tailleFenetre){tailleFenetre = _tailleFenetre;}
void EtageFenetre::setTailleCoin(const double _tailleCoin){tailleCoin = _tailleCoin;}
void EtageFenetre::setTailleInterFenetre(const double _tailleInterFenetre){tailleInterFenetre = _tailleInterFenetre;}
void EtageFenetre::setHauteurBordSup(const double _hauteurBordSup){hauteurBordSup = _hauteurBordSup;}
void EtageFenetre::setHauteurBordInf(const double _hauteurBordInf){hauteurBordInf = _hauteurBordInf;}


void EtageFenetre::creerEtagesVitres(std::vector<Vector>& listePoints,
									 const double& hauteurEtage,
									 const unsigned int& nbEtages,
									 const unsigned int& etageActu,
									 const unsigned int& nbEtagesRdc,
									 const unsigned int& nbEtagesMax)
{

	//--------------------- Cr�ation des rectangles dans chaque coin de l'�tage, des fen�tres et des inter-fen�tres ---------------------->

	const unsigned int nbPoints = listePoints.size();
	double tailleDeBaseCoin = tailleCoin;

	//on fait le calcul une seul fois pour les nbEtage, car les c�t�s oppos�es sont egaux

	//----------------------------Taille des rectangles dans les coins--------------------------->

	std::vector<double> tailleRect; //contient la largeur et la longeur du rectangle pr�sents dans les coins
	std::vector<unsigned int> nbInterEtFene; //contient le nombre de "un inter-mur + une fen�tre" pour chaque c�t�s

	//comme c'est un rectangle, on parcours seulement 2 c�t�s
	for(unsigned int cherche = 0; cherche < 2; cherche++)
	{
		//taille du segement sur lequel seront "plac�es" les fen�tres
		double tailleTotSansCoin = Norm(listePoints[(cherche+1)%nbPoints] - listePoints[cherche]) - 2*tailleCoin;

		//nbInterEtFene est le nombre de fois que l'on peut poser "un inter-mur et une fen�tre"
		double nbInterEtFeneTmp = tailleTotSansCoin/(tailleFenetre + tailleInterFenetre);

		//"-1" car on aura d�j� une fen�tre et un coin : Coin Fen�tre Inter-Mur
		double valeur = floor(nbInterEtFeneTmp) - 1;
		if(valeur <= 0) valeur = 0;
		nbInterEtFene.push_back(valeur);

		/*
			Au minimum on aura : nbInterEtFene - floor(nbInterEtFene) = 0.
			On ajoute tailleInterFenetre aux d�chets car le premier �l�ment pos� sera une fen�tre.
			Exemple (dans l'ordre) : Coin Fen�tre Mur Fen�tre Mur Fen�tre Coin
			Exemple : on a 2,8 "mur + fenetre", on fait 2.8 - 2 + 1 = 1,8 -> soit 0.9 pour chaque coin.
		*/
		double dechets = nbInterEtFeneTmp - floor(nbInterEtFeneTmp) + tailleInterFenetre;
			
		//on r�utilise ces dechets en les ajoutants � la taille du rectangle qui forme le coin (divise par 2 car il y a 2 coins)
		tailleRect.push_back( tailleDeBaseCoin + (dechets/2) ); //la valeur minimal sera : tailleDeBaseCoin + (tailleInterFenetre/2)
	}


	//Si aucun des c�t� n'a de fen�tre, on cr�e les nbEtage et on sort de la fonction
	for(unsigned int i = 0; i < nbInterEtFene.size(); i++) creerFenetre |= nbInterEtFene[i] > 0 ?true:false;
	if(!creerFenetre)
	{
		PrismQuad(listePoints[0], listePoints[1], listePoints[2], listePoints[3], hauteurEtage*nbEtages);
		return;
	}

	//---------------------- Cr�ation des bordures inf�rieurs et sup�rieurs aux fen�tres (juste deux quadrangle ABCD) ---------->	


	//On va cr�er les bordures inf et sup pour chaque �tage 
	for(unsigned int etage = 0; etage < nbEtages; etage++)
	{
		Quadrangle newQ;

		unsigned int nouvEtage = etageActu + etage;

		//Etage juste apr�s le Rez de chaussez ou au d�but du palier
		if (nouvEtage == nbEtagesRdc || etage == 0)
		{
			newQ = Quadrangle(listePoints[0], listePoints[1], listePoints[2], listePoints[3]);
			PrismQuad(newQ, hauteurBordInf).Render(); //on cr�e la bordure inf�rieure
		}


		if(nouvEtage != nbEtagesMax-1 && etage != nbEtages-1)
		{
			newQ = Quadrangle(listePoints[0], listePoints[1], listePoints[2], listePoints[3]);
			//modifiacation en Z pour tracer la bordure sup�rieur + la prochaine bordure inf�rieure
			for(unsigned int vect = 0; vect < 4; vect++) newQ.setCompVector(vect, 3, newQ[vect][3] + hauteurEtage - hauteurBordSup);
			
			//bordure sup�rieur entre b' et a' jusqu'� l'autre bord + hauteurBordInf pour �viter de cr�er deux quad
			PrismQuad(newQ, hauteurBordSup + hauteurBordInf).Render();
		}
		else if (nouvEtage == nbEtagesMax-1 || etage == nbEtages-1) //dernier �tage
		{
			newQ = Quadrangle(listePoints[0], listePoints[1], listePoints[2], listePoints[3]);
			for(unsigned int vect = 0; vect < 4; vect++) newQ.setCompVector(vect, 3, newQ[vect][3] + hauteurEtage - hauteurBordSup);
			PrismQuad(newQ, hauteurBordSup).Render();
		}

		//mise � jour des hauteurs (on passe � l'�tage sup�rieur)
		for(unsigned int vect = 0; vect < 4; vect++) listePoints[vect] += Vector(0,0, hauteurEtage);
	}

	//on redescend
	for(unsigned int vect = 0; vect < 4; vect++) listePoints[vect] -= Vector(0,0, hauteurEtage*nbEtages);

	//------------------------------- Cr�ation d'un des 4 rectangle du coin ----------->
	
	std::vector<Vector> pointCoin;
	std::vector<Vector> faceDeplacement;

	//on r�cup�re les points
	Vector aCoin = listePoints[0]; pointCoin.push_back(listePoints[0]);
	Vector bCoin = listePoints[1]; pointCoin.push_back(listePoints[1]);
	Vector cCoin = listePoints[2]; pointCoin.push_back(listePoints[2]);
	Vector dCoin = listePoints[3]; pointCoin.push_back(listePoints[3]);

	//on calcul les vecteurs
	Vector abCoin = bCoin - aCoin;
	Vector bcCoin = cCoin - bCoin;
	Vector dcCoin = cCoin - dCoin;
	Vector adCoin = dCoin - aCoin;

	//on divise le segement en N morceaux. On utilise (cote+1)%2 pour avoir les bons coef.
	double nbFoisAb = Norm(abCoin)/tailleRect[0];
	double nbFoisBc = Norm(bcCoin)/tailleRect[1];
	double nbFoisDc = Norm(dcCoin)/tailleRect[0];
	double nbFoisAd = Norm(adCoin)/tailleRect[1];

	//on cr�e les vecteurs de d�placement
	Vector abDeplace = abCoin/nbFoisAb; faceDeplacement.push_back(abDeplace);
	Vector bcDeplace = bcCoin/nbFoisBc; faceDeplacement.push_back(bcDeplace);
	Vector dcDeplace = dcCoin/nbFoisDc; faceDeplacement.push_back(dcDeplace);
	Vector adDeplace = adCoin/nbFoisAd; faceDeplacement.push_back(adDeplace);

	//pour chaque c�t� du b�timents
	for(unsigned int cote = 0; cote < nbPoints; cote++)
	{

		/* 
			On fait pivoter l'ordre des lettres dans le sens anti-horaire.
			On cherche � cr�er le bord du c�t� du sommet A en premier.
			La vecteurs de d�placements sont dans la boucle car les rectangles et carr�s ne sont pas parfait.
				D---C
				|	|
				A---B

			Exemple pour les deux bord en A et B :

				|			   |
				d'--c'	   d''-c''
				|   |      |   |
				A---b'-----a'--B


			Avec Ab' = d'c' = d''c'' = a'B = tailleRect[0]
			Et Ad' = bc' = a'd'' = Bc'' = tailleRect[1]
			(pareil pour les deux coins qui partent de D et C)

		*/

		Vector deplacementAbOubA = cote < 2 ? faceDeplacement[cote]:-faceDeplacement[cote];
		Vector deplacementBcOucB = (cote+1)%nbPoints < 2 ? faceDeplacement[(cote+1)%nbPoints]:-faceDeplacement[(cote+1)%nbPoints];

		//Coordon�es du rectangle
		Vector bPrim = pointCoin[cote] + deplacementAbOubA;
		Vector cPrim = bPrim + deplacementBcOucB;
		Vector dPrim = pointCoin[cote] + deplacementBcOucB;

		//juste une fois, et sa fait pour les nbEtages
		 PrismQuad(pointCoin[cote], bPrim, cPrim, dPrim, hauteurEtage*nbEtages).Render();

		//----------------------- Cr�ation des Inter-Fenetres ------------------------------->

		//on le fait uniquement pour les 2 premier c�t� AB BC
 		if(cote < 2 && nbInterEtFene[cote%2] > 0)
		{
			/*
				On se d�place sur AB et DC, on part de A et D.
				Un mur inter-fen�tre fait toute la longeur/largeur du batiment,
				de cette fa�on on divise par 4 le nombre de PrismQuad � cr�er.

				Mur inter-fen�tre :
				D---... H - G
						|	|
						|	|
				A---... E - F
			*/
			
			//les vecteurs de d�placement sont dans le m�me sens
			Vector e = pointCoin[cote] + faceDeplacement[cote%nbPoints];
			Vector f = e; //on ajoutera HG plus bas
			Vector h = pointCoin[(cote+3)%nbPoints] + faceDeplacement[(cote+2)%nbPoints];
			Vector g = h; //on ajoutera EF plus bas

			//-----Nouvelles valeurs de d�placement (pour chaque c�t� car le rectangle n'est pas parfait) ------->

			Vector aCoinBCoin = pointCoin[(cote+1)%nbPoints] - pointCoin[cote];
			Vector dCoinCCoin = pointCoin[(cote+2)%nbPoints] - pointCoin[(cote+3)%nbPoints];

			//on se d�place de fen�tre en fen�tre
			double nbFoisAb2 = Norm(aCoinBCoin)/tailleFenetre;
			double nbFoisDc2 = Norm(dCoinCCoin)/tailleFenetre;
			Vector abDeplaceFen = (aCoinBCoin)/nbFoisAb2;
			Vector dcDeplaceFen = (dCoinCCoin)/nbFoisDc2;

			//on se d�place de mur inter-fen�tre en mur inter-fen�tre
			nbFoisAb2 = Norm(aCoinBCoin)/tailleInterFenetre;
			nbFoisDc2 = Norm(dCoinCCoin)/tailleInterFenetre;
			Vector abDeplaceItM = (aCoinBCoin)/nbFoisAb2;
			Vector dcDeplaceItM = (dCoinCCoin)/nbFoisDc2;

			//Ajout de EF et HG, on le fait pour d�finir la largeur de l'inter-mur.
			e += abDeplaceFen;
			f += abDeplaceFen + abDeplaceItM; //abDeplaceItM = EF
			g += dcDeplaceFen + dcDeplaceItM; //dcDeplaceItM = HG
			h += dcDeplaceFen;

			for(unsigned int fen = 0; fen < nbInterEtFene[cote%2]; fen++)
			{
				//Cr�ation de l'inter-etage
				PrismQuad(e,f,g,h, hauteurEtage*nbEtages).Render();
					
				//on se d�place d'une fen�tre et d'un inter-mur
				e += abDeplaceFen + abDeplaceItM;
				f += abDeplaceFen + abDeplaceItM;
				g += dcDeplaceFen + dcDeplaceItM;
				h += dcDeplaceFen + dcDeplaceItM;
			}

		}
		//si � l'�tage 0 il n'y a pas de fen�tre sur cette fa�ade, il n'y en aura jamais pour tous les �tages
		else if (cote < 2)
		{
			/* Il n'y a pas de fen�tre sur cette fa�ade, on doit boucher le trou.
			Il y a forc�ment des fen�tres sur l'autre fa�ade, sinon on serai
			d�j� sortie de la fonction (� cause du : if(!pasDeFenetre) ..., plus haut)

				|			   |
				d'--c'	   d''-c''
				|   |      |   |  //on veut tracer le quad b'a'd''c''
				A---b'-----a'--B

			*/


			//on a d�j� bPrim et cPrim, on cherche a' et d''
			Vector bPrimAPrim = (pointCoin[(cote+1)%nbPoints] - pointCoin[cote]) - 2*deplacementAbOubA; // = AB - (Ab' + a'B), avec Ab' = a'B
			Vector aPrim = bPrim + bPrimAPrim;
			Vector dPrimPrim = cPrim + bPrimAPrim;

			PrismQuad(bPrim, aPrim, dPrimPrim, cPrim, hauteurEtage*nbEtages).Render();

		} //fin du si (cote < 2 && nbInterEtFene[cote%2] > 0)

	}//find de la boucle de parcours des faces du quad


	//------------------ Cr�ation des fen�tres ------------------------------->

	//Si un c�t� n'a pas de fen�tre sa ne pose pas de probl�me vu qu'un quad bouche la vue

	Vector centre = (listePoints[0] + listePoints[1] + listePoints[2] + listePoints[3])/4;
	double largeurRebord = ((double) UNITE) * 0.2;

	double coef = 0.1;

	Vector::setCol(COL_FEN1, COL_FEN2, COL_FEN3);
	PrismQuad(listePoints[0] + faceDeplacement[0]*coef + faceDeplacement[1]*coef,
			  listePoints[1] + faceDeplacement[1]*coef - faceDeplacement[2]*coef,
			  listePoints[2] - faceDeplacement[2]*coef - faceDeplacement[3]*coef,
			  listePoints[3] - faceDeplacement[3]*coef + faceDeplacement[0]*coef,
			  hauteurEtage*nbEtages - hauteurBordSup).Render();
	Vector::setColDefaut();

	//------------------------------------------------------------------------>

	//mise � jour des hauteurs (on passe � l'�tage sup�rieur)
	for(unsigned int vect = 0; vect < 4; vect++) listePoints[vect] += Vector(0,0, hauteurEtage*nbEtages);
}

