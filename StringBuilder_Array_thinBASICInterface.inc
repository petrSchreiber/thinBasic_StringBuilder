                                      '
' StringBuilder thinBASIC interface
'

#Include Once ".\StringBuilder_Array_Implementation.inc"

  '-----------------------------------------------------------------------------
  Function StringBuilder_Array_LoadSymbols() As Long
  '-----------------------------------------------------------------------------  
    Local RetCode         As Long
    Local pStringBuilder  As Dword
  
    pStringBuilder = thinBasic_Class_Add("cStringBuilderArray", 0)
  
    If pStringBuilder Then
  
      thinBasic_Class_AddMethod   (pStringBuilder, "_Create"          , %thinBasic_ReturnNone       , CodePtr(StringBuilder_Array_Create)               )
      thinBasic_Class_AddMethod   (pStringBuilder, "_Destroy"         , %thinBasic_ReturnNone       , CodePtr(StringBuilder_Array_Destroy)              )
      thinBasic_Class_AddMethod   (pStringBuilder, "_GetClassObject"  , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_GetClassObject    ))
                                                                                                                                              
      thinBasic_Class_AddMethod   (pStringBuilder, "Add"          , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_Add)                  )
                                                                                                                                              
      thinBasic_Class_AddMethod   (pStringBuilder, "Clear"        , %thinBasic_ReturnCodeLong   , CodePtr(StringBuilder_Array_Clear)                )
      thinBasic_Class_AddMethod   (pStringBuilder, "ToString"     , %thinBasic_ReturnString     , CodePtr(StringBuilder_Array_ToString)             )
                                                                                                                                              

      thinBasic_Class_AddProperty (pStringBuilder, "LastUsed"     , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_Property_LastUsed)    ) 
      
      thinBasic_Class_AddProperty (pStringBuilder, "DataCount"    , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_Property_DataCount)   )
      thinBasic_Class_AddProperty (pStringBuilder, "DataSize"     , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_Property_DataSize)    )
      thinBasic_Class_AddProperty (pStringBuilder, "ArrayCount"   , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_Property_ArrayCount)  )

      thinBasic_Class_AddProperty (pStringBuilder, "Merge"        , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_Merge)                )
            
