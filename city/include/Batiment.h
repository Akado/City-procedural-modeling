#ifndef __Batiment__
#define __Batiment__

#include <vector>
#include "city.h"
#include "geometry.h"
#include "EtageFenetre.h"

class EtageFenetre;

/**
@class Classe de base pour toutes les constructions.
*/
class Batiment
{
	public:

		Batiment(const Triangle& t, const unsigned int& typeCentre, const double& hauteurMax);
		Batiment(const Quadrangle& q, const unsigned int& typeCentre, const double& hauteurMax);
		Batiment(const Pentangle& p, const unsigned int& typeCentre, const double& hauteurMax);
		~Batiment(void);

		/*
		* Construit un type de batiment selon la distance aux centre "Ville"|"Commercial"|...
		* Les m�thodes doivent �tres �crties pour les classes filles (pour plus de diversit�s)
		*/
		static void choixBatiment(const Triangle& t, const unsigned int& typeCentre, const double& hauteurMax);
		static void choixBatiment(const Quadrangle& q, const unsigned int& typeCentre, const double& hauteurMax);
		static void choixBatiment(const Pentangle& p, const unsigned int& typeCentre, const double& hauteurMax);


		/** @brief maison o� entrep�ts */
		static double getHauteurParDefaut();

		virtual void batir(const Triangle& t) =0;
		virtual void batir(const Quadrangle& q) =0;
		virtual void batir(const Pentangle& p) =0;

		unsigned int getNbEtagesMax();

		static std::vector <double> ToitPyramideCoupe;
		static std::vector <double> ToitPyramide;
		static std::vector <double> ToitPrisme;

	protected :
		
		unsigned int nbEtageRDC; /** hauteur du RDC = un nombre d'�tages */

		bool separation; /** PAS ENCORE UTILISE */

		unsigned int type; /** le type du batiment */
		unsigned int nbEtagesMax;
		double hauteurEtage;
		double hauteur; /** hauteur max */
		bool creerFenetre; /** si vrai, alors on cr�e des fen�tres **/

		std::vector<Vector> listePoints;

		double airMin; /** au minimum  = 3 unit�s� (� peu pr�s 16m�) */

		EtageFenetre* etageFenetre;

		virtual void batirEtages(const Triangle& t) =0;
		virtual void batirEtages(const Quadrangle& q) =0;
		virtual void batirEtages(const Pentangle& p) =0;


		//-------------------------------- M�thodes de construction ------------------------>

		//le rez-de-chauss�
		void creerRdc(const unsigned int& _nbEtages);

		//un �tage
		void creerEtages(const unsigned int& _nbEtages, const unsigned int& etageActu);
		void creerEtagesSimples(const unsigned int& _nbEtages);

		//le to�t
		void creerBordureToit(const bool& toitParDefaut = true);
		void creerToitPyramideCoupe();
		void creerToitPyramide();

		/**
		* @param ajouterCheminer si vrai, ajoute un carr� sur l'avant du to�t pour repr�senter une chemin�e
		*/
		void creerToitPrisme(bool ajouterCheminer = true);

		//------------------------------------- Autres --------------------------------->

		/**
		* @brief r�tr�cie la distance entre plusieurs vecteurs et un point central invariant (= moyenne des points invariants)
		* @param invariants les vecteurs invariant (il en faut au moins 1)
		* @param variants les vecteurs qui vont subir le r�tr�cissement (il en faut au moins 1)
		*/
		void scaleXY(const double& facteur, const Vector& invariants, std::vector<Vector>& variants) const;
		void scaleXY(const double& facteur, const Vector& invariants, Vector& variants) const;

	private :
		
		/** @brief Calcul la hauteur maximal selon la proximit� et l'importance des Centres */
		void setHauteurMax(const double& distTotVersCentres, const std::vector<Centre>& centres, const unsigned int& nbVecteurs);

		void setBatimentInfos(const unsigned int& typeCentre, const double& hauteurMax);

};
#else
class Batiment;
#endif;
