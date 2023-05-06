  '
  ' StringBuilder internal implementation
  '
  
  Class thinStringBuilder
  
    Instance defaultCapacity    As Dword
    Instance internalCapacity   As Dword
    
    Instance internalLength     As Dword
    Instance internalBuffer     As String 
    
    Instance MaxAddedStringLen  As Dword
  
    '--------------------------------------------
    Class Method Create()
    '--------------------------------------------
      defaultCapacity  = 16
    End Method
  
    '--------------------------------------------
    Interface iThinStringBuilder
      Inherit IUnknown
  
      '--------------------------------------------
      Method Initialize1()
      '--------------------------------------------
        internalBuffer   = Nul$(defaultCapacity)
        internalCapacity = Len(internalBuffer)
        internalLength   = 0
      End Method
  
      '--------------------------------------------
      Method Initialize2( initialCapacity As Dword )
      '--------------------------------------------
        defaultCapacity = initialCapacity
        Me.Initialize1()
      End Method
  
      '--------------------------------------------
      Method Initialize3(ByRef initialContent As String )
      '--------------------------------------------
        Me.Initialize2(Len(initialContent) * 2)
        Me.Add(initialContent)
      End Method
  
      '--------------------------------------------
      Method Add(ByRef addedText As String )
      '--------------------------------------------
        Local finalLength             As Dword
        Local addedTextLen            As Dword
        Local finalCapacity           As Dword  
        Local lIncrement              As Dword
        Local lIncreaseBufferStrategy As Long
  
        addedTextLen = Len(addedText)
        finalLength = internalLength + addedTextLen
        
        '---Determine max added string len so far
        If MaxAddedStringLen < addedTextLen Then MaxAddedStringLen = addedTextLen
        
        If finalLength > internalCapacity Then
          '---We need to increase internal string adding more capacity
          '---But how much more capacity? Here different strategies
            
            lIncreaseBufferStrategy = 1
            Select Case Long lIncreaseBufferStrategy
              Case 1  '---Startegy 1: increase by double current final len
                lIncrement = finalLength
                finalCapacity = finalLength + finalLength
            
              Case 2  '---Startegy 2: add 4 times the current added text ...
                lIncrement = addedTextLen * 4
                finalCapacity = finalLength + lIncrement

              Case 3  '---Startegy 3: add Max Added String Len so far ...
                lIncrement = MaxAddedStringLen
                finalCapacity = finalLength + lIncrement

            End Select
            
          internalBuffer += Nul$(finalCapacity - internalCapacity)

          internalCapacity = Len(internalBuffer)

        End If
  
        Mid$(internalBuffer, internalLength + 1) = addedText
  
        internalLength += addedTextLen
  
      End Method
  
      '--------------------------------------------
      Method AddLine(ByRef addedText As String )
      '--------------------------------------------
        Me.Add(addedText + $CrLf)
      End Method
  
      '--------------------------------------------
      Method Clear()
      '--------------------------------------------
        Me.Initialize1()
      End Method
  
      '--------------------------------------------
      Property Get Capacity As Dword
      '--------------------------------------------
        Property = internalCapacity
      End Property
  
      '--------------------------------------------
      Property Set Capacity(ByVal value As Dword )
      '--------------------------------------------
        If value < internalCapacity Then
          internalBuffer = Left$(internalBuffer, value)
  
          If internalLength > value Then
            internalLength = value
          End If
        End If
  
        If value > internalCapacity Then
          internalBuffer += Nul$(value - internalCapacity)
        End If
  
        internalCapacity = Len(internalBuffer)
      End Property
  
      '--------------------------------------------
      Property Get Length As Dword
      '--------------------------------------------
        Property = internalLength
      End Property
  
      '--------------------------------------------
      Property Get Char(ByVal position As Dword ) As String
      '--------------------------------------------
        If position > internalLength Then
          thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Position is outside text length. Passed:" + Str$(position) + ", Length:" + Str$(internalLength) )
          Exit Property
        End If
  
        Property = Mid$(internalBuffer, position, 1)
  
      End Property
  
      '--------------------------------------------
      Property Set Char(ByVal position As Dword, ByRef value As String )
      '--------------------------------------------
        If position > internalLength Then
          thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Position is outside text length. Passed:" + Str$(position) + ", Length:" + Str$(internalLength) )
          Exit Property
        End If
  
        If Len(value) > 1 Then
          thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Only single character allowed. Passed:" + value)
          Exit Property
        End If
        
        Mid$(internalBuffer, position) = value
  
      End Property
  
      '--------------------------------------------
      Property Get CharCode(ByVal position As Dword ) As Byte
      '--------------------------------------------
        If position > internalLength Then
          thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Position is outside text length. Passed:" + Str$(position) + ", Length:" + Str$(internalLength) )
          Exit Property
        End If
  
        Dim asciiCode(1 To position) As Byte At StrPtr(internalBuffer)
        Property = asciiCode(position)
  
      End Property
  
      '--------------------------------------------
      Property Set CharCode(ByVal position As Dword, value As Byte )
      '--------------------------------------------
        If position > internalLength Then
          thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Position is outside text length. Passed:" + Str$(position) + ", Length:" + Str$(internalLength) )
          Exit Property
        End If
  
        Dim asciiCode(1 To position) As Byte At StrPtr(internalBuffer)
        asciiCode(position) = value
  
      End Property
  
      '--------------------------------------------
      Property Get DataPtr() As Dword
      '--------------------------------------------
        Property = StrPtr(internalBuffer)
      End Property

      '--------------------------------------------
      Property Get VarPtr() As Dword
      '--------------------------------------------
        Property = VarPtr(internalBuffer)
      End Property
  
      '--------------------------------------------
      Method ToString() As String
      '--------------------------------------------
        Method = Left$(internalBuffer, internalLength)
      End Method
  
    End Interface
    
  End Class
