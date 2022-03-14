#!/bin/bash
pathappend() {
    if [ -z "${!1}" ]; then # Make sure the variable exists and is exported
        eval "${1}="""
    fi
    for ARG in "${@:2}"
    do
        if [[ ":${ARG}:" != *":${!1}:"* ]]; then
            eval "${1}=$ARG:${!1}"
        fi
    done
    export ${1}
}


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
DIR=$(dirname $(dirname ${DIR}))

BUILDMODE=opt
export SYSTEMINSTALL=cc7

source /cvmfs/sft.cern.ch/lcg/contrib/gcc/8.3.0/x86_64-centos7-gcc8-opt/setup.sh
source /cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/gdb/9.1/x86_64-centos7-gcc8-opt/gdb-env.sh
source /cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/ROOT/v6.22.00/x86_64-centos7-gcc8-${BUILDMODE}/bin/thisroot.sh
source /cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/podio/00.11/x86_64-centos7-gcc8-${BUILDMODE}/podio-env.sh
## after changing the version of Geant4, the GDML schema check should be performed once for all GDML files.
source /cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/Geant4/10.06.p02/x86_64-centos7-gcc8-${BUILDMODE}/bin/geant4.sh
source /cvmfs/sft.cern.ch/lcg/releases/clang/8.0.0.1/x86_64-centos7/setup.sh # For clang-format
source /cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/doxygen/1.8.15/x86_64-centos7-gcc8-opt/doxygen-env.sh
export LCGDIR=/cvmfs/sft.cern.ch/lcg/views/LCG_98python3/x86_64-centos7-gcc8-${BUILDMODE}
export VECGEOMDIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/VecGeom/v1.1.5/x86_64-centos7-gcc8-${BUILDMODE}
export BOOST=/cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/Boost/1.73.0/x86_64-centos7-gcc8-${BUILDMODE}
export PYTORCH=/cvmfs/na62.cern.ch/offline/NA62FW/pytorch/1.5.0-rc3/x86_64-centos7-gcc8-${BUILDMODE}
export XERCESCDIR=/cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/XercesC/3.2.3/x86_64-centos7-gcc8-${BUILDMODE}
export SQLITE=/cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/sqlite/3320300/x86_64-centos7-gcc8-${BUILDMODE}
export XROOTD=/cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/xrootd/4.12.3/x86_64-centos7-gcc8-${BUILDMODE}
export FFTW3=/cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/fftw3/3.3.8/x86_64-centos7-gcc8-${BUILDMODE}
export TBB=/cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/tbb/2020_U2/x86_64-centos7-gcc8-${BUILDMODE}
export CMAKE=/cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/CMake/3.17.3/x86_64-centos7-gcc8-${BUILDMODE}
export GSL=/cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/GSL/2.6/x86_64-centos7-gcc8-${BUILDMODE}
export CLHEP=/cvmfs/sft.cern.ch/lcg/releases/LCG_98python3/clhep/2.4.1.3/x86_64-centos7-gcc8-${BUILDMODE}
export QTDIR=/cvmfs/sft.cern.ch/lcg/releases/qt5/5.12.4-59ee6/x86_64-centos7-gcc8-${BUILDMODE}

pathappend CMAKE_PREFIX_PATH ${XERCESCDIR} ${VECGEOMDIR}
export QTINC=${QTDIR}/include
export QTLIB=${QTDIR}/lib
export QT_PLUGIN_PATH=${QTDIR}/plugins
export FONTCONFIG_PATH=/etc/fonts

# add paths to CMAKE_PREFIX_PATH
pathappend CMAKE_PREFIX_PATH ${ROOTSYS} ${CLHEP} ${QTDIR}

pathappend ROOT_INCLUDE_PATH ${ROOTSYS}/include
pathappend LD_LIBRARY_PATH ${LCGDIR}/lib ${LCGDIR}/lib64 ${XROOTD}/lib64 $TBB/lib
pathappend LD_LIBRARY_PATH $BOOST/lib $SQLITE/lib $FFTW3/lib $GSL/lib $VECGEOMDIR/lib #$XERCESCDIR/lib
pathappend LD_LIBRARY_PATH ${ROOUNFOLD}
pathappend ROOT_INCLUDE_PATH ${ROOUNFOLD}
pathappend PATH ${CMAKE}/bin ${SQLITE}/bin ${XROOTD}/bin

export CC=`which gcc`
export CXX=`which g++`
gccPath=`which gcc`
pathappend CMAKE_PREFIX_PATH ${CLHEP}
