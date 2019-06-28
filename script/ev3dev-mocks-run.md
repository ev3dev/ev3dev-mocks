% EV3DEV-MOCKS-RUN
% David Lechner
% June 2019

NAME
====

ev3dev-mocks-run - test environment for ev3dev


SYNOPSIS
========

**ev3dev-mocks-run**


DESCRIPTION
===========

This tool runs commands in a special environment that simulates ev3dev
hardware. This is done by using *umockdev-run* to simulate */sys* and */dev*
and using an ALSA plugin to simulate the sound card.


USAGE
=====

ev3dev-mocks-run *command*

*command* is ELF executable (with or without additional arguments) to be run in
the special environment.
