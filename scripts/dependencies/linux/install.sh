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

function dep_install_packages {
  DEBIAN_FRONTEND=noninteractive \
    sudo apt-get -y install ${DEP_DEPENDENCIES_MANDATORY}


  local dep
  for dep in ${DEP_DEPENDENCIES_OPTIONAL} ; do
    DEBIAN_FRONTEND=noninteractive \
      sudo apt-get -y install ${dep} || true
  done
}
