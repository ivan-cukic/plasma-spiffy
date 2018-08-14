# Plasma Spiffy

Plasma Spiffy is an applet for note creation.

It is under heavy development.

## GitHub

Plasma applets are usually found on KDE's git infrastructure.
The reason why Spiffy is on GitHub is simply because I consider it to be a toy at the moment,
and not something that should bear the responsibility that comes with being a KDE project.

If Spiffy matures and proves to be useful, I'll move it to the KDE infrastructure.


## Build instructions

    git clone https://github.com/ivan-cukic/plasma-spiffy.git
    cd plasma-spiffy
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=MYPREFIX ..
    make
    make install

(MYPREFIX is where you install your Plasma setup, replace it accordingly)

