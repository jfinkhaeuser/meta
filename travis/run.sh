#!/bin/bash
# Based on instructions from https://www.tomaz.me/2013/12/02/running-travis-ci-tests-on-arm.html

set -e
set -x

CHROOT_ARCH=${ARCH}
CHROOT_DIR=/tmp/chroot-${CHROOT_ARCH}
MIRROR=http://archive.debian.org/debian
DEB_VERSION=lenny

# Debian package dependencies for the host
HOST_DEPENDENCIES="debootstrap qemu-user-static binfmt-support sbuild"

# Debian package dependencies for the chrooted environment
GUEST_DEPENDENCIES="build-essential git m4 sudo python"

function setup_chroot {
    # Host dependencies
    sudo apt-get install -qq -y ${HOST_DEPENDENCIES}

    # Create chrooted environment
    sudo mkdir ${CHROOT_DIR}
    sudo debootstrap --foreign --no-check-gpg --include=fakeroot,build-essential \
        --arch=${CHROOT_ARCH} ${DEB_VERSION} ${CHROOT_DIR} ${MIRROR}
    sudo cp /usr/bin/qemu-${CHROOT_ARCH}-static ${CHROOT_DIR}/usr/bin/
    sudo chroot ${CHROOT_DIR} ./debootstrap/debootstrap --second-stage
    sudo sbuild-createchroot --arch=${CHROOT_ARCH} --foreign --setup-only \
        ${DEB_VERSION} ${CHROOT_DIR} ${MIRROR}

    # Create file with environment variables which will be used inside chrooted
    # environment
    echo "export ARCH=${ARCH}" > envvars.sh
    echo "export TRAVIS_BUILD_DIR=${TRAVIS_BUILD_DIR}" >> envvars.sh
    chmod a+x envvars.sh

    # Install dependencies inside chroot
    sudo chroot ${CHROOT_DIR} apt-get update
    sudo chroot ${CHROOT_DIR} apt-get --allow-unauthenticated install \
        -qq -y ${GUEST_DEPENDENCIES}

    # Create build dir and copy travis build files to our chroot environment
    sudo mkdir -p ${CHROOT_DIR}/${TRAVIS_BUILD_DIR}
    sudo rsync -av ${TRAVIS_BUILD_DIR}/ ${CHROOT_DIR}/${TRAVIS_BUILD_DIR}/

    # Indicate chroot environment has been set up
    sudo touch ${CHROOT_DIR}/.chroot_is_done

    # Call ourselves again which will cause tests to run
    sudo chroot ${CHROOT_DIR} bash -c "cd ${TRAVIS_BUILD_DIR} && ./travis/run.sh"
}

if [ -e "/.chroot_is_done" ]; then
  # We are inside chroot
  echo "Running inside chrooted environment"

  . ./envvars.sh
else
  if [ "${ARCH}" != "native" ]; then
    if [ "${TRAVIS_OS_NAME}" = "osx" ]; then
      echo "Won't emulate on OS X, all done!"
      exit 0
    fi

    # Emulated test run, need to set up chrooted environment first
    echo "Setting up chrooted environment"
    setup_chroot
  fi
fi

echo "Running tests"
echo "Environment: $(uname -a)"

cd "${TRAVIS_BUILD_DIR}"
./travis/before_install.sh
./travis/install.sh
./travis/build.sh
