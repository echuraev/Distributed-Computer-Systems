#!/bin/bash

make clean

ps -ax | grep rmiregistry | awk '{if ($5 == "rmiregistry") {print $1; system("kill -9 "$1)}}'
