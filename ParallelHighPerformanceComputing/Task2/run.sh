#!/bin/bash
mpicxx batcher.cpp -O3 -o run

N=6144
mpisubmit.bg -n 512 -w 00:05:00 --stdout attempt\_${N}\_512.out --stderr attempt\_${N}\_512.err ./run -- ${N} ${N};
mpisubmit.bg -n 256 -w 00:10:00 --stdout attempt\_${N}\_256.out --stderr attempt\_${N}\_256.err ./run -- ${N} ${N};
mpisubmit.bg -n 128 -w 00:15:00 --stdout attempt\_${N}\_128.out --stderr attempt\_${N}\_128.err ./run -- ${N} ${N};
mpisubmit.bg -n 64 -w 00:15:00 --stdout attempt\_${N}\_64.out --stderr attempt\_${N}\_64.err ./run -- ${N} ${N};
mpisubmit.bg -n 32 -w 00:15:00 --stdout attempt\_${N}\_32.out --stderr attempt\_${N}\_32.err ./run -- ${N} ${N};
mpisubmit.bg -n 16 -w 00:15:00 --stdout attempt\_${N}\_16.out --stderr attempt\_${N}\_16.err ./run -- ${N} ${N};
mpisubmit.bg -n 8 -w 00:15:00 --stdout attempt\_${N}\_8.out --stderr attempt\_${N}\_8.err ./run -- ${N} ${N};
mpisubmit.bg -n 4 -w 00:15:00 --stdout attempt\_${N}\_4.out --stderr attempt\_${N}\_4.err ./run -- ${N} ${N};
mpisubmit.bg -n 2 -w 00:15:00 --stdout attempt\_${N}\_2.out --stderr attempt\_${N}\_2.err ./run -- ${N} ${N};
mpisubmit.bg -n 1 -w 00:15:00 --stdout attempt\_${N}\_1.out --stderr attempt\_${N}\_1.err ./run -- ${N} ${N};

N=8192
mpisubmit.bg -n 512 -w 00:05:00 --stdout attempt\_${N}\_512.out --stderr attempt\_${N}\_512.err ./run -- ${N} ${N};
mpisubmit.bg -n 256 -w 00:10:00 --stdout attempt\_${N}\_256.out --stderr attempt\_${N}\_256.err ./run -- ${N} ${N};
mpisubmit.bg -n 128 -w 00:15:00 --stdout attempt\_${N}\_128.out --stderr attempt\_${N}\_128.err ./run -- ${N} ${N};
mpisubmit.bg -n 64 -w 00:15:00 --stdout attempt\_${N}\_64.out --stderr attempt\_${N}\_64.err ./run -- ${N} ${N};
mpisubmit.bg -n 32 -w 00:15:00 --stdout attempt\_${N}\_32.out --stderr attempt\_${N}\_32.err ./run -- ${N} ${N};
mpisubmit.bg -n 16 -w 00:15:00 --stdout attempt\_${N}\_16.out --stderr attempt\_${N}\_16.err ./run -- ${N} ${N};
mpisubmit.bg -n 8 -w 00:15:00 --stdout attempt\_${N}\_8.out --stderr attempt\_${N}\_8.err ./run -- ${N} ${N};
mpisubmit.bg -n 4 -w 00:15:00 --stdout attempt\_${N}\_4.out --stderr attempt\_${N}\_4.err ./run -- ${N} ${N};
mpisubmit.bg -n 2 -w 00:15:00 --stdout attempt\_${N}\_2.out --stderr attempt\_${N}\_2.err ./run -- ${N} ${N};
mpisubmit.bg -n 1 -w 00:15:00 --stdout attempt\_${N}\_1.out --stderr attempt\_${N}\_1.err ./run -- ${N} ${N};

