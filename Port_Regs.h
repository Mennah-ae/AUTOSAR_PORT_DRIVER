/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Regs.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver Registers
 *
 * Author: Menna Saeed
 ******************************************************************************/

#ifndef PORT_REGS_H
#define PORT_REGS_H

#include "Std_Types.h"


/*******************************************************************************
 *                              PORT Module Registers                                *
 *******************************************************************************/

/* RCC register which is responsible of enabling GPIO port Clock */
#define SYSCTL_REGCGC2_REG                (*((volatile uint32*) 0x400FE108) 

/* GPIO Registers base addresses */
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000

/* GPIO Registers offset addresses */
#define PORT_DATA_REG_OFFSET              0x3FC
#define PORT_DIR_REG_OFFSET               0x400
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_PULL_UP_REG_OFFSET           0x510
#define PORT_PULL_DOWN_REG_OFFSET         0x514
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_ANALOG_MODE_SEL_REG_OFFSET   0x528
#define PORT_CTL_REG_OFFSET               0x52C

/*******************************************************************************
 *                              PORTA Registers                                *
 *******************************************************************************/
#define GPIO_PORTA_DATA_REG               (*((volatile uint32 *)0x400043FC))
#define GPIO_PORTA_DIR_REG                (*((volatile uint32 *)0x40004400))  
#define GPIO_PORTA_ALT_FUNC_REG           (*((volatile uint32 *)0x40004420))
#define GPIO_PORTA_PULL_UP_REG            (*((volatile uint32 *)0x40004510))
#define GPIO_PORTA_PULL_DOWN_REG          (*((volatile uint32 *)0x40004514)) 
#define GPIO_PORTA_DIGITAL_ENABLE_REG     (*((volatile uint32 *)0x4000451C))
#define GPIO_PORTA_LOCK_REG               (*((volatile uint32 *)0x40004520))
#define GPIO_PORTA_COMMIT_REG             (*((volatile uint32 *)0x40004524))         
#define GPIO_PORTA_ANALOG_MODE_SEL_REG    (*((volatile uint32 *)0x40004528))
#define GPIO_PORTA_CTL_REG                (*((volatile uint32 *)0x4000452C)) 

/*******************************************************************************
 *                              PORTB Registers                                *
 *******************************************************************************/
#define GPIO_PORTB_DATA_REG               (*((volatile uint32 *)0x400053FC))
#define GPIO_PORTB_DIR_REG                (*((volatile uint32 *)0x40005400))  
#define GPIO_PORTB_ALT_FUNC_REG           (*((volatile uint32 *)0x40005420))
#define GPIO_PORTB_PULL_UP_REG            (*((volatile uint32 *)0x40005510))
#define GPIO_PORTB_PULL_DOWN_REG          (*((volatile uint32 *)0x40005514)) 
#define GPIO_PORTB_DIGITAL_ENABLE_REG     (*((volatile uint32 *)0x4000551C))
#define GPIO_PORTB_LOCK_REG               (*((volatile uint32 *)0x40005520))
#define GPIO_PORTB_COMMIT_REG             (*((volatile uint32 *)0x40005524))         
#define GPIO_PORTB_ANALOG_MODE_SEL_REG    (*((volatile uint32 *)0x40005528))
#define GPIO_PORTB_CTL_REG                (*((volatile uint32 *)0x4000552C)) 
  
/*******************************************************************************
 *                              PORTC Registers                                *
 *******************************************************************************/
#define GPIO_PORTC_DATA_REG               (*((volatile uint32 *)0x400063FC))
#define GPIO_PORTC_DIR_REG                (*((volatile uint32 *)0x40006400))  
#define GPIO_PORTC_ALT_FUNC_REG           (*((volatile uint32 *)0x40006420))
#define GPIO_PORTC_PULL_UP_REG            (*((volatile uint32 *)0x40006510))
#define GPIO_PORTC_PULL_DOWN_REG          (*((volatile uint32 *)0x40006514)) 
#define GPIO_PORTC_DIGITAL_ENABLE_REG     (*((volatile uint32 *)0x4000651C))
#define GPIO_PORTC_LOCK_REG               (*((volatile uint32 *)0x40006520))
#define GPIO_PORTC_COMMIT_REG             (*((volatile uint32 *)0x40006524))         
#define GPIO_PORTC_ANALOG_MODE_SEL_REG    (*((volatile uint32 *)0x40006528))
#define GPIO_PORTC_CTL_REG                (*((volatile uint32 *)0x4000652C)) 
   
 /*******************************************************************************
 *                              PORTD Registers                                *
 *******************************************************************************/
