/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

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
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;        /* From ant_api.c */

extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */



/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static u32 UserApp1_u32DataMsgCount = 0;             /* Counts the number of ANT_DATA packets received */
static u32 UserApp1_u32TickMsgCount = 0;             /* Counts the number of ANT_TICK packets received */

static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

static AntChannelNumberType ANT_CHANNEL_USERAPP;

static AntAssignChannelInfoType sAntSetupData_Seeker;
static AntAssignChannelInfoType sAntSetupData_Master;
static u32 u32Time10s=0;

static bool bNewgame=FALSE;
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
  u8 au8WelcomeMessage[] = "Walcome the Game";
  u8 au8Instructions[] = "B1:Seeker  B2:Master";
  

  /* Clear screen and place start messages */
#ifdef EIE1
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, au8WelcomeMessage); 
  LCDMessage(LINE2_START_ADDR, au8Instructions);

  /* Start with LED0 in RED state = channel is not configured */
  LedOn(RED);
#endif /* EIE1 */
   
 /* Configure ANT for this application (seek)*/
  sAntSetupData_Seeker.AntChannel          = ANT_CHANNEL_USERAPP_SEEKER;
  sAntSetupData_Seeker.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP_SEEKER;
  sAntSetupData_Seeker.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sAntSetupData_Seeker.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  
  sAntSetupData_Seeker.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  sAntSetupData_Seeker.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  sAntSetupData_Seeker.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  sAntSetupData_Seeker.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  sAntSetupData_Seeker.AntFrequency        = ANT_FREQUENCY_USERAPP;
  sAntSetupData_Seeker.AntTxPower          = ANT_TX_POWER_USERAPP;
  /* Configure ANT for this application (master)*/
  sAntSetupData_Master.AntChannel          = ANT_CHANNEL_USERAPP_HIDER;
  sAntSetupData_Master.AntChannelType      = ANT_CHANNEL_TYPE_USERAPP_MASTER;
  sAntSetupData_Master.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sAntSetupData_Master.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  
  sAntSetupData_Master.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  sAntSetupData_Master.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  sAntSetupData_Master.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  sAntSetupData_Master.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  sAntSetupData_Master.AntFrequency        = ANT_FREQUENCY_USERAPP;
  sAntSetupData_Master.AntTxPower          = ANT_TX_POWER_USERAPP;

  sAntSetupData_Master.AntNetwork = ANT_NETWORK_DEFAULT;
  sAntSetupData_Seeker.AntNetwork = ANT_NETWORK_DEFAULT;
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    sAntSetupData_Seeker.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
    sAntSetupData_Master.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
    
  /* If good initialization, set state to Idle */
  if( AntAssignChannel(&sAntSetupData_Seeker) )
  {
      /* Channel assignment is queued so start timer */
#ifdef EIE1
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      LedOn(RED);
#endif /* EIE1 */
  
      UserApp1_StateMachine = UserApp1SM_WaitChannelAssign_Seeker;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
#ifdef EIE1
    LedBlink(RED, LED_4HZ);
#endif /* EIE1 */

    UserApp1_StateMachine = UserApp1SM_Error;
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


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for the ANT channel assignment to finish */
static void UserApp1SM_WaitChannelAssign_Seeker(void)
{
  /* Check if the channel assignment is complete */
  if( AntRadioStatusChannel(ANT_CHANNEL_USERAPP_SEEKER)==ANT_CONFIGURED )
  {
     if( AntAssignChannel(&sAntSetupData_Master) )
    {
        /* Channel assignment is queued so start timer */
#ifdef EIE1
        UserApp1_u32Timeout = G_u32SystemTime1ms;
        LedOn(RED);
#endif /* EIE1 */
  
        UserApp1_StateMachine = UserApp1SM_WaitChannelAssign_Master;
    }
  }
  
  /* Monitor for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, 5000) )
  {
    DebugPrintf("\n\r***Channel assignment timeout***\n\n\r");
    UserApp1_StateMachine = UserApp1SM_Error;
  }
      
} /* end UserApp1SM_WaitChannelAssign_() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for the ANT channel assignment to finish */
static void UserApp1SM_WaitChannelAssign_Master(void)
{
  /* Check if the channel assignment is complete */
  if( AntRadioStatusChannel(ANT_CHANNEL_USERAPP_HIDER)==ANT_CONFIGURED )
  {
    LedOff(RED);
    LedOn(YELLOW);
    
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  
  /* Monitor for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, 5000) )
  {
    DebugPrintf("\n\r***Channel assignment timeout***\n\n\r");
    UserApp1_StateMachine = UserApp1SM_Error;
  }
      
} /* end UserApp1SM_WaitChannelAssign() */

  /*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserApp1SM_Idle(void)
{
  /* Look for BUTTON 0 to open channel */
  if(WasButtonPressed(BUTTON1))
  {
    /* Got the button, so complete one-time actions before next state */
    ButtonAcknowledge(BUTTON1);
    ANT_CHANNEL_USERAPP = ANT_CHANNEL_USERAPP_SEEKER;
  
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Hide and Go Seek!");
    LCDMessage(LINE2_START_ADDR, "Press B0 to Start");

  }
  
    if(WasButtonPressed(BUTTON2))
  {
    /* Got the button, so complete one-time actions before next state */
    ButtonAcknowledge(BUTTON2);
    ANT_CHANNEL_USERAPP = ANT_CHANNEL_USERAPP_HIDER;

    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Hide and Go Seek!");
    LCDMessage(LINE2_START_ADDR, "Press B0 to Start");
  }
  
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    /* Queue open channel and change LED0 from yellow to blinking green to indicate channel is opening */
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    LedOff(YELLOW);
    LedBlink(GREEN, LED_2HZ); 
    
    if( ANT_CHANNEL_USERAPP == ANT_CHANNEL_USERAPP_SEEKER )
    {
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR, "Seeker");
      LedBlink(CYAN,LED_1HZ);
      u32Time10s = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_Wait10s;
    }
    
    if( ANT_CHANNEL_USERAPP == ANT_CHANNEL_USERAPP_HIDER )
    {
      LCDCommand(LCD_CLEAR_CMD);
      LCDMessage(LINE1_START_ADDR, "Hide!");
      /* Set timer and advance states */
      UserApp1_u32Timeout = G_u32SystemTime1ms;
      UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
    }
    

  }
    
} /* end UserApp1SM_Idle() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait 10s */
static void UserApp1SM_Wait10s(void)
{
  LedOff(GREEN);
  
  if( IsTimeUp(&u32Time10s,10000) )
  {
    LCDCommand(LCD_CLEAR_CMD);
    LCDMessage(LINE1_START_ADDR, "Ready or not?");
    LCDMessage(LINE2_START_ADDR, "Here I come!");
    LedOff(CYAN);
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelOpen;
  }

}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for channel to open */
static void UserApp1SM_WaitChannelOpen(void)
{
  /* Monitor the channel status to check if channel is opened */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_OPEN)
  {
#ifdef MPG1
    LedOn(GREEN);
#endif /* MPG1 */    
   
    UserApp1_StateMachine = UserApp1SM_ChannelOpen;
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, TIMEOUT_VALUE) )
  {
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);

