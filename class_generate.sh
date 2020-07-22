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

print_help() {
    echo "-c to specify class name"
    echo "-d to specify directory"

}

while getopts 'c:d:h' flag; do
    case "${flag}" in
        h) print_help
            exit 1;;
        c) class_name="${OPTARG}" ;;  
        d) generate_files "${OPTARG}" "$class_name" ;;
        
    esac
done 
    