N=4096
mpisubmit.bg -n 512 -w 00:05:00 --stdout attempt\_${N}\_512.out --stderr attempt\_${N}\_512.err ./run -- ${N} ${N};
mpisubmit.bg -n 256 -w 00:10:00 --stdout attempt\_${N}\_256.out --stderr attempt\_${N}\_256.err ./run -- ${N} ${N};
mpisubmit.bg -n 128 -w 00:15:00 --stdout attempt\_${N}\_128.out --stderr attempt\_${N}\_128.err ./run -- ${N} ${N};
mpisubmit.bg -n 64 -w 00:15:00 --stdout attempt\_${N}\_64.out --stderr attempt\_${N}\_64.err ./run -- ${N} ${N};
mpisubmit.bg -n 32 -w 00:15:00 --stdout attempt\_${N}\_32.out --stderr attempt\_${N}\_32.err ./run -- ${N} ${N};
mpisubmit.bg -n 16 -w 00:15:00 --stdout attempt\_${N}\_16.out --stderr attempt\_${N}\_16.err ./run -- ${N} ${N};
mpisubmit.bg -n 8 -w 00:15:00 --stdout attempt\_${N}\_8.out --stderr attempt\_${N}\_8.err ./run -- ${N} ${N};
mpisubmit.bg -n 4 -w 00:15:00 --stdout attempt\_${N}\_4.out --stderr attempt\_${N}\_4.err ./run -- ${N} ${N};
mpisubmit.bg -n 2 -w 00:15:00 --stdout attempt\_${N}\_2.out --stderr attempt\_${N}\_2.err ./run -- ${N} ${N};
mpisubmit.bg -n 1 -w 00:15:00 --stdout attempt\_${N}\_1.out --stderr attempt\_${N}\_1.err ./run -- ${N} ${N};

N=2048
mpisubmit.bg -n 512 -w 00:05:00 --stdout attempt\_${N}\_512.out --stderr attempt\_${N}\_512.err ./run -- ${N} ${N};
mpisubmit.bg -n 256 -w 00:10:00 --stdout attempt\_${N}\_256.out --stderr attempt\_${N}\_256.err ./run -- ${N} ${N};
mpisubmit.bg -n 128 -w 00:15:00 --stdout attempt\_${N}\_128.out --stderr attempt\_${N}\_128.err ./run -- ${N} ${N};
mpisubmit.bg -n 64 -w 00:15:00 --stdout attempt\_${N}\_64.out --stderr attempt\_${N}\_64.err ./run -- ${N} ${N};
mpisubmit.bg -n 32 -w 00:15:00 --stdout attempt\_${N}\_32.out --stderr attempt\_${N}\_32.err ./run -- ${N} ${N};
mpisubmit.bg -n 16 -w 00:15:00 --stdout attempt\_${N}\_16.out --stderr attempt\_${N}\_16.err ./run -- ${N} ${N};
mpisubmit.bg -n 8 -w 00:15:00 --stdout attempt\_${N}\_8.out --stderr attempt\_${N}\_8.err ./run -- ${N} ${N};
mpisubmit.bg -n 4 -w 00:15:00 --stdout attempt\_${N}\_4.out --stderr attempt\_${N}\_4.err ./run -- ${N} ${N};
mpisubmit.bg -n 2 -w 00:15:00 --stdout attempt\_${N}\_2.out --stderr attempt\_${N}\_2.err ./run -- ${N} ${N};
mpisubmit.bg -n 1 -w 00:15:00 --stdout attempt\_${N}\_1.out --stderr attempt\_${N}\_1.err ./run -- ${N} ${N};

N=1024
mpisubmit.bg -n 512 -w 00:05:00 --stdout attempt\_${N}\_512.out --stderr attempt\_${N}\_512.err ./run -- ${N} ${N};
mpisubmit.bg -n 256 -w 00:10:00 --stdout attempt\_${N}\_256.out --stderr attempt\_${N}\_256.err ./run -- ${N} ${N};
mpisubmit.bg -n 128 -w 00:15:00 --stdout attempt\_${N}\_128.out --stderr attempt\_${N}\_128.err ./run -- ${N} ${N};
mpisubmit.bg -n 64 -w 00:15:00 --stdout attempt\_${N}\_64.out --stderr attempt\_${N}\_64.err ./run -- ${N} ${N};
mpisubmit.bg -n 32 -w 00:15:00 --stdout attempt\_${N}\_32.out --stderr attempt\_${N}\_32.err ./run -- ${N} ${N};
mpisubmit.bg -n 16 -w 00:15:00 --stdout attempt\_${N}\_16.out --stderr attempt\_${N}\_16.err ./run -- ${N} ${N};
mpisubmit.bg -n 8 -w 00:15:00 --stdout attempt\_${N}\_8.out --stderr attempt\_${N}\_8.err ./run -- ${N} ${N};
mpisubmit.bg -n 4 -w 00:15:00 --stdout attempt\_${N}\_4.out --stderr attempt\_${N}\_4.err ./run -- ${N} ${N};
mpisubmit.bg -n 2 -w 00:15:00 --stdout attempt\_${N}\_2.out --stderr attempt\_${N}\_2.err ./run -- ${N} ${N};
mpisubmit.bg -n 1 -w 00:15:00 --stdout attempt\_${N}\_1.out --stderr attempt\_${N}\_1.err ./run -- ${N} ${N};

