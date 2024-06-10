#!/usr/bin/sh

DIR_SYNTHETIC_DATA="SyntheticData"
DIR_RESULTS="Results_m1_w_qgd"
BASEDIR_EULER_RESULTS="eulerResult"
BASEDIR_QUAT_RESULTS="quatResult"


cd $DIR_SYNTHETIC_DATA

# ***** delete previous simulation data ***** 
#rm -rf *.dat
#rm -rf *.cfg
#rm -rf $DIR_RESULTS/*


# ***** Generate 1000 synthetic IMU datasets *****
#python3 genData.py 1000 -c 4


# ***** delete previous results *****
#rm -rf $DIR_RESULTS/*


# ***** run fusion with sense *****
#for file in ../*cfg; do
#    ./sense -c $file

pwd

ls $DIR_RESULTS
cd $DIR_RESULTS


# ***** compress all results folders *****
for file in ../*cfg; do
    echo "converting $file ..."
    NUMBER=$(echo "$file" | grep -Eo "[[:digit:]]+")
    DIR_EULER_RESULTS="${BASEDIR_EULER_RESULTS}_${NUMBER}"    
    DIR_QUAT_RESULTS="${BASEDIR_QUAT_RESULTS}_${NUMBER}"    
#    mkdir $DIR_EULER_RESULTS
#    cd $DIR_EULER_RESULTS
#    touch test.txt
#    cd ..
    zip -r $DIR_EULER_RESULTS.zip $DIR_EULER_RESULTS
    zip -r $DIR_QUAT_RESULTS.zip $DIR_QUAT_RESULTS
    rm -r $DIR_EULER_RESULTS
    rm -r $DIR_QUAT_RESULTS
done


cd ..


# google => linux find and compress all subfolders
# find . -type d -maxdepth 1 -exec zip -r {}.zip {} \;




#for folder in $DIR_RESULTS/$BASEDIR_EULER_RESULTS*; do
#    zip -r $folder.zip $folder
#done

#for folder in quatResult*; do
#    zip -r $folder.zip $folder
#done

#cd ..
# ***** run script for euler analysis on all results *****
#mkdir ./SyntheticData/Results/convergence
#rm -rf ./SyntheticData/Results/convergence/*
#for folder in ./SyntheticData/Results/euler*; do python3 euler_analysis.py $folder; done

# ***** calculate average convergence *****
#python3 averageConvergence.py

# ***** plot results *****
#python3 plot.py

