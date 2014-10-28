#!/usr/bin/env bash
# run as root
# tested on (OS X 10.9.3)


function pre_install_ ()
{
  if [ -f "`which apt-get`" ] ; then
    sudo apt-get -y update
    sudo apt-get -y install libpam0g-dev libcurl4-openssl-dev libssl-dev
    sudo apt-get -y install gcc make
  elif [ -f "`which yum`" ] ; then
    sudo yum -y update
    sudo yum -y install pam-devel libcurl-devel openssl-devel
    sudo yum -y install gcc make
  fi
}

function install_ ()
{
  # change to root directory
  cd ..

  # configure & make & install
  ./configure prefix=/usr sysconfdir=/etc && make && sudo make install

  # move pam_latch.so to PAM directory
  echo 'Moving pam_latch.so to PAM directory ...' 
  if test -d /lib*/*/security/ ; then
    PAM_DIR=/lib*/*/security/
  elif test -d /lib*/security/ ; then
    PAM_DIR=/lib*/security/
  elif test -d /usr/lib/security/ ; then
    PAM_DIR=/usr/lib/security/
  else
    PAM_DIR=/usr/lib/pam/
  fi

  if test -d $PAM_DIR && test -f /usr/lib/pam_latch.so ; then
    echo 'PAM directory: '$PAM_DIR
    sudo mv /usr/lib/pam_latch.so $PAM_DIR
  else
    echo 'Move /usr/lib/pam_latch.so manually to PAM dir'
    exit 1
  fi

  # change to centos directory
  echo "Setting up $1 ..."
  cd examples/$1/

  # configure pam services
  echo 'Configuring pam services...'
  for i in `ls etc/pam.d/` ; do
    if [[ $i == *latch ]] ; then
      sudo cp etc/pam.d/$i /etc/pam.d/
      continue
    fi
    if test ! -f /etc/pam.d/$i ; then
      continue
    fi
    if test ! -f /etc/pam.d/$i.lchsave ; then
      sudo mv /etc/pam.d/$i /etc/pam.d/$i.lchsave
    fi
    sudo cp etc/pam.d/$i /etc/pam.d/
  done

  # configure ssh server
  echo 'Configuring ssh server...'
  if test -d /etc/ssh/ ; then
    SSH_CONFIG_DIR=etc/ssh
  else
    SSH_CONFIG_DIR=etc
  fi

  if test -f /$SSH_CONFIG_DIR/sshd_config ; then
    if test ! -f /$SSH_CONFIG_DIR/sshd_config.lchsave ; then
      sudo mv /$SSH_CONFIG_DIR/sshd_config /$SSH_CONFIG_DIR/sshd_config.lchsave
    fi
    sudo cp $SSH_CONFIG_DIR/sshd_config /$SSH_CONFIG_DIR/sshd_config
  else
    echo 'SSH server not found'
  fi

  # restart ssh
  echo 'Restarting ssh server...'
  if [[ "$1" == "debian" || "$1" == "ubuntu" ]] ; then
    sudo service ssh restart
  elif [[ "$1" == 'fedora' || "$1" == 'centos' ]] ; then
    sudo service sshd restart
  fi
}

function uninstall_ ()
{
  # change to root directory
  cd ..

  # configure pam services
  echo 'Re-configuring pam services...'
  for i in `ls /etc/pam.d/ | grep '.lchsave' | cut -d "." -f 1` ; do
    sudo mv /etc/pam.d/$i.lchsave /etc/pam.d/$i
  done
  for i in `ls /etc/pam.d/ | grep 'latch'` ; do
    sudo rm /etc/pam.d/$i
  done

  # configure ssh server
  echo 'Re-configuring ssh server ...'
  if test -d /etc/ssh/ ; then
    SSH_CONFIG_DIR=/etc/ssh
  else
    SSH_CONFIG_DIR=/etc
  fi

  if test -f $SSH_CONFIG_DIR/sshd_config.lchsave ; then
    sudo mv $SSH_CONFIG_DIR/sshd_config.lchsave $SSH_CONFIG_DIR/sshd_config
  fi

  # configure & uninstall
  ./configure prefix=/usr sysconfdir=/etc && make && sudo make uninstall && make clean

  # restart ssh
  echo 'Restarting ssh server...'
  if [[ "$1" == "debian" || "$1" == "ubuntu" ]] ; then
    sudo service ssh restart
  elif [[ "$1" == 'fedora' || "$1" == 'centos' ]] ; then
    sudo service sshd restart
  fi
}


if [ "$1" == 'uninstall' ] ; then
  echo 'Uninstalling latch ...'
  uninstall_
elif [ "$1" != '' ] ; then
  echo 'Installing prerequisites ...'
  pre_install_
  echo 'Installing latch ...'
  install_ $1
else
  echo 'Usage: sudo ./setup DIST | uninstall'
fi










