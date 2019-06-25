# ev3dev-mocks

Tools for faking ev3dev hardware for unit testing.

This package contains [umockdev][1] configuration files for simulating ev3dev
Linux kernel drivers. It also includes an [ALSA][2] plugin for simulating
the ev3dev sound driver.


[1]: https://github.com/martinpitt/umockdev
[2]: https://www.alsa-project.org



## Installation

This package only work on Linux. It is available on Ubuntu LTS via the [ev3dev
tools PPA][3].

    sudo apt-add-repository ppa:ev3dev/tools --update
    sudo apt install ev3dev-mocks


[3]: https://launchpad.net/~ev3dev/+archive/ubuntu/tools



## Usage

Use the `ev3dev-mocks-run` script to wrap your unit tests.

    ev3dev-mocks-run $UNIT_TEST_CMD


## Hacking

### Prequsites

* Linux
* compiler toolchain (`build-essential`)
* `cmake`
* `pkg-config`
* `libasound2-dev`
* `pandoc` (optional)

### Build

    git clone https://github.com/ev3dev/ev3dev-mocks
    cd ev3dev-mocks
    cmake -P setup.cmake
    cmake --build build/
    sudo cmake --build build/ --target install
