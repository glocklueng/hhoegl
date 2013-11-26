
#include  <stdio.h>

extern void  initialise_monitor_handles(void);

#define STACK_TOP 0x20002000

void nmi_handler(void);
void hardfault_handler(void);
void delay(void);
int main(void);


unsigned int * myvectors[4]
    __attribute__ ((section("vectors")))= {
    (unsigned int *) STACK_TOP,
    (unsigned int *) main,
    (unsigned int *) nmi_handler,
    (unsigned int *) hardfault_handler
};


int main(void)
{
  unsigned long count = 0;
  initialise_monitor_handles();
  printf("Hello, world!\n");
  while (1) {
      count++;
      printf("count = %ld\n", count);
  }
  return 0;
}


void nmi_handler(void)
{
    	while(1);
}


void hardfault_handler(void)
{
	while(1);
}

