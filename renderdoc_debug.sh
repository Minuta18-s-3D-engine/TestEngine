gdb -ex "set print thread-events off" \
    -ex "set exec-wrapper env 'LD_PRELOAD=/home/minuta18/renderdoc/lib/librenderdoc.so'" \
    -ex run \
    --args ./TestEng --project ./demo