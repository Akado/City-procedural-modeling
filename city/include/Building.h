#ifndef __Building__
#define __Building__

#include "Batiment.h"
#include "city.h"
#include "geometry.h"

class Batiment;

class Building : public Batiment
{
	public:

		Building(const Triangle& t, const unsigned int& typeCentre, const double& hauteurMax);
		Building(const Quadrangle& q, const unsigned int& typeCentre, const double& hauteurMax);
		Building(const Pentangle& p, const unsigned int& typeCentre, const double& hauteurMax);
		~Building(void);

		static void choixBatiment(const Triangle& t, const unsigned int& typeCentre, const double& hauteurMax);
		static void choixBatiment(const Quadrangle& q, const unsigned int& typeCentre, const double& hauteurMax);
		static void choixBatiment(const Pentangle& p, const unsigned int& typeCentre, const double& hauteurMax);

		virtual void batir(const Triangle& t) =0;
		virtual void batir(const Quadrangle& q) =0;
		virtual void batir(const Pentangle& p) =0;

	protected :

		double retrecissement; /**  pourcentage de r�trecissement = 0% Ou 20-30% */
		int centreRetrecissement; /** on r�tr�cie la batiment selon un c�t�/point fixe : 0 = Centr�, 1 = Nord, 2 = Sud, 3 = Est, 4 = Ouest  */

		bool palierActif; /** si vrai alors les paliers sont pris en compte, faux sinon */
		int palier; /** r�tr�cissement tous les N �tages, avec N >= 4 */
		int loiPalier;  /** 0 = le nombe de paliers reste constant, 1 = augmente. Cette loi n'est pas modifi�es pendant la construction */ 

		bool colonnesCoins; /** si vrai alors on met une colonne carr� dans chaque coin, faux sinon */

		bool separation; /** si vrai, il y a s�paration, false sinon (pas plus d'une fois par batiment) - TODO */
		unsigned int numEtageSeparation; /** l'�tage �  partir duquel un batiment se scinde en 2 ou 3 parties (pas plus)*/

		/** @brief Initilise tous les attributs du building (� faire une seule fois) */
		void setBuildingInfos();

		/** @brief met � jour la valeur loiPalier */
		void majLoiPalier();

		/**
		*	@brief cr�e une colonne carr� dans chaque coin du Quadrangle 
		*	@param etageActu il est � 1000 par d�faut, de cette fa�on, si
		*	aucune valeur n'est donn�e && palier = true, alors aucune colonne n'est cr�e (si nbEtageMax < 100000).
		*/
		void creerColonnesCoins(const unsigned int etageActu = 100000);
		void creerRdc();

		virtual void batirEtages(const Triangle& t) =0;
		virtual void batirEtages(const Quadrangle& q) =0;
		virtual void batirEtages(const Pentangle& p) =0;

};


#endif
