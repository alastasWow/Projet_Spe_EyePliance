# Projet_Sp-_EyePliance
0-  Décompresser Projet_Sp-_EyePliance-master.zip

1-  Décompresser projet_spe_EyePliance.tar.gz

2-  Lire le README.md dans ./bd

3-  Se placer dans projetSpe

4-  mkdir build

5-  cd build

6-  cmake ../

7-  make

8-  ./bin/track \<Entrée caméra\*\> \<Type de traitement\*\*\> \<Bool Reco des visages\*\*\*\> \<System\*\*\*\*\>

\*: Correspond au numéro de votre caméra, pour connaitre le numéro des caméras connectées à votre machine il existe la commande: "ls /dev/video\*" ceci listera toutes les caméras connectées.

\*\* :Il y a deux traitement possible soit : "webcam" soit "IR".

\*\*\* :Si vous voulez activer la reconnaissance des visages mettez 1 sinon 0.

\*\*\*\* :Il exsite dans le dossier commands deux types de fichier un pour les systèmes Linux et l'autre pour OSX, cet argument prend soit "Linux" ou bien "OSX".
