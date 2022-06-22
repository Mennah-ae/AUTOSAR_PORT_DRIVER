 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Mohamed Tarek
 ******************************************************************************/

#include "Port.h"
#include "Port_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Dio Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif
   /*Static global variable to hold the status of 
    *the port module so that other API's can not 
    *be called if the Port module was uninitialized*/
   STATIC uint8 g_Port_Status = PORT_NOT_INITIALIZED;

  /*Static global variable to hold the status of the JTAG
   *pins to secure them and prevent the access to them*/
   STATIC uint8 g_JTAG_flag = STD_LOW;

   /*Pointer to hold the address of the passed configuration
    *array of structures*/
   STATIC const volatile Port_ConfigType *g_Port_ConfigPtr = NULL_PTR;
/************************************************************************************
* Service Name: Port_Init
* Service ID[hex]: 0x00
* Sync/Async:   Synchronous
* Reentrancy:   Non Reentrant
* Parameters (in)   :     ConfigPtr->Pointer to configuration set.
* Parameters (inout):     None
* Parameters (out)  :     None
* Return value:           None
* Description: Function to initialize the Port Driver module:
*              - Initializes ALL ports and port pins with the configuration set pointed to by the parameter ConfigPtr
*              - If Port_Init function is not called first, then no operation can occur on the MCU ports and port pins.
*              - Initializes all configured resources.
************************************************************************************/
void Port_Init(const Port_ConfigType* ConfigPtr)
{
  
   volatile uint32 *PortGpio_Ptr = NULL_PTR; /* Pointer to point to the required Port Registers base address */
   volatile uint32 delay = 0;
   
   g_Port_ConfigPtr = ConfigPtr->Pins;     /*points to the 1st address of the array of structure*/
   Port_Status = PORT_INITIALIZED;
   uint8 Pin_Index;              /* index to configure each pin (43)*/
    
  #if (DIO_DEV_ERROR_DETECT == STD_ON)

  /* check if the input configuration pointer is not a NULL_PTR */
  if (NULL_PTR == ConfigPtr)
  {
          Det_ReportError(PORT_MODULE_ID, 
                          PORT_INSTANCE_ID, 
                          PORT_INIT_SID,
                          PORT_E_PARAM_CONFIG);
  }
  else
  {
    /* Do Nothing */
  }

  #endif
  
  /*Choosing the required Port number*/
  switch(ConfigPtr->port_num)
    {
        case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break;
	case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
  /* Enable clock for PORT and allow time for clock to start*/
    SYSCTL_REGCGC2_REG |= (1<<ConfigPtr->port_num);
    delay = SYSCTL_REGCGC2_REG;
  
    /*Unlocking locked pins to be able to configure and use them*/
    if( ((ConfigPtr->port_num == PORT_D) && (ConfigPtr->pin_num == NMI_PIN2)) || ((ConfigPtr->port_num == PORT_F) && (ConfigPtr->pin_num == NMI_PIN1)) ) /* PD7 or PF0 */
    {
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = UNLOCK_VALUE;                     /* Unlock the GPIOCR register */   
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , ConfigPtr->pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
}

/*Preventing any actions to be done upon JTAG pins*/
     else if(  
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN0) )||
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN1) )||
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN2) )||
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN3) ))
      {
        /* Do Nothing ...  this is the JTAG pins */
        
        g_JTAG_flag = STD_HIGH;
    }
    else
    {
        /* Do Nothing ... No need to unlock the commit register for this pin */
    } 
    
    /*Configure Port DIO mode -> Input/Output 
   *And if Input pin -> configure the internal resistor Pull-up/Pull-down*/

    if(ConfigPtr-> Pin_mode == PORT_DIO_MODE)
            {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , ConfigPtr->pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , ConfigPtr->pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << ((ConfigPtr->pin_num) * 4));     /* Clear the PMCx bits for this pin */
                /*If output pin*/
                if(ConfigPtr-> Pin_direction == PORT_PIN_OUT)
                {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , ConfigPtr->pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                    
                    if(ConfigPtr-> Pin_level_initial_value== STD_HIGH)
                    {
                        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , ConfigPtr->pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                    }
                    else
                    {
                        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , ConfigPtr->pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                    }
                }
                /*If Input pin*/
                else if(ConfigPtr-> Pin_direction == PORT_PIN_IN)
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , ConfigPtr->pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                    /*For input pins, internal resistor should be configured*/
                    if(ConfigPtr->resistor == PULL_UP)
                    {
                        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , ConfigPtr->pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                    }
                    else if(ConfigPtr->resistor == PULL_DOWN)
                    {
                        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , ConfigPtr->pin_num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                    }
                    else
                    {
                        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , ConfigPtr->pin_num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */                   
                        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , ConfigPtr->pin_num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */                }
                }
                else
                {
                    /* Do Nothing */
                }
                
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , ConfigPtr->pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            }

    /*Configure Analog mode used for ADC only*/

      else if(ConfigPtr-> Pin_mode == PORT_ALTERNATE_FUNCTION_ADC_MODE)
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , ConfigPtr->pin_num);    /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
            
            if(ConfigPtr-> Pin_direction== PORT_PIN_OUT)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , ConfigPtr->pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
            }
            else if(ConfigPtr-> Pin_direction == PORT_PIN_IN)
            {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , ConfigPtr->pin_num);                /* clear the corresponding bit in the GPIODIR register to configure it as input pin */              
            }
            
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , ConfigPtr->pin_num);     /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
            
        }

