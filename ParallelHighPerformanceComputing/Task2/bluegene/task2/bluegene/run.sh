mpicxx batcher.cpp -O3 -o run

# 128 proc 
mpisubmit.bg -n 128 -w 00:15:00 --stdout res\_128\_512\_512.out --stderr res\_128\_512\_512.err ./run -- 512 512;
mpisubmit.bg -n 128 -w 00:15:00 --stdout res\_128\_1024\_1024.out --stderr res\_128\_1024\_1024.err ./run -- 1024 1024;
mpisubmit.bg -n 128 -w 00:15:00 --stdout res\_128\_2048\_2048.out --stderr res\_128\_2048\_2048.err ./run -- 2048 2048;
mpisubmit.bg -n 128 -w 00:15:00 --stdout res\_128\_4096\_4096.out --stderr res\_128\_4096\_4096.err ./run -- 4096 4096;
mpisubmit.bg -n 128 -w 00:15:00 --stdout res\_128\_8192\_8192.out --stderr res\_128\_8192\_8192.err ./run -- 8192 8192;
# mpisubmit.bg -n 128 -w 00:15:00 --stdout res2\_128\_16384\_16384.out --stderr res2\_128\_16384\_16384.err ./run -- 16384 16384;

# # 64 proc 
# mpisubmit.bg -n 64 -w 00:30:00 --stdout res\_64\_512\_512.out --stderr res\_64\_512\_512.err ./run -- 512 512;
# mpisubmit.bg -n 64 -w 00:30:00 --stdout res\_64\_1024\_1024.out --stderr res\_64\_1024\_1024.err ./run -- 1024 1024;
# mpisubmit.bg -n 64 -w 00:30:00 --stdout res\_64\_2048\_2048.out --stderr res\_64\_2048\_2048.err ./run -- 2048 2048;
# mpisubmit.bg -n 64 -w 00:30:00 --stdout res\_64\_4096\_4096.out --stderr res\_64\_4096\_4096.err ./run -- 4096 4096;
# mpisubmit.bg -n 64 -w 00:30:00 --stdout res\_64\_8192\_8192.out --stderr res\_64\_8192\_8192.err ./run -- 8192 8192;
# mpisubmit.bg -n 64 -w 00:30:00 --stdout res\_64\_16384\_16384.out --stderr res\_64\_16384\_16384.err ./run -- 16384 16384;

# # 32 proc 
# mpisubmit.bg -n 32 -w 01:00:00 --stdout res\_32\_512\_512.out --stderr res\_32\_512\_512.err ./run -- 512 512;
# mpisubmit.bg -n 32 -w 01:00:00 --stdout res\_32\_1024\_1024.out --stderr res\_32\_1024\_1024.err ./run -- 1024 1024;
# mpisubmit.bg -n 32 -w 01:00:00 --stdout res\_32\_2048\_2048.out --stderr res\_32\_2048\_2048.err ./run -- 2048 2048;
# mpisubmit.bg -n 32 -w 01:00:00 --stdout res\_32\_4096\_4096.out --stderr res\_32\_4096\_4096.err ./run -- 4096 4096;
# mpisubmit.bg -n 32 -w 01:00:00 --stdout res\_32\_8192\_8192.out --stderr res\_32\_8192\_8192.err ./run -- 8192 8192;
# mpisubmit.bg -n 32 -w 01:00:00 --stdout res\_32\_16384\_16384.out --stderr res\_32\_16384\_16384.err ./run -- 16384 16384;

# # 16 proc 
# mpisubmit.bg -n 16 -w 01:00:00 --stdout res\_16\_512\_512.out --stderr res\_16\_512\_512.err ./run -- 512 512;
# mpisubmit.bg -n 16 -w 01:00:00 --stdout res\_16\_1024\_1024.out --stderr res\_16\_1024\_1024.err ./run -- 1024 1024;
# mpisubmit.bg -n 16 -w 01:00:00 --stdout res\_16\_2048\_2048.out --stderr res\_16\_2048\_2048.err ./run -- 2048 2048;
# mpisubmit.bg -n 16 -w 01:00:00 --stdout res\_16\_4096\_4096.out --stderr res\_16\_4096\_4096.err ./run -- 4096 4096;
# mpisubmit.bg -n 16 -w 01:00:00 --stdout res\_16\_8192\_8192.out --stderr res\_16\_8192\_8192.err ./run -- 8192 8192;
# mpisubmit.bg -n 16 -w 01:00:00 --stdout res\_16\_16384\_16384.out --stderr res\_16\_16384\_16384.err ./run -- 16384 16384;

