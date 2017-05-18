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

extern u8 G_au8DebugScanfBuffer[];
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
  
 
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE); 
  LedOff(CYAN);
  LedOff(GREEN);      
  LedOff(YELLOW);
  LedOff(ORANGE);       
  LedOff(RED);
  
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    
    LCDCommand(LCD_CLEAR_CMD);
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


/*Move the String2 and spme part of String3.One bit One Time*/
static void MoveLeft(u8 au8String2[],u8 au8String3[],u8 u8Lenghtof3)
{
  u8 u8Counter2;
  u8 u8First1;
  u8 u8First2;
  u8First1=au8String2[0];
  u8First2=au8String3[0];
  
  
  for(u8Counter2=0;u8Counter2<39;u8Counter2++)
    {
      au8String2[u8Counter2]=au8String2[u8Counter2+1];
    }
  
  au8String2[39]=u8First2;
  
  for(u8Counter2=0;u8Counter2<(u8Lenghtof3-1);u8Counter2++)
  {
    au8String3[u8Counter2]=au8String3[u8Counter2+1];
  }
  
  au8String3[u8Lenghtof3-1]=u8First1;
  
}

/*To counter how many bits we input*/
static u8 COUNTER(u8 au8String4[])
{
  u8 u8Counter3=0;
  
  for(;u8Counter3<255;u8Counter3++)
  {
    if(au8String4[u8Counter3]=='\r')
    {
      break;
    }
  }
   
  return u8Counter3;
}

/*To know the inputing end or not*/
static bool TheInputEnd(u8 au8String5[])
{
  u8 u8Counter4=0;
  bool bEnd=FALSE;
  
  for(;u8Counter4<255;u8Counter4++)
  {
    if(au8String5[u8Counter4]=='\r')
    {
      bEnd=TRUE;
      break;
    }
  }
  
  return bEnd;
}

/*The top 20 of au8Words is given to Line1*/
static void LINE1(u8 au8Line1[],u8 au8Words[])
{
  u8 u8Counter01;
  for(u8Counter01=0;u8Counter01<20;u8Counter01++)
  {
    au8Line1[u8Counter01]=au8Words[u8Counter01];
  }
}

/*The last 20 of au8Words is given to Line2*/
static void LINE2(u8 au8Line1[],u8 au8Words[])
{
  u8 u8Counter02;
  for(u8Counter02=0;u8Counter02<20;u8Counter02++)
  {
    au8Line1[u8Counter02]=au8Words[u8Counter02+20];
  }
}
     
           
/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  static u8 u8Led=0;
  static u8 u8Counter1=0;
  static u8 au8ReadWords[255]="\0";
  static u8 au8Move[40]="                                        ";
  static u8 u8OneTime=1;
  static u8 au8DisplayName[20]="***Shi**Chen**Jun***";
  static u8 au8Line1[20]="\0";
  static u8 au8Line2[20]="\0";
  static u32 u32Time1=0;
  static u32 u32Time2=0;
  static u32 u32Time3=0;
  static u32 u32Time4=0;
  static bool bMove1=FALSE;
  static bool bMove2=FALSE; 
  static bool bInput=FALSE;
  static bool bLed=TRUE;
/*To check the inputing.If it is ended.Turn off the diaplaying of name and go to display the 
  words inputed*/  
  if(!bInput)
  {
  strcpy(au8ReadWords,G_au8DebugScanfBuffer);
  }
  
  bInput=TheInputEnd(au8ReadWords);
