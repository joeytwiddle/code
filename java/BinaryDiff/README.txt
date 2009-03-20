Er I think you might have made a mistake with the current algorithm.
The partial sums are a short-cut for generating the sum of any region of data.
So the partial sums should be generated without any window size.
Then the scanning can begin, and can get the sums of any block from the final partial sum - the initial partial sum.

TODO:
stream the files, rather than read them into Strings, which takes more memory.
Ah no unfortunately we need the whole file in memory, so that when a checksum matches, we can check that the originaly data does really match.
No silly, the partial sums can actually be used to retrieve the original file data anyway!  So do stream the files in!

