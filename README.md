# TinyVPS
  This project is a VPS which aims to consume few resources. This will be in tree parts: the vps script itself, an installation script and a security set-up script. At this moment i don't have the necessary time to continue with the C programming language so i'm switched to BASH. Moreover i would like to build a website to control Tinyvps with Python and Flask framework.

# features
  ## security
  * [unattended upgrades](https://wiki.debian.org/UnattendedUpgrades)
  * [disable root login, fail2ban, ufw](https://raspberrytips.com/security-tips-raspberry-pi/)
  * [configure ssh key based authentication](https://www.digitalocean.com/community/tutorials/how-to-configure-ssh-key-based-authentication-on-a-linux-server)

  ## virtualisation
  * create disk image with [fai api](https://fai-project.org/doc/api.html)
  * emulate the machine with [qemu kvm](https://www.qemu.org/docs/master/)
  * in c compile with `-luuid`. [System-Calls](https://www.gnu.org/software/libc/manual/html_node/System-Calls.html), [syscalls.2](https://man7.org/linux/man-pages/man2/syscalls.2.html), [multithreading](https://www.geeksforgeeks.org/multithreading-in-c/), [generating-a-random-uuid-in-c](https://stackoverflow.com/questions/51053568/generating-a-random-uuid-in-c).

  ## installation
  * install tools
  * [systemd creer des services](https://www.linuxtricks.fr/wiki/systemd-creer-des-services-timers-unites)

# memo
  ```
  qemu-system-x86_64 -drive file=server.raw,format=raw -m 2G -enable-kvm -smp 4 -device e1000,netdev=net0 -netdev user,id=net0,hostfwd=tcp::5555-:22
  ```
  -----
  ```
  fai-diskimage -v -S5G -u debian -cDEBIAN,DHCPC,DEMO,FAIBASE,CLOUD,BOOKWORM,SSH_SERVER,STANDARD,FAIME,GRUB_PC,AMD64 faime-2ZHWDLYP.raw.zst
  ```
  -----
  ```
  https://fai-project.org/cgi/faime.cgi?type=cloud;disksize=3;format=raw.zst;hostname=uuid;username=debian;userpw=debian;partition=ONE;keyboard=fr;suite=bookworm;cl5=SSH_SERVER;cl6=STANDARD;cl8=REBOOT;sbm=2
  ```