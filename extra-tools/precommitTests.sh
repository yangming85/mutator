#!/bin/bash

Red="\033[0;31m"
Green="\033[0;32m"
Lblue="\033[1;34m"
Orange="\033[0;33m"
NC="\033[0m"

REP_FILE="test/precommit.rep"

printf "${Lblue}switching to mutator root...\n${NC}" | tee ../test/precommit.rep
cd ..

printf "${Lblue}running make clean...\n${NC}" | tee -a ./test/precommit.rep
"make" clean

printf "${Lblue}testing the build in C++11 mode...\n${NC}" | tee -a ./test/precommit.rep
"make" CXX=clang++ BIULD_MODE=COV_NO_CLANG -j2
echo ""

if [[ $? == 0 ]]; then
  printf "${Green}mutator c++11 test build passed.\n${NC}" | tee -a ./test/precommit.rep
else
  printf "${Red}mutator c++11 test build failed.\n${NC}" | tee -a ./test/precommit.rep
fi

printf "${Lblue}running mutator-lvl0 on the tdd sources...\n${NC}" | tee -a ./test/precommit.rep

printf "${Orange}./mutator-lvl0 -SysHeader=false -MainOnly=true ./test/testFuncs1.c ./test/testFuncs2.c ./test/testFuncs3.c -- -std=c90 -I/lib/gcc/x86_64-redhat-linux/6.3.1/include -Wall > ./test/misra-log\n${NC}" | tee -a ./test/precommit.rep
"./mutator-lvl0" -SysHeader=false -MainOnly=true ./test/testFuncs1.c ./test/testFuncs2.c ./test/testFuncs3.c -- -std=c90 -I/lib/gcc/x86_64-redhat-linux/6.3.1/include -Wall > ./test/misra-log

if [[ $? == 0 ]]; then
  printf "${Green}mutator-lvl0 smoke test on the tdds passed.\n${NC}" | tee -a ./test/precommit.rep
else
  printf "${Red}mutator-lvl0 smoke test on the tdds failed.\n${NC}" | tee -a ./test/precommit.rep
fi

printf "${Lblue}running c++11 mutator-lvl0 xml report schema test...\n${NC}" | tee -a ./test/precommit.rep
"./mutator.sh" -t mutator-lvl0 xsd

if [[ $? == 0 ]];then
  printf "${Green}c++11 mutator-lvl0 xml report xsd passed.\n${NC}" | tee -a ./test/precommit.rep
else
  printf "${Red}c++11 mutator-lvl0 xml report xsd passed.\n${NC}" | tee -a ./test/precommit.rep
fi

printf "${Lblue}running smoke tests on mutator-lvl0...\n${NC}" | tee -a ./test/precommit.rep
printf "${Orange}./mutator-lvl0 -SysHeader=false -MainOnly=true ./test/testFuncs3.h -- -std=c90 -I/lib/gcc/x86_64-redhat-linux/6.3.1/include -Wall > ./test/misra-log\n${NC}" | tee -a ./test/precommit.rep
"./mutator-lvl0" -SysHeader=false -MainOnly=true ./test/testFuncs3.h -- -std=c90 -I/lib/gcc/x86_64-redhat-linux/6.3.1/include -Wall > ./test/misra-log

if [[ $? == 0 ]]; then
  printf "${Green}mutator-lvl0 C++11 smoke test passed...\n${NC}" | tee -a ./test/precommit.rep
else
  printf "${Red}mutator-lvl0 C++11 smoke test failed...\n${NC}" | tee -a ./test/precommit.rep
fi

printf "${Lblue}running make clean...\n${NC}" | tee -a ./test/precommit.rep
"make" clean

printf "${Lblue}testing the build in C++1z mode...\n${NC}" | tee -a ./test/precommit.rep
"make" CXX=clang++ BUILD_MODE=COV_NO_CLANG_1Z -j2
echo ""

