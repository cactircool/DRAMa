# DRAMa

Ellison -> Research caching algorithms

Arjun -> Research caching algorithms

Long -> Getting familiar with C, work on some projects and show them later. Try to get familiar with effecient solutions to leetcode problems too.

Quan -> Getting familiar with C, work on some projects and show them later. Try to get familiar with effecient solutions to leetcode problems too.

Plan right now is to generate kernels that use traditional cache replacement algorithms, vs a custom algorithm that uses something like memory tagging with whether or not it's a pointer type. This will probably require a custom malloc function or something for stack allocation. Later problems.

Also it would be nice to create a caching algorithm that analyzes patterns in programs to optimize cache usage on multiple runs of the same program. This way, an ineffecient program can slowly become more effecient on more run throughs.

To manage virtualization of memory, we can use a preallocated pool and use that are unallocated memory to allocated data. We just have to be careful about optimizations CPUs make at the hardware level that we may not be able to implement in this.