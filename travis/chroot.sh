##############################################################################
# Utility Functions

function chroot_save_env {
  local tmpfile="$(mktemp)"
  printenv >"$tmpfile"
  sed -i.bak "s/^\([A-Z_]*\)=\(.*\)/\1='\2'/" "$tmpfile"
  sed -i.bak 's/^\(.*\)$/export \1 || true/g' "$tmpfile"
  sudo mv "$tmpfile" "${CHROOT_DIR}/environment"
  cat "${CHROOT_DIR}/environment"
}

function chroot_prepare_host {
  # Install host dependencies
  sudo apt-get install -qq -y ${CHROOT_HOST_DEPENDENCIES}
}

function chroot_create_chroot {
  chroot_clean
  sudo mkdir "${CHROOT_DIR}"

  # Debootstrap first stage
  sudo debootstrap --arch="${CHROOT_ARCH}" --variant=buildd --verbose --foreign \
      --include="${CHROOT_GUEST_DEPENDENCIES}" --components=main,restricted \
      --no-check-gpg --resolve-deps \
      "${CHROOT_DEB_VERSION}" "${CHROOT_DIR}" "${CHROOT_DEB_MIRROR}"

  # Copy emulator binary
  sudo cp "${CHROOT_EMULATOR}" "${CHROOT_DIR}/usr/bin/"

  # Bind proc and dev
  local fs
  for fs in dev proc ; do
    sudo rm -rf "${CHROOT_DIR}/${fs}/*"
    sudo mkdir -p "${CHROOT_DIR}/${fs}"
    sudo mount --bind "/${fs}" "${CHROOT_DIR}/${fs}"
  done

  # Debootstrap second stage in emulator
  sudo chroot "${CHROOT_DIR}" ./debootstrap/debootstrap --second-stage

  # Update the chroot's Apt repository
  sudo sed -i.sed 's/httpredir/archive/' "${CHROOT_DIR}/etc/apt/sources.list"
  sudo chroot "${CHROOT_DIR}" "${CHROOT_EMULATOR}" /usr/bin/apt-get update
}

function chroot_sync_sources {
  # Create dir and copy files to our chroot environment
  sudo mkdir -p "${CHROOT_DIR}/${CHROOT_SOURCE_DIR}"
  sudo rsync -av "${CHROOT_SOURCE_DIR}/" "${CHROOT_DIR}/${CHROOT_SOURCE_DIR}/"
}

function chroot_mark_done {
  sudo touch "${CHROOT_DIR}/${CHROOT_FLAG_FILE}"
}

function chroot_setup {
  # Setup tasks
  chroot_prepare_host
  chroot_create_chroot
  chroot_save_env
  chroot_sync_sources
  chroot_mark_done
}



##############################################################################
# API Functions

function chroot_clean {
  local fs
  for fs in dev proc ; do
    sudo umount -f "${CHROOT_DIR}/${fs}" || true
  done

  sudo rm -rf "${CHROOT_DIR}"
}

##
# usage:
#   chroot_try_enter /path/to/caller/script target-architecture [host-os] [source-dir]
#
# Exit status:
#   0 - we're inside target architecture - might be a chroot, or the host OS
#   1 - host OS can't run a emulated chroot, we're on the host OS
#   3 - exited from inside chroot
function chroot_try_enter {
  ##############################################################################
  # Parameter checking
  local the_script="${1}"
  shift
  CHROOT_ARCH="${1}"
  shift

  local p
  for p in the_script CHROOT_ARCH ; do
    test -z "${!p}" && exit 1
  done

  CHROOT_HOST_OS="${1}"
  test -z "${CHROOT_HOST_OS}" && CHROOT_HOST_OS="$(uname -o)"
  shift

  CHROOT_SOURCE_DIR="${1}"
  test -z "${CHROOT_SOURCE_DIR}" && CHROOT_SOURCE_DIR="$(pwd)"
  shift

  # Otherwise, check if the host is supported for chroot emulation
  case "${CHROOT_HOST_OS}" in
    osx|Darwin)
      echo "Won't emulate on host OS '${CHROOT_HOST_OS}', nothing to do."
      return 1
      ;;
    *)
      ;;
  esac

  # It's possible that the host architecture is already the same as the
  # requested, in which case we don't need to do anything.
  if [ "$(uname -m)" = "${CHROOT_ARCH}" ] ; then
    echo "Host and requested architecture are both ${CHROOT_ARCH}, nothing to do."
    return 0
  fi

  ##############################################################################
  # Constants and defaults
  CHROOT_DEB_MIRROR=http://archive.debian.org/debian
  CHROOT_DEB_VERSION=lenny

  if test -z "${CHROOT_DIR}" ; then
    CHROOT_DIR="/tmp/chroot-${CHROOT_ARCH}"
  fi
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
  # Main part

  # If we're already inside the chroot, we don't need to do anything more except
  # restore the environment.
  if [ -e "${CHROOT_FLAG_FILE}" ] ; then
    echo "Running inside chrooted environment"

    . /environment
    echo "environment is set"

    return 0
  fi

  # Great, now we re-run the same script we're already in, but inside the
  # chroot.
  echo "Setting up chroot on '${CHROOT_HOST_OS}' for '${CHROOT_ARCH}'..."
  chroot_setup

  echo "Switching to chroot."
  . "${CHROOT_DIR}/environment"
  sudo chroot "${CHROOT_DIR}" "${CHROOT_EMULATOR}" \
      /bin/bash -c "cd '${CHROOT_SOURCE_DIR}' && '${the_script}' \"$@\""

  return 3
}
