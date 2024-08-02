#!/usr/bin/bash

for dir in ./*; do
    if [[ -d $dir ]]; then
	mkdir $dir/resources
	for file in $dir/*; do
	    if [[ $file != $dir/resources ]]; then
		mv $file $dir/resources
	    fi
	done
    fi
done

