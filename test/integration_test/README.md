#Latch-Unix integration Tests

## Tests prerrequisites

* Tested on Ubuntu 13.10, Debian 7.6.  

* Python 2.7.5+ is ok.  

* Extra: latchclient module.  


## Latch-Unix installation and configuration.

* Compile and install package.  
```
./configure prefix=/usr sysconfdir=/etc && make && sudo make install  
```

* Configure latch-unix plugin correctly, adding an operation called **"test_op"**.  


## Tests usage

* if using the latchclient module, replace the following values:  
```
app.authenticate("THE_LATCH_ACCOUNT", "PASSWORD")
```

* Run python script.  
```
sudo python latchtest.py  
```
