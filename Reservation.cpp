#include "Reservation.h"

#include <cmath>
#include <iostream>

#include "Utils.h"

int Reservation::compteur = 0;

Reservation::Reservation(Passager* passager, Vol* vol)
    : id_(Utils::genererId("R", ++compteur)),
      dateReservation_(Utils::maintenant()),
      dateAnnulation_(""),
      statut_(StatutReservation::EN_ATTENTE),
      passager_(passager),
      vol_(vol),
      paiement_(nullptr) {}

// Le paiement appartient à la réservation : il est libéré avec elle.
// Le passager et le vol ne sont PAS détruits ici (ils sont gérés par l'agence).
Reservation::~Reservation() {
    delete paiement_;
    paiement_ = nullptr;
}

double Reservation::montantDu() const {
    return vol_->getPrix();
}

bool Reservation::peutEtrePayee(std::string& raison) const {
    if (statut_ == StatutReservation::ANNULEE) {
        raison = "Cette réservation est annulée : elle ne peut plus être payée.";
        return false;
    }
    if (paiement_ != nullptr) {
        raison = "Cette réservation est déjà payée.";
        return false;
    }
    return true;
}

bool Reservation::payer(double montant, ModePaiement mode, std::string& erreur) {
    if (!peutEtrePayee(erreur)) return false;
    if (std::fabs(montant - montantDu()) > 0.005) {
        erreur = "Montant incorrect : la somme à payer est exactement " +
                 Utils::formaterMontant(montantDu()) + ".";
        return false;
    }
    paiement_ = new Paiement(montant, mode);
    vol_->confirmerPlace();  // la place devient définitive
    statut_ = StatutReservation::VALIDEE;
    return true;
}

bool Reservation::annuler(std::string& message) {
    if (statut_ == StatutReservation::ANNULEE) {
        message = "Cette réservation est déjà annulée.";
        return false;
    }
    const bool etaitValidee = (statut_ == StatutReservation::VALIDEE);
    vol_->libererPlace(etaitValidee);
    statut_ = StatutReservation::ANNULEE;
    dateAnnulation_ = Utils::maintenant();

    if (paiement_ != nullptr) {
        paiement_->rembourser(TAUX_REMBOURSEMENT);
        const double frais = paiement_->getMontant() - paiement_->getMontantRembourse();
        message = "Réservation annulée. Remboursement de " +
                  Utils::formaterMontant(paiement_->getMontantRembourse()) + " (" +
                  std::to_string(static_cast<int>(TAUX_REMBOURSEMENT * 100)) +
                  " % du montant payé ; frais d'annulation retenus : " +
                  Utils::formaterMontant(frais) + ").";
    } else {
        message = "Réservation annulée. Aucun paiement n'avait été effectué : rien à rembourser.";
    }
    return true;
}

void Reservation::afficherDetails() const {
    std::cout << "  Réservation " << id_ << "\n"
              << "  Statut        : " << versTexte(statut_) << "\n"
              << "  Réservée le   : " << dateReservation_ << "\n";
    if (statut_ == StatutReservation::ANNULEE) std::cout << "  Annulée le    : " << dateAnnulation_ << "\n";
    std::cout << "  Passager      : " << passager_->getId() << " - " << passager_->nomComplet()
              << " (passeport " << passager_->getNumeroPasseport() << ")\n"
              << "  Vol           : " << vol_->getNumero() << " - " << vol_->getVilleDepart() << " -> "
              << vol_->getVilleArrivee() << ", le " << vol_->getDate() << " à " << vol_->getHeure()
              << "\n"
              << "  Prix du billet: " << Utils::formaterMontant(vol_->getPrix()) << "\n";
    if (paiement_ != nullptr) {
        paiement_->afficher();
    } else {
        std::cout << "  Paiement      : aucun\n";
    }
}

void Reservation::afficherEntete() {
    std::cout << "  " << Utils::cadrer("ID", 7) << Utils::cadrer("Passager", 24)
              << Utils::cadrer("Vol", 8) << Utils::cadrer("Trajet", 26) << Utils::cadrer("Statut", 12)
              << "Payée\n";
    std::cout << "  " << std::string(86, '-') << "\n";
}

void Reservation::afficherLigne() const {
    std::cout << "  " << Utils::cadrer(id_, 7) << Utils::cadrer(passager_->nomComplet(), 24)
              << Utils::cadrer(vol_->getNumero(), 8)
              << Utils::cadrer(vol_->getVilleDepart() + " -> " + vol_->getVilleArrivee(), 26)
              << Utils::cadrer(versTexte(statut_), 12) << (estPayee() ? "Oui" : "Non") << "\n";
}