/* GPIO Registers base addresses */
#define GPIO_PORTD_DATA_REG               (*((volatile uint32 *)0x400073FC))
#define GPIO_PORTD_DIR_REG                (*((volatile uint32 *)0x40007400))  
#define GPIO_PORTD_ALT_FUNC_REG           (*((volatile uint32 *)0x40007420))
#define GPIO_PORTD_PULL_UP_REG            (*((volatile uint32 *)0x40007510))
#define GPIO_PORTD_PULL_DOWN_REG          (*((volatile uint32 *)0x40007514)) 
#define GPIO_PORTD_DIGITAL_ENABLE_REG     (*((volatile uint32 *)0x4000751C))
#define GPIO_PORTD_LOCK_REG               (*((volatile uint32 *)0x40007520))
#define GPIO_PORTD_COMMIT_REG             (*((volatile uint32 *)0x40007524))         
#define GPIO_PORTD_ANALOG_MODE_SEL_REG    (*((volatile uint32 *)0x40007528))
#define GPIO_PORTD_CTL_REG                (*((volatile uint32 *)0x4000752C)) 
   
 /*******************************************************************************
 *                              PORTE Registers                                *
 *******************************************************************************/
#define GPIO_PORTE_DATA_REG               (*((volatile uint32 *)0x4000243FC))
#define GPIO_PORTE_DIR_REG                (*((volatile uint32 *)0x400024400))  
#define GPIO_PORTE_ALT_FUNC_REG           (*((volatile uint32 *)0x400024420))
#define GPIO_PORTE_PULL_UP_REG            (*((volatile uint32 *)0x400024510))
#define GPIO_PORTE_PULL_DOWN_REG          (*((volatile uint32 *)0x400024514)) 
#define GPIO_PORTE_DIGITAL_ENABLE_REG     (*((volatile uint32 *)0x40002451C))
#define GPIO_PORTE_LOCK_REG               (*((volatile uint32 *)0x400024520))
#define GPIO_PORTE_COMMIT_REG             (*((volatile uint32 *)0x400024524))         
#define GPIO_PORTE_ANALOG_MODE_SEL_REG    (*((volatile uint32 *)0x400024528))
#define GPIO_PORTE_CTL_REG                (*((volatile uint32 *)0x40002452C)) 
 /*******************************************************************************
 *                              PORTF Registers                                *
 *******************************************************************************/
#define GPIO_PORTF_DATA_REG               (*((volatile uint32 *)0x4000253FC))
#define GPIO_PORTF_DIR_REG                (*((volatile uint32 *)0x400025400))  
#define GPIO_PORTF_ALT_FUNC_REG           (*((volatile uint32 *)0x400025420))
#define GPIO_PORTF_PULL_UP_REG            (*((volatile uint32 *)0x400025510))
#define GPIO_PORTF_PULL_DOWN_REG          (*((volatile uint32 *)0x400025514)) 
#define GPIO_PORTF_DIGITAL_ENABLE_REG     (*((volatile uint32 *)0x40002551C))
#define GPIO_PORTF_LOCK_REG               (*((volatile uint32 *)0x400025520))
#define GPIO_PORTF_COMMIT_REG             (*((volatile uint32 *)0x400025524))         
#define GPIO_PORTF_ANALOG_MODE_SEL_REG    (*((volatile uint32 *)0x400025528))
#define GPIO_PORTF_CTL_REG                (*((volatile uint32 *)0x40002552C)) 
   
   /***************************************************************************/

#endif /* PORT_REGS_H */
