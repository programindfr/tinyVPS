# tinyVPS
 This project is a VPS which aims to consume few resources.
 
 ## memo
  `qemu-system-x86_64 -drive file=server.raw,format=raw -m 2G -enable-kvm -smp 4 -device e1000,netdev=net0 -netdev user,id=net0,hostfwd=tcp::5555-:22`
  `fai-diskimage -v -S5G -u debian -cDEBIAN,DHCPC,DEMO,FAIBASE,CLOUD,BOOKWORM,SSH_SERVER,STANDARD,FAIME,GRUB_PC,AMD64 faime-2ZHWDLYP.raw.zst`

# roadmap
 ## security
  * security auto update
  * disable root login (change port ?)
  * ssh keys setup
  * fail2ban
  * ufw
  * links:
    [security-tips](https://raspberrytips.com/security-tips-raspberry-pi/)
    [configure-ssh-key-based-authentication](https://www.digitalocean.com/community/tutorials/how-to-configure-ssh-key-based-authentication-on-a-linux-server)
 ## virtualisation
  * debian iso auto install (may some other to ?)
    link: [ai-diskimage](https://fai-project.org/doc/man/fai-diskimage.html)
  * qemu kvm settings
    link: [qemu-doc](https://www.qemu.org/docs/master/)
  * main c executable.
    Make sure to compile with `-luuid`.
    links: [System-Calls](https://www.gnu.org/software/libc/manual/html_node/System-Calls.html)
    [syscalls.2](https://man7.org/linux/man-pages/man2/syscalls.2.html)
    [multithreading](https://www.geeksforgeeks.org/multithreading-in-c/)
    [generating-a-random-uuid-in-c](https://stackoverflow.com/questions/51053568/generating-a-random-uuid-in-c)
  * service file
    link: [systemd-creer-des-services](https://www.linuxtricks.fr/wiki/systemd-creer-des-services-timers-unites)
 ## access
  * installation script.
    fai-mk-configspace
  * python flask website
