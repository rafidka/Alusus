#!/bin/bash

# Dependencies
LLVM_SRC_URL="http://llvm.org/releases/3.3/llvm-3.3.src.tar.gz"
LLVM_NAME="llvm-3.3"
BOOST_SRC_URL="http://sourceforge.net/projects/boost/files/boost/1.55.0/boost_1_55_0.tar.gz"
BOOST_SRC_NAME="boost_1_55_0"
CATCH_SRC_URL="http://alusus.net/Downloads/CATCH.tar.gz"
CATCH_SRC_NAME="CATCH"

# Paths
ALUSUS_ROOT="$(pwd)/.."
INSTALL_PATH="$(pwd)/.."
RELEASE_INSTALL_PATH="/opt/Alusus"
BUILDS_PATH="$(pwd)/../Builds"
DEPS_PATH="$(pwd)/../Builds/Dependencies"
PACKAGES_PATH="$(pwd)/../Builds/Packages"

# Build Args
MAKE_THREAD_COUNT=8
BUILD_TYPE="debug"

# Package Creation Args.
VERSION_FILE="version"
CREATE_PACKAGES="no"
DEB_CHANGE_LOG="deb-changelog.en.txt"
RPM_CHANGE_LOG="rpm-changelog.en.txt"
ARCHITECTURE="native"
PACKAGE_NAME="alusus"
PACKAGE_DESCRIPTION="Alusus is a programming language that is flexible, comprehensive, and decentralized. It's open to all programming methodologies by allowing anyone to upgrade and enhance it without limitations, without permission or coordination with a central team, without needing to rebuild or redistribute the compiler, and most importantly without needing to persuade the users to update their development environments or settings."
PACKAGE_MAINTAINER="hicham OUALI ALAMI<hicham@oualialami.com>"
PACKAGE_URL="http://alusus.net/?lang=en"
AFTER_INSTALL_SCRIPT="post_install.sh"
AFTER_REMOVAL_SCRIPT="post_remove.sh"


#------------------------
# Command Line Processing

