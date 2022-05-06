# Parallelize several image preprocessing methods (image filtering) using OpenMP

CPU: Intel(R) Core(TM) i5-4310U CPU @ 2.00GHz</br>
Total Physical Cores: 2</br>
Total Threads: 4 (thanks to Hyper-Threading Technology)

## Some explanation

**Q: Why is the execution time with 4 threads only slightly improvement (negligible, even slower) compared to 2 threads?**

**A:** With twice the number of threads, one would expect a double performance improvement but this is very difficult to achieve when we use more threads than physical cores (when Hyper-Threading is used).</br>
Hyper-Threading make it possible for each core to run more than one threads (specifically 2) in parallel, but these threads must share physical resources with each other so they have to wait for each other, this has a significant impact on performance.

**Q: Why is there no execution time evaluation for image "test7680.jpg" even though it is in the images folder?**

**A:** Because the size of this image is too big (7680x7680) whereas my laptop is as weak as my programming skills.
