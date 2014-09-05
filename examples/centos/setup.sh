#!/usr/bin/env bash
# run as root
# tested on (Centos 6.5)


function pre_install_ ()
{
  # install dependencies
  sudo yum update
  sudo yum install pam-devel libcurl-devel openssl-devel
  sudo yum install gcc make

}

function install_ ()
{
  # change to root directory
  cd ../..

  # configure & make & install
  ./configure && make && sudo make install

  # change to centos directory
  echo 'Setting up CentOS...'
  cd examples/centos/

  # configure pam services
  echo 'Configuring pam services...'
  for i in `ls etc/pam.d/` ; do
    if test -f /etc/pam.d/$i && test ! -f /etc/pam.d/$i.lchsave ; then
      sudo mv /etc/pam.d/$i /etc/pam.d/$i.lchsave
    fi
    sudo cp etc/pam.d/$i /etc/pam.d/
  done

  # configure ssh server
  echo 'Configuring ssh server...'
  if test -f /etc/ssh/sshd_config ; then
    if test ! -f /etc/ssh/sshd_config.lchsave ; then
      sudo mv /etc/ssh/sshd_config /etc/ssh/sshd_config.lchsave
    fi
    sudo cp etc/ssh/sshd_config /etc/ssh/sshd_config
  else
    echo 'SSH server not found'
  fi

  # move pam_latch.so to PAM directory
  echo 'Moving pam_latch.so to PAM directory...' 
  if test -d /lib/*/security/ ; then
    PAM_DIR=/lib/*/security/
  elif test -d /lib64/security/ ; then
    PAM_DIR=/lib64/security/
  else 
    PAM_DIR=/lib/security/
  fi

  if test -d $PAM_DIR ; then
    sudo mv /usr/local/lib/pam_latch.so $PAM_DIR
    echo 'PAM directory: '$PAM_DIR
  else
    echo 'Move /usr/local/lib/pam_latch.so manually to PAM dir'
  fi

  # move binary files to /usr/bin/
  echo 'Moving binaries to /usr/bin/ ...' 
  sudo mv /usr/local/bin/latch /usr/local/bin/latch-shell /usr/bin/

  # update permissions
  echo 'Updating permissions...' 
  sudo chmod 4755 /usr/bin/latch /usr/bin/latch-shell

  # restart ssh
  echo 'Restarting ssh server...'
  sudo service sshd restart
}

function uninstall_ ()
{
  # change to root directory
  cd ../..

  # configure & uninstall
  ./configure && make clean && sudo make uninstall

  # configure pam services
  echo 'Re-configuring pam services...'
  for i in `ls /etc/pam.d/ | grep '.lchsave' | cut -d "." -f 1` ; do
    sudo mv /etc/pam.d/$i.lchsave /etc/pam.d/$i
  done
  for i in `ls /etc/pam.d/ | grep 'latch'` ; do
    sudo rm /etc/pam.d/$i
  done

  # configure ssh server
  echo 'Re-configuring ssh server...'
  if test -f /etc/ssh/sshd_config.lchsave ; then
    sudo mv /etc/ssh/sshd_config.lchsave /etc/ssh/sshd_config
  fi

  # remove binaries
  echo 'Removing binaries from /usr/bin/ ...' 
  if test -f /usr/bin/latch ; then
    sudo rm /usr/bin/latch 
  fi
  if test -f /usr/bin/latch-shell ; then
    sudo rm /usr/bin/latch-shell
  fi

  # restart ssh
  echo 'Restarting ssh server...'
  sudo service sshd restart
}


if [ "$1" == "" ] ; then
  echo 'Installing prerequisites..'
  pre_install_
  echo 'Installing latch-Unix...'
  install_
elif [ "$1" == 'uninstall' ] ; then
  echo 'Uninstalling latch-Unix...'
  uninstall_
else
  echo 'Usage: sudo ./setup [ uninstall ]'
fi










