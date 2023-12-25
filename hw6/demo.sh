#! /bin/bash

EXTRACT_DIR="test"
ANSWER_DIR="answer"

TESTCASE="test.tar.xz"

# Reference answer
HARD_LINK_CNT="hardlink.txt"

# argv[1]
PROGRAM_PATH=$( readlink -f $1 )


# Prepare directory
if [ -d "$EXTRACT_DIR" ]; then
    rm -rf $EXTRACT_DIR/*  
else
    mkdir -p $EXTRACT_DIR       
fi

if [ -d "$ANSWER_DIR" ]; then
    rm -rf $ANSWER_DIR/*  
else
    mkdir -p $ANSWER_DIR     
fi


# Extract 
tar -xf $TESTCASE --directory=$EXTRACT_DIR
tar -xf $TESTCASE --directory=$ANSWER_DIR


# Execute & time
elapsed_time=$( { time -p "${PROGRAM_PATH}" $EXTRACT_DIR; } 2>&1 )

real_time=$(echo "$elapsed_time" | grep "real" | awk '{print $2}')


# Efficiency validate
# threshold=4.5
if (( $(echo "$real_time < 4.5 " | bc -l) )); then
    echo -e "Time\t:\t[0;32mEfficiency[m"
else
    echo -e "Time\t:\t[0;31mSlow[m : ${real_time}"
fi


# Compare hard link count
diff -w $HARD_LINK_CNT <(find $EXTRACT_DIR -type f -exec ls -l {} + | awk '{print $2, $NF}' | sort -k 2 -d) > /dev/null

if [ $? -eq 0 ]; then
    echo -e "HardLink:\t[0;32mCorrect[m"
else
    echo -e "HardLink:\t[0;31mWrong[m"
fi

# Compare file content
diff -rq $EXTRACT_DIR $ANSWER_DIR

if [ $? -eq 0 ]; then
    echo -e "File\t:\t[0;32mCorrect[m"
else
    echo -e "File\t:\t[0;31mWrong[m"
fi
