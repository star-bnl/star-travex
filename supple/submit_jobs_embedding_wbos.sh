#!/bin/sh

sample=Wplus_enu
#sample=Wplus_taunu
#sample=Wminus_enu
#sample=Wminus_taunu
#sample=Z_eplus_eminus_inter
#sample=QCD
codePath=/star/u/jlzhang/KFVF/embedding
#outPath=/star/u/jlzhang/data05/wEmbedding2013/${sample}_2013/
outPath=/star/data19/KFVertex/${sample}_2013
inPath=/star/data19/KFVertex/daq
cd JOBS

#create output directories
mkdir -p $outPath/
outPath=$outPath/
mkdir -p $outPath/
mkdir -p $outPath/root/
mkdir -p $outPath/log/
mkdir -p $outPath/out/
mkdir -p $outPath/fzd/

echo outPath=$outPath= 

index=2000
norm=1.0
#norm=1.0 for 110K events (W+)
#norm=0.32 for 35K events (W-)
#norm=0.23 for 25K events (Z/gamma*)

IFS=, #set comma as delimeter
while read fillNumber runNumber m2 m3 m4 m5 m6 m7 nEvents nEventTau nEventZ 
do

  echo $index
  echo $nEvents
  nEvents=$(expr $nEvents*$norm |tr -d $'\r'| bc)
  echo $nEvents
  nEvents=`echo $nEvents | awk '{printf("%d\n",$1+0.5)}'`
  echo $nEvents
  runNumber=${runNumber:1:8}
  echo $runNumber 
  daqFile=`ls $inPath/*$runNumber*.daq`
  echo $daqFile

  #set queue length by # of target events
  filesPerHour=0.01 
  if [ $nEvents -lt 50 ] 
  then
    filesPerHour=0.21
  fi
  if [ $nEvents -lt 10 ]
  then
    filesPerHour=0.34
  fi
  echo $filesPerHour

  star-submit-template -template ../EMBEDDING_TEMPLATE.xml -entities  nEvents=$nEvents,inPath=$inPath,outPath=$outPath,codePath=$codePath,runNumber=$runNumber,daqFile=$daqFile,sample=$sample,nIndex=$index,filesPerHour=$filesPerHour

  index=`expr $index \+ 1`

# exit
done <  ../runlist_wbos.csv
