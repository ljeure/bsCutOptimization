#!/bin/bash
DOTMVA=1
PLOTROC=0 # doesn't work
GETCUTVAL=0 # also doesn't work

PTBIN=(10 60) #for pp
COLSYST=('PbPb') #('PbPb')
isPbPb=1
MVA=('CutsGA')
nvIni=0
nVAR=1

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
	    cd myTMVA/
	    echo "-- Processing pT bin: ${PTBIN[i]} - ${PTBIN[i+1]} GeV/c"
	    echo
	    root -l -b -q 'TMVAClassification2.C+("'${MVA[k]}'",'$isPbPb','${PTBIN[i]}','${PTBIN[i+1]}')'
	    mv weights/TMVAClassification_${MVA[k]}.weights.xml weights/TMVA_${MVA[k]}_${COLSYST[j]}_${PTBIN[i]}_${PTBIN[i+1]}.weights.xml
	    mv weights/TMVAClassification_${MVA[k]}.class.C weights/TMVA_${MVA[k]}_${COLSYST[j]}_${PTBIN[i]}_${PTBIN[i+1]}.class.C
	    cd ..  
      i=$(($i+1))
    done
    j=$(($j+1))
  done
  k=$(($k+1))
done

fi


if [ $PLOTROC -eq 1 ]; then
	cd plotROC
	j=0
	while ((j<$nCOL))
	do
    	i=0
		while ((i<$nPT))
    	do
			k=0
			while ((k<$nMVA))
			do
				#root -l -b -q 'plotROC_MethodWise.cc++('\"${MVA[k]}\"','\"${COLSYST[j]}\"','${PTBIN[i]}','${PTBIN[i+1]}','${nvIni}','${nVAR}','0')'
				#root -l -b -q 'plotROC_MethodWise.cc++('\"${MVA[k]}\"','\"${COLSYST[j]}\"','${PTBIN[i]}','${PTBIN[i+1]}','${nvIni}','${nVAR}','1')'
				#rm plotROC_MethodWise_cc_ACLiC_dict_rdict.pcm  plotROC_MethodWise_cc.d  plotROC_MethodWise_cc.so
			  k=$(($k+1))
			done
			s=$nvIni
      echo "s "
      echo "$s"
      echo "nvIni"
      echo "$nvIni"
			while ((s<$nVAR))
			do
        echo "do root stagewise"
				root -l -b -q 'plotROC_StageWise.cc++('\"${MVAStr}\"','\"${COLSYST[j]}\"','${PTBIN[i]}','${PTBIN[i+1]}','$(($s+1))','0')'
				root -l -b -q 'plotROC_StageWise.cc++('\"${MVAStr}\"','\"${COLSYST[j]}\"','${PTBIN[i]}','${PTBIN[i+1]}','$(($s+1))','1')'
				rm plotROC_StageWise_cc_ACLiC_dict_rdict.pcm  plotROC_StageWise_cc.d  plotROC_StageWise_cc.so
			    s=$(($s+1))
			done
		    i=$(($i+1))
		done
		j=$(($j+1))
	done
	cd ../
fi

EffB=0.1
if [ $GETCUTVAL -eq 1 ]; then
    cd plotROC
    j=0
    while ((j<$nCOL))
    do
        i=0
        while ((i<$nPT))
        do
            k=0
            while ((k<$nMVA))
            do
	            s=nvIni
	            while ((s<$nVAR))
    	        do
	                root -l -b -q 'getCutGivenEffB.cc++('\"${MVA[k]}\"','\"${COLSYST[j]}\"','${PTBIN[i]}','${PTBIN[i+1]}','$(($s+1))','$EffB')'
    	            rm getCutGivenEffB_cc.d getCutGivenEffB_cc.so getCutGivenEffB_cc_ACLiC_dict_rdict.pcm
        	        s=$(($s+1))
	            done
                k=$(($k+1))
            done
            i=$(($i+1))
        done
        j=$(($j+1))
    done
    cd ../
fi
