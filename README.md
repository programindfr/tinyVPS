# tinyVPS
 This project is a VPS which aims to consume few resources.


# roadmap
 ## security
  * security auto update
  * disable root login (change port ?)
  * ssh keys setup
  * fail2ban
  * ufw
  * links:
    https://raspberrytips.com/security-tips-raspberry-pi/
    https://www.digitalocean.com/community/tutorials/how-to-configure-ssh-key-based-authentication-on-a-linux-server
 ## virtualisation
  * debian iso auto install (may some other to ?)
    link: https://fai-project.org/doc/man/fai-diskimage.html
  * qemu kvm settings
    link: https://www.qemu.org/docs/master/
  * main c executable.
    Make sure to compile with `-luuid`.
    links: https://www.gnu.org/software/libc/manual/html_node/System-Calls.html
    https://man7.org/linux/man-pages/man2/syscalls.2.html
    https://www.geeksforgeeks.org/multithreading-in-c/
    https://stackoverflow.com/questions/51053568/generating-a-random-uuid-in-c
  * service file
    link: https://www.linuxtricks.fr/wiki/systemd-creer-des-services-timers-unites
 ## access
  * installation script.
    fai-mk-configspace
  * python flask website