
#include  <stdio.h>

extern void  initialise_monitor_handles(void);

int main(void)
{
  unsigned long count = 0;
  initialise_monitor_handles();
  printf("Hello, world!\n");
  while (1) {
      printf("count = %ld\n", count++);
  }
  return 0;
}

