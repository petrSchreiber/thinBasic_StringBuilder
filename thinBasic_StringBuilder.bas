'
' StringBuilder module for thinBASIC
'

' -----------------------------------------------------------------------------

#COMPILE DLL
#REGISTER NONE
#DIM ALL

' -- The version itself should reflect the targetted thinBasic version
'------------------------------------------------------------------
#RESOURCE RES "thinBasic_StringBuilder.RES"
' -----------------------------------------------------------------------------

' -- thinBASIC module SDK
#INCLUDE "..\module_core\thinCore.inc"

' -- functionality exposed to ThinBASIC
#Include "StringBuilder_thinBASICInterface.inc"

' -- functionality exposed to ThinBASIC
#Include ".\StringBuilder_Array_thinBASICInterface.inc"

'----------------------------------------------------------------------------

FUNCTION LoadLocalSymbols ALIAS "LoadLocalSymbols" (OPTIONAL BYVAL sPath AS STRING) EXPORT AS LONG

  ' -- exposes the functionality to thinBasic
  StringBuilder_LoadSymbols()
  
  ' -- exposes the functionality to thinBasic
  StringBuilder_Array_LoadSymbols()

  FUNCTION = 0&

END FUNCTION

' -----------------------------------------------------------------------------

FUNCTION UnLoadLocalSymbols ALIAS "UnLoadLocalSymbols" () EXPORT AS LONG

  FUNCTION = 0&

END FUNCTION

' -----------------------------------------------------------------------------

%DLL_PROCESS_ATTACH   = 1
%DLL_THREAD_ATTACH    = 2
%DLL_THREAD_DETACH    = 3
%DLL_PROCESS_DETACH   = 0
FUNCTION LIBMAIN ALIAS "LibMain" (BYVAL hInstance   AS LONG, _
                                  BYVAL fwdReason   AS LONG, _
                                  BYVAL lpvReserved AS LONG) EXPORT AS LONG
  SELECT CASE fwdReason
    CASE %DLL_PROCESS_ATTACH

      FUNCTION = 1
      EXIT FUNCTION
    CASE %DLL_PROCESS_DETACH

      FUNCTION = 1
      EXIT FUNCTION
    CASE %DLL_THREAD_ATTACH

      FUNCTION = 1
      EXIT FUNCTION
    CASE %DLL_THREAD_DETACH

      FUNCTION = 1
      EXIT FUNCTION
  END SELECT

END FUNCTION
