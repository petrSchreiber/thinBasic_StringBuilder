'
' StringBuilder module for thinBASIC
'

' -----------------------------------------------------------------------------

#COMPILE DLL
#REGISTER NONE
#DIM ALL

' -- The version itself should reflect the targetted thinBasic version
'------------------------------------------------------------------
#Include "thinBasic_StringBuilder_resources.inc"
#Resource VERSIONINFO
#Resource FILEVERSION %THINBASIC_STRINGBUILDER_VERSIONMAJOR, %THINBASIC_STRINGBUILDER_VERSIONMINOR, %THINBASIC_STRINGBUILDER_VERSIONBUILD, %THINBASIC_STRINGBUILDER_VERSIONREVISION
#Resource PRODUCTVERSION %THINBASIC_STRINGBUILDER_VERSIONMAJOR, %THINBASIC_STRINGBUILDER_VERSIONMINOR, %THINBASIC_STRINGBUILDER_VERSIONBUILD, %THINBASIC_STRINGBUILDER_VERSIONREVISION

#Resource STRINGINFO "0409", "04E4"

#Resource VERSION$ "CompanyName",      $THIN_COMMON_COMPANYNAME
#Resource VERSION$ "FileDescription",  $THINBASIC_STRINGBUILDER_FILEDESCRIPTION
#Resource VERSION$ "FileVersion",      $THINBASIC_STRINGBUILDER_VERSIONLITERAL
#Resource VERSION$ "InternalName",     $THINBASIC_STRINGBUILDER_INTERNALNAME
#Resource VERSION$ "OriginalFilename", $THINBASIC_STRINGBUILDER_ORIGINALFILENAME
#Resource VERSION$ "LegalCopyright",   $THIN_COMMON_LEGALCOPYRIGHT
#Resource VERSION$ "ProductName",      $THINBASIC_STRINGBUILDER_PRODUCTNAME
#Resource VERSION$ "ProductVersion",   $THINBASIC_STRINGBUILDER_VERSIONLITERAL
#Resource VERSION$ "Comments",         $THIN_COMMON_COMMENT
' -----------------------------------------------------------------------------

' -- thinBASIC module SDK
#INCLUDE "thinCore.inc"

' -- functionality exposed to ThinBASIC
#Include "StringBuilder_thinBASICInterface.inc"

'----------------------------------------------------------------------------

FUNCTION LoadLocalSymbols ALIAS "LoadLocalSymbols" (OPTIONAL BYVAL sPath AS STRING) EXPORT AS LONG

  ' -- exposes the functionality to thinBasic
  StringBuilder_LoadSymbols()

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
