#!/bin/bash
target_dir=`readlink -f $1`
short_dir=$1
process_short_dir()
{
    dir=$1
    len=${#dir}
    tail_idx=$(($len-1))
    tail_char=${dir:$tail_idx}
    slash='/'
    if [ "$tail_char" == "$slash" ]
    then
    dir=${dir%/*} # strip last /
    fi
    #${var##*/} take leaf dir name 
    short_dir=${dir##*/}
    
    # fix the issue of .
    dot='.'
    if [ "$short_dir" == "$dot" ]
    then
    dir=$target_dir
    short_dir=${dir##*/}
    fi
}

process_short_dir $short_dir
output_path=`readlink -f $2`

print_files_of_a_dir()
{
   OIFS=$IFS
   IFS=$'\n'
	directory=$1
   short_dir=$2
	if [ ! -d $directory ]
	then
		echo "$directory is not a directory"
	else
		echo "[$short_dir]" 
		for filename in `ls $directory`
		do
			echo "$directory/$filename"
		done
	fi
   printf "\n"
   IFS="$OIFS"
}

main()
{
    OIFS=$IFS
    IFS=$'\n'

    front=0
    tail=1
    file_count=0
    dir_queue=($1)
    short_dir_queue=($2)

    while (( $front < $tail ))
    do
        # pop
        current_dir=${dir_queue[$front]}
        front=$(($front+1))
        for file in `ls $current_dir`
        do
            file_count=$file_count+1
            if [ -d "$current_dir/$file" ]
            then
                # push
                dir_queue[$tail]="$current_dir/$file"
                short_dir_queue[$tail]="$file"
                tail=$(($tail+1))
            fi  
        done
    done

    for (( i=0;i<$tail;i++))
    do
        print_files_of_a_dir ${dir_queue[$i]} ${short_dir_queue[$i]}
    done

    IFS="$OIFS"

    dir_num=$(($tail-1))
    file_num=$(($file_count-$dir_num))
    printf "[Directories Count]:%d\n[Files Count]:%d\n" $dir_num $file_num
}

main $target_dir $short_dir > $output_path
