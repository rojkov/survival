#!/bin/sh

COV_INFO=/tmp/merged_coverage.info

lcov -c -b out/Coverage -d out/Coverage -o $COV_INFO

genhtml $COV_INFO -o out/Coverage/report

rm -f $COV_INFO
