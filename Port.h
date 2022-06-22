 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Mohamed Tarek
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

#include "Common_Macros.h"
#include "Std_Types.h"

/* Id for the company in the AUTOSAR*/

#define PORT_VENDOR_ID    (1000U)

/* PORT Module Id */
#define PORT_MODULE_ID    (120U)

/* PORT Instance Id */
#define PORT_INSTANCE_ID  (0U)
/*
 * Module Version 1.0.0
 */
#define PORT_PBCFG_SW_MAJOR_VERSION              (1U)
#define PORT_PBCFG_SW_MINOR_VERSION              (0U)
#define PORT_PBCFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

/*
 * Macros for PORT Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/******************************************************************************
 *                      Versions Check                                        *
 ******************************************************************************/

/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* PORT Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif

/* Non AUTOSAR files */
#include "Common_Macros.h"
   
 /******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
    /*Service ID for Port Driver module initializing function*/
   #define PORT_INIT_SID                                (uint8)0x00
  
   /*Service ID for Setting the port pin direction function*/
   #define PORT_SET_PIN_DIRECTION_SID         		(uint8)0x01
   
   /*service ID for Refreshing port direction function*/
   #dfine PORT_REFRESH_PORT_DIRECTION_SID               (uint8)0x02
   
   /*Service ID for Port module GetVersionInfo function*/
   #define PORT_GET_VERSION_INFO _SID                   (uint8)0x03
    
   /*Service ID Setting the Port pin mode*/
   #define PORT_SET_PIN_MODE_SID                        (uint8)0x04
    
/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
   /*DET error for: Incorrect Port Pin ID passed to Port_SetPinDirection function*/
   #define  PORT_E_PARAM_PIN                    (uint8)0x0A
   
   /*DET error for: Port Pin not configured as changeable passed to Port_SetPinDirection*/
   #define  PORT_E_DIRECTION_UNCHANGEABLE       (uint8)0x0B
   
   /*DET error for: Port_Init service called with wrong parameter.*/
   #define PORT_E_PARAM_CONFIG                  (uint8)0x0C
    
   /*DET error for: API Port_SetPinMode service called when mode is not valid.*/
   #define  PORT_E_PARAM_INVALID_MODE           (uint8)0x0D
   
   /*DET error for: API Port_SetPinMode service called when mode is unchangeable.*/
   #define  PORT_E_MODE_UNCHANGEABLE            (uint8)0x0E
   
   /*DET error for: API service called without module initialization*/
   #define  PORT_E_UNINIT                       (uint8)0x0F
   
   /*DET error for:API Port_GetVersionInfo called with a Null Pointer*/ 
   #define PORT_E_PARAM_POINTER                 (uint8)0x10
   


/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

   /*Type definition for the symbolic name of a port pin. That shall cover all available port pins.*/
#define uint8    Port_PinType
     
  /*Type definition for different port pin modes*/
#define uint8    Port_PinModeType
     
/* Description: Enum to hold PIN direction. */
typedef enum
{
    PORT_PIN_IN, /*Sets port pin as input.*/
    PORT_PIN_OUT /*Sets port pin as output.*/
}Port_PinDirectionType;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_InternalResistor;

/*Description: Enum to choose whether the pin direction is changeable during the runtime or not*/
typedef enum 
{
    PIN_DIRECTION_CHANGEABLE_OFF,
    PIN_DIRECTION_CHANGEABLE_ON
}Pin_directionChangeable;

/*Description: Enum to choose whether the pin mode is changeable during the runtime or not*/
typedef enum 
{
    PIN_MODE_CHANGEABLE_OFF,
    PIN_MODE_CHANGEABLE_ON
}Pin_modeChangeable;

typedef enum 
{
  PORT_A,PORT_B,PORT_C,PORT_D,PORT_E,PORT_F
}Port_ID;

/* Description: Structure to configure each individual PIN:
 *	1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
 *	2. the number of the pin in the PORT.
 *      3. the pin mode in the port, GPIO or other Alternative mode.
 *      4. the direction of pin --> INPUT or OUTPUT
 *      5. the initial value of the pin -> STD_HIGH / STD_LOW
 *      6. the status of the pin direction changeability -> STD_ON/STD_OFF
 *      7. the status of the pin mode changeability -> STD_ON/STD_OFF
 *      8. the internal resistor --> Disable, Pull up or Pull down
 */
typedef struct 
{
    Port_ID                             port_num; 
    
    Port_PinType                        pin_num;
  /*
    Name: Port_PinModeType
    Type: uint8,uint16,uint32 -> Port_PinType
    Range:Implementation specific. As several port pin modes shall be configurable on one pin, the range shall be determined by the implementation.
    Description: Different port pin modes.
  */
    Port_PinModeType                    Pin_mode; 
    /*
    Name: Port_PinDirectionType
    Type: Enumeration
    Range: PORT_PIN_IN & PORT_PIN_OUT
    Description: Possible directions of a port pin.
  */
    Port_PinDirectionType               Pin_direction;
    
    uint8                               Pin_level_initial_value;
    
    Pin_directionChangeable             Pin_directionChange;
    
    Pin_modeChangeable                  Pin_modeChange;
      
    Port_InternalResistor               resistor;
 
}Port_ConfigChannel;

/*Data structure required to initialize the Port driver*/
typedef struct Port_ConfigType
{
  Port_ConfigChannel Channels[PORT_CONFIGURED_CHANNLES];
}Port_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/************************************************************************************
* Service Name: Port_Init
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
void Port_Init(const Port_ConfigType* ConfigPtr );

#if (PORT_SET_PIN_DIRECTION_API ==STD_ON)
/************************************************************************************
* Service Name: Port_SetPinDirection
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
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction );
#endif

/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Sync/Async:   Synchronous
* Reentrancy:   Non Reentrant
* Parameters (in):      None
* Parameters (inout):   None
* Parameters (out):     None
* Return value:         None
* Description: Function to refresh port direction.:
*              - refreshs the direction of all configured ports to the configured direction (PortPinDirection).
************************************************************************************/
void Port_RefreshPortDirection(void );

#if (PORT_VERSION_INFO_API==STD_ON)
/************************************************************************************
* Service Name: Port_GetVersionInfo
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
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo);
#endif

#if (PORT_SET_PIN_MODE_API==STD_ON)
/************************************************************************************
* Service Name: Port_SetPinMode
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
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode );
#endif

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Port and other modules */
extern const Port_ConfigType Port_Configuration;

#endif /* PORT_H */
