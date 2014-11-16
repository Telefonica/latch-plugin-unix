#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# vim: set fileencoding=utf-8
# Run as root

'''
 This script allows to test latch plugin
 Copyright (C) 2013 Eleven Paths
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
'''

import unittest
import random
import shutil
import os

from subprocess import *



def internal_test(f):
    try:
        import latchclient
    except:
        return 
    return f


def modify_conf(key, value, cfile="/etc/latch/latch.conf"):
    if not os.path.isfile(cfile + ".orig"):
        shutil.copy2(cfile, cfile + ".orig")  

    f = open(cfile, "r")
    lines = f.readlines()
    f.close()

    f = open(cfile, "w")
    for line in lines:
        if line.startswith(key):
            f.write(key + " = " + value + "\n")
        else:
            f.write(line)    
    f.close()

    shutil.copystat(cfile + ".orig", cfile)

def modify_acc(key, value, afile="/etc/latch/latch.accounts"):
    if not os.path.isfile(afile + ".orig"):
        shutil.copy2(afile, afile + ".orig")  

    f = open(afile, "r")
    lines = f.readlines()
    f.close()

    f = open(afile, "w")
    f.write(key + ": " + value + "\n")
    for line in lines:
        f.write(line)    
    f.close()

    shutil.copystat(afile + ".orig", afile)

def restore_file(f):
    shutil.copy2(f + ".orig", f) 
    os.remove(f + ".orig")

def getConfigParameter(name, configFile="/etc/latch/latch.conf"):
    try:
        f = open(configFile,"r")
    except IOError as e:
        return None

    lines = f.readlines()
    f.close()

    for line in lines:
        if line.find(name) != -1:
            break;

    words = line.split()
    if len(words) == 3:
        return words[2]
    return None


