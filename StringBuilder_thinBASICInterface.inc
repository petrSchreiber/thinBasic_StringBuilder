'
' StringBuilder thinBASIC interface
'

#INCLUDE "StringBuilder_Implementation.inc"

' -----------------------------------------------------------------------------

FUNCTION StringBuilder_LoadSymbols() AS LONG

  LOCAL RetCode AS LONG
  LOCAL pStringBuilder AS LONG

  pStringBuilder = thinBasic_Class_Add("StringBuilder", 0)

  IF pStringBuilder THEN

    thinBasic_Class_AddMethod(pStringBuilder, "_Create", %thinBasic_ReturnNone , CODEPTR(StringBuilder_Create))
    thinBasic_Class_AddMethod(pStringBuilder, "_Destroy", %thinBasic_ReturnNone , CODEPTR(StringBuilder_Destroy))

    thinBasic_Class_AddMethod(pStringBuilder, "Add", %thinBasic_ReturnCodeLong , CODEPTR(StringBuilder_Add))
    thinBasic_Class_AddMethod(pStringBuilder, "AddLine", %thinBasic_ReturnCodeLong , CODEPTR(StringBuilder_AddLine))
    thinBasic_Class_AddMethod(pStringBuilder, "Clear", %thinBasic_ReturnCodeLong , CODEPTR(StringBuilder_Clear))
    thinBasic_Class_AddMethod(pStringBuilder, "ToString", %thinBasic_ReturnString , CODEPTR(StringBuilder_ToString))

    thinBasic_Class_AddProperty(pStringBuilder, "Char", %thinBasic_ReturnString , CODEPTR(StringBuilder_Property_Char))
    thinBasic_Class_AddProperty(pStringBuilder, "CharCode", %thinBasic_ReturnCodeByte , CODEPTR(StringBuilder_Property_CharCode))
    thinBasic_Class_AddProperty(pStringBuilder, "Length", %thinBasic_ReturnCodeLong , CODEPTR(StringBuilder_Property_Length))
    thinBasic_Class_AddProperty(pStringBuilder, "Capacity", %thinBasic_ReturnCodeLong , CODEPTR(StringBuilder_Property_Capacity))

    FUNCTION = 0
  ELSE
    FUNCTION = 1
  END IF
END FUNCTION

' -----------------------------------------------------------------------------

SUB StringBuilder_Create(BYVAL pObject AS LONG)

  DIM oObject(1& TO 1&) AS iThinStringBuilder AT pObject

  oObject(1&) = CLASS "thinStringBuilder"

  IF ISNOTHING(oObject(1&)) THEN
    thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Failed to instantiate StringBuilder" )
  END IF

  DIM initialCapacity AS EXT
  DIM initialText AS STRING
  IF thinBasic_CheckOpenParens_Optional() AND thinBasic_ErrorFree() THEN

    IF thinBasic_CheckCloseParens_Optional() AND thinBasic_ErrorFree() THEN
      oObject(1&).Initialize1()
    ELSE

      SELECT CASE thinBasic_DetermineType
        CASE %thinBasic_ReturnNumber
          thinBasic_ParseNumber initialCapacity

          IF thinBasic_CheckCloseParens_Mandatory() AND thinBasic_ErrorFree() THEN
            oObject(1&).Initialize2(BYCOPY initialCapacity)
          END IF

        CASE %thinBasic_ReturnString
          thinBasic_ParseString initialText

          IF thinBasic_CheckCloseParens_Mandatory() AND thinBasic_ErrorFree() THEN
            oObject(1&).Initialize3(initialText)
          END IF
      END SELECT

    END IF
  ELSE
    oObject(1&).Initialize1()
  END IF

END SUB

FUNCTION StringBuilder_Destroy(BYVAL pObject AS LONG) AS LONG

  DIM oObject(1& TO 1&) AS iThinStringBuilder AT pObject
  oObject(1&) = NOTHING

END FUNCTION

' -----------------------------------------------------------------------------

FUNCTION StringBuilder_Add(BYVAL pObject AS LONG) AS LONG

  DIM oObject(1& TO 1&) AS iThinStringBuilder AT pObject

  DIM addedText AS STRING
  IF thinBasic_CheckOpenParens_Mandatory() AND thinBasic_ErrorFree() THEN
    thinBasic_ParseString addedText

    IF thinBasic_ErrorFree() THEN
      oObject(1&).Add(addedText)
    END IF

    WHILE thinBasic_CheckComma_Optional() AND thinBasic_ErrorFree()
      thinBasic_ParseString addedText
      oObject(1&).Add(addedText)
    WEND

    thinBasic_CheckCloseParens_Mandatory()

  END IF

END FUNCTION

