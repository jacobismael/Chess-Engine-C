#!/bin/bash

valgrind --leak-check=full -v --track-origins=yes --show-leak-kinds=all ./chess y y y
