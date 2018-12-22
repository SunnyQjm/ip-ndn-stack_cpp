#include <sys/time.h>

extern void StartTheClock();
extern long StopTheClock();
extern int MedSelect(int, int, int[]);
extern long LMedSelect(int, int, long[]);
extern long long LLMedSelect(int, int, long long[]);
extern unsigned long long LLUMedSelect(int k, int n, unsigned long long arr[]);
extern double DMedSelect(int, int, double[]);
extern void CheckMemory(void *);