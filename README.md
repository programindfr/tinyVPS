# Introduction

TinyVPS est un projet qui a pour vocation d'être une alternative économe en ressources face aux hyperviseurs d'entreprise. Le couple [QEMU/KVM](https://www.qemu.org/) étant très puissant et bien documenté ma permis de faire mûrir ma réflexion pour en arriver à cette troisième version du projet (`src.7z` et `model.7z` étant respectivement la première et la deuxième version). Dans cette version le but est de fournir un tutoriel pour rapidement mettre en place un VPS sur une machine dédiée.

Pour l'instant ce tutoriel est essentiellement écrit pour moi-même.

# Le système

J'utilise ici une machine dédiée (Home Lab) avec la distribution [Debian](https://www.debian.org/) sans interface graphique pour avoir un système stable et largement compatible.

Pour plus de confort j'ai assigné une adresse IP locale statique à ma machine via l'outil d'administration de ma box internet. Cette adresse sera représentée par `192.168.2.9`, une adresse fictive que vous remplacerez par cette de votre machine.

# Configuration de la machine

## SSH

Le protocole SSH est très confortable pour faire de l'administration de système à distance. En revanche il est impératif de le sécuriser pour éviter de vous faire pirater par n'importe qui.

### Configuration de l'authentification ssh par clé

L'utilisation d'un mot de passe comme méthode d'authentification n'est pas recommandé. Il est préférable d'utiliser une authentification par clé. Si vous n'avez pas de paire de clé privé/publique il est possible de la générer via la commande `ssh-keygen`.

Une fois que l'on possède une paire de clé il faut placer la clé publique sur la machine. Pour ce faire la commande suivante utilise l'authentification par mot de passe pour ajouté la clé publique `id_rsa.pub`.

```bash
ssh-copy-id -i .ssh/id_rsa.pub 192.168.2.9
```

[Source](https://www.digitalocean.com/community/tutorials/how-to-configure-ssh-key-based-authentication-on-a-linux-server)

### Configuration de sshd

A présent il faut paramétrer `sshd` pour interdire l'authentification par mot de passe. La commande suivante permet d'éditer le fichier de configuration avec Nano.

```bash
sudo nano /etc/ssh/sshd_config
```

On cherche la ligne `#PasswordAuthentication yes` et on la change en `PasswordAuthentication no` on enregistre puis on exécute la commande pour relancer le service.

```bash
sudo service ssh restart
```

[Source](https://raspberrytips.com/security-tips-raspberry-pi/)

## Mises à jour

Les mises à jour régulières sont un gage de sécurité.

Même si le système vient d'être installé il ne coûte rien de vérifier les mises à jours.

```bash
sudo apt update
sudo apt upgrade
```

### Unattended Upgrades

Unattended Upgrades va périodiquement et automatiquement installer les mises à jour des paquets. On édite ensuite le fichier de configuration.

```bash
sudo apt install unattended-upgrades
sudo nano /etc/apt/apt.conf.d/50unattended-upgrades
```

Décommentez ces deux lignes pour avoir l'ensemble des mises à jour.

```bash
"origin=Debian,codename=${distro_codename}-updates";
"origin=Debian,codename=${distro_codename}-proposed-updates";
```

Décommentez et changez la valeur de ces deux autres lignes pour un redémarrage automatique de la machine à 2h00 lorsque cela est nécessaire.

```bash
Unattended-Upgrade::Automatic-Reboot "true";
Unattended-Upgrade::Automatic-Reboot-Time "02:00";
```

On enregistre puis on redémarre le processus.

```bash
sudo systemctl restart unattended-upgrades.service
```

[Source](https://www.linuxcapable.com/how-to-configure-unattended-upgrades-on-ubuntu-linux/)

## Défense face à l'extérieur

Plus haut j'ai expliqué avoir configuré une adresse IP statique dans mon réseau local pour plus de confort. Il est aussi très pratique et nécessaire selon les cas d'autoriser les connections rentrante, c'est-à-dire d'autoriser le monde extérieur à se connecter à notre machine. Il faut donc se défendre face aux tentatives de piratage provenant d'internet.

### Fail2ban

Fail2ban est un utilitaire qui va bloquer les tentatives d'attaque par force brute.

```bash
sudo apt install fail2ban
```

Comme pour `sshd` il est possible d'éditer le fichier de configuration. Pour Fail2ban cela n'est pas vraiment obligé (sauf si vous avez une erreur comme moi, d'où l'ajout de `backend = systemd`) mais j'aime bien apporter ma petite touche personnel. C'est pourquoi je met un temps de bannissement d'une semaine au bout de 3 essais infructueux.

```bash
sudo nano /etc/fail2ban/jail.local
```

```bash
[DEFAULT]
bantime = 1w
maxretry = 3
backend = systemd
```

On relance ensuite le service après avoir enregistré les changement.

```bash
sudo service fail2ban restart 
```

[Source](https://raspberrytips.com/security-tips-raspberry-pi/) 

### Ufw

Ufw pour Uncomplicated FireWall  est un par feu qui permet de mettre des règles sur comment votre machine peut interagir avec d'autres réseaux tel qu'internet.

```bash
sudo apt install ufw
sudo ufw allow ssh
sudo ufw enable
```

[Source](https://raspberrytips.com/security-tips-raspberry-pi/)

## Des outils bien pratiques

Je vais ici parler de quelques outils que je trouve pratiques et/ou amusant.

### Monitoring

Ces utilitaires me permettent de surveiller l'état de ma machine. Je vous laisse le loisir de vous renseigner sur eux avant de les installer.

```bash
sudo apt install vnstat powertop htop tree lm-sensors
```

### DDclient

DDclient permet de mettre à jour votre DDNS.

```bash
sudo apt install ddclient
```

### Nginx

Nginx est un reverse proxy.

```bash
sudo apt install nginx
```

### Oh My Zsh

```bash
sudo apt install zsh git
sh -c "$(wget https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh -O -)"
```

### Les autres

```bash
sudo apt install vim rsync
```

### Motd

Il est possible de changer le message affiché lors de la connexion. Dans `/etc/update-motd.d` créer le fichier `10-welcome` et retirer le droit d'exécution aux autres. Enfin, vider le contenu de `/etc/motd`

```bash
sudo apt install lolcat
sudo nano /etc/update-motd.d/10-welcome
```

```bash
#!/bin/bash

uname -a | /usr/games/lolcat -f
uptime | /usr/games/lolcat -f
echo "Used memory: $(vmstat -s -S M | grep 'used memory' | awk '{print $1,$2}')" | /usr/games/lolcat -f
```

```bash
sudo chmod -x /etc/update-motd.d/*
sudo chmod +x /etc/update-motd.d/10-welcome
sudo bash -c 'printf "" > /etc/motd'
```

# TinyVPS

Création du dossier `.tinyvps`

```bash
mkdir ~/.tinyvps
```

On place `tinyvps.sh` dans ce dossier. Le fichier `tinyvps.sh` est disponible dans les [releases](https://github.com/programindfr/tinyVPS/releases).
Installation de QEMU/KVM sur Debian.

```bash
sudo apt install qemu-system-x86
```

## tinyvps.service

`/etc/systemd/system/`

```systemd
[Unit]
Description=TinyVPS
After=network-online.target
ConditionFileIsExecutable=/home/<user>/.tinyvps/tinyvps.sh

[Service]
Type=forking
ExecStart=/home/<user>/.tinyvps/tinyvps.sh
RemainAfterExit=yes
Restart=on-failure
RestartSec=10s

[Install]
WantedBy=multi-user.target
```

```bash
sudo systemctl daemon-reload
sudo systemctl enable tinyvps.service
sudo systemctl start tinyvps.service
```

[Source1](https://linuxhandbook.com/create-systemd-services/)

[Source2](https://www.man7.org/linux/man-pages/man5/systemd.unit.5.html)

[Source3](https://unix.stackexchange.com/questions/516749/how-best-to-start-my-systemd-service-to-run-multiple-apps)

## Et plus encore

Ici il est question d'éventuellement rajouter des options comme un site internet ou un portail SSH.

# Création d'une nouvelle machine

Dans cette partie je vais installer Debian sur image disque puis la configurer sur mon serveur.

## Création de l'image disque

Il faut tout d'abord télécharger [l'instalateur Debian](https://www.debian.org/download) puis créer une image disque en format RAW avec une taille fixé de gigaoctets. [Documentation](https://www.qemu.org/docs/master/system/images.html)

```bash
qemu-img create <machine_name.raw> <disk_size_G>
```

On procède ensuite à l'installation. [Documentation](https://www.qemu.org/docs/master/system/invocation.html)

```bash
qemu-system-x86_64 -boot order=cd -m 1G -cdrom <debian.iso> -drive file=<machine_name.raw>,format=raw -enable-kvm -rtc base=localtime
```

Compression de l'image.

```bash
zstd <machine_name.raw>
```

Envoi.

```bash
scp "/path/to/machine_name.raw.zst" username@domain:"/path/to/machine_name.raw.zst"
```

Décompréssion de l'image.

```bash
unzstd <machine_name.raw.zst>
```

## Configuration de la machine

A l'installation, un dossier `~/.tinyvps` est créé. On créer un nouveau dossier `machine_name` à l'intérieur dans lequel on place notre image disque `machine_name.raw`. La configuration de la machine se fait via un fichier `config` qui contient la commande QEMU/KVM.

```bash
echo '-smp <number of cores> -m <ram size M or G> -k fr -name <name> -drive file=</absolute/path/to/disk.raw>,format=raw -nographic -nic user,hostfwd=tcp::5555-:22 -serial none -monitor none -enable-kvm -rtc base=localtime' > config
```

La commande précédente est à configurer selon les besoin. Ci-dessous une brève explication des paramètres. La [documentation](https://www.qemu.org/docs/master/system/invocation.html) officielle fournie une description plus avancée.

```bash
-smp <number of cores>             # core allocation
-boot order=cd                     # disk,cdrom
-m <ram size M or G>               # RAM allocation
-k fr                              # keyboard layout
-name <name>                       # process name
-cdrom <file>                      # iso image
-drive file=<disk.raw>,format=raw  # disk image
-nographic                         # disable graphical output
-nic user,hostfwd=tcp::5555-:22    # network
-serial none                       # no serial output
-monitor none                      # no monitor output
-pidfile <file>                    # process PID
-enable-kvm                        # KVM full virtualization
-rtc base=localtime                # RTC clock
```
