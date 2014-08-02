'
' StringBuilder internal implementation
'

CLASS thinStringBuilder

  INSTANCE internalCapacity AS LONG
  INSTANCE internalLength   AS LONG
  INSTANCE internalBuffer AS STRING

  CLASS METHOD CREATE()
    internalCapacity = 16
    internalLength   = 0
  END METHOD

  INTERFACE iThinStringBuilder
    INHERIT IUNKNOWN

    METHOD Initialize1()
      internalBuffer = NUL$(internalCapacity)
    END METHOD

    METHOD Initialize2( initialCapacity AS LONG )
      internalCapacity = initialCapacity
      internalBuffer = NUL$(internalCapacity)
    END METHOD

    METHOD Initialize3( initialContent AS STRING )
      me.Add(initialContent)
    END METHOD

    ' --

    METHOD ADD( addedText AS STRING )

      DIM finalLength AS LONG
      DIM addedTextLen AS LONG

      addedTextLen = LEN(addedText)
      finalLength = internalLength + addedTextLen

      IF finalLength > internalCapacity THEN
        LOCAL finalCapacity AS LONG
        finalCapacity = finalLength * 2

        internalBuffer += NUL$(finalCapacity - internalCapacity)

        internalCapacity = LEN(internalBuffer)
      END IF

      MID$(internalBuffer, internalLength+1) = addedText

      internalLength += addedTextLen

    END METHOD

    METHOD AddLine( addedText AS STRING )

      me.Add(addedText + $CRLF)

    END METHOD

    METHOD CLEAR()
      internalCapacity = 0
      internalLength = 0
      internalBuffer = ""
    END METHOD

    PROPERTY GET Capacity AS LONG

      PROPERTY = internalCapacity

    END PROPERTY

    PROPERTY SET Capacity( value AS LONG )

      IF value < internalCapacity THEN
        internalBuffer = LEFT$(internalBuffer, value)

        IF internalLength > value THEN
          internalLength = value
        END IF
      END IF

      IF value > internalCapacity THEN
        internalBuffer += NUL$(value - internalCapacity)
      END IF

      internalCapacity = LEN(internalBuffer)
    END PROPERTY

    PROPERTY GET Length AS LONG

      PROPERTY = internalLength

    END PROPERTY

    PROPERTY GET Char( position AS LONG ) AS STRING

      IF position > internalLength THEN
        thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Position is outside text length. Passed:" + STR$(position) + ", Length:"+STR$(internalLength) )
        EXIT PROPERTY
      END IF

      PROPERTY = MID$(internalBuffer, position, 1)

    END PROPERTY

    PROPERTY SET Char( position AS LONG, value AS STRING )

      IF position > internalLength THEN
        thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Position is outside text length. Passed:" + STR$(position) + ", Length:"+STR$(internalLength) )
        EXIT PROPERTY
      END IF

      IF LEN(value) > 1 THEN
        thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Only single character allowed. Passed:" + value)
        EXIT PROPERTY
      END IF
      MID$(internalBuffer, position) = value

    END PROPERTY

    PROPERTY GET CharCode( position AS LONG ) AS BYTE

      IF position > internalLength THEN
        thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Position is outside text length. Passed:" + STR$(position) + ", Length:"+STR$(internalLength) )
        EXIT PROPERTY
      END IF

      DIM asciiCode(1 TO position) AS BYTE AT STRPTR(internalBuffer)
      PROPERTY = asciiCode(position)

    END PROPERTY

    PROPERTY SET CharCode( position AS LONG, value AS BYTE )

      IF position > internalLength THEN
        thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Position is outside text length. Passed:" + STR$(position) + ", Length:"+STR$(internalLength) )
        EXIT PROPERTY
      END IF

      DIM asciiCode(1 TO position) AS BYTE AT STRPTR(internalBuffer)
      asciiCode(position) = value

    END PROPERTY

    METHOD ToString() AS STRING
      METHOD = LEFT$(internalBuffer, internalLength)
    END METHOD

  END INTERFACE

END CLASS
