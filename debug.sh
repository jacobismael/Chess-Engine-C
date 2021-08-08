#!/bin/bash

valgrind --leak-check=full -v --track-origins=yes --show-leak-kinds=all ./chess -w bot4 -b bot1 
