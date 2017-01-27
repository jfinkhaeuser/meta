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

function dep_install_packages_helper {
  local pkg="${1}"

  if brew list | grep -q "${pkg}" ; then
    # Installed
    if brew outdated | grep -q "${pkg}" ; then
      # Outdated
      brew upgrade "$@"
    else
      # Up-to-date
      true
    fi
  else
    # Not installed
    brew install "$@"
  fi
}


function dep_install_packages {
  local dep
  for dep in ${DEP_DEPENDENCIES_MANDATORY} ; do
    dep_install_packages_helper ${dep}
  done

  for dep in ${DEP_DEPENDENCIES_OPTIONAL} ; do
    dep_install_packages_helper ${dep} || true
  done
}