# N=512
# mpisubmit.bg -n 256 -w 00:10:00 --stdout attempt\_${N}\_256.out --stderr attempt\_${N}\_256.err ./run -- ${N} ${N};
# mpisubmit.bg -n 128 -w 00:15:00 --stdout attempt\_${N}\_128.out --stderr attempt\_${N}\_128.err ./run -- ${N} ${N};
# mpisubmit.bg -n 64 -w 00:15:00 --stdout attempt\_${N}\_64.out --stderr attempt\_${N}\_64.err ./run -- ${N} ${N};
# mpisubmit.bg -n 32 -w 00:15:00 --stdout attempt\_${N}\_32.out --stderr attempt\_${N}\_32.err ./run -- ${N} ${N};
# mpisubmit.bg -n 16 -w 00:15:00 --stdout attempt\_${N}\_16.out --stderr attempt\_${N}\_16.err ./run -- ${N} ${N};
# mpisubmit.bg -n 8 -w 00:15:00 --stdout attempt\_${N}\_8.out --stderr attempt\_${N}\_8.err ./run -- ${N} ${N};
# mpisubmit.bg -n 4 -w 00:15:00 --stdout attempt\_${N}\_4.out --stderr attempt\_${N}\_4.err ./run -- ${N} ${N};
# mpisubmit.bg -n 2 -w 00:15:00 --stdout attempt\_${N}\_2.out --stderr attempt\_${N}\_2.err ./run -- ${N} ${N};
# mpisubmit.bg -n 1 -w 00:15:00 --stdout attempt\_${N}\_1.out --stderr attempt\_${N}\_1.err ./run -- ${N} ${N};

# N=256
# mpisubmit.bg -n 128 -w 00:15:00 --stdout attempt\_${N}\_128.out --stderr attempt\_${N}\_128.err ./run -- ${N} ${N};
# mpisubmit.bg -n 64 -w 00:15:00 --stdout attempt\_${N}\_64.out --stderr attempt\_${N}\_64.err ./run -- ${N} ${N};
# mpisubmit.bg -n 32 -w 00:15:00 --stdout attempt\_${N}\_32.out --stderr attempt\_${N}\_32.err ./run -- ${N} ${N};
# mpisubmit.bg -n 16 -w 00:15:00 --stdout attempt\_${N}\_16.out --stderr attempt\_${N}\_16.err ./run -- ${N} ${N};
# mpisubmit.bg -n 8 -w 00:15:00 --stdout attempt\_${N}\_8.out --stderr attempt\_${N}\_8.err ./run -- ${N} ${N};
# mpisubmit.bg -n 4 -w 00:15:00 --stdout attempt\_${N}\_4.out --stderr attempt\_${N}\_4.err ./run -- ${N} ${N};
# mpisubmit.bg -n 2 -w 00:15:00 --stdout attempt\_${N}\_2.out --stderr attempt\_${N}\_2.err ./run -- ${N} ${N};
# mpisubmit.bg -n 1 -w 00:15:00 --stdout attempt\_${N}\_1.out --stderr attempt\_${N}\_1.err ./run -- ${N} ${N};

# N=128
# mpisubmit.bg -n 128 -w 00:15:00 --stdout attempt\_${N}\_128.out --stderr attempt\_${N}\_128.err ./run -- ${N} ${N};
# mpisubmit.bg -n 64 -w 00:15:00 --stdout attempt\_${N}\_64.out --stderr attempt\_${N}\_64.err ./run -- ${N} ${N};
# mpisubmit.bg -n 32 -w 00:15:00 --stdout attempt\_${N}\_32.out --stderr attempt\_${N}\_32.err ./run -- ${N} ${N};
# mpisubmit.bg -n 16 -w 00:15:00 --stdout attempt\_${N}\_16.out --stderr attempt\_${N}\_16.err ./run -- ${N} ${N};
# mpisubmit.bg -n 8 -w 00:15:00 --stdout attempt\_${N}\_8.out --stderr attempt\_${N}\_8.err ./run -- ${N} ${N};
# mpisubmit.bg -n 4 -w 00:15:00 --stdout attempt\_${N}\_4.out --stderr attempt\_${N}\_4.err ./run -- ${N} ${N};
# mpisubmit.bg -n 2 -w 00:15:00 --stdout attempt\_${N}\_2.out --stderr attempt\_${N}\_2.err ./run -- ${N} ${N};
# mpisubmit.bg -n 1 -w 00:15:00 --stdout attempt\_${N}\_1.out --stderr attempt\_${N}\_1.err ./run -- ${N} ${N};

