# Plan de Développement - Projet IA Ultimate Tic-Tac-Toe

Ce document présente la stratégie pour répondre aux exigences du projet de création d'une IA pour l'Ultimate Tic-Tac-Toe, conformément au cahier des charges.

## 1. Phase de Recherche et d'Analyse
- **Étude du jeu :** Compréhension approfondie des règles spécifiques (mouvement contraint par le coup précédent, victoire par majorité en cas d'égalité).
- **État de l'art IA :** Recherche sur les algorithmes adaptés au Morpion Ultimate (Minimax avec élagage Alpha-Beta, Monte Carlo Tree Search - MCTS).
- **Analyse du Code Existant :** Exploration du template fourni et de l'intégration avec la bibliothèque Allegro 5.

## 2. Conception Architecturale (POO)
Le projet sera structuré en classes modulaires pour respecter les principes de la Programmation Orientée Objet :
- **Classe `Cell` :** Représente une case individuelle (Vide, X, O).
- **Classe `SmallBoard` :** Gère une grille 3x3 (victoire locale, cases disponibles).
- **Classe `MainBoard` :** Gère la grille globale 9x9 (coordination des `SmallBoard`, règles de déplacement, victoire finale).
- **Classe `Player` (Interface) :** Classe abstraite pour définir le comportement d'un joueur (Humain ou IA).
- **Classe `AI` :** Implémentation du moteur de recherche et de l'heuristique.

## 3. Développement de l'Intelligence Artificielle
- **Étape 1 : Heuristique simple.** Évaluation basée sur le nombre de lignes/colonnes/diagonales presque complétées.
- **Étape 2 : Algorithme Minimax.** Recherche dans l'arbre des coups avec élagage **Alpha-Beta** pour augmenter la profondeur de réflexion.
- **Étape 3 : Optimisations.**
    - Utilisation de **Bitboards** pour des calculs ultra-rapides de l'état de la grille.
    - **Transposition Tables** pour éviter de recalculer des positions déjà vues.
    - Affinement de la fonction d'évaluation (priorité aux centres, coins, et contrôle stratégique des grilles envoyées).

## 4. Tests et Validation
- **Mode Debug :** Vérification de la validité des coups et de la stabilité de l'application.
- **Optimisation du Temps :** S'assurer que l'IA répond dans les limites indicatives (200ms à 500ms selon le niveau) pour éviter les timeouts en mode Arène.
- **Validation Arène :** Tests intensifs contre les IA fournies (`EASY` à `VERY_HARD`) avec pour objectif de dépasser **80% de victoires**.

## 5. Préparation de la Soutenance et Rendu
- **Commentaires et Documentation :** Nettoyage du code et ajout de commentaires explicatifs.
- **Bilan de Répartition :** Documentation de la répartition des tâches entre les membres de l'équipe.
- **Préparation Orale :** Synthèse des choix techniques (structures de données STL, optimisations) pour la présentation de 10 minutes.

---
**Date de création :** 5 mai 2026
**Auteur :** Gemini CLI (Assistant)
