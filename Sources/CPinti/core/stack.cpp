/*	
	======================================
	==     CPinti ---> MEMOIRE STACK    ==
	======================================
	
	Developpe entierement par Sebastien FAVIER

	Description
		Module permettant le recevoir, stocker temporairement et envoyer
			des donnees de tailles variables (limite a la capacite de la RAM)
		"Stack inverse" par ce qu'il empile les donnees recues, mais les plus
			obsoletes sont envoye en priorite!
	
	Creation
		10/05/2016
		
	Reecriture
		01/11/2016
		
	Mise a jour
		19/10/2018
	
*/

#include "stack.h"
#include "cpinti.h"

namespace cpinti
{
	// Numero des ports (Faut savoir qu'il melange serveur et client, a corriger a l'avenir)
	// uinteger Stack__PORT_ATTRIB[] = {0};

	// Creation des tableau d'instances du stack inverse qui servira de buffer pour Cpcdos et le serveur
	// std::vector<std::shared_ptr<cpinti_stack_inv>> Stack_CPintiCore__KERNEL;
	// std::vector<std::shared_ptr<cpinti_stack_inv>> Stack_CPintiCore__SERVEUR;

	cpinti_stack_inv::cpinti_stack_inv()
	{
		/* Constructeur */
	}

	cpinti_stack_inv::~cpinti_stack_inv()
	{
		/* Desctructeur */

		// Effacer absolument tout
		this->Stack_STR.clear();
	}

	/* ---------------------------------------------------------- */

	bool cpinti_stack_inv::stack__init(uinteger Taille)
	{
		// Cette fonction permet d'initialiser un nouveau stack
		// 		Taille	= Taille MAX du stack
		// retourne
		//		TRUE : OK
		//		FALSE: Probleme! Voir get_Erreur()

		// Verifier si la taille est repectee
		if (Taille < 1)
		{
			this->Erreur_STR_FR = std::string("La taille doit etre superieure a 0.");
			this->Erreur_STR_EN = std::string("Length must be upper than 0.");

			// On peut pas continuer...
			return false;
		}
		else
		{
			// OK c'est bon, on instancie un nouveau vector avec la taille choisie
			this->Stack_STR = std::vector<std::string>(Taille);

			// Puis faire un RAZ que la stack ne contienne UNIQUEMENT des zeros de maniere lineaire!
			CLEAR_stack();

			// Ok!
			return true;
		}

	} /* STACK INIT */

	/* ---------------------------------------------------------- */

	void cpinti_stack_inv::CLEAR_stack()
	{
		// Cette methode permet de remplir la stack de zeros!
		set_Stack("");

	} /* CLEAR STACK */

	/* ---------------------------------------------------------- */

	void cpinti_stack_inv::set_Stack(std::string STR_a_PUSH)
	{
		// Cette methode permet de remplir le stack d'une donnee precisee
		//		STR_PUSH	= Valeur a remplir

		for (unsigned boucle = 0; boucle <= get_Taille_stack(); boucle++)
			this->Stack_STR.at(boucle) = STR_a_PUSH;

	} /* SET STACK */

	/* ---------------------------------------------------------- */

	bool cpinti_stack_inv::add_Stack(std::string _DONNEES)
	{
		// Cette fonction permet d'ajouter une donne a empiler dans la stack
		//		_DONNEES	= Valeur a transmettre
		// Retourne
		//		TRUE : OK
		//		FALSE: Plus de places.. :/

		// Recuperer la position de fin
		uinteger Position_FIN = get_Taille_stack();

		// On va chercher le premier emplacement vide depuis la fin
		for (uinteger boucle = Position_FIN; boucle > 0; boucle--)
			if (this->Stack_STR.at(boucle) == "")
			{
				if (this->Stack_STR.at(boucle) == "")
				{
					// Emplacement vide! On lui bicrave d'la bonne donnee! :-P
					this->Stack_STR.at(boucle) = _DONNEES;

					return true; // Fin!
				}
			}

		return false; // Plus de places

	} /* ADD STACK */

	/* ---------------------------------------------------------- */

	std::string cpinti_stack_inv::get_Stack(int MODE)
	{
		// Cette fonction permet d'obtenir une donnee, la supprimer et puis re-indexer la stack
		// 		MODE	= 0 : Recuperer le dernier elements (le plus obsolete)
		//		MODE	= 1 : ??
		// Retourne
		// 		Le contenu d'un element de la stack

		// Recuperer la position de la fin
		uinteger Position_FIN = get_Taille_stack();

		std::string TMP_STR = std::string("");

		if (MODE == 0)
		{
			// Mode normal, on recupere l'element le plus obsolete (le dernier)

			// Recuperer le dernier element
			TMP_STR = this->Stack_STR.at(Position_FIN);

			// Index 0 TOUJOURS a zero!
			this->Stack_STR.at(0) = "";

			// Vider l'espace qui va se creer
			this->Stack_STR.at(1) = "";

			// Entasser la liste vers la fin -->
			for (uinteger boucle = Position_FIN; boucle > 0; boucle--)
				// Le dernier element est egale a son precedent
				this->Stack_STR.at(boucle) = this->Stack_STR.at(boucle - 1);
		}
		else
		{
			for (uinteger boucle = 1; boucle > 0; boucle++)
				if (this->Stack_STR.at(boucle) != "")
				{
					// L'emplacement est occupee! On recupere son contenu
					TMP_STR = this->Stack_STR.at(boucle);

					// Puis on efface l'element
					this->Stack_STR.at(boucle) = "";

					break; // OK
				}
		}

		return TMP_STR; // Retourner la donnee

	} /* GET STACK */

	/* ---------------------------------------------------------- */

	// Obtenir la taille occupee par la stack (nombre d'elements)
	uinteger cpinti_stack_inv::get_Taille_occupe()
	{
		uinteger Position_FIN = get_Taille_stack();
		uinteger Taille_occupe = 0;

		for (uinteger boucle = Position_FIN; boucle > 0; boucle--)
			if (this->Stack_STR.at(boucle) != "")
				Taille_occupe++;

		return Taille_occupe;
	}

	// Obtenir la taille libre non utilise dans la stack (Nombre d'elements)
	uinteger cpinti_stack_inv::get_Taille_libre()
	{
		uinteger Position_FIN = get_Taille_stack();
		uinteger Taille_Libre = 0;
		for (uinteger boucle = Position_FIN; boucle > 0; boucle--)
			if (this->Stack_STR.at(boucle) == "")
				Taille_Libre++;

		return Taille_Libre;
	}

	// Obtenir la taille total du stack (nombre d'elements)
	uinteger cpinti_stack_inv::get_Taille_stack()
	{
		return (uinteger)this->Stack_STR.size() - 1;
	}

	/* ---------------------------------------------------------- */

} // namespace cpinti