/*If any other alternative functionality between DIO Id and ADC id*/
        else if((ConfigPtr-> Pin_mode > PORT_DIO_MODE) && (ConfigPtr-> Pin_mode < PORT_ALTERNATE_FUNCTION_ADC_MODE))
        {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , ConfigPtr->pin_num);            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , ConfigPtr->pin_num);                     /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << ((ConfigPtr->pin_num) * 4));         /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (ConfigPtr-> Pin_mode<< ((ConfigPtr->pin_num)* 4));  /* Set the PMCx bits for this pin to the selected Alternate function in the configurations */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , ConfigPtr->pin_num);               /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            
        }
      }
    }
  }
}

    
#if (PORT_SET_PIN_DIRECTION_API==STD_ON)
/************************************************************************************
* Service Name: Port_SetPinDirection
* Service ID[hex]: 0x01
* Sync/Async:   Synchronous
* Reentrancy:   Reentrant
* Parameters (in):
                  -Pin -> Port Pin ID number.
                  -Direction -> Port Pin Direction
* Parameters (inout):   None
* Parameters (out):     None
* Return value:         None
* Description: Function to set the port pin direction:
*              - sets the port pin direction during runtime
************************************************************************************/
                              
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirection Direction)
{ 
 
  volatile uint32 * Port_directionPtr = NULL_PTR; /* point to the required Port Registers base address */
  boolean error = FALSE;
  
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  /*Check if the API service called prior to module initialization*/
	if (g_Port_Status == PORT_NOT_INITIALIZED)
	{
		Det_ReportError(PORT_MODULE_ID, 
                                PORT_INSTANCE_ID, 
                                PORT_INIT_ID,
                                PORT_E_UNINIT);
                error = TRUE;
	}
	else
	{
            /* Do nothing*/
	}
        
        /*Dev error for Incorrect Port Pin ID passed*/
	if (Pin >= PORT_CONFIGURED_CHANNLES)
	{
		Det_ReportError(PORT_MODULE_ID, 
                                PORT_INSTANCE_ID, 
                                PORT_INIT_ID,
                                PORT_E_PARAM_PIN);
                error = TRUE;
	} 
        else
	{

	}
        
       if(((*g_Port_ConfigPtr->Channels)[Pin]).Pin_directionChange == PIN_DIRECTION_CHANGEABLE_OFF)
        {
          Det_ReportError(PORT_MODULE_ID, 
                          PORT_INSTANCE_ID, 
                          PORT_INIT_ID,
                          PORT_E_DIRECTION_UNCHANGEABLE);
          error = TRUE;
        }
          else 
          {
          }
#endif

        /* In-case there are no errors */
	if(FALSE == error)
        {
	Port_Status = PORT_INITIALIZED;
	
	 switch(ConfigPtr->port_num)
    {
        case  0: Port_directionPtr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break; 
	case  1: Port_directionPtr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: Port_directionPtr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: Port_directionPtr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: Port_directionPtr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: Port_directionPtr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
    
    /*Preventing any actions to be done upon JTAG pins*/
          if(  
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN0) )||
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN1) )||
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN2) )||
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN3) ))
      {
        /* Do Nothing ...  this is the JTAG pins */
        
        g_JTAG_flag = STD_HIGH;
    }
    else
    {
          switch (Direction)
          {
            case PORT_PIN_IN:
              
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_directionPtr + PORT_DIR_REG_OFFSET) , Pin);
                  break; 
            
            case PORT_PIN_OUT:
        
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_directionPtr + PORT_DIR_REG_OFFSET) , Pin); 
                  break;
          default:
            /*do nothing*/
            break;
            
                }/*End of the switch..case*/
          }/*end of the else condition*/
      }/*end of the if(FALSE == error)*/
}
#endif