#ifdef MPG1
    LedOff(GREEN);
    LedOn(YELLOW);
#endif /* MPG1 */    
  
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
    
} /* end UserApp1SM_WaitChannelOpen() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Channel is open, so monitor data */
static void UserApp1SM_ChannelOpen(void)
{
  static u8 u8LastState = 0xff;
  static u8 au8TickMessage[] = "EVENT x\n\r";  /* "x" at index [6] will be replaced by the current code */
  static u8 au8DataContent[] = "xxxxxxxxxxxxxxxx";
  static u8 au8LastAntData[ANT_APPLICATION_MESSAGE_BYTES] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  static u8 au8TestMessage[] = {0, 0, 0, 0, 0xA5, 0, 0, 0};
  static u8 u8RSSI=95;
  bool bGotNewData;
  

  if( ANT_CHANNEL_USERAPP == ANT_CHANNEL_USERAPP_SEEKER )
  {

  }
  
  if( ANT_CHANNEL_USERAPP == ANT_CHANNEL_USERAPP_HIDER )
  {
    
  }
  
  /* Check for BUTTON0 to close channel */
  if(WasButtonPressed(BUTTON0))
  {
    /* Got the button, so complete one-time actions before next state */
    ButtonAcknowledge(BUTTON0);
    
    /* Queue close channel and change LED to blinking green to indicate channel is closing */
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    u8LastState = 0xff;

#ifdef MPG1
    LedOff(YELLOW);
    LedOff(BLUE);
    LedBlink(GREEN, LED_2HZ);
#endif /* MPG1 */    
   
    /* Set timer and advance states */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelClose;
  } /* end if(WasButtonPressed(BUTTON0)) */
  
  /* Always check for ANT messages */
  if( AntReadAppMessageBuffer() )
  {
     /* New data message: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      u8RSSI = 0-G_sAntApiCurrentMessageExtData.s8RSSI;
      UserApp1_u32DataMsgCount++;
      
      /* We are synced with a device, so blue is solid */
      LedOff(GREEN);
      LedOn(BLUE);

      /* Check if the new data is the same as the old data and update as we go */
      bGotNewData = FALSE;
      for(u8 i = 0; i < ANT_APPLICATION_MESSAGE_BYTES; i++)
      {
        if(G_au8AntApiCurrentMessageBytes[i] != au8LastAntData[i])
        {
          bGotNewData = TRUE;
          au8LastAntData[i] = G_au8AntApiCurrentMessageBytes[i];

          au8DataContent[2 * i]     = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] / 16);
          au8DataContent[2 * i + 1] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] % 16); 
        }
      }
      
      if(bGotNewData)
      {
        /* We got new data: show on LCD */
#ifdef MPG1
        LCDClearChars(LINE2_START_ADDR, 20); 
        LCDMessage(LINE2_START_ADDR, au8DataContent); 
#endif /* MPG1 */    

        /* Update our local message counter and send the message back */
        au8TestMessage[7]++;
        if(au8TestMessage[7] == 0)
        {
          au8TestMessage[6]++;
          if(au8TestMessage[6] == 0)
          {
            au8TestMessage[5]++;
          }
        }
        AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, au8TestMessage);

        /* Check for a special packet and respond */
