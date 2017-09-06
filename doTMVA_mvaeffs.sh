#!/bin/bash
DOBDTPLOTS=1

PTBIN=(10 60)
COLSYST=('pp')
MVA=('MLP')

nPT=$((${#PTBIN[@]}-1))
nCOL=${#COLSYST[@]}
nMVA=${#MVA[@]}

echo "-- Number of methods: $nMVA"
echo "-- Number of pt bins: $nPT"

if [ $DOBDTPLOTS -eq 1 ]; then
k=0
  while ((k<$nMVA))
   do 
    j=0
     while ((j<1))
      do
       i=0
         while ((i<$nPT))
          do 
           cd myTMVA
           echo "-- Processing pT bin: ${PTBIN[i]} - ${PTBIN[i+1]} GeV/c"
           echo
           root -l -q  'mvaeffs.C+("'${MVA[k]}'","'${COLSYST[j]}'",'${PTBIN[i]}','${PTBIN[i+1]}')'
           cd ..
         i=$(($i+1))
         done
      j=$(($j+1))
      done
   k=$(($k+1))
done
fi

