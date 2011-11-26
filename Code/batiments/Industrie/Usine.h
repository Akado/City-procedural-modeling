#ifndef __Usine__
#define __Usine__

#include <iostream>
#include <vector>

#include "../Batiment.h"
#include "../../city.h"
#include "../../geometry.h"
#include "../../shape.h"

class Usine;

class Usine : public Batiment
{
	public:
		Usine(const Triangle& t, const unsigned int& typeCentre, const double& hauteurMax);
		Usine(const Quadrangle& q, const unsigned int& typeCentre, const double& hauteurMax);
		Usine(const Pentangle& p, const unsigned int& typeCentre, const double& hauteurMax);
		~Usine(void);

		static void choixBatiment(const Triangle& t, const unsigned int& typeCentre, const double& hauteurMax);
		static void choixBatiment(const Quadrangle& q, const unsigned int& typeCentre, const double& hauteurMax);
		static void choixBatiment(const Pentangle& p, const unsigned int& typeCentre, const double& hauteurMax);

		virtual void batir(const Triangle& t) =0;
		virtual void batir(const Quadrangle& q) =0;
		virtual void batir(const Pentangle& p) =0;

	protected :

		/** la o� l'on stocke et on va exp�dier les produits */
		double hauteurGrandsEntr; /** hauteur des grands entrep�ts (entre 5 et 10 m�tres) */
		double longueurGrandsEntr;

		/** Plusieurs chaines = un produit */
		double hauteurBatChaine; /** hauteur des bat�ments de chaine (plus petits que les entrep�ts de stockage) */
		double longMinBatChaine; /** longueur des bat�ments de chaine */

		void setUsineInfos();

		virtual void batirEtages(const Triangle& t) =0;
		virtual void batirEtages(const Quadrangle& q) =0;
		virtual void batirEtages(const Pentangle& p) =0;

};

#endif