# Introduction

TinyVPS est un projet qui a pour vocation d'être une alternative économe en ressources face aux hyperviseurs d'entreprise. Le couple [QEMU/KVM](https://www.qemu.org/) étant très puissant et bien documenté ma permis de faire mûrir ma réflexion pour en arriver à cette troisième version du projet (`src.7z` et `model.7z` étant respectivement la première et la deuxième version). Dans cette version le but est de fournir un tutoriel pour rapidement mettre en place un VPS sur une machine dédiée.

Pour l'instant ce tutoriel est essentiellement écrit pour moi-même.

# Le système

J'utilise ici une machine dédiée (Home Lab) avec la distribution [Debian](https://www.debian.org/) sans interface graphique pour avoir un système stable et largement compatible.

Pour plus de confort j'ai assigné une adresse IP locale statique à ma machine via l'outil d'administration de ma box internet. Cette adresse sera représentée par `192.168.2.9`, une adresse fictive que vous remplacerez par cette de votre machine.

# Configuration

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

*à venir*

## Défense face à l'extérieur

Plus haut j'ai expliqué avoir configuré une adresse IP statique dans mon réseau local pour plus de confort. Il est aussi très pratique et nécessaire selon les cas d'autoriser les connections rentrante, c'est-à-dire d'autoriser le monde extérieur à se connecter à notre machine. Il faut donc se défendre face aux tentatives de piratage provenant d'internet.

### Fail2ban

Fail2ban est un utilitaire qui va bloquer les tentatives d'attaque par force brute.

```bash
sudo apt install fail2ban
```

Comme pour `sshd` il est possible d'éditer le fichier de configuration. Pour Fail2ban cela n'est pas vraiment obligé mais j'aime bien apporter ma petite touche personnel. C'est pourquoi je met un temps de bannissement d'une semaine au bout de 3 essais infructueux.

```bash
sudo nano /etc/fail2ban/jail.local
```

```bash
[DEFAULT]
bantime = 1w
maxretry = 3
```

On relance ensuite le service après avoir enregistré les changement.

```bash
sudo service fail2ban restart 
```



* [unattended upgrades](https://wiki.debian.org/UnattendedUpgrades)

* [disable root login, fail2ban, ufw](https://raspberrytips.com/security-tips-raspberry-pi/)
  vnstat, powertop, htop, neofetch, lolcat, vim, ddclient, nginx, tree, lm-sensors, rsync, zsh & ohmyzsh, (? bridge-utils)
  Dans `/etc/update-motd.d` créer le fichier `10-welcome` et retirer le droit d'execution aux autres

```sh
#!/bin/bash

uname -a | /usr/games/lolcat -f
uptime | /usr/games/lolcat -f
echo "Used memory: $(vmstat -s -S M | grep 'used memory' | awk '{print $1,$2}')" | /usr/games/lolcat -f
```

## Les outils

Création du dossier `tinyvps`

```sh
cd
mkdir tinyvps
```

On place `tinyvps.sh` dans ce dossier. Le fichier `tinyvps.sh` est disponible dans les releases.
Installation de QEMU/KVM sur Debian.

```sh
sudo apt install qemu qemu-system-x86 bridge-utils
```

Il faut créer un bridge.

```sh
sudo brctl addbr br0
```

On récupère le nom de l'interface ethernet avec la commande `ip` puis on l'ajoute au bridge.

```sh
sudo brctl addif br0 <interface>
```

* [systemd creer des services](https://www.linuxtricks.fr/wiki/systemd-creer-des-services-timers-unites)

## Et plus encore

Ici il est question d'éventuellement rajouter des options comme un site internet ou un portail SSH.

# Création d'une nouvelle machine

Dans cette partie je vais installer Debian sur image disque puis la configurer sur mon serveur.

## Création de l'image disque

Il faut tout d'abord télécharger [l'instalateur Debian](https://www.debian.org/download) puis créer une image disque en format RAW avec une taille fixé de gigaoctets. [Documentation](https://www.qemu.org/docs/master/system/images.html)

```sh
qemu-img create <machine_name.raw> <disk_size_G>
```

On procède ensuite à l'installation. [Documentation](https://www.qemu.org/docs/master/system/invocation.html)

```sh
qemu-system-x86_64 -boot order=cd -m 1G -cdrom <debian.iso> -drive file=<machine_name.raw>,format=raw -enable-kvm -rtc base=localtime
```

Compression de l'image.

```sh
zstd <machine_name.raw>
```

Envoi.

```sh
scp "/path/to/machine_name.raw.zst" username@domain:"/path/to/machine_name.raw.zst"
```

Décompréssion de l'image.

```sh
unzstd <machine_name.raw.zst>
```

## Configuration de la machine

A l'installation, un dossier `~/tinyvps` est créé. On créer un nouveau dossier `machine_name` à l'intérieur dans lequel on place notre image disque `machine_name.raw`. La configuration de la machine se fait via un fichier bash `run.sh` qui contient la commande QEMU/KVM.

```sh
echo #!/bin/bash >> run.sh
chmod +x run.sh
```

La commande suivante est à configurer selon les besoin et à mettre à la suite de `run.sh`.

```sh
qemu-system-x86_64 \
    -smp <number of cores> \            # core allocation
    -boot order=cd \                    # disk,cdrom
    -m <ram size M or G> \                # RAM allocation
    -k fr \                                # keyboard layout
    -name <name> \                        # process name
    -cdrom <file> \                        # iso image
    -drive file=<disk.raw>,format=raw \    # disk image
    -nographic \                        # disable graphical output
    -nic tap \                            # TAP network
    -serial none \                        # no serial output
    -monitor none \                        # no monitor output
    -pidfile <file> \                    # process PID
    -enable-kvm \                        # KVM full virtualization
    -rtc base=localtime \                # RTC clock
```
