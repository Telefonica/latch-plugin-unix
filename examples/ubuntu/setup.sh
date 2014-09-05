#!/usr/bin/env bash
# run as root
# tested on (Ubuntu 14.04, Ubuntu 13.10)



function install_dependencies ()
{
  # install dependencies
  sudo apt-get update
  sudo apt-get install libpam0g-dev, libcurl4-openssl-dev, libssl-dev
  sudo apt-get install gcc make

}

function install_ ()
{
  # change to root directory
  cd ../..

  # configure & make & install
  ./configure && make && sudo make install

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

  # change to ubuntu directory
  echo 'Setting up Ubuntu...'
  cd examples/ubuntu/

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
  if test -f /etc/ssh/sshd_config ; then
    if test ! -f /etc/ssh/sshd_config.lchsave ; then
      sudo mv /etc/ssh/sshd_config /etc/ssh/sshd_config.lchsave
    fi
    sudo cp etc/ssh/sshd_config /etc/ssh/sshd_config
  else
    echo 'SSH server not found'
  fi

  # restart ssh
  echo 'Restarting ssh server...'
  sudo service ssh restart
}

function uninstall_ ()
{
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

  # change to root directory
  cd ../..

  # configure & uninstall
  ./configure && make clean && sudo make uninstall

  # restart ssh
  echo 'Restarting ssh server...'
  sudo service ssh restart
}


if [ "$1" == "" ] ; then
  echo 'Installing prerequisites..'
  install_dependencies
  echo 'Installing latch-Unix...'
  install_
elif [ "$1" == 'uninstall' ] ; then
  echo 'Uninstalling latch-Unix...'
  uninstall_
else
  echo 'Usage: sudo ./setup [ uninstall ]'
fi

