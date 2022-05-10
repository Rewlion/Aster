#ifdef _TARGET_SHADER
  #define DSET_PER_FRAME    space0
  #define DSET_PER_MATERIAL space1
  #define DSET_PER_MODEL    space2
#else
  #define DSET_PER_FRAME    0
  #define DSET_PER_MATERIAL 1
  #define DSET_PER_MODEL    2
#endif