if [[ $? == 0 ]]; then
  printf "${Green}mutator c++1z test build passed.\n${NC}" | tee -a ./test/precommit.rep
else
  printf "${Red}mutator c++1z test build failed.\n${NC}" | tee -a ./test/precommit.rep
fi

printf "${Lblue}running mutator-lvl0 on the tdd sources...\n${NC}" | tee -a ./test/precommit.rep
printf "${Orange}./mutator-lvl0 -SysHeader=false -MainOnly=true ./test/testFuncs1.c ./test/testFuncs2.c ./test/testFuncs3.c -- -std=c90 -I/lib/gcc/x86_64-redhat-linux/6.3.1/include -Wall > ./test/misra-log\n${NC}" | tee -a ./test/precommit.rep
"./mutator-lvl0" -SysHeader=false -MainOnly=true ./test/testFuncs1.c ./test/testFuncs2.c ./test/testFuncs3.c -- -std=c90 -I/lib/gcc/x86_64-redhat-linux/6.3.1/include -Wall > ./test/misra-log

if [[ $? == 0 ]]; then
  printf "${Green}mutator-lvl0 smoke test on the tdds passed.\n${NC}" | tee -a ./test/precommit.rep
else
  printf "${Red}mutator-lvl0 smoke test on the tdds failed.\n${NC}" | tee -a ./test/precommit.rep
fi

printf "${Lblue}running c++1z mutator-lvl0 xml report schema test...\n${NC}" | tee -a ./test/precommit.rep
"./mutator.sh" -t mutator-lvl0 xsd

if [[ $? == 0 ]];then
  printf "${Green}c++1z mutator-lvl0 xml report xsd passed.\n${NC}" | tee -a ./test/precommit.rep
else
  printf "${Red}c++1z mutator-lvl0 xml report xsd passed.\n${NC}" | tee -a ./test/precommit.rep
fi

printf "${Lblue}running smoke tests on mutator-lvl0...\n${NC}" | tee -a ./test/precommit.rep
printf "${Orange}./mutator-lvl0 -SysHeader=false -MainOnly=true ./test/testFuncs3.h -- -std=c90 -I/lib/gcc/x86_64-redhat-linux/6.3.1/include -Wall > ./test/misra-log\n${NC}" | tee -a ./test/precommit.rep
"./mutator-lvl0" -SysHeader=false -MainOnly=true ./test/testFuncs3.h -- -std=c90 -I/lib/gcc/x86_64-redhat-linux/6.3.1/include -Wall > ./test/misra-log

if [[ $? == 0 ]]; then
  printf "${Green}mutator-lvl0 C++1z smoke test passed...\n${NC}" | tee -a ./test/precommit.rep
else
  printf "${Red}mutator-lvl0 C++1z smoke test failed...\n${NC}" | tee -a ./test/precommit.rep
fi

printf "${Lblue}cleaning the objects and exexutables...\n${NC}" | tee -a ./test/precommit.rep
"make" clean

printf "${Lblue}running make clean...\n${NC}" | tee -a ./test/precommit.rep
"make" clean

printf "${Lblue}testing the build in C++14 mode...\n${NC}" | tee -a ./test/precommit.rep
"make" CXX=clang++ BUILD_MODE=COV_NO_CLANG_14 -j2
echo ""

if [[ $? == 0 ]]; then
  printf "${Green}mutator c++14 test build passed.\n${NC}" | tee -a ./test/precommit.rep
else
  printf "${Red}mutator c++14 test build failed.\n${NC}" | tee -a ./test/precommit.rep
fi

printf "${Lblue}running mutator-lvl0 on the tdd sources...\n${NC}" | tee -a ./test/precommit.rep
printf "${Orange}./mutator-lvl0 -SysHeader=false -MainOnly=true ./test/testFuncs1.c ./test/testFuncs2.c ./test/testFuncs3.c -- -std=c90 -I/lib/gcc/x86_64-redhat-linux/6.3.1/include -Wall > ./test/misra-log\n${NC}" | tee -a ./test/precommit.rep
"./mutator-lvl0" -SysHeader=false -MainOnly=true ./test/testFuncs1.c ./test/testFuncs2.c ./test/testFuncs3.c -- -std=c90 -I/lib/gcc/x86_64-redhat-linux/6.3.1/include -Wall > ./test/misra-log

