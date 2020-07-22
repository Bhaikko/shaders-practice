#!/bin/bash


generate_files() {
    mkdir "$1/$2"
    touch "$1/$2/$2.h"
    touch "$1/$2/$2.cpp"

    mkdir "$1/$2/shaders"
    touch "$1/$2/shaders/$2.vert"
    touch "$1/$2/shaders/$2.frag"
    
}

class_name=''

while getopts 'c:d:' flag; do
    case "${flag}" in
        c) class_name="${OPTARG}" ;;  
        d) generate_files "${OPTARG}" "$class_name" ;;
        
    esac
done 
    