/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Service ID[hex]: 0x02
* Sync/Async:   Synchronous
* Reentrancy:   Non Reentrant
* Parameters (in):      None
* Parameters (inout):   None
* Parameters (out):     None
* Return value:         None
* Description: Function to refresh port direction.:
*              - refreshs the direction of all configured ports to the configured direction (PortPinDirection).
************************************************************************************/
void Port_RefreshPortDirection(void )
{
  boolean error=FALSE;
  /* point to the required Port Registers base address */
  volatile uint32 *Port_refreshDirectionPtr = NULL_PTR;
 
 #if (PORT_DEV_ERROR_DETECT == STD_ON)
  /*Check if the API service called prior to module initialization*/
	if (g_Port_Status == PORT_NOT_INITIALIZED)
	{
		Det_ReportError(PORT_MODULE_ID, 
                                PORT_INSTANCE_ID, 
                                PORT_INIT_ID,
                                PORT_E_UNINIT);
                error = TRUE;
	}
        
        else 
        {
          /*Do nothing*/
        }
#endif
        /* In-case there are no errors */
	if(FALSE == error)
        {
	Port_Status = PORT_INITIALIZED;
	
	 switch(ConfigPtr->port_num)
    {
        case  0: Port_refreshDirectionPtr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break; 
	case  1: Port_refreshDirectionPtr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: Port_refreshDirectionPtr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: Port_refreshDirectionPtr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: Port_refreshDirectionPtr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: Port_refreshDirectionPtr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
    
    /*Preventing any actions to be done upon JTAG pins*/
          if(  
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN0) )||
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN1) )||
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN2) )||
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN3) ))
      {
        /* Do Nothing ...  this is the JTAG pins */
        
        g_JTAG_flag = STD_HIGH;
    }
    else 
    {
    }
    if (g_JTAG_flag == STD_LOW)
	{
         /*The function Port_RefreshPortDirection shall exclude those port pins from refreshing that are configured as ‘pin direction changeable during runtime*/ 
          if(ConfigPtr-> Pin_directionChange == PIN_DIRECTION_CHANGEABLE_OFF )
		{
                  switch (ConfigPtr->Pin_direction)
                  {
                  case PORT_PIN_OUT:
                       /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
			SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_refreshDirectionPtr + PORT_DIR_REG_OFFSET) , ConfigPtr->pin_num);
                  break;
                  
                  case PORT_PIN_IN:
                       /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
			CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_refreshDirectionPtr + PORT_DIR_REG_OFFSET) , ConfigPtr->pin_num);
		  break;

		  default:
			/* Do Nothing */
		   break;
                    }/*End of Switch..case*/
                  }
          else 
          {
          }
        }
        else 
        {
          g_JTAG_flag = STD_LOW;
        }
        }
        else 
        {
        }
        
}

#if (PORT_VERSION_INFO_API == STD_ON)
/************************************************************************************
* Service Name: Port_GetVersionInfo
* Service ID[hex]: 0x03
* Sync/Async:   Synchronous
* Reentrancy:   Non Reentrant
* Parameters (in):      None
* Parameters (inout):   None
* Parameters (out):     versioninfo -> Pointer to where to store the version information of this module.
* Return value:         None
* Description: Function to return the version information of this module.:
*              - returns the version information of this module. The version information includes:
                  + Module Id
                  + Vendor Id
                  + Vendor specific version numbers (BSW00407).
************************************************************************************/

void Port_GetVersionInfo(Std_VersionInfoType* versioninfo )
{
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
  /* Check if input pointer is not Null pointer */
	if(NULL_PTR == versioninfo)
	{
		/* Report to DET  */
		Det_ReportError(PORT_MODULE_ID, 
                                PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID, 
                                PORT_E_PARAM_POINTER);
	}
	else
        {
          /*Do nothing*/
        }
        /*check if the Port driver is initiaalized or not*/
        if (g_Port_Status ==PORT_NOT_INITIALIZED)
        {
          /*Reporting error that the module is not initialized, i.e., the Port_init wa not called*/
          Det_ReportError(PORT_MODULE_ID,
			  PORT_INSTANCE_ID,
			  PORT_SET_PIN_MODE_SID,
			  PORT_E_UNINIT);
        }
        else
        {
          /*Do nothing*/
        }
#endif  /* (PORT_DEV_ERROR_DETECT == STD_ON) */
        
	{
		/* Copy the vendor Id */
		versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
		/* Copy the module Id */
		versioninfo->moduleID = (uint16)PORT_MODULE_ID;
		/* Copy Software Major Version */
		versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
		/* Copy Software Minor Version */
		versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
		/* Copy Software Patch Version */
		versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
	}
}
#endif

