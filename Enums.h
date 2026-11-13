#pragma once
#include <string>

// Statut d'une réservation.
//  EN_ATTENTE : la place est retenue provisoirement, en attente de paiement.
//  VALIDEE    : la réservation est payée, la place est définitive (règle 6).
//  ANNULEE    : la réservation ne peut plus être utilisée (règle 5).
enum class StatutReservation { EN_ATTENTE, VALIDEE, ANNULEE };

enum class ModePaiement { ESPECES, MOBILE_MONEY, CARTE_BANCAIRE };

// Statut de remboursement d'un paiement (règle 7).
enum class StatutRemboursement { AUCUN, REMBOURSE };

// Critères disponibles pour la recherche d'un vol.
enum class CritereRecherche { NUMERO, DEPART, ARRIVEE, DATE };

inline std::string versTexte(StatutReservation s) {
    switch (s) {
        case StatutReservation::EN_ATTENTE: return "En attente";
        case StatutReservation::VALIDEE:    return "Validée";
        case StatutReservation::ANNULEE:    return "Annulée";
    }
    return "Inconnu";
}

inline std::string versTexte(ModePaiement m) {
    switch (m) {
        case ModePaiement::ESPECES:        return "Espèces";
        case ModePaiement::MOBILE_MONEY:   return "Mobile Money";
        case ModePaiement::CARTE_BANCAIRE: return "Carte bancaire";
    }
    return "Inconnu";
}

inline std::string versTexte(StatutRemboursement s) {
    switch (s) {
        case StatutRemboursement::AUCUN:     return "Aucun";
        case StatutRemboursement::REMBOURSE: return "Remboursé";
    }
    return "Inconnu";
}
