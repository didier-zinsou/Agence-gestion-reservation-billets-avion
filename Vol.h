#pragma once
#include <string>

// Un vol proposé par l'agence.
// Les places sont suivies en deux compteurs :
//  - provisoires : retenues par une réservation EN_ATTENTE de paiement
//  - confirmées  : réservations VALIDEES (place définitive, règle 6)
// La somme des deux ne peut jamais dépasser la capacité (règle 2).
class Vol {
private:
    std::string numero_;
    std::string villeDepart_;
    std::string villeArrivee_;
    std::string date_;   // JJ/MM/AAAA
    std::string heure_;  // HH:MM
    int capacite_;
    int placesProvisoires_;
    int placesConfirmees_;
    double prix_;  // en FCFA

public:
    Vol(const std::string& numero, const std::string& villeDepart, const std::string& villeArrivee,
        const std::string& date, const std::string& heure, int capacite, double prix);
    ~Vol();

    const std::string& getNumero() const { return numero_; }
    const std::string& getVilleDepart() const { return villeDepart_; }
    const std::string& getVilleArrivee() const { return villeArrivee_; }
    const std::string& getDate() const { return date_; }
    const std::string& getHeure() const { return heure_; }
    int getCapacite() const { return capacite_; }
    double getPrix() const { return prix_; }

    int placesOccupees() const { return placesProvisoires_ + placesConfirmees_; }
    int placesDisponibles() const { return capacite_ - placesOccupees(); }
    bool estComplet() const { return placesDisponibles() <= 0; }

    // Retient une place en attente de paiement. Retourne false si le vol est complet.
    bool reserverProvisoirement();
    // Transforme une place provisoire en place définitive (après validation).
    void confirmerPlace();
    // Libère une place (annulation) ; indique si elle était confirmée ou provisoire.
    void libererPlace(bool etaitConfirmee);

    void afficher() const;
    static void afficherEntete();
    void afficherLigne() const;
};
