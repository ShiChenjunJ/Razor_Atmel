/**********************************************************************************************************************
File: user_app1.c                                                                
ShiChenjun
----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_FailedInit;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
extern u8 G_u8DebugScanfCharCount;

/*Judge the digits of the decimal number*/
static u32 u32NumberDigit(u32 u32Number)
{
  u8 u8Digit=0;
  
  while(u32Number)
  {
    u32Number=(u32Number/10);
    u8Digit++;
  }
  
  return u8Digit;
}

/*Make the border*/
static void OutputBorder(u8 au8String[],u8 u8Digit)
{
  u8 u8Counter3;
  u8Digit=(u8Digit+2);
  for(u8Counter3=0;u8Counter3<u8Digit;u8Counter3++)
  {
    au8String[u8Counter3]='*';
  }
}

/*shift the word*/
static void Shift(u8 au8String1[],u8 au8String2[])
{
  u8 u8Location1;
  
    for(u8Location1=0;u8Location1<2;u8Location1++)
    {
      au8String1[u8Location1]=au8String1[u8Location1+1];
    }
    
    au8String1[2]=au8String2[0];
}

/*judge the two strings*/
static bool StringSame(u8 au8String3[],u8 au8String4[])
{
  u8 u8Location2;
  u8 u8Counter=0;
  bool bSame=FALSE;
  
  for(u8Location2=0;u8Location2<4;u8Location2++)
  {
    if(au8String3[u8Location2]==au8String4[u8Location2])
    {
      u8Counter++;
    }
  }
  
  if(u8Counter==4)
  {
    bSame=TRUE;
  }
  
  return bSame;
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
static u8 u8Counter1=0;
  static u8 u8TimesDigit=0;
  
  static u8 au8IsInputWord[1]="0";
  static u8 au8InputName[3]={0,0,0};
  static u8 au8Name[3]={'s','s','j'};
  
  static u8 au8Border[100]="0";
  static u8 au8Border2[]="\n\r";
  static u8 au8Border3[1];
  
  static u32 u32CounterOfName=0;
  static bool bNameAppear=FALSE;
  
  au8Border3[0]='*';
  
/*Read the word and recognize the name*/ 
  if(G_u8DebugScanfCharCount==1)
  { 
    DebugScanf(au8IsInputWord);
    
    if(u8Counter1<3)
    {
      au8InputName[u8Counter1]=au8IsInputWord[0];
      
      if(u8Counter1==2)
      {
        bNameAppear=StringSame(au8InputName,au8Name);
      }
        
      u8Counter1++;
    }
  else
    {
      Shift(au8InputName,au8IsInputWord);
      bNameAppear=StringSame(au8InputName,au8Name);
      
    }
  
  /*Output the times of Name*/
  if(bNameAppear)
  {
    
    u32CounterOfName++;
    u8TimesDigit=u32NumberDigit(u32CounterOfName);
    
    OutputBorder(au8Border,u8TimesDigit);
    
    DebugPrintf(au8Border2);
    DebugPrintf(au8Border);
    DebugPrintf(au8Border2);
    DebugPrintf(au8Border3);
    DebugPrintNumber(u32CounterOfName);
    DebugPrintf(au8Border3);
    DebugPrintf(au8Border2);
    DebugPrintf(au8Border);
    DebugPrintf(au8Border2);
   
    u8Counter1=0;
    bNameAppear=FALSE;
  }
  
 }
} /* end UserApp1SM_Idle() */
    
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp1SM_FailedInit(void)          
{
    
} /* end UserApp1SM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