# # 8 proc 
# mpisubmit.bg -n 8 -w 01:00:00 --stdout res\_8\_512\_512.out --stderr res\_8\_512\_512.err ./run -- 512 512;
# mpisubmit.bg -n 8 -w 01:00:00 --stdout res\_8\_1024\_1024.out --stderr res\_8\_1024\_1024.err ./run -- 1024 1024;
# mpisubmit.bg -n 8 -w 01:00:00 --stdout res\_8\_2048\_2048.out --stderr res\_8\_2048\_2048.err ./run -- 2048 2048;
# mpisubmit.bg -n 8 -w 01:00:00 --stdout res\_8\_4096\_4096.out --stderr res\_8\_4096\_4096.err ./run -- 4096 4096;
# mpisubmit.bg -n 8 -w 01:00:00 --stdout res\_8\_8192\_8192.out --stderr res\_8\_8192\_8192.err ./run -- 8192 8192;
# mpisubmit.bg -n 8 -w 01:00:00 --stdout res\_8\_16384\_16384.out --stderr res\_8\_16384\_16384.err ./run -- 16384 16384;

# # 4 proc 
# mpisubmit.bg -n 4 -w 01:00:00 --stdout res\_4\_512\_512.out --stderr res\_4\_512\_512.err ./run -- 512 512;
# mpisubmit.bg -n 4 -w 01:00:00 --stdout res\_4\_1024\_1024.out --stderr res\_4\_1024\_1024.err ./run -- 1024 1024;
# mpisubmit.bg -n 4 -w 01:00:00 --stdout res\_4\_2048\_2048.out --stderr res\_4\_2048\_2048.err ./run -- 2048 2048;
# mpisubmit.bg -n 4 -w 01:00:00 --stdout res\_4\_4096\_4096.out --stderr res\_4\_4096\_4096.err ./run -- 4096 4096;
# mpisubmit.bg -n 4 -w 01:00:00 --stdout res\_4\_8192\_8192.out --stderr res\_4\_8192\_8192.err ./run -- 8192 8192;
# mpisubmit.bg -n 4 -w 01:00:00 --stdout res\_4\_16384\_16384.out --stderr res\_4\_16384\_16384.err ./run -- 16384 16384;

# # 2 proc 
# mpisubmit.bg -n 2 -w 01:00:00 --stdout res\_2\_512\_512.out --stderr res\_2\_512\_512.err ./run -- 512 512;
# mpisubmit.bg -n 2 -w 01:00:00 --stdout res\_2\_1024\_1024.out --stderr res\_2\_1024\_1024.err ./run -- 1024 1024;
# mpisubmit.bg -n 2 -w 01:00:00 --stdout res\_2\_2048\_2048.out --stderr res\_2\_2048\_2048.err ./run -- 2048 2048;
# mpisubmit.bg -n 2 -w 01:00:00 --stdout res\_2\_4096\_4096.out --stderr res\_2\_4096\_4096.err ./run -- 4096 4096;
# mpisubmit.bg -n 2 -w 01:00:00 --stdout res\_2\_8192\_8192.out --stderr res\_2\_8192\_8192.err ./run -- 8192 8192;
# mpisubmit.bg -n 2 -w 01:00:00 --stdout res\_2\_16384\_16384.out --stderr res\_2\_16384\_16384.err ./run -- 16384 16384;

# # 1 proc 
# mpisubmit.bg -n 1 -w 01:00:00 --stdout res\_1\_512\_512.out --stderr res\_1\_512\_512.err ./run -- 512 512;
# mpisubmit.bg -n 1 -w 01:00:00 --stdout res\_1\_1024\_1024.out --stderr res\_1\_1024\_1024.err ./run -- 1024 1024;
# mpisubmit.bg -n 1 -w 01:00:00 --stdout res\_1\_2048\_2048.out --stderr res\_1\_2048\_2048.err ./run -- 2048 2048;
# mpisubmit.bg -n 1 -w 01:00:00 --stdout res\_1\_4096\_4096.out --stderr res\_1\_4096\_4096.err ./run -- 4096 4096;
# mpisubmit.bg -n 1 -w 01:00:00 --stdout res\_1\_8192\_8192.out --stderr res\_1\_8192\_8192.err ./run -- 8192 8192;
# mpisubmit.bg -n 1 -w 01:00:00 --stdout res\_1\_16384\_16384.out --stderr res\_1\_16384\_16384.err ./run -- 16384 16384;

