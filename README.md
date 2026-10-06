# PacmanUE5

Un clone de Pac-Man développé sous Unreal Engine 5 en C++ et Blueprints, réalisé dans le cadre du TP2 sur l'Intelligence Artificielle. Ce projet met l'accent sur le comportement autonome des fantômes via l'utilisation de *Behavior Trees* et intègre une navigation sur mesure par système de nœuds (Waypoints) adaptée à la géométrie de la carte.

## 🕹️ Fonctionnalités Principales

* **Déplacement par Graphe (Waypoints) :** Pac-Man et les entités naviguent sur un réseau de nœuds interconnectés (Haut, Bas, Gauche, Droite). Ce système garantit des virages stricts et des demi-tours instantanés fidèles à la jouabilité arcade.
* **Comportement de l'IA (Behavior Trees) :** Les quatre fantômes possèdent leurs propres personnalités et naviguent dynamiquement entre trois états principaux :
  * **Chasse / Patrouille :** Traque active de Pac-Man.
  * **Fuite :** État de vulnérabilité de 10 secondes déclenché par l'ingestion d'une super pac-gomme.
  * **Retour à la base :** Retour automatique au centre du labyrinthe lors de la destruction d'un fantôme.
* **Tâches Personnalisées :** Intégration de nœuds de comportement développés sur mesure (ex: `UBTTaskNode_ChasePlayer`, `UBTTaskNode_FindAmbushPosition`).
* **Mécaniques d'Arcade :** Téléportation instantanée aux extrémités latérales du labyrinthe et contrôle fluide aux joysticks (Manette Xbox) et au clavier.

## 🛠️ Architecture Technique

* **Moteur :** Unreal Engine 5.8
* **Langages :** C++ & Blueprints
* **Contrôleur IA :** Utilisation de l'AI Controller et du Blackboard pour stocker les variables d'état des fantômes.
* **Système d'Input :** Utilisation des *Axis Mappings* pour lire les intentions directionnelles en continu et appliquer un système de *buffer* (mémoire tampon) lors des croisements.

## 🚀 Installation et Compilation

1. Cloner le dépôt sur votre machine :
   ```bash
   git clone https://github.com/jbouillasse/PacmanUE5.git
   ```
2. Effectuer un clic droit sur le fichier PacmanUE5.uproject et sélectionner Generate Visual Studio project files (ou ouvrir directement avec JetBrains Rider).

3. Ouvrir la solution générée dans votre IDE et compiler le projet avec la configuration Development Editor - Win64.

4. Ouvrir le projet dans l'éditeur Unreal Engine et lancer la simulation (Play) sur la carte principale.