class TestLatch(unittest.TestCase):

    def setUp(self):
        self.user = "latch_test_user"
        self.password = str(random.random())
        call(["./adduser.sh", self.user, self.password], stdout=PIPE, stderr=PIPE)

    def tearDown(self):
        call(["sudo", "su", self.user, "-c", "latch -u"], stdout=PIPE, stderr=PIPE)
        call(["sudo", "deluser", self.user], stdout=PIPE, stderr=PIPE)


    def test_pair_with_invalid_configuration(self):
        modify_conf("app_id", "XXXX")

        response = Popen(["sudo", "su", self.user, "-c", "latch -p XXXXXX"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()

        restore_file("/etc/latch/latch.conf")
        self.assertEqual("Invalid Application ID or Secret Key\n", err)

    def test_pair_invalid_token(self):
        response = Popen(["sudo", "su", self.user, "-c", "latch -p XXXXXX"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertEqual("Invalid token\n", err)

    def test_pair_invalid_token_len(self):
        response = Popen(["sudo", "su", self.user, "-c", "latch -p XXXXXXXX"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertEqual("Invalid token\n", err)

    def test_pair_invalid_token_chars(self):
        response = Popen(["sudo", "su", self.user, "-c", "latch -p *10OI_"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertEqual("Invalid token\n", err)

    def test_pair_blank_token(self):
        response = Popen(["sudo", "su", self.user, "-c", "latch -p"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertTrue("Option -p requires an argument." in err)

    def test_status_being_unpaired(self):
        call(["sudo", "su", self.user, "-c", "latch -u"], stdout=PIPE, stderr=PIPE)

        response = Popen(["sudo", "su", self.user, "-c", "latch -s"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertEqual("The user " + self.user + " is not paired with latch\n", err)

    def test_status_without_acc_file(self):
        shutil.copy2("/etc/latch/latch.accounts", "/etc/latch/latch.accounts.orig") 
        os.remove("/etc/latch/latch.accounts")

        response = Popen(["sudo", "su", self.user, "-c", "latch -s"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()

        restore_file("/etc/latch/latch.accounts")

        self.assertEqual("The user " + self.user + " is not paired with latch\n", err)

    def test_status_invalid_acc_id(self):
        modify_acc(self.user, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX")

        response = Popen(["sudo", "su", self.user, "-c", "latch -s"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()

        restore_file("/etc/latch/latch.accounts")

        self.assertEqual("The user " + self.user + " is not really paired with latch. Try to unpair and pair again.\n", err)

    def test_op_status_being_unpaired(self):
        call(["sudo", "su", self.user, "-c", "latch -u"], stdout=PIPE, stderr=PIPE)

        response = Popen(["sudo", "su", self.user, "-c", "latch -o test_op"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertEqual("The user " + self.user + " is not paired with latch\n", err)

    def test_op_status_without_acc_file(self):
        shutil.copy2("/etc/latch/latch.accounts", "/etc/latch/latch.accounts.orig") 
        os.remove("/etc/latch/latch.accounts")

        response = Popen(["sudo", "su", self.user, "-c", "latch -o test_op"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()

        restore_file("/etc/latch/latch.accounts")

        self.assertEqual("The user " + self.user + " is not paired with latch\n", err)

    def test_op_status_invalid_operation(self):
        response = Popen(["sudo", "su", self.user, "-c", "latch -o invalid_op"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()

        self.assertEqual("Operation invalid_op does not exist in /etc/latch/latch.conf\n", err)

    def test_op_status_invalid_acc_id(self):
        modify_acc(self.user, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX")

        response = Popen(["sudo", "su", self.user, "-c", "latch -o test_op"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()

        restore_file("/etc/latch/latch.accounts")

        self.assertEqual("The user " + self.user + " is not really paired with latch. Try to unpair and pair again.\n", err)

    def test_unpair_being_unpaired(self):
        call(["sudo", "su", self.user, "-c", "latch -u"], stdout=PIPE, stderr=PIPE)

        response = Popen(["sudo", "su", self.user, "-c", "latch -u"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertEqual("The user " + self.user + " is not paired with latch\n", err)

    def test_unpair_without_acc_file(self):
        shutil.copy2("/etc/latch/latch.accounts", "/etc/latch/latch.accounts.orig") 
        os.remove("/etc/latch/latch.accounts")

        response = Popen(["sudo", "su", self.user, "-c", "latch -u"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()

        restore_file("/etc/latch/latch.accounts")

        self.assertEqual("The user " + self.user + " is not paired with latch\n", err)

    @internal_test
    def test_all_correct(self):
        import latchclient

        app = latchclient.LatchClient() 
        res = app.pairing_token()
        token = res.get_data()["token"]

        # pair
        response = Popen(["sudo", "su", self.user, "-c", "latch -p " + token], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertEqual("Account successfully paired to the user " + self.user + "\n", out)

        # try pair again
        response = Popen(["sudo", "su", self.user, "-c", "latch -p " + token], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertEqual("The user " + self.user + " is already paired with latch\n", err)

        app_id = getConfigParameter("app_id")
        app = latchclient.LatchClient()
        app.status_update(app_id, "on")

        # status locked
        response = Popen(["sudo", "su", self.user, "-c", "latch -s"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertTrue("Status: latch locked\n", err)

        app.status_update(app_id, "off")

        # status unlocked
        response = Popen(["sudo", "su", self.user, "-c", "latch -s"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertTrue("Status: latch not locked\n", err)

        op_id = getConfigParameter("test_op")
        app.status_update(op_id, "on")

        # operation status locked
        response = Popen(["sudo", "su", self.user, "-c", "latch -o test_op"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertTrue("Status: latch locked\n", err)

        app.status_update(op_id, "off")

        # operation status unlocked
        response = Popen(["sudo", "su", self.user, "-c", "latch -o test_op"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertTrue("Status: latch not locked\n", err)

        # status invalid configuration
        modify_conf("app_id", "XXXX")

        response = Popen(["sudo", "su", self.user, "-c", "latch -s"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()

        restore_file("/etc/latch/latch.conf")
        self.assertEqual("Invalid Application ID or Secret Key\n", err)

        # unpair
        response = Popen(["sudo", "su", self.user, "-c", "latch -u"], stdout=PIPE, stderr=PIPE)
        out, err = response.communicate()
        self.assertEqual("Account belonging to the user " + self.user + " successfully unpaired from latch\n", out)



if __name__ == '__main__':
    unittest.main()
