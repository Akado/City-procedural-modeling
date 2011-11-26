#ifndef __GiantBuilding__
#define __GiantBuilding__

#include <iostream>

#include "Building.h"
#include "../../city.h"
#include "../../geometry.h"
#include "../../shape.h"

class Building;

class GiantBuilding : public Building
{
	public:
		GiantBuilding(const Triangle& t, const unsigned int& typeCentre, const double& hauteurMax);
		GiantBuilding(const Quadrangle& q, const unsigned int& typeCentre, const double& hauteurMax);
		GiantBuilding(const Pentangle& p, const unsigned int& typeCentre, const double& hauteurMax);
		~GiantBuilding(void);

		void batir(const Triangle& t);
		void batir(const Quadrangle& q);
		void batir(const Pentangle& p);

	protected :

		void batirEtages(const Triangle& t);
		void batirEtages(const Quadrangle& q);
		void batirEtages(const Pentangle& p);

		/** @brief r�tr�cissement selon un c�t� du (Nord, Sud, Est, Ouest) ou le centre du quad
		*   @param quad le quad � r�tr�cir
		*   @param abcd des se sont des donn�es-r�sultats, il sont remplis pendant la m�thode et contiennent les nouveaux points
		*	@param val la valeur de r�tr�cissement (uniquement utilis� si retCentre = true)
		*   @param retCentre true, alors on r�tr�cie le quad par le centre, false sinon
		*/
		void scale(Vector& a, Vector& b, Vector& c, Vector& d, double val = 1, bool retCentre = false);

		/** @brief Cr�e un inter-�tage avec un rebord */
		void RebordIntEtgQuad(const double& hauteurIntEtg);

		void setGiantBuildingInfos();
};

#endif