FUNCTION StringBuilder_AddLine(BYVAL pObject AS LONG) AS LONG

  DIM oObject(1& TO 1&) AS iThinStringBuilder AT pObject

  DIM addedText AS STRING
  IF thinBasic_CheckOpenParens_Mandatory() AND thinBasic_ErrorFree() THEN
    thinBasic_ParseString addedText

    IF thinBasic_CheckCloseParens_Mandatory() AND thinBasic_ErrorFree() THEN
      oObject(1&).AddLine(addedText)
    END IF
  END IF

END FUNCTION

FUNCTION StringBuilder_Clear(BYVAL pObject AS LONG) AS LONG

  DIM oObject(1& TO 1&) AS iThinStringBuilder AT pObject

  IF thinBasic_CheckOpenParens_Mandatory() AND thinBasic_ErrorFree() THEN
    IF thinBasic_CheckCloseParens_Mandatory() AND thinBasic_ErrorFree() THEN
      oObject(1&).Clear()
    END IF
  END IF

END FUNCTION

FUNCTION StringBuilder_ToString(BYVAL pObject AS LONG) AS STRING

  DIM oObject(1& TO 1&) AS iThinStringBuilder AT pObject

  IF thinBasic_CheckOpenParens_Mandatory() AND thinBasic_ErrorFree() THEN
    IF thinBasic_CheckCloseParens_Mandatory() AND thinBasic_ErrorFree() THEN
      FUNCTION = oObject(1&).ToString()
    END IF
  END IF

END FUNCTION

' -----------------------------------------------------------------------------

FUNCTION StringBuilder_Property_Char(BYVAL pObject AS LONG, BYVAL lAction AS LONG) AS STRING

  REDIM oObject(1& TO 1&) AS iThinStringBuilder AT pObject

  DIM position AS EXT
  IF thinBasic_CheckOpenParens_Mandatory() AND thinBasic_ErrorFree() THEN
    thinBasic_ParseNumber position

    IF thinBasic_CheckCloseParens_Mandatory() AND thinBasic_ErrorFree() THEN

      SELECT CASE LONG lAction

        CASE %Class_Action_Get
          FUNCTION = oObject(1&).Char(BYCOPY position)

        CASE %Class_Action_Set
          LOCAL lEqualType    AS LONG
          lEqualType = thinBasic_CheckEqualType_Mandatory

          IF thinBasic_ErrorFree THEN
            IF lEqualType = %Equal_EQ THEN
              DIM character AS STRING
              thinBasic_ParseString character

              IF thinBasic_ErrorFree THEN
                oObject(&1).Char(BYCOPY position) = character
              END IF
            END IF
          END IF

      END SELECT

    END IF
  END IF

END FUNCTION

FUNCTION StringBuilder_Property_CharCode(BYVAL pObject AS LONG, BYVAL lAction AS LONG) AS BYTE

  REDIM oObject(1& TO 1&) AS iThinStringBuilder AT pObject

  DIM position AS EXT
  IF thinBasic_CheckOpenParens_Mandatory() AND thinBasic_ErrorFree() THEN
    thinBasic_ParseNumber position

    IF thinBasic_CheckCloseParens_Mandatory() AND thinBasic_ErrorFree() THEN

      SELECT CASE LONG lAction

        CASE %Class_Action_Get
          FUNCTION = oObject(1&).CharCode(BYCOPY position)

        CASE %Class_Action_Set
          LOCAL lEqualType    AS LONG
          lEqualType = thinBasic_CheckEqualType_Mandatory

          IF thinBasic_ErrorFree THEN
            IF lEqualType = %Equal_EQ THEN
              DIM characterCode AS EXT
              thinBasic_ParseNumber characterCode

              IF thinBasic_ErrorFree THEN
                oObject(&1).CharCode(BYCOPY position) = characterCode
              END IF
            END IF
          END IF

      END SELECT

    END IF
  END IF

END FUNCTION


FUNCTION StringBuilder_Property_Length(BYVAL pObject AS LONG, BYVAL lAction AS LONG) AS LONG

  REDIM oObject(1& TO 1&) AS iThinStringBuilder AT pObject

  SELECT CASE LONG lAction

    CASE %Class_Action_Get
      FUNCTION = oObject(1&).Length

    CASE %Class_Action_Set
      thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Assignment to Length is not supported." )

  END SELECT

END FUNCTION

FUNCTION StringBuilder_Property_Capacity(BYVAL pObject AS LONG, BYVAL lAction AS LONG) AS LONG

  REDIM oObject(1& TO 1&) AS iThinStringBuilder AT pObject

  SELECT CASE LONG lAction

    CASE %Class_Action_Get
      FUNCTION = oObject(1&).Capacity

    CASE %Class_Action_Set
      LOCAL lEqualType    AS LONG
      lEqualType = thinBasic_CheckEqualType_Mandatory

      IF thinBasic_ErrorFree THEN
        IF lEqualType = %Equal_EQ THEN
          DIM capacity AS EXT
          thinBasic_ParseNumber capacity

          IF thinBasic_ErrorFree THEN
            oObject(&1).Capacity = capacity
          END IF
        END IF
      END IF


  END SELECT

END FUNCTION
