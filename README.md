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
<img width="781" height="266" alt="Screenshot from 2025-12-18 08-22-23" src="https://github.com/user-attachments/assets/bfbb0f53-ffcd-4469-b4e6-09fecc7da9c3" />

On créer 2 fichiers spplémentaires, un test_signal.c (tourne en boucle pour l'interrompre de deux manoeres differentes) et test_exit.c (exit 3 parce que le fichier test exit retourne 3)

Lorsque le processus se termine normalement, le code de retour est extrait à l’aide de la macro WIFEXITED, puis WEXITSTATUS. En revanche, si le processus est interrompu par un signal, cette situation est détectée avec WIFSIGNALED, et le numéro du signal est obtenu via WTERMSIG. Ces informations sont ensuite intégrées dynamiquement au prompt sous la forme [exit:x] ou [sign:y].

Le choix du chiffre qui est retourné lors d'une commande non reconnue est arbitraire et se définie à l'aide du bout de code suivant : 
<img width="870" height="186" alt="Capture d&#39;écran 2025-12-18 083350" src="https://github.com/user-attachments/assets/0ef88bfc-febc-4e1f-a581-7091a1574b9e" />

