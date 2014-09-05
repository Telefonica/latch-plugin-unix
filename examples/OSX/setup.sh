#!/usr/bin/env bash
# run as root
# tested on (OS X 10.9.3)




function install_ ()
{
  local SYS=$1

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
  elif test -d /lib/security/ ; then
    PAM_DIR=/lib/security/
  else
    PAM_DIR=/usr/lib/pam/
  fi

  if test -d $PAM_DIR && test -f /usr/local/lib/pam_latch.so ; then
    echo 'PAM directory: '$PAM_DIR
    sudo mv /usr/local/lib/pam_latch.so $PAM_DIR
  else
    echo 'Move /usr/local/lib/pam_latch.so manually to PAM dir'
    exit 1
  fi

  # move binary files to /usr/bin/
  echo 'Moving binaries to /usr/bin/ ...' 
  sudo mv /usr/local/bin/latch /usr/local/bin/latch-shell /usr/bin/

  # update permissions
  echo 'Updating permissions...' 
  sudo chmod 4755 /usr/bin/latch /usr/bin/latch-shell

  # change to centos directory
  echo 'Setting up '$SYS'...'
  cd examples/$SYS/

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
}

function uninstall_ ()
{
  # change to root directory
  cd ../..

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
  if test -d /etc/ssh/ ; then
    SSH_CONFIG_DIR=/etc/ssh
  else
    SSH_CONFIG_DIR=/etc
  fi

  if test -f $SSH_CONFIG_DIR/sshd_config.lchsave ; then
    sudo mv $SSH_CONFIG_DIR/sshd_config.lchsave $SSH_CONFIG_DIR/sshd_config
  fi

  # configure & uninstall
  ./configure && make clean && sudo make uninstall

  # remove binaries
  echo 'Removing binaries from /usr/bin/ ...' 
  if test -f /usr/bin/latch ; then
    sudo rm /usr/bin/latch 
  fi
  if test -f /usr/bin/latch-shell ; then
    sudo rm /usr/bin/latch-shell
  fi

}


if [ "$1" == "" ] ; then
  echo 'Installing latch for Mac OS X...'
  install_ OSX 
elif [ "$1" == 'uninstall' ] ; then
  echo 'Uninstalling latch for Mac OS X...'
  uninstall_
else
  echo 'Usage: sudo ./setup [ uninstall ]'
fi










