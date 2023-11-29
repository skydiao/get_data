#include "main.h"
#include "delay.h"
//在此函数中加入初始化sysytick定时器步骤
//参照正点原子例程进行修改
//void delay_us(uint32_t us) {
//    TIM1->CR1 &= ~TIM_CR1_CEN; // 关闭定时器
//    TIM1->PSC = 84 - 1; // 设置预分频为84
//    TIM1->ARR = us; // 设置自动重装载寄存器为所需的延时
//    TIM1->EGR |= TIM_EGR_UG; // 产生更新事件以更新预分频和自动重装载寄存器
//    TIM1->SR &= ~TIM_SR_UIF; // 清除更新中断标志
//    TIM1->CR1 |= TIM_CR1_CEN; // 启用定时器

//    while (!(TIM1->SR & TIM_SR_UIF)) {} // 等待更新中断标志被设置
//}
void delay_us(uint32_t us) //利用CPU循环实现的非精准应用的微秒延时函数
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us); //使用HAL_RCC_GetHCLKFreq()函数获取主频值，经算法得到1微秒的循环次数
    while (delay--); //循环delay次，达到1微秒延时
}
//void delay_us(u32 nus)
//{
//       u32 ticks;
//       u32 told,tnow,reload,tcnt=0;
//       //if((0x0001&(SysTick->CTRL)) ==0)    //定时器未工作
//              //vPortSetupTimerInterrupt();  //初始化定时器
// 
//       reload = SysTick->LOAD;                     //获取重装载寄存器值
//       ticks = nus * (SystemCoreClock / 1000000);  //计数时间值
//       told=SysTick->VAL;                          //获取当前数值寄存器值（开始时数值）
// 
//       while(1)
//       {
//              tnow=SysTick->VAL;          //获取当前数值寄存器值
//              if(tnow!=told)              //当前值不等于开始值说明已在计数
//              {         
// 
//                     if(tnow<told)             //当前值小于开始数值，说明未计到0
//                          tcnt+=told-tnow;     //计数值=开始值-当前值
// 
//                     else                  //当前值大于开始数值，说明已计到0并重新计数
//                            tcnt+=reload-tnow+told;   //计数值=重装载值-当前值+开始值  （已
//                                                      //从开始值计到0） 
// 
//                     told=tnow;                //更新开始值
//                     if(tcnt>=ticks)break;     //时间超过/等于要延迟的时间,则退出.
//              } 
//       }     
//}
 
//SystemCoreClock为系统时钟(system_stmf4xx.c中)，通常选择该时钟作为
//systick定时器时钟，根据具体情况更改

