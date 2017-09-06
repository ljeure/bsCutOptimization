#!/bin/bash
DOTMVA=1

PTBIN=(10 60)
COLSYST=('PbPb')
MVA=('CutsGA')

nPT=$((${#PTBIN[@]}-1))
nCOL=${#COLSYST[@]}
nMVA=${#MVA[@]}

echo "-- Number of methods: $nMVA"
echo "-- Number of pt bins: $nPT"

if [ $DOTMVA -eq 1 ]; then
k=0

while ((k<$nMVA))
do
j=0
    while ((j<$nCOL))
    do
    i=0
	while ((i<$nPT))
	do
	    
	    cd readxml
	    echo "-- Processing pT bin: ${PTBIN[i]} - ${PTBIN[i+1]} GeV/c"
	    echo
            root -l -q -b 'readxml.cc+(1,"'${MVA[k]}'",'$j','${PTBIN[i]}','${PTBIN[i+1]}')'
            cd ..

           i=$(($i+1))
         done
     j=$(($j+1))
    done
k=$(($k+1))
done

fi

