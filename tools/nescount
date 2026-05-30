#!/bin/sh

bytes_per_tile=16

case "$1" in
	--format-nes)
		bytes_per_tile=16
		;;
	--format-gb)
		bytes_per_tile=16
		;;
esac

full_bytes=$(wc -c)

num_tiles=$((full_bytes / bytes_per_tile))

printf "%s\n" "$num_tiles"
