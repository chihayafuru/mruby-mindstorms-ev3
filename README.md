mruby for MINDSTORMS EV3
=========

## Getting Started


### Install tool chain

    $ wget -c http://www.codesourcery.com/sgpp/lite/arm/portal/package4571/public/arm-none-linux-gnueabi/arm-2009q1-203-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2
    $ tar -jxvf arm-2009q1-203-arm-none-linux-gnueabi-i686-pc-linux-gnu.tar.bz2 -C ~/CodeSourcery/
    $ mv ~/CodeSourcery/arm-2009q1 ~/CodeSourcery/Sourcery_G++_Lite
    $ echo export PATH=~/CodeSourcery/Sourcery_G++_Lite/bin/:$PATH >> ~/.bashrc && . ~/.bashrc

### Get ev3sources

    $ cd ~
    $ git clone https://github.com/mindboards/ev3sources.git

### Get mruby

    $ cd ~
    $ git clone https://github.com/mruby/mruby.git

### Get this repository

    $ cd ~/mruby/mrbgems/
    $ git clone https://github.com/chihayafuru/mruby-mindstorms-ev3.git

### Build mruby for mindstorms-ev3

    $ cd ~/mruby/
    $ make MRUBY_CONFIG=mrbgems/mruby-mindstorms-ev3/ev3_config.rb

### Connect to EV3

    $ telnet 192.168.0.xxx
    Trying 192.168.0.xxx...
    Connected to 192.168.0.xxx.
    Escape character is '^]'.
    
     _____       _     _ ___
    |  _  |_ _ _| |___| | __|
    |    _| | | . | . | | _|
    |__|__|___|___|___|_|_|
    
    Rudolf 2011.01 EV3
    
    login: root
    Last login: Thu Jan  1 00:01:14 on pts/0
    root@EV3:~#

### Run ssh server (dropbear) on EV3

    root@EV3:~# dropbear

### Copy mruby & sample script to the external SD card on EV3

    $ cd ~/mruby/mrbgems/mruby-mindstorms-ev3/build/ev3/bin/
    $ scp mruby root@192.168.0.xxx:/media/card
    root@192.168.0.xxx's password:↵

    $ cd ~/mruby/mrbgems/mruby-mindstorms-ev3/sample
    $ scp motor_sample.rb root@192.168.0.xxx:/media/card
    root@192.168.0.xxx's password:↵


### Run mruby on EV3

    root@EV3:~# cd /media/card
    root@EV3:/media/card# ./mruby motor_sample.rb

## References

http://botbench.com/blog/2013/07/31/lego-mindstorms-ev3-source-code-available/
http://robotnav.wordpress.com/ev3/

