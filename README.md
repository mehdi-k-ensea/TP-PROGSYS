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

Question 4 -  Affichage du code de retour (ou du signal) de la commande précédente dans le prompt
<img width="781" height="266" alt="Screenshot from 2025-12-18 08-22-23" src="https://github.com/user-attachments/assets/bfbb0f53-ffcd-4469-b4e6-09fecc7da9c3" />

On créer 2 fichiers spplémentaires, un test_signal.c (qui tourne en boucle ce qui nous permettra de l'interrompre de deux manières differentes) et test_exit.c ( qui exit 3 parce que le fichier test exit retourne 3). 

Lorsque le processus se termine normalement, le code de retour est extrait à l’aide de la macro WIFEXITED, puis WEXITSTATUS. En revanche, si le processus est interrompu par un signal, cette situation est détectée avec WIFSIGNALED, et le numéro du signal est obtenu via WTERMSIG. Ces informations sont ensuite intégrées dynamiquement au prompt sous la forme [exit:x] ou [sign:y]. Notre code va renvoyer  [sign:15] lorsqu'on rentrera la commande kill et retournera  [sign:9] lorsqu'on rentrera la commmande kill -9 PID. Le renvoi de [exit:0] se fait de base lorsque le code compile sans erreur. 

Le choix du chiffre x qui est retourné lors d'une commande non reconnue '[exit:x]' est arbitraire et se définie à l'aide du bout de code suivant : 
<img width="870" height="186" alt="Capture d&#39;écran 2025-12-18 083350" src="https://github.com/user-attachments/assets/0ef88bfc-febc-4e1f-a581-7091a1574b9e" />

Question 5 - Mesure du temps d’exécution de la commande en utilisant l’appel clock_gettime
on se refere a la page de presentation de clock_gettime, on a la structure suivantee :
struct timespec {
               time_t   tv_sec;        /* seconds */
               long     tv_nsec;       /* nanoseconds */
           };

Il faut faire attention à la différence entre les différentes unités de temps (seconde et nanoseconde). Dans cette structure, long fait référence aux long int donc on y fera référence avec %ld.
<img width="1270" height="355" alt="erreurQ5SYS" src="https://github.com/user-attachments/assets/96ebc7e7-f1a2-4053-9eb1-50210547cc8b" />

<img width="581" height="210" alt="Q5sys" src="https://github.com/user-attachments/assets/f03fb0b2-bade-4661-a4cb-d8935cc1a829" />

La commande nano ouvre un fichier, la temps affiché dépend donc de quand nous décidons de fermer le fichier. Idem pour sign:15, tant qu'on ne génère pas le signal qui kill l'exécution, le timer tourne toujours. 
Cette durée est affichée dans le prompt conjointement avec le code de retour ou le signal, sous la forme [exit:x|y ms].

Question 6 -  Exécution d’une commande complexe
Afin de permettre l’exécution de commandes comportant des arguments, la fonction execlp, est remplacée par execvp. Cette fonction permet d’exécuter des commandes de la forme ls -a -b, où -a et -b sont des options passées en arguments.

La ligne de commande saisie par l’utilisateur est d’abord récupérée sous forme de chaîne de caractères, puis découpée en différents tokens correspondant au nom de la commande et à ses arguments. Ces éléments sont ensuite stockés dans un tableau de pointeurs sur caractères (char *argv[]), conforme au format attendu par execvp et terminé par un pointeur NULL.

Le tableau ainsi construit est transmis à la fonction execvp, qui exécute la commande en recherchant le binaire correspondant dans les répertoires définis par la variable d’environnement PATH.

<img width="917" height="282" alt="Q6SYS" src="https://github.com/user-attachments/assets/de57ae04-8a6c-4506-ba53-a7d66650dcbf" />

Concrètement, le programme analyse la ligne de commande caractère par caractère afin d’identifier les espaces. Chaque sous-chaîne comprise entre deux espaces correspond à un argument de la commande.
Ces sous-chaînes sont alors stockées successivement dans un tableau de pointeurs sur caractères, qui constitue le vecteur d’arguments passé à execvp. 


Queston 7 - Gestion des redirections vers stdin et stdout avec ‘<’et‘>’



<img width="1205" height="670" alt="Q7SYS" src="https://github.com/user-attachments/assets/969db72e-f5ec-4d24-b67d-f172ca204d2d" />

enseash % ls > contenue.txt
Cette commande exécute le programme ls, qui affiche normalement la liste des fichiers sur la sortie standard (stdout), c’est-à-dire le terminal.
Grâce à l’opérateur >, la sortie standard est redirigée vers le fichier contenue.txt.

enseash % cat contenue.txt
La commande cat affiche sur la sortie standard le contenu du fichier passé en argument.
Ici, cat contenue.txt lit le fichier contenue.txt et affiche son contenu dans le terminal.

enseash % wc -l < contenue.txt
La commande wc -l compte le nombre de lignes reçues sur son entrée standard (stdin).
L’opérateur < permet de rediriger l’entrée standard depuis le fichier contenue.txt au lieu du clavier.

Question 8 - Gestion de la redirection de type pipe avec ‘|’

Question 9 - 

