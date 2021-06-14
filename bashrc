export PVM_ROOT="/usr/lib/pvm3"
export PVM_ARCH=`$PVM_ROOT/lib/pvmgetarch`
export PVM_PATH=$HOME/pvm3/bin/$PVM_ARCH
export PVM_SRC=$HOME/pvm3/src
export PATH=$PATH:$PVM_ROOT/bin:$PVM_ROOT/lib:$PVM_HOME
export XPVM_ROOT=/usr/bin/xpvm
export PVM_RSH=`which ssh`

printf "%s\n%s\n" conf quit|${PVM_ROOT}/lib/pvm machines.conf

mkdir -p $PVM_PATH

gcc master.c -o master -lpvm3
gcc slave.c -o slave -lpvm3
cp master slave $PVM_PATH
pvm
spawn master