#ifdef MPG1
        if(G_au8AntApiCurrentMessageBytes[0] == 0xA5)
        {
          LedOff(LCD_RED);
          LedOff(LCD_GREEN);
          LedOff(LCD_BLUE);
          
          if(G_au8AntApiCurrentMessageBytes[1] == 1)
          {
            LedOn(LCD_RED);
          }
          
          if(G_au8AntApiCurrentMessageBytes[2] == 1)
          {
            LedOn(LCD_GREEN);
          }

          if(G_au8AntApiCurrentMessageBytes[3] == 1)
          {
            LedOn(LCD_BLUE);
          }
        }
#endif /* MPG1 */    

      } /* end if(bGotNewData) */
    } /* end if(G_eAntApiCurrentMessageClass == ANT_DATA) */
    
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      UserApp1_u32TickMsgCount++;

      /* Look at the TICK contents to check the event code and respond only if it's different */
      if(u8LastState != G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX])
      {
        /* The state changed so update u8LastState and queue a debug message */
        u8LastState = G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX];
        au8TickMessage[6] = HexToASCIICharUpper(u8LastState);
        DebugPrintf(au8TickMessage);

        /* Parse u8LastState to update LED status */
        switch (u8LastState)
        {
#ifdef MPG1
          /* If we are paired but missing messages, blue blinks */
          case EVENT_RX_FAIL:
          {
            LedOff(GREEN);
            LedBlink(BLUE, LED_2HZ);
            break;
          }

          /* If we drop to search, LED is green */
          case EVENT_RX_FAIL_GO_TO_SEARCH:
          {
            LedOff(BLUE);
            LedOn(GREEN);
            break;
          }
#endif /* MPG 1 */

          /* If the search times out, the channel should automatically close */
          case EVENT_RX_SEARCH_TIMEOUT:
          {
            DebugPrintf("Search timeout event\r\n");
            break;
          }

          case EVENT_CHANNEL_CLOSED:
          {
            DebugPrintf("Channel closed event\r\n");
            break;
          }

            default:
          {
            DebugPrintf("Unexpected Event\r\n");
            break;
          }
        } /* end switch (G_au8AntApiCurrentMessageBytes) */
      } /* end if (u8LastState != G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX]) */
    } /* end else if(G_eAntApiCurrentMessageClass == ANT_TICK) */
    
  } /* end AntReadAppMessageBuffer() */
  
  
  if((u8RSSI<45)&&( ANT_CHANNEL_USERAPP == ANT_CHANNEL_USERAPP_SEEKER ))
  {
    AntCloseChannelNumber(ANT_CHANNEL_USERAPP);
    u8LastState = 0xff;
    bNewgame=TRUE;
#ifdef MPG1
    LedOff(YELLOW);
    LedOff(BLUE);
    LedBlink(GREEN, LED_2HZ);
#endif /* MPG1 */    
   

    /* Set timer and advance states */
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelClose;    
  }
  
  /* A slave channel can close on its own, so explicitly check channel status */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) != ANT_OPEN)
  {
#ifdef MPG1
    LedBlink(GREEN, LED_2HZ);
    LedOff(BLUE);
#endif /* MPG1 */

    u8LastState = 0xff;
    
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_StateMachine = UserApp1SM_WaitChannelClose;
  } /* if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) != ANT_OPEN) */
      
} /* end UserApp1SM_ChannelOpen() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for channel to close */
static void UserApp1SM_WaitChannelClose(void)
{
  /* Monitor the channel status to check if channel is closed */
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CLOSED)
  {
#ifdef MPG1
    LedOff(GREEN);
    LedOn(YELLOW);
#endif /* MPG1 */

    UserApp1_StateMachine = UserApp1SM_Idle;
    if(bNewgame)
    {
      UserApp1_StateMachine = UserApp1SM_ChangeRole;
    }
  }
  
  /* Check for timeout */
  if( IsTimeUp(&UserApp1_u32Timeout, TIMEOUT_VALUE) )
  {
#ifdef MPG1
    LedOff(GREEN);
    LedOff(YELLOW);
    LedBlink(RED, LED_4HZ);
#endif /* MPG1 */

    UserApp1_StateMachine = UserApp1SM_Error;
  }
    
} /* end UserApp1SM_WaitChannelClose() */

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for change role */
static void UserApp1SM_ChangeRole(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, "Hide and Go Seek!");
  LCDMessage(LINE2_START_ADDR, "Press B0 to Start");

   if( ANT_CHANNEL_USERAPP == ANT_CHANNEL_USERAPP_SEEKER )
  {
    ANT_CHANNEL_USERAPP = ANT_CHANNEL_USERAPP_HIDER;

  }
  
  if( ANT_CHANNEL_USERAPP == ANT_CHANNEL_USERAPP_HIDER )
  {
    ANT_CHANNEL_USERAPP == ANT_CHANNEL_USERAPP_SEEKER;
      
  } 
  
  UserApp1_StateMachine = UserApp1SM_Idle;
}
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{

} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