/*if there is not inputing.Wait and display again and again*/
if(!bInput)
{
  u32Time1++;
  
  if(u32Time1==500)
  {
    bMove1=TRUE;
    u32Time1=0;
  }
  else
  {
    bMove1=FALSE;
  }  
  
  if(bMove1)
  {
    
    LINE1(au8Line1,au8Move);
    LINE2(au8Line2,au8Move);  
    LCDMessage(LINE1_START_ADDR,au8Line1);
    LCDMessage(LINE2_START_ADDR,au8Line2);
    MoveLeft(au8Move,au8DisplayName,20);
  }
}
/*Display the words*/
else
{
  u8Counter1=COUNTER(au8ReadWords);
  if(u8Counter1<=41)
  {
    LCDCommand(LCD_CLEAR_CMD);
    LINE1(au8Line1,au8ReadWords);
    LCDMessage(LINE1_START_ADDR,au8Line1);
    LINE2(au8Line2,au8ReadWords);
    LCDMessage(LINE2_START_ADDR,au8Line2);
  }
  else
  {
    u32Time2++;
  
    if(u32Time2==500)
    {
      bMove2=TRUE;
      u32Time2=0;
    }
    else
    {
      bMove2=FALSE;
    }
     
    if(bMove2)
    {
      if(u8OneTime)
      {
        u8 u8CON=0;
        for(u8CON=0;u8CON<40;u8CON++)
        {
          au8Move[u8CON]=' ';
        }
        u8OneTime=0;
      }
      
      LINE1(au8Line1,au8Move);
      LINE2(au8Line2,au8Move);  
      LCDMessage(LINE1_START_ADDR,au8Line1);
      LCDMessage(LINE2_START_ADDR,au8Line2);
      MoveLeft(au8Move,au8ReadWords,u8Counter1);
    }
    
  }
}
  
/*Change the colour of LED every 500ms.THe BUTTON0 can turn off the */
if(bLed)
 { 
  u32Time3++;
  
  if(u32Time3==500)
  {
     switch(u8Led)
       {
        case 0:LedOff(WHITE);
               LedOff(PURPLE);
               LedOff(BLUE); 
               LedOff(CYAN);
               LedOff(GREEN);      
               LedOff(YELLOW);
               LedOff(ORANGE);       
               LedOn(RED);break;  
    
        case 1:LedOff(WHITE);
               LedOff(PURPLE);
               LedOff(BLUE); 
               LedOff(CYAN);
               LedOff(GREEN);      
               LedOff(YELLOW);
               LedOn(ORANGE);       
               LedOff(RED);break;   
      
        case 2:LedOff(WHITE);
               LedOff(PURPLE);
               LedOff(BLUE); 
               LedOff(CYAN);
               LedOff(GREEN);      
               LedOn(YELLOW);
               LedOff(ORANGE);       
               LedOff(RED);break;   
      
        case 3:LedOff(WHITE);
               LedOff(PURPLE);
               LedOff(BLUE); 
               LedOff(CYAN);
               LedOn(GREEN);      
               LedOff(YELLOW);
               LedOff(ORANGE);       
               LedOff(RED);break;  
      
        case 4:LedOff(WHITE);
               LedOff(PURPLE);
               LedOff(BLUE); 
               LedOn(CYAN);
               LedOff(GREEN);      
               LedOff(YELLOW);
               LedOff(ORANGE);       
               LedOff(RED);break;  
      
        case 5:LedOff(WHITE);
               LedOff(PURPLE);
               LedOn(BLUE); 
               LedOff(CYAN);
               LedOff(GREEN);      
               LedOff(YELLOW);
               LedOff(ORANGE);       
               LedOff(RED);break; 
           
        case 6:LedOff(WHITE);
               LedOn(PURPLE);
               LedOff(BLUE); 
               LedOff(CYAN);
               LedOff(GREEN);      
               LedOff(YELLOW);
               LedOff(ORANGE);       
               LedOff(RED);break;         
           
        case 7:LedOn(WHITE);
               LedOff(PURPLE);
               LedOff(BLUE); 
               LedOff(CYAN);
               LedOff(GREEN);      
               LedOff(YELLOW);
               LedOff(ORANGE);       
               LedOff(RED);break;          
           
        default:break;       
       }
        u8Led++;
        u32Time3=0;
        
       if(u8Led==8)
       {
        u8Led=0;
       }
   }
 }

  if(WasButtonPressed(BUTTON0))
  {
   ButtonAcknowledge(BUTTON0);
   u8Led=0;
   u32Time3=0;
   bLed=!bLed;
   LedOff(WHITE);
   LedOff(PURPLE);
   LedOff(BLUE); 
   LedOff(CYAN);
   LedOff(GREEN);      
   LedOff(YELLOW);
   LedOff(ORANGE);       
   LedOff(RED);
  } 

/*Buzzer 2s*/  
if(1)
  {
   if(u32Time4<2000)
   {
   u32Time4++;
   ButtonAcknowledge(BUTTON1);
   PWMAudioOn(BUZZER1);
   PWMAudioSetFrequency(BUZZER1, 294);
   }
   
   else
   {
     PWMAudioOff(BUZZER1);
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
