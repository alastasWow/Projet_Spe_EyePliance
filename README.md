# Projet_Sp-_EyePliance
0-  Lire le README.md dans ./bd

1-  Se placer dans projetSpe

2-  mkdir build

3-  cd build

4-  cmake ../

5-  make

6-  ./bin/track <Entrée caméra*> <Type de traitement**> <Bool Reco des visages***> <System****>

*:Pour connaitre le numéro des caméras connectées à votre machine faites : ls /dev/video* ceci listera toutes les caméras vous choisissez par la suite
laquelle vous voulez.

**:Il y a deux traitement possible soit : "webcam" soit "IR".

***:Si vous voulez activer la reconnaissance des visages mettez 1 sinon 0.

****:Il exsite dans le dossier commands deux types de fichier un pour les systèmes Linux et l'autre pour OSX, cet argument prend soit "Linux" ou bien "OSX".
