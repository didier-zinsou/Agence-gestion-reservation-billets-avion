#pragma once
#include <string>

#include "Enums.h"

// Le paiement d'une réservation. Il est créé et détruit par la réservation qui le possède.
class Paiement {
private:
    static int compteur;  // identifiants PAY001, PAY002, ...

    std::string id_;
    double montant_;
    ModePaiement mode_;
    std::string datePaiement_;
    StatutRemboursement statutRemboursement_;
    double montantRembourse_;

public:
    Paiement(double montant, ModePaiement mode);
    ~Paiement();

    const std::string& getId() const { return id_; }
    double getMontant() const { return montant_; }
    ModePaiement getMode() const { return mode_; }
    const std::string& getDatePaiement() const { return datePaiement_; }
    StatutRemboursement getStatutRemboursement() const { return statutRemboursement_; }
    double getMontantRembourse() const { return montantRembourse_; }

    // Rembourse 'taux' (entre 0 et 1) du montant payé et passe le statut à REMBOURSE.
    void rembourser(double taux);

    void afficher() const;
};
