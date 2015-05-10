#!/bin/bash


FILES=`find common/ emulator/ discovery/ -name "*.c" -or -name "*.h" -or -name "*.cpp" | grep -v libs | grep -v /pixy/`

for FILE in $FILES; do
	echo "Adding Header to $FILE"
	
	#remove old header
	CONTENT=$(perl -0777 -pe  's%^/\*.*?discoverpixy.*?\*/%%igs' $FILE)

	#add new header
	./genheader.sh $FILE > $FILE
	
	#append file content
	echo "$CONTENT" >> $FILE

done

