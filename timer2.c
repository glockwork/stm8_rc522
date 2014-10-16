#include"iostm8l101f3.h"
#include"timer2.h"
#include"uart.h"
/*UART���嶨ʱ��*/

void timer2_init()
{
    CLK_PCKENR|=0x01;//TIM2 PCKEN0
    TIM2_IER=0X01;//UIE
    //TIM2_EGR=0X01;
    TIM2_PSCR=0x01;//2��Ƶ=1MHz
    TIM2_ARRH=1000>>8;
    TIM2_ARRL=1000&0xff;//��ʱ1ms
    //TIM2_CR1=0X94;//DOWN-COUNTER
}

void timer2_start()
{
    TIM2_CR1=0x00;
    TIM2_CNTRH=TIM2_ARRH;
    TIM2_CNTRL=TIM2_ARRL;
    TIM2_CR1=0x95;
}

void timer2_stop()
{
    TIM2_CR1=0x00;
}

void timer2_irq_on()
{
    TIM2_IER=0x01;
}

void timer2_irq_off()
{
    TIM2_IER=0X00;
    TIM2_SR1&=~0X01;//clear UIF bit
}

extern char state;
char cnt_dir=0;
char timecnt2=0;//���ϼ���
char timeout2=0;//���¼���
#pragma vector=TIM2_OVR_UIF_vector//��ʱ1ms���
__interrupt void timer2_overflow_ISR()
{
    TIM2_SR1&=~0X01;//clear UIF bit
    if(cnt_dir)//���¼���
    {
        if(--timeout2)
            return;
        timer2_stop();
    }
    else//���ϼ���
    {
        if(++timecnt2 > 5)
        {
            timer2_stop();
            state=UART_OVER;
        }
    }
}

void timer2_wait_ms(char t)
{
    cnt_dir=1;//���¼���
    timeout2=t;
    timer2_start();
    while(timeout2);
}

void set_timer2_wait_ms(char t)
{
    cnt_dir=1;//���¼���
    timeout2=t;
    timer2_start();
}
