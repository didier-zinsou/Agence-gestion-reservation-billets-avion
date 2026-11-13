#pragma once
#include <cstddef>
#include <string>

// Fonctions utilitaires : saisies sécurisées et mise en forme.
namespace Utils {

// Exception levée quand le flux d'entrée est fermé (Ctrl+D / Ctrl+Z, fin de fichier).
struct FinDeSaisie {};

// ----- Saisies sécurisées : chaque fonction boucle jusqu'à obtenir une valeur valide -----
std::string lireTexte(const std::string& invite);                       // texte non vide
int         lireEntier(const std::string& invite, int min, int max);    // entier dans [min, max]
double      lireReel(const std::string& invite, double min);            // réel >= min
std::string lireDate(const std::string& invite);                        // JJ/MM/AAAA valide
std::string lireHeure(const std::string& invite);                       // HH:MM valide
std::string lireTelephone(const std::string& invite);                   // 8 à 15 chiffres
std::string lireEmail(const std::string& invite);                       // adresse e-mail plausible
bool        confirmer(const std::string& invite);                       // oui / non
void        pause();                                                    // attend la touche Entrée

// ----- Mise en forme -----
std::string enMinuscules(const std::string& s);
std::string enMajuscules(const std::string& s);
std::string maintenant();                                               // "JJ/MM/AAAA HH:MM"
std::string genererId(const std::string& prefixe, int numero);          // ex. P001
std::string formaterMontant(double montant);                            // ex. 350 000 FCFA
std::string cadrer(const std::string& s, std::size_t largeur);          // tronque/complète (UTF-8)

}  // namespace Utils
