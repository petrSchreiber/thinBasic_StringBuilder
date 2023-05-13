  '
  ' cStringBuilder_Array internal implementation
  '
  %BufferArray_Increment_Strategy_10PercentOfCurrentArraySize = 1
  %BufferArray_Increment_Strategy_Fixed_Increment_By          = 2
  
  Class cStringBuilder_Array

    '---
      Instance BufferArray()                  As String
      Instance BufferArray_Size               As Dword
      Instance BufferArray_LastUsed           As Dword
      
      Instance BufferArray_NewToAdd           As Dword 
      
      Instance BufferArray_Increment_Strategy As Dword 
      Instance BufferArray_Increment_Initial  As Dword 
      Instance BufferArray_Increment_By       As Dword 
       
    '---Common usage
      Instance BufferArray_StringLen_Total    As Dword
      Instance BufferArray_StringLen_Max      As Dword

    '---Object characteristics
      Instance sName                          As String 
      
         
    '--------------------------------------------
    Class Method Create()
    '--------------------------------------------
      
    End Method
  
    '--------------------------------------------
    Interface iStringBuilder_Array
      Inherit IUnknown
  
      '--------------------------------------------
      Method Initialize()
      '--------------------------------------------
        '---Set a default initial array size to be used the very first time array is dimensioned
        BufferArray_Increment_Initial   = 1000
        BufferArray_Increment_By        =  100
        
        BufferArray_Increment_Strategy  = %BufferArray_Increment_Strategy_10PercentOfCurrentArraySize
        
        Reset BufferArray() 

        BufferArray_StringLen_Total     = 0
        BufferArray_StringLen_Max       = 0
        BufferArray_LastUsed            = 0
                
      End Method


      '----------------------------------------------------------------------------
      ' GetClassObject returns a pointer to itself
      '----------------------------------------------------------------------------
      Method GetClassObject() As Dword
        Local lPtr As Dword         
        lPtr = ObjPtr(Me)
'MsgBox FuncName$ + " " + Hex$(lPtr)
        Method = lPtr
      End Method

  
      '--------------------------------------------
      Method Add(ByVal AddedText As String) As Dword
      '--------------------------------------------
        Local AddedText_Len     As Dword
 
        '---If internal array is not jet dimensioned
        If IsFalse(ArrayAttr(BufferArray(), 0)) Or BufferArray_Size = 0 Then
          ReDim BufferArray(1 To BufferArray_Increment_Initial)
          BufferArray_Size = BufferArray_Increment_Initial 
          BufferArray_LastUsed = 0
'MsgBox FuncName$ + " INIT " + Str$(BufferArray_Size)          
        End If
                            
        BufferArray_NewToAdd = BufferArray_LastUsed + 1&
        
        '---If we need more room ...
        If BufferArray_NewToAdd > BufferArray_Size Then
        
          Select Case Long BufferArray_Increment_Strategy
          
            Case %BufferArray_Increment_Strategy_Fixed_Increment_By
              BufferArray_Size = BufferArray_NewToAdd + (BufferArray_Increment_By)
              
            Case %BufferArray_Increment_Strategy_10PercentOfCurrentArraySize
              BufferArray_Size = BufferArray_NewToAdd + (BufferArray_Size / 100 * 10)
          
          End Select
            
          ReDim Preserve BufferArray(1 To BufferArray_Size)

'MsgBox FuncName$ + " REDIM " + Str$(BufferArray_Size)          

        End If
        
        '---Store Text
        BufferArray(BufferArray_NewToAdd) = AddedText 

        '---Make some calculation
        BufferArray_LastUsed = BufferArray_NewToAdd
                                    
        '---Get added string len
        AddedText_Len = Len(AddedText)
        '---Add it to total array buffer string len
        BufferArray_StringLen_Total += AddedText_Len
        '---Save if its len is more than current max (just for stats)
        If BufferArray_StringLen_Max < AddedText_Len Then
          BufferArray_StringLen_Max = AddedText_Len
        End If
        
        Method = BufferArray_LastUsed
                  
      End Method

      '--------------------------------------------
      Method DataByIndex(ByVal lIndex As Dword) As String
      '--------------------------------------------      
        If lIndex >= 1 And lIndex <= BufferArray_LastUsed Then
          Method = BufferArray(lIndex)
        End If
      End Method
      
  
      '--------------------------------------------
      Method Clear()
      '--------------------------------------------
        Me.Initialize()
      End Method

    
      '--------------------------------------------
      Property Get LastUsed As Dword
      '--------------------------------------------
        Property = BufferArray_LastUsed 
      End Property

      '--------------------------------------------
      Property Get DataSize As Dword
      '--------------------------------------------
        Property = BufferArray_StringLen_Total 
      End Property

      '--------------------------------------------
      Property Get DataCount As Dword
      '--------------------------------------------
        Property = BufferArray_LastUsed 
      End Property

      '--------------------------------------------
      Property Get ArrayCount As Dword
      '--------------------------------------------
        Property = BufferArray_Size 
      End Property

      '--------------------------------------------
      Property Get Name As String
        Property = sName 
      End Property
      Property Set Name (ByVal NewName As String)
        sName = NewName
      End Property
      '--------------------------------------------
  
      '--------------------------------------------
      Method ToString(Optional ByVal sSeparator As String) As String
      '--------------------------------------------

        '---Redim in order not to take over allocated elements
        ReDim Preserve BufferArray(1& To BufferArray_LastUsed)
        BufferArray_Size = BufferArray_LastUsed

        Method = Join$(BufferArray(), sSeparator) 

      End Method
  
    End Interface
    
  End Class
