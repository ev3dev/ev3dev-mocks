#!/bin/bash
#
# Maintainer script for publishing releases.

set -e

source=$(dpkg-parsechangelog -S Source)
version=$(dpkg-parsechangelog -S Version)

debuild -S

dput ppa:ev3dev/tools ../${source}_${version}_source.changes

debuild -- clean

gbp buildpackage --git-tag-only
