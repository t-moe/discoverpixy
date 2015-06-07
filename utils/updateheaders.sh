#!/bin/bash


FILES=`find common/ emulator/ discovery/ -name "*.c" -or -name "*.h" -or -name "*.cpp" | grep -v libs | grep -v /pixy/`


for FILE in $FILES; do
	echo "Adding Header to $FILE"
	
	#remove old header, and format file with astyle
	CONTENT=$(perl -0777 -pe  's%^/\*.*?discoverpixy.*?\*/%%igs' $FILE | astyle --options=./utils/style.astylerc) 

	#add new header
	./utils/genheader.sh $FILE > $FILE
	
	#append file content
	echo "$CONTENT" >> $FILE

done

