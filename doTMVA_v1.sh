#!/bin/bash
DOTMVA=0
DOBDTPLOTS=1

PTBIN=(10 60)
COLSYST=('pp')
#MVA=('CutsGA') #'CutsGA')
MVA=('BDT' 'CutsGA' )

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
   # while ((j<1))
    do
    i=0
	while ((i<$nPT))
	do
	    cd myTMVA
	    echo "-- Processing pT bin: ${PTBIN[i]} - ${PTBIN[i+1]} GeV/c"
	    echo
	    root -l -b -q 'TMVAClassification.C+("'${MVA[k]}'",'$j','${PTBIN[i]}','${PTBIN[i+1]}')'
	    mv weights/TMVAClassification_${MVA[k]}.weights.xml weights/TMVA_${MVA[k]}_${COLSYST[j]}_${PTBIN[i]}_${PTBIN[i+1]}.weights.xml
	    mv weights/TMVAClassification_${MVA[k]}.class.C weights/TMVA_${MVA[k]}_${COLSYST[j]}_${PTBIN[i]}_${PTBIN[i+1]}.class.C
	    
           # if [ "${MVA[k]}" != "CutsSA" -a "${MVA[k]}" != "CutsGA" ]; then
               
           # cd ../mva/
           # root -l -b -q 'classmaker.C("'../myTMVA/weights/TMVA_${MVA[k]}_${COLSYST[j]}_${PTBIN[i]}_${PTBIN[i+1]}.class.C'")'
           # root -l -b -q ''${MVA[k]}'.C('$j','${PTBIN[i]}','${PTBIN[i+1]}',"'${MVA[k]}'")'
            
            #cd ..
            #else 
            #cd ../readxml
            #root -l -b -q 'readxml.cc+(1,"'${MVA[k]}'",'$j','${PTBIN[i]}','${PTBIN[i+1]}')'
            #cd ..
#            fi

           i=$(($i+1))
         done
     j=$(($j+1))
    done
k=$(($k+1))
done

fi

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
           cd /data/leure/bsTMVACDozen/myTMVA
           echo "-- Processing pT bin: ${PTBIN[i]} - ${PTBIN[i+1]} GeV/c"
           echo
           root -l -q -b 'mvaeffs.C+("'${MVA[k]}'","'${COLSYST[j]}'",'${PTBIN[i]}','${PTBIN[i+1]}')'
           cd ..
         i=$(($i+1))
         done
      j=$(($j+1))
      done
   k=$(($k+1))
done
fi

