#!/usr/bin/env bash
# run as root

RESULTS_TXT=test/installation/text-results.txt


assert_sysconf () {
    echo "assert sysconf: $1" >> $RESULTS_TXT
    test -d $1/latch && \
    stat -c '%U' $1/latch/latch.accounts | grep root && \
    stat -c '%a' $1/latch/latch.accounts | grep "600" && \
    stat -c '%U' $1/latch/latch.conf | grep root && \
    stat -c '%a' $1/latch/latch.conf | grep "600" && \
    echo "...passed" >> $RESULTS_TXT
}

assert_binary () {
    echo "assert binary: $1" >> $RESULTS_TXT
    test -f $1 && \
    stat -c '%U' $1 | grep root && \
    stat -c '%a' $1 | grep "4755" && \
    echo "...passed" >> $RESULTS_TXT
}


test_no_flags () {
    ./configure
    make
    sudo make install   

    echo "TEST_NO_FLAGS" >> $RESULTS_TXT
    assert_sysconf /usr/local/etc/
    assert_binary /usr/local/bin/latch
    assert_binary /usr/local/bin/latch-ssh-cmd
    echo >> $RESULTS_TXT

    sudo make uninstall
}

test_sysconfdir_flag () {
    ./configure --sysconfdir=$1
    make
    sudo make install   

    echo "TEST_SYSCONFDIR_FLAG -> sysconfdir=$1" >> $RESULTS_TXT
    assert_sysconf $1
    assert_binary /usr/local/bin/latch
    assert_binary /usr/local/bin/latch-ssh-cmd
    echo >> $RESULTS_TXT

    sudo make uninstall
}

test_bindir_flag () {
    ./configure --bindir=$1
    make
    sudo make install   

    echo "TEST_BINDIR_FLAG -> bindir=$1" >> $RESULTS_TXT
    assert_sysconf /usr/local/etc/
    assert_binary $1/latch
    assert_binary $1/latch-ssh-cmd
    echo >> $RESULTS_TXT

    sudo make uninstall
}

test_prefix_flag () {
    ./configure --prefix=$1
    make
    sudo make install   

    echo "TEST_PREFIX_FLAG -> prefix=$1" >> $RESULTS_TXT
    assert_sysconf $1/etc/
    assert_binary $1/bin/latch
    assert_binary $1/bin/latch-ssh-cmd
    echo >> $RESULTS_TXT

    sudo make uninstall
}



### main ###
cd ../../

echo "--CONFIGURE TEST BEGIN--" > $RESULTS_TXT
test_no_flags
test_sysconfdir_flag /etc
test_bindir_flag /usr/bin
test_prefix_flag /usr
test_prefix_flag /opt/latch
echo "--CONFIGURE TEST END--" >> $RESULTS_TXT