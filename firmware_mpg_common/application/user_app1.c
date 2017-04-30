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
  LedOff(WHITE);
  LedOff(PURPLE);
  LedOff(BLUE);
  LedOff(CYAN);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(ORANGE);
  LedOff(RED);
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

/*Read the code which we input.*/
static void ReadButton(void)
      {  
            u8 u8Button_Value=9;
              
             if(WasButtonPressed(BUTTON0))
             {
                ButtonAcknowledge(BUTTON0);
                u8ButtonValue = 1;
             }
             
             if(WasButtonPressed(BUTTON1))
             {
              ButtonAcknowledge(BUTTON1);
              u8ButtonValue = 2;
             }
             
             if(WasButtonPressed(BUTTON2))
             {
              ButtonAcknowledge(BUTTON2);
              u8ButtonValue = 3;
             }
             
             return u8Button_Value;
      }

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
static u8 u8ButtonValue=0;  
static u8 u8Counter1=0;
static u8 u8Counter2=0;
static u8 u8Lenght_of_Code=0;
static u8 au8True_Password[]={1,1,1,1,1,1};
static u8 au8Input_Password[]={9,9,9,9,9,9};
static u32 u32HeldTime=1000;
static b bSetup=FALSE;
static b bInput=FALSE;
 
u8ButtonValue=ReadButton();
u8Lenght_of_Code=(sizeof(au8True_Password)/sizeof(au8True_Password[0]);

/*Choose the settings or import*/
 if(u8Counter==0&&IsButtonHeld(BUTTON3,u32HeldTime))
 {
  bSetup=TRUE;
  bInput=FALSE;
 }
 else
 {
  bSetup=FALSE;
  bInput=TRUE; 
 }

/*Set up the new code*/
 if(bSetup)
 {
   LedBlink(RED,LED_1HZ);
   LedBlink(GREEN,LED_1HZ);
   
   if(u8ButtonValue!=9)
   {
     au8True_Password[u8Counter1]=u8ButtonValue;
     u8Counter++;
     ?????????????????
   }   
 else
 {
  bInput=TRUE;
  u8Counter1=0;
 }
  
 /*Read the code*/
  if(bInput)
  {
    RedOn(RED);
    if(u8Counter2<u8Lenght_of_Code)
    {
      if(u8ButtonValue!=9)
      {      
       au8Input_Password[u8Counter2]=u8ButtonValue;
       u8Counter2++;
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