#if ( PORT_SET_PIN_MODE_API == STD_ON)
/************************************************************************************
* Service Name: Port_SetPinMode
* Service ID[hex]: 0x04
* Sync/Async:   Synchronous
* Reentrancy:   Reentrant
* Parameters (in):      -Pin  -> Port Pin ID number.
                        -Mode -> New Port Pin mode to be set on port pin.
* Parameters (inout):   None
* Parameters (out):     None
* Return value:         None
* Description: Function to set the port pin mode..:
*              - sets the port pin mode of the referenced pin during runtime.
************************************************************************************/
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode )
{
  volatile uint32 * Port_modePtr = NULL_PTR; /* point to the required Port Registers base address */
  boolean error = FALSE;
  
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
  /*Check if the API service called prior to module initialization*/
	if (g_Port_Status == PORT_NOT_INITIALIZED)
	{
		Det_ReportError(PORT_MODULE_ID, 
                                PORT_INSTANCE_ID, 
                                PORT_INIT_ID,
                                PORT_E_UNINIT);
                error = TRUE;
	}
	else
	{
            /* Do nothing*/
	}
  /*Dev error for Incorrect Port Pin ID passed*/
	if (Pin >= PORT_CONFIGURED_CHANNLES)
	{
		Det_ReportError(PORT_MODULE_ID, 
                                PORT_INSTANCE_ID, 
                                PORT_INIT_ID,
                                PORT_E_PARAM_PIN);
                error = TRUE;
	} 
        else
	{

	}
        
       if(((*g_Port_ConfigPtr->Channels)[Pin]).Pin_modeChange == PIN_MODE_CHANGEABLE_OFF)
        {
              Det_ReportError(PORT_MODULE_ID, 
                              PORT_INSTANCE_ID, 
                              PORT_INIT_ID,
                              PORT_E_MODE_UNCHANGEABLE);
          error = TRUE;
        }
          else 
          {
          }
#endif
  if(FALSE == error)
  {
     switch(ConfigPtr->port_num)
    {
        case  0: Port_modePtr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
		 break; 
	case  1: Port_modePtr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
		 break;
	case  2: Port_modePtr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
		 break;
	case  3: Port_modePtr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
		 break;
        case  4: Port_modePtr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
		 break;
        case  5: Port_modePtr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
		 break;
    }
    
    /*Preventing any actions to be done upon JTAG pins*/
          if(  
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN0) )||
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN1) )||
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN2) )||
            ( (ConfigPtr->port_num == PORT_C) && (ConfigPtr->pin_num == JTAG_PIN3) ))
      {
        /* Do Nothing ...  this is the JTAG pins */
        
        g_JTAG_flag = STD_HIGH;
    }
    
    else
    {
          switch (Mode)
          {
            case PORT_DIO_MODE:
                /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_modePtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);
                
                 /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_modePtr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);
                
                /*Close the alternative function register by 0 as it is DIO mode*/
		CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_modePtr + PORT_ALT_FUNC_REG_OFFSET) , Pin);
                
                /* Clear the PMCx bits for this pin */
		*(volatile uint32 *)((volatile uint8 *)Port_modePtr + PORT_PCTL_REG_OFFSET) &= ~(0x0000000F << (Pin * 4));
                
                        break; 
            
         case PORT_ALTERNATE_FUNCTION_ADC_MODE:
            /* Set the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_modePtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);
           
            /* Clear the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_modePtr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);
        
          /*Open alternative function register by 1 as it is ADC mode*/
	  SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_modePtr + PORT_ALT_FUNC_REG_OFFSET) , Pin);
                        /*
			Clear the PMCx bits for this pin 
			 it is the same mode number for DIO but it will work in ADC mode as PMCx bits for this pin is configured as analog pin
			*/
                        *(volatile uint32 *)((volatile uint8 *)Port_modePtr + PORT_PCTL_REG_OFFSET) &= ~(0x0000000F << (Pin * 4));
                  
                        break;
         
          default:
                /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_modePtr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Pin);

		/* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_modePtr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Pin);

		/*Open alternative function register by 1 as it is any mode except DIO*/
		SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_modePtr + PORT_ALT_FUNC_REG_OFFSET) , Pin);

		/*Enter the mode number in the control register in PMCx bits for this pin*/
		*(volatile uint32 *)((volatile uint8 *)Port_modePtr + PORT_PCTL_REG_OFFSET) |= (0x00000000 + (Mode << (Pin * 4)));
			
                      break;
            
                }/*End of the switch..case*/
          }/*end of the else condition*/
      }/*end of the if(FALSE == error)*/
else
	{
		/*No Action Needed*/
	}


  }
#endif
  

