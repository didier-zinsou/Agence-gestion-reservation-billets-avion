# Conception - Agence de voyage de Cotonou

Ce document répond à la partie I de l'exercice (analyse puis diagramme de classes).
Le diagramme PlantUML se trouve dans [`diagramme_classes.puml`](diagramme_classes.puml).

## 1. Classes identifiées

| Classe | Rôle |
|---|---|
| `Passager` | Un client de l'agence. Doit être enregistré avant de réserver. |
| `Vol` | Un vol proposé (trajet, date, capacité, prix) avec le suivi de ses places. |
| `Reservation` | Lie un passager à un vol ; porte le statut et le paiement éventuel. |
| `Paiement` | Le règlement d'une réservation, avec son statut de remboursement. |
| `Agence` | Classe de gestion : stocke les objets dans des `vector` et applique les règles. |
| `Application` | Interface console (menu et dialogue avec l'utilisateur). |
| `Utils` (namespace) | Saisies sécurisées et mise en forme. |
| `StatutReservation`, `ModePaiement`, `StatutRemboursement`, `CritereRecherche` | Énumérations. |

## 2. Attributs et méthodes

Voir le diagramme pour la liste complète. Points importants :

- **`Vol`** suit ses places avec deux compteurs : `placesProvisoires` (réservation en attente de paiement) et `placesConfirmees` (réservation validée). Leur somme ne dépasse jamais `capacite`.
- **`Reservation`** porte la logique de cycle de vie : `peutEtrePayee()`, `payer()`, `annuler()`.
- **`Paiement`** connaît son statut de remboursement (`AUCUN` ou `REMBOURSE`) et le montant remboursé.
- **`Agence`** contient trois `vector<...*>` (passagers, vols, réservations) et les opérations de recherche/création.

## 3. Associations

| Association | Type | Justification |
|---|---|---|
| `Application` - `Agence` | composition | L'application crée et détruit son agence. |
| `Agence` - `Passager` | agrégation | L'agence stocke et libère les passagers. |
| `Agence` - `Vol` | agrégation | L'agence stocke et libère les vols. |
| `Agence` - `Reservation` | composition | Les réservations n'existent que dans l'agence. |
| `Reservation` - `Passager` | association (flèche) | La réservation référence son passager sans le posséder. |
| `Reservation` - `Vol` | association (flèche) | La réservation référence son vol sans le posséder. |
| `Reservation` - `Paiement` | composition | Le paiement est créé et détruit par la réservation. |

## 4. Multiplicités

| Association | Multiplicités | Lecture |
|---|---|---|
| Agence - Passager | `1` - `0..*` | Une agence enregistre zéro ou plusieurs passagers. |
| Agence - Vol | `1` - `0..*` | Une agence propose zéro ou plusieurs vols. |
| Agence - Reservation | `1` - `0..*` | Une agence gère zéro ou plusieurs réservations. |
| Reservation - Passager | `0..*` - `1` | Une réservation concerne **un seul** passager (règle 4) ; un passager peut avoir plusieurs réservations. |
| Reservation - Vol | `0..*` - `1` | Une réservation porte sur **un seul** vol (règle 4) ; un vol reçoit plusieurs réservations, dans la limite de sa capacité (règle 2). |
| Reservation - Paiement | `1` - `0..1` | Une réservation a au plus un paiement (aucun tant qu'elle n'est pas payée). |

## 5. Où chaque règle est appliquée

| Règle | Application dans le code |
|---|---|
| 1 - Passager enregistré | `Agence::reserver` refuse un identifiant de passager inconnu. |
| 2 - Capacité du vol | `Vol::reserverProvisoirement` refuse quand `placesDisponibles() == 0`. |
| 3 - Pas de double réservation | `Agence::reserver` refuse si le passager a déjà une réservation **non annulée** sur ce vol. |
| 4 - 1 passager et 1 vol | `Reservation` a deux pointeurs obligatoires, fixés à la construction. |
| 5 - Annulée = non payable | `Reservation::peutEtrePayee` / `payer` refusent le statut `ANNULEE`. |
| 6 - Place définitive après validation | `payer()` passe la réservation à `VALIDEE` et appelle `Vol::confirmerPlace`. |
| 7 - Payée puis annulée | `annuler()` appelle `Paiement::rembourser` : 80 % remboursés, statut `REMBOURSE`. |

## 6. Choix de conception (hypothèses)

- **Validation = paiement complet.** Le menu ne propose pas d'option « valider » : une réservation passe de *En attente* à *Validée* quand son paiement est enregistré.
- **Place retenue dès la réservation.** Une réservation en attente retient une place provisoire (elle compte dans la capacité, donc pas de surréservation) ; la place devient définitive à la validation.
- **Annulation libère la place**, qu'elle soit provisoire ou confirmée, et permet à ce passager de réserver à nouveau le même vol.
- **Politique de remboursement.** Une réservation payée puis annulée est remboursée à 80 % (20 % de frais d'annulation). Le taux est la constante `Reservation::TAUX_REMBOURSEMENT`.
- **Paiement exact.** Le montant versé doit être égal au prix du billet.
- **Données en mémoire.** Pas de persistance : les données sont perdues à la fermeture (voir les pistes d'amélioration du README).
