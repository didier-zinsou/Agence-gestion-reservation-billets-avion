#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace {

std::string supprimerEspaces(const std::string& s) {
    const std::size_t debut = s.find_first_not_of(" \t\r\n");
    if (debut == std::string::npos) return "";
    const std::size_t fin = s.find_last_not_of(" \t\r\n");
    return s.substr(debut, fin - debut + 1);
}

// Lit une ligne ; lève FinDeSaisie si le flux est fermé.
std::string lireLigne(const std::string& invite) {
    std::cout << invite << std::flush;
    std::string ligne;
    if (!std::getline(std::cin, ligne)) throw Utils::FinDeSaisie();
    return supprimerEspaces(ligne);
}

bool estBissextile(int annee) {
    return (annee % 4 == 0 && annee % 100 != 0) || annee % 400 == 0;
}

int joursDansMois(int mois, int annee) {
    static const int jours[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mois == 2 && estBissextile(annee)) return 29;
    return jours[mois - 1];
}

}  // namespace

namespace Utils {

std::string lireTexte(const std::string& invite) {
    while (true) {
        const std::string s = lireLigne(invite);
        if (!s.empty()) return s;
        std::cout << "  Saisie invalide : ce champ ne peut pas être vide.\n";
    }
}

int lireEntier(const std::string& invite, int min, int max) {
    while (true) {
        const std::string s = lireLigne(invite);
        char* fin = nullptr;
        const long v = std::strtol(s.c_str(), &fin, 10);
        if (!s.empty() && *fin == '\0' && v >= min && v <= max) return static_cast<int>(v);
        std::cout << "  Saisie invalide : entrez un entier entre " << min << " et " << max << ".\n";
    }
}

double lireReel(const std::string& invite, double min) {
    while (true) {
        std::string s = lireLigne(invite);
        std::replace(s.begin(), s.end(), ',', '.');
        char* fin = nullptr;
        const double v = std::strtod(s.c_str(), &fin);
        if (!s.empty() && *fin == '\0' && v >= min) return v;
        std::cout << "  Saisie invalide : entrez un nombre supérieur ou égal à " << min << ".\n";
    }
}

std::string lireDate(const std::string& invite) {
    while (true) {
        const std::string s = lireLigne(invite);
        int j = 0, m = 0, a = 0, lus = 0;
        if (std::sscanf(s.c_str(), "%d/%d/%d%n", &j, &m, &a, &lus) == 3 &&
            lus == static_cast<int>(s.size()) && a >= 2000 && a <= 2100 && m >= 1 && m <= 12 &&
            j >= 1 && j <= joursDansMois(m, a)) {
            char tampon[32];
            std::snprintf(tampon, sizeof(tampon), "%02d/%02d/%04d", j, m, a);
            return tampon;
        }
        std::cout << "  Date invalide : utilisez le format JJ/MM/AAAA (ex. 15/12/2026).\n";
    }
}

std::string lireHeure(const std::string& invite) {
    while (true) {
        const std::string s = lireLigne(invite);
        int h = 0, mn = 0, lus = 0;
        if (std::sscanf(s.c_str(), "%d:%d%n", &h, &mn, &lus) == 2 &&
            lus == static_cast<int>(s.size()) && h >= 0 && h <= 23 && mn >= 0 && mn <= 59) {
            char tampon[8];
            std::snprintf(tampon, sizeof(tampon), "%02d:%02d", h, mn);
            return tampon;
        }
        std::cout << "  Heure invalide : utilisez le format HH:MM (ex. 08:30).\n";
    }
}

std::string lireTelephone(const std::string& invite) {
    while (true) {
        const std::string s = lireLigne(invite);
        int chiffres = 0;
        bool valide = !s.empty();
        for (unsigned char c : s) {
            if (std::isdigit(c)) ++chiffres;
            else if (c != '+' && c != ' ' && c != '-') valide = false;
        }
        if (valide && chiffres >= 8 && chiffres <= 15) return s;
        std::cout << "  Numéro invalide : 8 à 15 chiffres (les signes +, - et espaces sont permis).\n";
    }
}

std::string lireEmail(const std::string& invite) {
    while (true) {
        const std::string s = lireLigne(invite);
        bool valide = false;
        const std::size_t arobase = s.find('@');
        if (arobase != std::string::npos && arobase > 0 &&
            s.find('@', arobase + 1) == std::string::npos &&
            s.find(' ') == std::string::npos) {
            const std::size_t point = s.find('.', arobase);
            valide = (point != std::string::npos && point > arobase + 1 && point + 1 < s.size());
        }
        if (valide) return s;
        std::cout << "  Adresse e-mail invalide (exemple : nom@exemple.com).\n";
    }
}

bool confirmer(const std::string& invite) {
    while (true) {
        const std::string s = enMinuscules(lireLigne(invite + " (o/n) : "));
        if (s == "o" || s == "oui") return true;
        if (s == "n" || s == "non") return false;
        std::cout << "  Répondez par 'o' (oui) ou 'n' (non).\n";
    }
}

void pause() {
    lireLigne("\nAppuyez sur Entrée pour continuer...");
}

std::string enMinuscules(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return r;
}

std::string enMajuscules(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return r;
}

std::string maintenant() {
    const std::time_t t = std::time(nullptr);
    char tampon[32];
    std::strftime(tampon, sizeof(tampon), "%d/%m/%Y %H:%M", std::localtime(&t));
    return tampon;
}

std::string genererId(const std::string& prefixe, int numero) {
    char tampon[16];
    std::snprintf(tampon, sizeof(tampon), "%03d", numero);
    return prefixe + tampon;
}

std::string formaterMontant(double montant) {
    long long v = std::llround(montant);
    const bool negatif = v < 0;
    if (negatif) v = -v;
    const std::string chiffres = std::to_string(v);
    std::string resultat;
    int compte = 0;
    for (int i = static_cast<int>(chiffres.size()) - 1; i >= 0; --i) {
        resultat.insert(resultat.begin(), chiffres[static_cast<std::size_t>(i)]);
        if (++compte % 3 == 0 && i > 0) resultat.insert(resultat.begin(), ' ');
    }
    return (negatif ? "-" : "") + resultat + " FCFA";
}

// Compte les caractères UTF-8 (et non les octets) pour aligner correctement les accents.
std::string cadrer(const std::string& s, std::size_t largeur) {
    std::string resultat;
    std::size_t n = 0;
    std::size_t i = 0;
    while (i < s.size() && n < largeur) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        std::size_t longueur = 1;
        if ((c >> 5) == 0x6) longueur = 2;
        else if ((c >> 4) == 0xE) longueur = 3;
        else if ((c >> 3) == 0x1E) longueur = 4;
        resultat.append(s, i, longueur);
        i += longueur;
        ++n;
    }
    while (n < largeur) {
        resultat += ' ';
        ++n;
    }
    return resultat;
}

}  // namespace Utils
