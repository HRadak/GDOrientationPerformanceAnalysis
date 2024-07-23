#!/usr/bin/sh

DIR_EXAMPLE_DATA="ExampleData"
DIR_RESULTS="Results"
BASEDIR_EULER_RESULTS="eulerResult"
BASEDIR_QUAT_RESULTS="quatResult"

cd $DIR_EXAMPLE_DATA

# ***** delete previous results *****
#rm -rf $DIR_RESULTS/*

# ***** run fusion with sense *****
#for file in ./*cfg; do
#    echo $file
#    ./qgd -c $file

pwd

# ***** run script for euler analysis on all results *****
#mkdir ./$DIR_RESULTS/convergence
#rm -rf ./SyntheticData/Results/convergence/*
#for folder in ./$DIR_RESULTS/euler*; do python3 euler_analysis.py $folder; done

# ***** calculate average convergence *****
#python3 averageConvergence.py

# ***** plot results *****
python3 plot.py

done