'---IDEAS           
' Calculate data size
' Calculate memory size: each element has: data size + 4 bytes for the string pointer + 4 bytes for string len + 1 byte for null terminator
' Possibility to get data by index
' Possibility to change data by index
' Possibility to delete data by index
'-------------------------------------------------

      Function = 0
    Else
      Function = 1
    End If
  
  End Function

  '-----------------------------------------------------------------------------
  Sub StringBuilder_Array_Create(ByVal pObject As Dword)
  '-----------------------------------------------------------------------------
  
    Dim oObject(1& To 1&) As iStringBuilder_Array At pObject
  
    oObject(1&) = Class "cStringBuilder_Array"
  
    If IsNothing(oObject(1&)) Then
      thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Failed to instantiate StringBuilder" )
    Else
      oObject(1&).Initialize()
    End If
  
  End Sub

  '-----------------------------------------------------------------------------
  Function StringBuilder_Array_Destroy(ByVal pObject As Dword) As Long
  '-----------------------------------------------------------------------------  
    Dim oObject(1& To 1&) As iStringBuilder_Array At pObject 
    
    oObject(1&) = Nothing
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Array_Add(ByVal pObject As Dword) As Dword
  '-----------------------------------------------------------------------------  
    Dim oObject(1& To 1&) As iStringBuilder_Array At pObject
  
    Dim addedText As String
    If thinBasic_CheckOpenParens_Mandatory() And thinBasic_ErrorFree() Then
      thinBasic_ParseString addedText
  
      If thinBasic_ErrorFree() Then
        Function = oObject(1&).Add(addedText)
      End If
  
      While thinBasic_CheckComma_Optional() And thinBasic_ErrorFree()
        thinBasic_ParseString addedText
        Function = oObject(1&).Add(addedText)
      Wend
  
      thinBasic_CheckCloseParens_Mandatory()
  
    End If
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Array_Clear(ByVal pObject As Dword) As Long
  '-----------------------------------------------------------------------------    
    If thinBasic_CheckOpenParens_optional Then
      thinBasic_CheckCloseParens_Mandatory
    End If
    
    If thinBasic_ErrorFree Then
      Dim oObject(1& To 1&) As iStringBuilder_Array At pObject
      oObject(1&).Clear()
    End If
    
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Array_ToString(ByVal pObject As Dword) As String
  '-----------------------------------------------------------------------------    
    Local sSeparator As String

    'If thinBasic_CheckOpenParens_optional Then
      sSeparator = thinBasic_Parse1String 
    '  thinBasic_CheckCloseParens_Mandatory
    'End If
    
    If thinBasic_ErrorFree Then                                   
      Dim oObject(1& To 1&) As iStringBuilder_Array At pObject  
      Function = oObject(1&).ToString(sSeparator)
    End If
  
  End Function

  '----------------------------------------------------------------------------
  Function StringBuilder_Array_GetClassObject(ByVal pObject As Long) As Dword
  '----------------------------------------------------------------------------
    'If thinBasic_CheckOpenParens_Optional Then thinBasic_CheckCloseParens_Mandatory

    '---If all went fine ...
    If thinBasic_ErrorFree Then
      ReDim oObject(1& To 1&) As iStringBuilder_Array At pObject
      Function = oObject(1&).GetClassObject()
      
    End If
  End Function


  Function StringBuilder_Array_Merge(ByVal pObject As Dword) As Dword
  '-----------------------------------------------------------------------------    
    Local pStringBuilder_Array_ToMerge As Dword
    Local lIdx As Dword

    '---thinBasic will call class GetClassObject to get a pointer to the interface of the passed object
    pStringBuilder_Array_ToMerge = thinBasic_Parse1Number 
    
    If thinBasic_ErrorFree Then
      If pStringBuilder_Array_ToMerge > 0 Then 

        '---Create a new object of iStringBuilder_Array ...
        Dim oMergeObject(1& To 1&) As iStringBuilder_Array
        oMergeObject(1&) = Class "cStringBuilder_Array"
        
        '---Now let the new object point to the memory location where the passed pStringBuilder_Array_ToMerge point
        '---So it is like dim ... at double indirection @@
        Poke Dword, VarPtr(oMergeObject(1&)), pStringBuilder_Array_ToMerge
        '---At the end ... an object is a pointer that contains a pointer to a memory area.
        '---Very similar to a Ole32 String BSTR
        
        '---If all went fine ...'
        If IsObject(oMergeObject(1&)) Then
          '---IMPORTANT ... Call IUnknown.AddRef otherwise when we will exit from
          '---this function releae of oMergeObject(1&) will also release the original object
          '---And we do not want this to happen !!!!
          oMergeObject(1&).AddRef
                    
          '---Now we are ready to use our 2 obejcts        
          Dim oObject(1& To 1&) As iStringBuilder_Array At pObject 

          For lIdx = 1 To oMergeObject(1&).LastUsed         
            oObject(1&).Add(oMergeObject(1&).DataByIndex(lIdx))
          Next

        End If
      End If

    End If
  
  End Function








  '-----------------------------------------------------------------------------
  Function StringBuilder_Array_Property_LastUsed(ByVal pObject As Dword, ByVal lAction As Long) As Dword
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iStringBuilder_Array At pObject
  
    Select Case Long lAction
  
      Case %Class_Action_Get
        Function = oObject(1&).LastUsed
  
      Case %Class_Action_Set
      '  thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Assignment to Length is not supported." )
  
    End Select
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Array_Property_DataSize(ByVal pObject As Dword, ByVal lAction As Long) As Dword
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iStringBuilder_Array At pObject
  
    Select Case Long lAction
  
      Case %Class_Action_Get
        Function = oObject(1&).DataSize
  
      Case %Class_Action_Set
      '  thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Assignment to Length is not supported." )
  
    End Select
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Array_Property_DataCount(ByVal pObject As Dword, ByVal lAction As Long) As Dword
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iStringBuilder_Array At pObject
  
    Select Case Long lAction
  
      Case %Class_Action_Get
        Function = oObject(1&).DataCount
  
      Case %Class_Action_Set
      '  thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Assignment to Length is not supported." )
  
    End Select
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Array_Property_ArrayCount(ByVal pObject As Dword, ByVal lAction As Long) As Dword
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iStringBuilder_Array At pObject
  
    Select Case Long lAction
  
      Case %Class_Action_Get
        Function = oObject(1&).ArrayCount
  
      Case %Class_Action_Set
      '  thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Assignment to Length is not supported." )
  
    End Select
  
  End Function
