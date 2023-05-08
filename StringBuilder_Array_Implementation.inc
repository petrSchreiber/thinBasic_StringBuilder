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
      Instance MaxAddedStringLen              As Dword
    
    '--------------------------------------------
    Class Method Create()
    '--------------------------------------------
      
    End Method
  
    '--------------------------------------------
    Interface iStringBuilderArray
      Inherit IUnknown
  
      '--------------------------------------------
      Method Initialize()
      '--------------------------------------------
        '---Set a default initial array size to be used the very first time array is dimensioned
        BufferArray_Increment_Initial   = 1000
        BufferArray_Increment_By        =  100
        
        BufferArray_Increment_Strategy  = %BufferArray_Increment_Strategy_10PercentOfCurrentArraySize
        
        Reset BufferArray()         
      End Method

  
      '--------------------------------------------
      Method Add(ByRef addedText As String) As Dword
      '--------------------------------------------
        Local finalCapacity           As Dword  
        Local lIncrement              As Dword
        Local lIncreaseBufferStrategy As Long
 
        '---If internal array is not jet dimensioned
        If IsFalse(ArrayAttr(BufferArray(), 0)) Or BufferArray_Size = 0 Then
          ReDim BufferArray(1 To BufferArray_Increment_Initial)
          BufferArray_Size = BufferArray_Increment_Initial 
          BufferArray_LastUsed = 0
        End If
                            
        BufferArray_NewToAdd = BufferArray_LastUsed + 1
        
        '---If we need more room ...
        If BufferArray_NewToAdd > BufferArray_Size Then
        
          Select Case Long BufferArray_Increment_Strategy
          
            Case %BufferArray_Increment_Strategy_Fixed_Increment_By
              BufferArray_Size = BufferArray_Size + (BufferArray_Increment_By)
              
            Case %BufferArray_Increment_Strategy_10PercentOfCurrentArraySize
              BufferArray_Size = BufferArray_Size + (BufferArray_Size / 100 * 10)
          
          End Select
            
          ReDim Preserve BufferArray(1 To BufferArray_Size)

        End If
        
        BufferArray(BufferArray_NewToAdd) = addedText 

        BufferArray_LastUsed = BufferArray_NewToAdd
        
        Method = BufferArray_LastUsed
                  
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
      Property Get Size As Dword
      '--------------------------------------------
        Property = BufferArray_Size 
      End Property
  
      '--------------------------------------------
      Method ToString(Optional ByVal sSeparator As String) As String
      '--------------------------------------------

        '---Redim in order not to take over allocated elements
        ReDim Preserve BufferArray(1 To BufferArray_LastUsed)
        BufferArray_Size = BufferArray_LastUsed

        Method = Join$(BufferArray(), sSeparator) 

      End Method
  
    End Interface
    
  End Class