if [[ $? == 0 ]]; then
  printf "${Green}mutator-lvl0 smoke test on the tdds passed.\n${NC}" | tee -a ./test/precommit.rep
else
  printf "${Red}mutator-lvl0 smoke test on the tdds failed.\n${NC}" | tee -a ./test/precommit.rep
fi

printf "${Lblue}running c++14 mutator-lvl0 xml report schema test...\n${NC}" | tee -a ./test/precommit.rep
"./mutator.sh" -t mutator-lvl0 xsd

if [[ $? == 0 ]];then
  printf "${Green}c++14 mutator-lvl0 xml report xsd passed.\n${NC}" | tee -a ./test/precommit.rep
else
  printf "${Red}c++14 mutator-lvl0 xml report xsd passed.\n${NC}" | tee -a ./test/precommit.rep
fi

printf "${Lblue}running smoke tests on mutator-lvl0...\n${NC}" | tee -a ./test/precommit.rep
printf "${Orange}./mutator-lvl0 -SysHeader=false -MainOnly=true ./test/testFuncs3.h -- -std=c90 -I/lib/gcc/x86_64-redhat-linux/6.3.1/include -Wall > ./test/misra-log\n${NC}" | tee -a ./test/precommit.rep
"./mutator-lvl0" -SysHeader=false -MainOnly=true ./test/testFuncs3.h -- -std=c90 -I/lib/gcc/x86_64-redhat-linux/6.3.1/include -Wall > ./test/misra-log

if [[ $? == 0 ]]; then
  printf "${Green}mutator-lvl0 C++14 smoke test passed...\n${NC}" | tee -a ./test/precommit.rep
else
  printf "${Red}mutator-lvl0 C++14 smoke test failed...\n${NC}" | tee -a ./test/precommit.rep
fi

printf "${Lblue}cleaning the objects and exexutables...\n${NC}" | tee -a ./test/precommit.rep
"make" clean

cd daemon
printf "${Lblue}changing to the daemon directory\n${NC}" | tee -a ../test/precommit.rep

printf "${Lblue}cleaning all objects and executables\n${NC}" | tee -a ../test/precommit.rep
"make" clean

printf "${Lblue}testing mutatord's build...\n${NC}" | tee -a ../test/precommit.rep
"make" mutatord

if [[ $? == 0 ]]; then
  printf "${Green}mutatord build test passed...\n${NC}" | tee -a ../test/precommit.rep
else
  printf "${Red}mutatord build test failed...\n${NC}" | tee -a ../test/precommit.rep
fi

printf "${Lblue}testing mutatorclient's build...\n${NC}" | tee -a ../test/precommit.rep
"make" mutatorclient

if [[ $? == 0 ]]; then
  printf "${Green}mutatorclient build test passed...\n${NC}" | tee -a ../test/precommit.rep
else
  printf "${Red}mutatorclient build test failed...\n${NC}" | tee -a ../test/precommit.rep
fi

printf "${Lblue}testing mutatorserver's build...\n${NC}" | tee -a ../test/precommit.rep
"make" mutatorserver

if [[ $? == 0 ]]; then
  printf "${Green}mutatorserver build test passed...\n${NC}" | tee -a ../test/precommit.rep
else
  printf "${Red}mutatorserver build test failed...\n${NC}" | tee -a ../test/precommit.rep
fi

printf "${Lblue}cleaning the objects and exexutables...\n${NC}" | tee -a ../test/precommit.rep
"make" clean

printf "${Lblue}finished running all tests...\n${NC}" | tee -a ../test/precommit.rep
