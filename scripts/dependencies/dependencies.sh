#
# This file is part of build-dependencies.
#
# Author(s): Jens Finkhaeuser <jens@finkhaeuser.de>
#
# Copyright (c) 2017 Jens Finkhaeuser
#
# This software is licensed under the terms of the GNU GPLv3 for personal,
# educational and non-profit use. For all other uses, alternative license
# options are available. Please contact the copyright holder for additional
# information, stating your intended usage.
#
# You can find the full text of the GPLv3 in the COPYING file in this code
# distribution.
#
# This software is distributed on an "AS IS" BASIS, WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

##############################################################################
# Utility Functions

function dep_osname {
  uname | tr -cd '[[:alnum:]]._-' | tr '[:upper:]' '[:lower:]'
}

function dep_read_dependencies {
  local deps="$(cat "${DEP_SOURCE_DIR}/${DEP_OSNAME}.txt" | grep -v '^#')"

  DEP_DEPENDENCIES_MANDATORY=
  DEP_DEPENDENCIES_OPTIONAL=
  local dep
  for dep in ${deps} ; do
    local flag="$(echo "${dep}" | cut -d: -f2)"
    dep="$(echo "${dep}" | cut -d: -f1)"
    test "${flag}" = "${dep}" && flag=1
    echo "Depdendency ${dep} is ${flag}"

    if [ "${flag}" = 0 ] ; then
      DEP_DEPENDENCIES_OPTIONAL="${DEP_DEPENDENCIES_OPTIONAL} ${dep}"
    else
      DEP_DEPENDENCIES_MANDATORY="${DEP_DEPENDENCIES_MANDATORY} ${dep}"
    fi
  done
}

##############################################################################
# API Functions

function dep_install {
  # Get source directory
  DEP_SOURCE_DIR="${1}"
  test -z "${DEP_SOURCE_DIR}" && DEP_SOURCE_DIR="$(pwd)"
  test -d "${DEP_SOURCE_DIR}" || exit 1
  shift

  # Determine OS name
  DEP_OSNAME="$(dep_osname)"
  test -z "${DEP_OSNAME}" && exit 1

  # Find script directory
  local script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
  DEP_OSDIR="${script_dir}/${DEP_OSNAME}"

  if [ ! -d "${DEP_OSDIR}" ] ; then
    echo "Unsupported OS ${DEP_OSNAME}, aborting!"
    exit 2
  fi

  echo "OS is ${DEP_OSNAME}"
  echo "OS scripts are in '${DEP_OSDIR}'"

  # Update the package index
  source "${DEP_OSDIR}/update.sh"

  # Load installation function
  source "${DEP_OSDIR}/install.sh"

  dep_read_dependencies
  dep_install_packages # from install.sh
}
