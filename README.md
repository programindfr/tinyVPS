# Introduction
TinyVPS est un projet qui a pour vocation d'être une alternative économe en ressources face aux hyperviseurs d'entreprise. Le couple [QEMU/KVM](https://www.qemu.org/) étant trés puissant et bien documenté ma permis de faire murir ma réflexion pour en arriver à cette troisième version du projet (`src.7z` et `model.7z` étant respéctivement la première et la deuxième version). Dans cette version le but est de fournir un tutoriel pour rapidement mettre en place un VPS. Pour l'instant ce tutoriel est essentiellement écrit pour moi-même.

# Le système d'exploitation
J'utilise ici la distribution [Debian](https://www.debian.org/) sans interface graphique pour avoir un système léger, stable et largement compatible.

# Configuration
## Sécurité
*à venir*
* [unattended upgrades](https://wiki.debian.org/UnattendedUpgrades)
* [disable root login, fail2ban, ufw](https://raspberrytips.com/security-tips-raspberry-pi/)
* [configure ssh key based authentication](https://www.digitalocean.com/community/tutorials/how-to-configure-ssh-key-based-authentication-on-a-linux-server)

## Les outils
Installation de [QEMU/KVM](https://www.qemu.org/) sur [Debian](https://www.debian.org/).
```sh
sudo apt install qemu qemu-system-x86
```
* [systemd creer des services](https://www.linuxtricks.fr/wiki/systemd-creer-des-services-timers-unites)

## Et plus encore
Ici il est question d'éventuellement rajouter des options comme un site internet ou un portail SSH.

# Création d'une nouvelle machine
Dans cette partie je vais installer [Debian](https://www.debian.org/) sur image disque puis la configurer sur mon serveur.
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
A l'installation, un dossier `~/tinyvps` est créé. On créer un nouveau dossier `machine_name` à l'intérieur dans lequel on place notre image disque `machine_name.raw`. La configuration de la machine se fait via un fichier bash `run.sh` qui contient la commande [QEMU/KVM](https://www.qemu.org/).
```sh
echo #!/bin/bash >> run.sh
chmod +x run.sh
```
La commande suivante est à configurer selon les besoin et à mettre à la suite de `run.sh`.
```sh
qemu-system-x86_64 \
	-smp <number of cores> \			# core allocation
	-boot order=cd \					# disk,cdrom
	-m <ram size M or G> \				# RAM allocation
	-k fr \								# keyboard layout
	-name <name> \						# process name
	-cdrom <file> \						# iso image
	-drive file=<disk.raw>,format=raw \	# disk image
	-nographic \						# disable graphical output
	-nic tap \							# TAP network
	-serial none \						# no serial output
	-monitor none \						# no monitor output
	-pidfile <file> \					# process PID
	-enable-kvm \						# KVM full virtualization
	-rtc base=localtime \				# RTC clock
```
