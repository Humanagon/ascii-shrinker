# ASCII Shrinker

This application shrinks ASCII files by 12.5%, which can then be compressed further with another compression method.

### How it works

One thing to keep in mind about ASCII is that it is 7-bit. Because of this, we can simply remove the 8th bit on all ascii characters and bit shift to the left, then again one more than the last time, and so on, until a full cycle is complete.

```batch
shrinker.exe compress file input.txt output.saf
```

Enjoy!
