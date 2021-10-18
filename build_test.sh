#!/bin/bash

export MAKEFLAGS=-j$(nproc)
WD="${WD:-$(pwd)}"

install_prefix=$WD/build/install

debian=$WD/misc/debian/pprzgcs
deb_install_prefix=$debian/usr/local

echo "install_prefix set to $install_prefix."

# exit on error
set -e

ivyqt_src=$WD/ext/IvyQt
ivyqt_build=$WD/build/IvyQt

pprzlinkqt_src=$WD/ext/pprzlinkQt/
pprzlinkqt_build=$WD/build/ext/pprzlinkQt

cmake -S $ivyqt_src -B $ivyqt_build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$install_prefix
cmake --build $ivyqt_build
cmake --install $ivyqt_build

cmake -S $pprzlinkqt_src -B $pprzlinkqt_build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$install_prefix
cmake --build $pprzlinkqt_build
cmake --install $pprzlinkqt_build

#cmake -S . -B $WD/build/pprzgcs -DCMAKE_INSTALL_PREFIX=$deb_install_prefix -DCMAKE_PREFIX_PATH=$install_prefix -DCMAKE_BUILD_TYPE=Release -DPPRZGCS_VERSION=$(git describe --tags)
cmake -S . -B $WD/build/pprzgcs -DCMAKE_PREFIX_PATH=$install_prefix -DCMAKE_BUILD_TYPE=Release
#cmake --build $WD/build/pprzgcs
#cmake --install $WD/build/pprzgcs

#strip --strip-unneeded $deb_install_prefix/bin/pprzgcs


