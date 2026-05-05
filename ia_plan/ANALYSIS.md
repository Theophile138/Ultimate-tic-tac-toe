# Analyse Stratégique - Ultimate Tic-Tac-Toe Bot

Ce document détaille le fonctionnement interne, les algorithmes et les heuristiques utilisés par l'IA du projet `ultimate-tic-tac-toe.py`.

## 1. Architecture Générale
Le bot maintient une vision duale de l'état du jeu pour optimiser ses prises de décision :
*   **Macro-Board (3x3)** : Représente les 9 sous-grilles. Indique qui a remporté chaque secteur.
*   **Board Complet (9x9)** : Représente l'intégralité des 81 cases du jeu.

### Boucle de décision
1. **Mise à jour** des grilles avec le dernier coup adverse.
2. **Identification** de la zone de jeu valide (sous-grille imposée ou libre).
3. **Évaluation stratégique** pour choisir la meilleure action.
4. **Mise à jour** interne avant l'envoi de la commande.

## 2. Intelligence Artificielle : Minimax
Le bot utilise un algorithme **Minimax avec élagage Alpha-Beta** pour explorer l'arbre des possibilités. L'algorithme est appliqué sur des grilles de 3x3 (soit pour gagner une sous-grille, soit pour choisir la meilleure sous-grille globale).

### Heuristiques et Scores
Le bot évalue les feuilles de l'arbre de recherche avec les coefficients suivants :

| État du Jeu | Score de l'Heuristique | Description |
| :--- | :--- | :--- |
| **Victoire Bot (X)** | `10 - depth` | Favorise les victoires les plus rapides. |
| **Défaite Bot (O)** | `depth - 10` | Cherche à retarder la défaite le plus longtemps possible. |
| **Match Nul** | `0` | État neutre. |

*Note : La variable `depth` permet de pondérer l'urgence d'un coup.*

## 3. Stratégies Codées (Heuristiques Pré-calculées)
Pour optimiser les performances et la solidité en début de partie (Early Game), certaines actions sont prioritaires avant le lancement du Minimax :

| Condition | Action Prioritaire | Coefficient / Cible |
| :--- | :--- | :--- |
| **Ouverture (9 cases vides)** | Jouer dans un angle | `[0,0], [0,2], [2,0], [2,2]` (Aléatoire) |
| **Contrôle (8 cases vides)** | Prendre le centre | `[1,1]` (Si disponible) |
| **Milieu/Fin (< 8 cases)** | Analyse Minimax | Calcul complet via Alpha-Beta |

## 4. Axes d'Amélioration Identifiés
1. **Pondération des zones** : Le centre du Macro-Board devrait avoir un coefficient de valeur plus élevé que les bords.
2. **Évaluation Intermédiaire** : Actuellement, le bot ne "comprend" pas l'avantage avant la fin de la partie (ex: avoir 2 cases alignées).
3. **Anticipation du Macro-Board** : Intégrer une pénalité si un coup envoie l'adversaire dans une sous-grille qu'il est sur le point de gagner.
