#!/bin/bash

#月份名转月份序号
declare -A name_to_number=(
	["jan"]=1
	["feb"]=2
	["mar"]=3
	["apr"]=4
	["may"]=5
	["jun"]=6
	["jul"]=7
	["aug"]=8
	["sep"]=9
	["oct"]=10
	["nov"]=11
	["dec"]=12
)

#月份序号转完整月份名
number_to_name=(
	"January"
	"February"
	"March"
	"April"
	"May"
	"June"
	"July"
	"August"
	"September"
	"October"
	"November"
	"December"
)

#计算指定月有几天
cal_days_in_month() {
	n_year=`expr $1 + 0`
	n_month=`expr $2 + 0`
	n_day=0;
	case $n_month in
		1|3|5|7|8|10|12)
			n_day=31;;
		4|6|9|11) 
			n_day=30;;
		2)
			if [ `expr $n_year % 4` -eq 0 ];then
				if [ `expr $n_year % 400` -eq 0 ];then
					n_day=29;
				elif [ `expr $n_year % 100` -eq 0 ];then
					n_day=28;
				else
					n_day=29;
				fi
			else
				n_day=28;
			fi
			;;
	esac
	echo $n_day
}

if [ ! -n "$2" ]; then
	year=`date +%Y`
	n_month=`date +%m`
else
	year=$2
	case "$1" in
		[1-9]|1[0-2])
			n_month=$1 #数字形式的月份
			;;
		*)
			n_month=${name_to_number[${1:0:3}]} #月份名
			if [ ! -n "$n_month" ]; then
				echo "invalid month $1"
				exit -1
			fi
			;;
	esac
fi
month=${number_to_name[$n_month-1]}
start_day=`date -d "$month 1 $year" +%w` #该月1号是星期几
let start_day=start_day%7
total=`cal_days_in_month $year $n_month`

#居中显示标题
title="$month $year"
let title_width=${#title}
let indent=(20-title_width)/2
for ((i=1; i<=$indent; i++))
do
	echo -n " "
done
echo $month $year

echo "Su Mo Tu We Th Fr Sa"
let now=1
for ((i=1; i<=$start_day; i++))
do
	echo -n "   "
	let now++
done

let today_year=`date +%Y`
let today_month=`date +%m`
let today_day=`date +%d`

for ((i=1; i<=$total; i++))
do
	if [ $today_year -eq $year ] && [ $today_month -eq $n_month ] && [ $today_day -eq $i ]; then
		#突出显示当天
		echo -e -n "\033[47;30m"
		printf "%2s" $i
		echo -e -n "\033[0m"
		echo -n " "
	else
		printf "%2s " $i
	fi
	let row=now%7
	let now++
	if [ $row -eq 0 ]; then
		echo
	fi
done

# if [ ! $row -eq 0 ]; then
# 	echo
# fi
echo
