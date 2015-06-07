#!/bin/bash

echo "/**************************************************************************************************************************************"
echo "* Project:       discoverpixy"
echo "* Authors:       Aaron Schmocker, Timo Lang"
echo "* Institution:   BFH Bern University of Applied Sciences"
echo "* File:          $1"
echo "*"
echo "* Version History:"
echo "* Date			Autor Email			SHA		Changes"

git log --pretty=format:"* %ad%x09%ae%x09%h%x09%s" --date=short --date-order --no-merges --reverse $1 | grep -v -i "fileheader"

echo "*"
echo "**************************************************************************************************************************************/"