for var in "$@"
do
  if [[ $var == "--help" ]]; then
    echo "Options:"
    echo "-d: Use 'debug' build type. This is the default."
    echo "-r: Use 'release' build type."
    echo "-p: Create release packages after the build."
    exit
  elif [[ $var == "-d" ]]; then
    BUILD_TYPE="debug"
  elif [[ $var == "-r" ]]; then
    BUILD_TYPE="release"
  elif [[ $var == "-p" ]]; then
    BUILD_TYPE="release"
    CREATE_PACKAGES="yes"
    INSTALL_PATH="$RELEASE_INSTALL_PATH"
    sudo rm -rf $INSTALL_PATH/*
  fi
done


#---------------------------
# Message Printing Functions

# Color Constants
BLU_CLR='\033[0;34m'
GRN_CLR='\033[0;32m'
RED_CLR='\033[0;31m'
YEL_CLR='\033[1;33m'
NO_CLR='\033[0m'

# INFORMATION MESSAGE
InformationMessage () {
   echo -e "${BLU_CLR}INFO: ${1}${NO_CLR}"
}

# SUCCESS MESSAGE
SuccessMessage () {
   echo -e "${GRN_CLR}DONE: ${1}${NO_CLR}"
}

# WARNING MESSAGE
WarnMessage () {
   echo -e "${YEL_CLR}WARNING: ${1}${NO_CLR}"
}

# ERROR MESSAGE
ErrorMessage () {
   echo -e "${RED_CLR}ERROR: ${1}${NO_CLR}"
}

# FAIL MESSAGE
FailMessage () {
   echo -e "${RED_CLR}FAILED: ${1}${NO_CLR}"
}

mkdir -p $BUILDS_PATH
mkdir -p $DEPS_PATH


#-------------------------
# Dependencies Preparation

InformationMessage "Preparing dependencies..."

cd $DEPS_PATH

if [[ ! -e "${LLVM_NAME}.src" ]]; then
  wget ${LLVM_SRC_URL}
  tar -xvf ${LLVM_NAME}.src.tar.gz
else
  InformationMessage "LLVM sources is already available."
fi

if [[ ! -e ${BOOST_SRC_NAME} ]]; then
  wget ${BOOST_SRC_URL}
  tar -xvf ${BOOST_SRC_NAME}.tar.gz
else
  InformationMessage "BOOST sources is already available."
fi

if [[ ! -e ${CATCH_SRC_NAME} ]]; then
  wget ${CATCH_SRC_URL}
  tar -xvf ${CATCH_SRC_NAME}.tar.gz
else
  InformationMessage "CATCH is already available."
fi

if [[ ! -e ${LLVM_NAME}.install ]]; then
  mkdir ${LLVM_NAME}.install
  if [[ ! -e ${LLVM_NAME}.build ]]; then
    mkdir ${LLVM_NAME}.build
  fi
  cd ${LLVM_NAME}.build
  cmake ../${LLVM_NAME}.src -DCMAKE_INSTALL_PREFIX=$DEPS_PATH/${LLVM_NAME}.install
  make -j${MAKE_THREAD_COUNT} || { FailMessage "Building LLVM."; exit 1; }
  make install
else
  InformationMessage "LLVM is already built."
fi

SuccessMessage "Preparing dependencies."


#----------------
# Alusus Building

InformationMessage "Building Alusus..."

cd $BUILDS_PATH

cmake $ALUSUS_ROOT/Sources -DBOOST_PATH=$DEPS_PATH/$BOOST_SRC_NAME -DCATCH_PATH=$DEPS_PATH/$CATCH_SRC_NAME -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DCMAKE_INSTALL_PREFIX=${INSTALL_PATH} -DLLVM_PATH=$DEPS_PATH/${LLVM_NAME}.install

make -j${MAKE_THREAD_COUNT} || { FailMessage "Building Alusus."; exit 1; }

if [[ $CREATE_PACKAGES == "no" ]]; then
  make install
fi

SuccessMessage "Building Alusus."

if [[ $CREATE_PACKAGES == "no" ]]; then
  cd $ALUSUS_ROOT/Tools
  exit
fi


#-------------------------------
# Installation Packages Creation

InformationMessage "Creating installation packages..."

sudo make install

cd $ALUSUS_ROOT

sudo cp -r Doc ${INSTALL_PATH}/
sudo mkdir -p ${INSTALL_PATH}/Tools/Gtk_Syntax_Highlighting
sudo cp Tools/Gtk_Syntax_Highlighting/alusus.lang ${INSTALL_PATH}/Tools/Gtk_Syntax_Highlighting/

if [[ ! -e ${PACKAGES_PATH} ]]; then
   mkdir ${PACKAGES_PATH}
fi

cd ${PACKAGES_PATH}

rm -rf *

INPUT_TYPE="dir"

PACKAGE_VERSION=`cat $ALUSUS_ROOT/$VERSION_FILE | head -1 | awk '{split($0,a," "); print a[2]}'`

fpm -s ${INPUT_TYPE} -t deb -a "${ARCHITECTURE}" -n "${PACKAGE_NAME}" -v "${PACKAGE_VERSION}" --description "${PACKAGE_DESCRIPTION}" --url "${PACKAGE_URL}" -m "${PACKAGE_MAINTAINER}" --deb-changelog "$ALUSUS_ROOT/${DEB_CHANGE_LOG}" --after-install "$ALUSUS_ROOT/Tools/Package_Scripts/${AFTER_INSTALL_SCRIPT}" --after-remove "$ALUSUS_ROOT/Tools/Package_Scripts/${AFTER_REMOVAL_SCRIPT}" --directories "${INSTALL_PATH}" "${INSTALL_PATH}" || { FailMessage "Creating Debian Package."; exit 1; }

fpm -s ${INPUT_TYPE} -t rpm -a "${ARCHITECTURE}" -n "${PACKAGE_NAME}" -v "${PACKAGE_VERSION}" --description "${PACKAGE_DESCRIPTION}" --url "${PACKAGE_URL}" -m "${PACKAGE_MAINTAINER}" --after-install "$ALUSUS_ROOT/Tools/Package_Scripts/${AFTER_INSTALL_SCRIPT}" --after-remove "$ALUSUS_ROOT/Tools/Package_Scripts/${AFTER_REMOVAL_SCRIPT}" --directories "${INSTALL_PATH}" "${INSTALL_PATH}" || { FailMessage "Creating RPM Package."; exit 1; }

SuccessMessage "Creating installation packages."
