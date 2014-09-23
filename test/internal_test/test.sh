#!/usr/bin/env bash
# run as root


## constants
OUTPUT_LOG=test.log

USER_TEST=testlatch
ADDRESS=$USER_TEST@localhost
SSH_KEYS_DIR=$HOME/.ssh
SSH_USER_TEST_KEYS_DIR=/home/$USER_TEST/.ssh

PASSWD=$(echo $RANDOM$RANDOM$RANDOM | sha256sum | base64 | head -c 32)



# functions

function init_test {
    sudo adduser $USER_TEST --gecos "First Last,RoomNumber,WorkPhone,HomePhone" --disabled-password > $OUTPUT_LOG 2>&1
    echo -e "$PASSWD\n$PASSWD\n" | sudo passwd $USER_TEST >> $OUTPUT_LOG 2>&1

    # force unpair
    sudo su $USER_TEST -c 'latch -u' >> $OUTPUT_LOG 2>&1
}

function end_test {
    sudo su $USER_TEST -c 'latch -u' >> $OUTPUT_LOG 2>&1
    sudo deluser $USER_TEST >> $OUTPUT_LOG 2>&1
}

function prev_info {
    if grep "testpath2.11paths.com" /etc/latch/latch.conf >> $OUTPUT_LOG 2>&1 ; then
        local ENVIRONMENT=test
    else
        local ENVIRONMENT=produccion
    fi

    local TEST_NAME=${1:-Indefinida}
    local VERSION=$(latch -v)
    local SYSTEM=$(lsb_release -d 2>&1 | cut -d ":" -f 2 2>&1)
    local DATE=$(date)

    sed "s/TEST_NAME/$TEST_NAME/g" $OUTPUT_TEMPLATE | \
    sed "s/LATCH_ENVIRONMENT/$ENVIRONMENT/g" | \
    sed "s/LATCH_VERSION/$VERSION/g" | \
    sed "s/SYSTEM/$SYSTEM/g" | \
    sed "s/DATE/$DATE/g" > $OUTPUT_FILE

    sudo chmod a+rw $OUTPUT_FILE
}

function config_wrong {
    sudo mv /etc/latch/latch.conf /etc/latch/latch_orig.conf

    sudo sed 's/app_id.*/app_id = XXXX/g' /etc/latch/latch_orig.conf | \
    sudo sed 's/secret_key.*/secret_key = XXXX/g' > /etc/latch/latch.conf
    sudo chmod 600 /etc/latch/latch.conf
}

function config_back {
    if test -f /etc/latch/latch_orig.conf; then
        sudo mv /etc/latch/latch_orig.conf /etc/latch/latch.conf
    fi
    sudo chmod 600 /etc/latch/latch.conf
}

function config_open {
    sudo mv /etc/latch/latch.conf /etc/latch/latch_orig.conf

    sudo sed 's/action.*/action = open/g' /etc/latch/latch_orig.conf > /etc/latch/latch.conf
    sudo chmod 600 /etc/latch/latch.conf
}

function config_close {
    sudo mv /etc/latch/latch.conf /etc/latch/latch_orig.conf

    sudo sed 's/action.*/action = close/g' /etc/latch/latch_orig.conf > /etc/latch/latch.conf
    sudo chmod 600 /etc/latch/latch.conf
}

function network_down {
    sudo ifconfig eth0 down
    sleep 2
}

function network_up {
    # ubuntu
    sudo service network-manager restart >> $OUTPUT_LOG 2>&1
    sudo ifconfig eth0 up >> $OUTPUT_LOG 2>&1

    # centos
    sudo service network restart >> $OUTPUT_LOG 2>&1
    sudo ifup eth0 >> $OUTPUT_LOG 2>&1

    sleep 20
}

