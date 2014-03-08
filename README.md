# Qwertickle
A typewriter sound scheme for your keyboard!

Qwertickle is a Linux clone of Qwertick (http://www.nattyware.com/qwertick.php) by Nattyware, written by Gtk+ and Gstreamer.

## Contributing
1. Fork it
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request

## Note
The sound effect used in this program is from http://www.soundjay.com, any form of redistribution is prohibited.


## Build on Ubuntu 13.10 
1. Install build dependences
sudo apt-get install autoconf automake libtool libgtk2.0-dev libgstreamer0.10-dev libxtst-dev xorg-dev

2. Update 
AM_VERSION=-1.14
to
AM_VERSION=-1.13
in autogen.sh

3. run 
./autogen.sh
./configure
make

## Install on Ubuntu 13.10
1. run
sudo make install

2. Install mp3 codec for gstreamer
sudo apt-get install gstreamer0.10-fluendo-mp3 gstreamer1.0-fluendo-mp3
