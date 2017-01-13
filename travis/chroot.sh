##############################################################################
# Constants and defaults
CHROOT_DEB_MIRROR=http://archive.debian.org/debian
CHROOT_DEB_VERSION=lenny

CHROOT_HOST_OS="${HOST_OS}"
CHROOT_ARCH="${ARCH}"
CHROOT_SYNC_SOURCES="${SOURCE_DIR}"

CHROOT_DIR=/home/chroot-test
# FIXME CHROOT_DIR=/tmp/chroot-${CHROOT_ARCH}
CHROOT_NAME="${CHROOT_DEB_VERSION}-${CHROOT_ARCH}-sbuild"

# Debian package dependencies for the host
CHROOT_HOST_DEPENDENCIES="debootstrap qemu-user-static binfmt-support"

# Debian package dependencies for the chrooted environment
CHROOT_GUEST_DEPENDENCIES="fakeroot,build-essential,git,sudo"

# Flag file for noticing we're in chroot
CHROOT_FLAG_FILE="/.chroot-${CHROOT_DEB_VERSION}-${CHROOT_ARCH}"

# Emulator binary
CHROOT_EMULATOR="/usr/bin/qemu-${CHROOT_ARCH}-static"

##############################################################################
# Utility Functions

function chroot_save_env {
  (set -o posix ; set) > "${CHROOT_DIR}/environment"
}

function chroot_prepare_host {
  # Install host dependencies
  sudo apt-get install -qq -y ${CHROOT_HOST_DEPENDENCIES}
}

function chroot_create_chroot {
  chroot_clean
  sudo mkdir "${CHROOT_DIR}"

  sudo debootstrap --arch="${CHROOT_ARCH}" --variant=buildd --verbose --foreign \
      --include="${CHROOT_GUEST_DEPENDENCIES}" --components=main,restricted \
      --no-check-gpg --resolve-deps \
      "${CHROOT_DEB_VERSION}" "${CHROOT_DIR}" "${CHROOT_DEB_MIRROR}"

# FIXME remove?
#  sudo sbuild-createchroot --arch="${CHROOT_ARCH}" --foreign --setup-only \
#      "${CHROOT_DEB_VERSION}" "${CHROOT_DIR}" "${CHROOT_DEB_MIRROR}"

  sudo chroot "${CHROOT_DIR}" ./debootstrap/debootstrap --second-stage
}

function chroot_prepare_chroot {
  # Copy emulator binary
  sudo cp "${CHROOT_EMULATOR}" "${CHROOT_DIR}/usr/bin/"

  # Update the chroot's Apt repository
  sudo sed -i.sed 's/httpredir/archive/' "${CHROOT_DIR}/etc/apt/sources.list"
  sudo chroot "${CHROOT_DIR}" "${CHROOT_EMULATOR}" /usr/bin/apt-get \
      -o Acquire::CompressionTypes::Order::=bz2 update
}

function chroot_sync_sources {
  # Create dir and copy files to our chroot environment
  sudo mkdir -p "${CHROOT_DIR}/${CHROOT_SYNC_SOURCES}"
  sudo rsync -av "${CHROOT_SYNC_SOURCES}/" "${CHROOT_DIR}/${CHROOT_SYNC_SOURCES}/"
}

function chroot_mark_done {
  touch "${CHROOT_DIR}/${CHROOT_FLAG_FILE}"
}


##############################################################################
# API Functions

function chroot_clean {
  sudo rm -rf "${CHROOT_DIR}"
  # FIXME remove if we don't use sbuild
  sudo rm -rf "/etc/sbuild/chroot/${CHROOT_NAME}"
  sudo rm -rf "/etc/schroot/chroot.d/${CHROOT_NAME}*"
}

function chroot_setup {
  # Setup tasks
  chroot_prepare_host
  chroot_create_chroot
  chroot_prepare_chroot
  chroot_save_env
  chroot_sync_sources
  chroot_mark_done
}

function chroot_try_enter {
  # If we're already inside the chroot, we don't need to do anything more except
  # restore the environment.
  if [ -e "${CHROOT_FLAG_FILE}" ] ; then
    echo "Running inside chrooted environment"

    . /environment

    return 0
  fi

  # Otherwise, check if the host is supported for chroot emulation
  case "${CHROOT_HOST_OS}" in
    osx|Darwin)
      echo "Won't emulate on host OS '${CHROOT_HOST_OS}', nothing to do."
      return 0
      ;;
    *)
      ;;
  esac

  # Great, now we re-run the same script we're already in, but inside the
  # chroot.
  echo "Setting up chroot on '${CHROOT_HOST_OS}' for '${CHROOT_ARCH}'..."
  chroot_setup

  echo "Switching to chroot."
  sudo chroot "${CHROOT_DIR}" "${CHROOT_EMULATOR}" \
      /bin/bash -c "cd '${CHROOT_SYNC_DIR}' && '${BASH_SOURCE[0]}'"
}
