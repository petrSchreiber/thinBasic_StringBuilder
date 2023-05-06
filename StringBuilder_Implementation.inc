  '
  ' StringBuilder internal implementation
  '
  %internalStorage_UseUndefined   = 0
  %internalStorage_UseString      = 1
  %internalStorage_UseArray       = 2
  
  Class thinStringBuilder

    '---Indicate what storage type this StringBuilder is using
      Instance StorageType            As Long
    
    '---Used when StorageType is %internalStorage_UseString  
      Instance defaultCapacity        As Dword
      Instance internalCapacity       As Dword
      
      Instance internalLength         As Dword
      Instance internalBuffer         As String 

    '---Used when StorageType is %internalStorage_UseArray
      Instance BufferArray()          As String
      Instance BufferArray_Size       As Dword
      Instance BufferAyyay_LastUsed   As Dword
      Instance BufferAyyay_NewToAdd   As Dword
       
    '---Common usage
      Instance MaxAddedStringLen      As Dword
    
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
        
        Reset BufferArray() 
        
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
 
        Select Case Long StorageType 
        
          Case %internalStorage_UseString, %internalStorage_UseUndefined 

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

          Case %internalStorage_UseArray
            '---If internal array is not jet dimensioned
            If IsFalse(ArrayAttr(BufferArray(), 0)) or BufferArray_Size = 0 Then
              ReDim BufferArray(1 To defaultCapacity)
              BufferArray_Size = defaultCapacity 
              BufferAyyay_LastUsed = 0
            End If
                                
            BufferAyyay_NewToAdd = BufferAyyay_LastUsed + 1
            
            '---If we need more room redim preserve
            If BufferAyyay_NewToAdd > BufferArray_Size Then
              BufferArray_Size = BufferArray_Size + (defaultCapacity * 2)
              ReDim Preserve BufferArray(1 To BufferArray_Size)
            End If
            BufferArray(BufferAyyay_NewToAdd) = addedText 

            BufferAyyay_LastUsed = BufferAyyay_NewToAdd
            
        End Select
                  
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
        Select Case Long StorageType 
        
          Case %internalStorage_UseString, %internalStorage_UseUndefined 
            Property = internalCapacity
          Case %internalStorage_UseArray
            Property = UBound(BufferArray(1)) 
        End Select

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
        Select Case Long StorageType 
        
          Case %internalStorage_UseString, %internalStorage_UseUndefined 
            Property = internalLength 

          Case %internalStorage_UseArray
            Property = BufferAyyay_LastUsed 
        
        End Select
        
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
      Property Get StorageType As Dword
      '--------------------------------------------
        Property = StorageType
      End Property
  
      '--------------------------------------------
      Property Set StorageType(ByVal value As Dword )
      '--------------------------------------------
        StorageType = value
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
        Select Case Long StorageType 
        
          Case %internalStorage_UseString, %internalStorage_UseUndefined 
            Method = Left$(internalBuffer, internalLength)
            
          Case %internalStorage_UseArray   
            '---Redim in order not to take over allocated elements
            ReDim Preserve BufferArray(1 To BufferAyyay_LastUsed)
            BufferArray_Size = BufferAyyay_LastUsed

            Method = Join$(BufferArray(), "") 
            
        End Select

      End Method
  
    End Interface
    
  End Class
