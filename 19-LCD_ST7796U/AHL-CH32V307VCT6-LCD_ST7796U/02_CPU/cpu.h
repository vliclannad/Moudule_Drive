//======================================================================
//�ļ����ƣ�cpu.h��cpuͷ�ļ���
//������λ��SD-Arm(sumcu.suda.edu.cn)
//���¼�¼��20200831-20200903
//======================================================================

#ifndef CPU_H
#define CPU_H

//��1�����䶯����������оƬͷ�ļ�����CPU��ͬ����
#include "core_riscv.h"
#include "ch32v30x.h"

//��3�����䶯����λ���
// ������λ
#define IS_PIN_RESET_OCCURED    ((RCC->RSTSCKR & RCC_PINRSTF) \
                                 && (!(RCC->RSTSCKR & 0xFB000000)))
// �ϵ縴λ
#define IS_POWERON_RESET          (RCC->RSTSCKR & RCC_PORRSTF)
//д1�����Ÿ�λ��־λ
#define CLEAR_PIN_RESET_FLAG      (RCC->RSTSCKR |= RCC_RMVF)

//��3�����̶����жϺ궨��,����ARM�ܹ����򲻱䶯
#define ENABLE_INTERRUPTS    __asm volatile ("csrw mstatus, %0" : : "r" (0x88) )    //�����ж�
#define DISABLE_INTERRUPTS   __asm volatile ("csrw mstatus, %0" : : "r" (0x00) )    //�����ж�
//������Ҳ�����ã�
//#define ENABLE_INTERRUPTS    ({ __asm("li t0, 0x8"); __asm("csrs mstatus, t0"); }) //�����ж�
//#define DISABLE_INTERRUPTS   ({ __asm("li t0, 0x8"); __asm("csrc mstatus, t0"); }) //�����ж� 

//��4�����̶������Ż����ͱ����궨��
typedef volatile uint8_t      vuint8_t;   // ���Ż��޷���8λ�����ֽ�
typedef volatile uint16_t     vuint16_t;  // ���Ż��޷���16λ������
typedef volatile uint32_t     vuint32_t;  // ���Ż��޷���32λ��������
typedef volatile  int8_t      vint8_t;    //���Ż��з��ţ�λ��
typedef volatile  int16_t     vint16_t;   //���Ż��з��ţ���λ��
typedef volatile  int32_t     vint32_t;   //���Ż��з��ţ���λ��

//��5�����̶���λ�����꺯������λ����λ����üĴ���һλ��״̬��
#define BSET(bit,Register)  ((Register)|= (1<<(bit)))    //�üĴ�����һλ
#define BCLR(bit,Register)  ((Register) &= ~(1<<(bit)))  //��Ĵ�����һλ
#define BGET(bit,Register)  (((Register) >> (bit)) & 1)  //��üĴ���һλ��״̬

#endif    //CPU_H
