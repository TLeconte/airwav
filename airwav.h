#define FSINT 25000
#define IFFREQ 50000

#define OUTFREQ 8000

#ifdef WITH_RTL
#define INRATE 2000000
#endif
#ifdef WITH_AIRSPY
#define INRATE 2500000
#endif

#define DOWNSC (INRATE/FSINT)
#define INBUFSZ (DOWNSC*2048)

extern int freq;
extern char *stid;
extern char *directory;