function add_certs {
    cp certs/* $SSH_KEYS_DIR >> $OUTPUT_LOG 2>&1

    eval `ssh-agent` >> $OUTPUT_LOG 2>&1
    echo $SSH_AUTH_SOCK >> $OUTPUT_LOG 2>&1
    ssh-add $SSH_KEYS_DIR/id_rsa_testlatch >> $OUTPUT_LOG 2>&1

    if [ ! -d $SSH_USER_TEST_KEYS_DIR ]; then
        sudo mkdir $SSH_USER_TEST_KEYS_DIR
    fi

    echo -n 'command="latch-shell -o sshd-keys" ' >> $SSH_USER_TEST_KEYS_DIR/authorized_keys
    cat certs/id_rsa_testlatch.pub >> $SSH_USER_TEST_KEYS_DIR/authorized_keys
}

function rm_certs {
    if [ -d $SSH_USER_TEST_KEYS_DIR ]; then
        test -f $SSH_USER_TEST_KEYS_DIR/authorized_keys && sed -i.bak '/latch-shell/d' $SSH_KEYS_DIR/authorized_keys
    fi
    
    test -f $SSH_KEYS_DIR/id_rsa_testlatch.pub && sudo rm $SSH_KEYS_DIR/id_rsa_testlatch.pub
    test -f $SSH_KEYS_DIR/id_rsa_testlatch && sudo rm $SSH_KEYS_DIR/id_rsa_testlatch
}

function process_request {
    local TEST_N="$1"
    local REQUEST="$2"

    if [ "$3" = expect ]; then
        local RESPONSE=$( $REQUEST 2>&1 )
    elif [ "$3" = simple ]; then
        local RESPONSE=$( $REQUEST 2>&1 )
    else
        local RESPONSE=$( sudo su $USER_TEST -c "$REQUEST" 2>&1 )
    fi  

    local str1="TEST_"$TEST_N"_"
    local str2="RESPONSE_"$TEST_N"_"

    local SED=$(echo "$RESPONSE" | sed ':begin;$!N;s/\n/newLine/;tbegin' | sed -e 's/[\/&]/\\&/g')
    sudo sed -i.bak "s/$str1/$REQUEST/g" $OUTPUT_FILE
    sudo sed -i.bak "s/$str2/$SED/g" $OUTPUT_FILE
    sudo sed -i.bak 's/newLine/\n/g' $OUTPUT_FILE
}

function ensure_paired_state {
    local token=$( latch-client-app pairingcode )
    local exp="latch -p $token"

    sudo su $USER_TEST -c "$exp"
}


## tests 

function test_ {

    if [ "$2" == "pair" ]; then
        case "$3" in
            "blank")
                local REQUEST="latch -p"
                ;;
            *)
                local REQUEST="latch -p $3"
                ;;
        esac        
                
    elif [ "$2" == "status" ]; then
    	local OPERATION_NAME=app_id
        local REQUEST="latch -s"

    elif [ "$2" == "unpair" ]; then
        local REQUEST="latch -u"    
            
    elif [ "$2" == "ssh-pam" ]; then
    	local OPERATION_NAME=sshd-login
        local P_OPTION=expect
        rm_certs

        case "$3" in
            "password")
                local REQUEST="./sshd.exp $ADDRESS $PASSWD"           
                ;;

            "passwrong")
                local REQUEST="./sshd.exp $ADDRESS XXXX"      
                ;;

            "otp")
                local OPERATION_ID=$( sudo latch-client-app --config getid $OPERATION_NAME )
                local REQUEST="./sshd.exp $ADDRESS $PASSWD $OPERATION_ID"              
                ;;

            "otpwrong")
                local OPERATION_ID=$( sudo latch-client-app --config getid $OPERATION_NAME )
                local REQUEST="./sshd.exp $ADDRESS $PASSWD $OPERATION_ID wrong"               
                ;;

            "otpblank")
                local OPERATION_ID=$( sudo latch-client-app --config getid $OPERATION_NAME )
                local REQUEST="./sshd.exp $ADDRESS $PASSWD $OPERATION_ID blank"                 
                ;;
        esac
            
    elif [ "$2" == "ssh-pkey" ]; then
    	local OPERATION_NAME=sshd-keys
        local P_OPTION=simple
        add_certs
        local REQUEST='ssh '$ADDRESS' whoami'               

    fi

    for arg in "$@"; do
        case "$arg" in
            "latch-on")
                sudo latch-client-app lock -o $OPERATION_NAME
                sleep 1
                ;;

            "latch-off")
                sudo latch-client-app unlock -o $OPERATION_NAME off  
                sleep 1          
                ;;

            "latch-otp")
                sudo latch-client-app unlock -o $OPERATION_NAME on  
                sleep 1                  
                ;;

            "invalid-configuration")
                config_wrong
            ;;

            "action-open")
                config_open
            ;;

            "action-close")
                config_close
            ;;

        esac
    done

    process_request "$1" "$REQUEST" $P_OPTION
    
    config_back
}


## suit tests ##

suit_test_pair () {
    token=$( latch-client-app pairingcode )
    test_ 1 pair $token invalid-configuration 
    test_ 2 pair XXXXXX
    test_ 3 pair blank
    test_ 4 pair $token
    test_ 5 pair $token
}

suit_test_status () {
    test_ 7 status latch-on
    test_ 8 status latch-off
}

suit_test_unpair () {
    test_ 9 unpair
    test_ 10 unpair
}

suit_test_ssh_pam () {
    test_ 1 ssh-pam password latch-on
    test_ 2 ssh-pam password latch-on invalid-configuration
    test_ 3 ssh-pam password latch-off
    test_ 4 ssh-pam passwrong latch-otp 
    test_ 5 ssh-pam otp latch-otp 
    test_ 6 ssh-pam otpblank latch-otp 
    test_ 7 ssh-pam otpwrong latch-otp  
}

suit_test_ssh_pkey () {
    test_ 8 ssh-pkey latch-on
    test_ 9 ssh-pkey latch-off
}

suit_test_without_network () {
    network_down

    network_up
}



## main ##

if [ "$1" == "latch" ]; then
    OUTPUT_FILE=informe_pruebas_latch.md
    OUTPUT_TEMPLATE=informe_pruebas_latch.template

    init_test

    case  "$2" in
        "all")
            prev_info "Completa"
            suit_test_pair
            suit_test_status
            suit_test_unpair
            ;;

        "pair")
            prev_info "Pareado"
            suit_test_pair
            ;;

        "status")
            prev_info "Consulta de estado de latch"
            ensure_paired_state
            suit_test_status
            ;;

        "unpair")
            prev_info "Despareado"
            ensure_paired_state
            suit_test_unpair
            ;;

        *)
    esac

    end_test

elif [ "$1" == "ssh" ]; then
    OUTPUT_FILE=informe_pruebas_ssh.md
    OUTPUT_TEMPLATE=informe_pruebas_ssh.template
    
    init_test
    ensure_paired_state

    case  "$2" in
        "all")
            prev_info "Completa"
            suit_test_ssh_pam
            suit_test_ssh_pkey
            ;;
        "pam")
            prev_info "PAM"
            suit_test_ssh_pam
            ;;

        "pkey")
            prev_info "Public-private keys"
            suit_test_ssh_pkey
            ;;

        *)
    esac

    end_test

else
    # superuser required
    echo 'Usage: sudo ./test.sh latch [ all | pair | status | unpair ]'
    echo ''
    echo 'version 1.0'
fi

