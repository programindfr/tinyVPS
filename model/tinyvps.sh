#!/bin/bash

_baseurl="https://fai-project.org/cgi/faime.cgi?"       # API base url
_type="cloud"                                           # Install type
_disksize="3"                                           # Disk size in gigabytes
_format="raw.zst"                                       # Image format
_uuid=""                                                # Name of the vm (uuid) (name of the disk file)
_username="debian"                                      # Username of the vm
_userpw="debian"                                        # Password for this user
_partition="ONE"                                        # Partition setting
_keyboard="fr"                                          # Keyboard layout
_suite="bookworm"                                       # Vm distro
_variables="cl5=SSH_SERVER;cl6=STANDARD;cl8=REBOOT"     # Some settings
_varend="sbm=2"                                         # Idk the purpuse of this var
_ram="512M"                                             # Ram size with unit (M/G)
_core="1"                                               # Number of allocated cores for the vm
_port="5555:22"                                         # Port forwarding host:guest,...
_datapath="$HOME/tinyvps"                               # Location for all tinyvps data
_cfgfile="$datapath/machine.cfg"                        # Config file path

# Link will look like this (https://fai-project.org/doc/api.html):
# https://fai-project.org/cgi/faime.cgi?
# type=cloud
# ;disksize=3
# ;format=raw.zst
# ;hostname=uuid
# ;username=debian
# ;userpw=debian
# ;partition=ONE
# ;keyboard=fr
# ;suite=bookworm
# ;cl5=SSH_SERVER;cl6=STANDARD;cl8=REBOOT
# ;sbm=2

usage(){
    echo "Usage: $0
    [-C] Create a new virtual machine
    [-b arg] The API base url
    [-t arg] Select the install type
    [-d arg] Set the disk size in gigabytes (less than 99)
    [-i arg] Select the disk image format
    [-u arg] Set the name of the virtual machine (uuid) (name of the disk file)
    [-n arg] Set the username of the virtual machine user
    [-p arg] Set the password for the virtual machine user
    [-P arg] Select the partition type
    [-k arg] Select the keyboard layout
    [-s arg] Select the virtual machine distro
    [-v arg] Select the string variables
    [-V arg] Another string variable but idk the purpuse of this one...
    [-R arg] Run the associated virtual machine
    [-r arg] Set the amount of RAM with the unit (M/G) for the virtual machine
    [-c arg] Set the number of allocated cores for the virtual machine
    [-f hostPort1:guestPort1,hostPort2:guestPort2,...] Add port forwarding
    [-F] Use the config file to set-up this mess
    [-h] Show this message
    "
    exit 1
}

use_config_file(){
    while IFS= read -r line
    do
    	if [ $(echo $line | head -c 1) = '[' -a $(echo $line | tail -c 1) = ']' ]
    	then
    	
    	fi
    done <<< "$_cfgfile"
}

create_vm(){
    if [ -z "$_uuid" ]
    then
        _uuid="$(uuidgen)"
    fi
    mkdir -p "$_datapath/$_uuid"
    url="${_baseurl}type=$_type;disksize=$_disksize;format=$_format;hostname=$_uuid;username=$_username;userpw=$_userpw;partition=$_partition;keyboard=$_keyboard;suite=$_suite;$_variables;$_varend"
    cd "$_datapath/$_uuid"
    curl -# -O "$url"
    # unzstd file
}

run_vm(){
    hostfwd="user,id=net0"
    IFS=',' read -ra _PORT <<< "$_port"
    for x in "${_PORT[@]}"
    do
        hostfwd="$hostfwd,hostfwd=tcp::$(echo $x | cut -f 1 -d ':')-:$(echo $x | cut -f 2 -d ':')"
    done
    qemu-system-x86_64 -drive "file=$_uuid.raw,format=raw" -m "$_ram" -enable-kvm -smp "$_core" -device e1000,netdev=net0 -netdev "$hostfwd" -rtc base=localtime -D "$_uuid.log" -pidfile "$_uuid.pid" -nographic -serial none -monitor none &
}

createVmFlag=0          # C flag
runVmFlag=0             # R flag
useConfigFileFlag=0     # F flag

while getopts "Cb:t:d:i:u:n:p:P:k:s:v:V:R:r:c:f:F?h" OPTION; do
    case "$OPTION" in
        C)
            createVmFlag=1
            ;;

        b)
            _baseurl="$OPTARG"
            ;;

        t)
            _type="$OPTARG"
            ;;

        d)
            _disksize="$OPTARG"
            ;;

        i)
            _format="$OPTARG"
            ;;

        u)
            _uuid="$OPTARG"
            ;;

        n)
            _username="$OPTARG"
            ;;

        p)
            _userpw="$OPTARG"
            ;;

        P)
            _partition="$OPTARG"
            ;;

        k)
            _keyboard="$OPTARG"
            ;;

        s)
            _suite="$OPTARG"
            ;;

        v)
            _variables="$OPTARG"
            ;;

        V)
            _varend="$OPTARG"
            ;;

        R)
            runVmFlag=1
            _uuid="$OPTARG"
            ;;

        r)
            _ram="$OPTARG"
            ;;

        c)
            _core="$OPTARG"
            ;;

        f)
            _port="$OPTARG"
            ;;

        F)
            useConfigFileFlag=1
            ;;

        ? | h | *)
            usage
            ;;
    esac
done
shift "$(($OPTIND - 1))"

if [ $useConfigFileFlag -eq 1 ]
then
    use_config_file
elif [ $createVmFlag -eq 1 ]
then
    create_vm
elif [ $runVmFlag -eq 1 ]
then
    run_vm
fi
