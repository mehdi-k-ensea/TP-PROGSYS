# TP-PROGSYS
Question 1 – Message d’accueil et prompt
<img width="1379" height="145" alt="image" src="https://github.com/user-attachments/assets/2fab87d0-a7e0-4844-ba39-8df556c986c6" />
Dans un premier temps, le programme enseash-Q1 se contente d’afficher un message d’accueil puis un prompt simple de la forme
enseash % .
Cette étape nous a permis de mettre en place la structure de base du shell : initialisation du programme, affichage avec write() et préparation de la boucle de lecture qui sera utilisée dans les questions suivantes.

Question 2 – Boucle REPL et exécution d’une commande simple

Pour la question 2, nous avons transformé ce shell statique en une véritable boucle REPL (read–eval–print loop) :

2a – Lecture de la commande : le shell lit une ligne tapée au clavier (par exemple ls) à l’aide de read(), puis nettoie la chaîne (suppression du \n final).
<img width="1376" height="167" alt="image" src="https://github.com/user-attachments/assets/e5c12026-5f61-4d82-8a33-ad43a1cdf3ee" />

2b – Exécution d’une commande simple : après la lecture, le shell crée un processus fils avec fork().
Dans le fils, nous appelons execlp() / execvp() avec le nom de la commande (sans arguments à ce stade).
Dans le père, nous attendons la fin du fils avec wait() pour récupérer la main.

<img width="1379" height="145" alt="image" src="https://github.com/user-attachments/assets/6a0d0dd0-a91d-4831-8ee4-bca8fdaf3d16" />


2c – Retour au prompt : une fois la commande terminée, le shell ré-affiche le prompt enseash % et attend la commande suivante.
On peut ainsi lancer successivement des programmes comme ls, ./test_exit ou ./test_signal, comme montré dans les captures.
<img width="1376" height="167" alt="image" src="https://github.com/user-attachments/assets/af9904f2-c502-4ac8-bae1-ff5ca0b95756" />


Cette partie valide le fonctionnement de base du shell : enchaîner des commandes externes tout en restant dans notre programme enseash.

Question 3 – Sortie propre du shell (exit ou Ctrl+D)
<img width="1376" height="215" alt="image" src="https://github.com/user-attachments/assets/81262bba-5155-4f7e-a5c1-e1d7fd3a0ddb" />


À la question 3, nous avons ajouté la gestion de la sortie du shell :

Si l’utilisateur tape la commande exit, le shell ne lance aucun processus fils : il quitte simplement la boucle principale, affiche un message d’au revoir (par exemple Ciao, Hasta Luego) puis termine, rendant la main au shell système.

Si l’utilisateur envoie un Ctrl+D, la lecture standard retourne une fin de fichier (EOF). Nous testons ce cas (valeur de retour de read() égale à 0) pour sortir proprement de la boucle, avec le même message de fin.

Grâce à ces ajouts, notre mini-shell se comporte déjà comme un shell classique minimal : il accueille l’utilisateur, exécute des commandes simples en boucle, puis se ferme proprement sur exit ou Ctrl+D.

Question 4 -
<img width="1327" height="294" alt="image" src="https://github.com/user-attachments/assets/b11c51d3-430d-4958-8525-0119dd3bd46b" />
