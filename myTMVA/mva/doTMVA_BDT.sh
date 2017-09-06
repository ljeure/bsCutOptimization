#!/bin/bash

PTBIN=(10 60)
COLSYST=('PbPb')
isPbPb=1
MVA=('BDT')

nPT=$((${#PTBIN[@]}-1))
nMVA=${#MVA[@]}
nCOL=${#COLSYST[@]}

echo "-- Number of methods: $nMVA"
echo "-- Number of pt bins: $nPT"

i=0
j=0
k=0

cd myTMVA
while ((k<1))
  do
  j=0
  while ((j<$nCOL))
    do
    i=0
	  while ((i<$nPT))
	    do
	    echo "-- Processing pT bin: ${PTBIN[i]} - ${PTBIN[i+1]} GeV/c"
	    echo
      root -l -b -q 'classmaker.C("'../weights/TMVA_${MVA[k]}_${COLSYST[j]}_${PTBIN[i]}_${PTBIN[i+1]}.class.C'")'
	    root -l -b -q 'BDT.C('$isPbPb','${PTBIN[i]}','${PTBIN[i+1]}',"'${MVA[k]}'")' 
	    i=$(($i+1))
	  done
    j=$(($j+1))
  done
  k=$(($k+1))
done
