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
      thinBasic_Class_AddMethod   (pStringBuilder, "_GetClassObject"  , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_GetClassObject)       )
                                                                                                                                              
      thinBasic_Class_AddMethod   (pStringBuilder, "Add"              , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_Add)                  )
      thinBasic_Class_AddMethod   (pStringBuilder, "Clear"            , %thinBasic_ReturnCodeLong   , CodePtr(StringBuilder_Array_Clear)                )
      thinBasic_Class_AddMethod   (pStringBuilder, "ToString"         , %thinBasic_ReturnString     , CodePtr(StringBuilder_Array_ToString)             )
      thinBasic_Class_AddMethod   (pStringBuilder, "Append"           , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_Append)               )
                                                                                                                                                  
                                                                      
      thinBasic_Class_AddProperty (pStringBuilder, "Items"            , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_Property_Items)   )
      thinBasic_Class_AddProperty (pStringBuilder, "Size"             , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_Property_Size)    )
      thinBasic_Class_AddProperty (pStringBuilder, "Name"             , %thinBasic_ReturnCodeString , CodePtr(StringBuilder_Array_Property_Name)    )

'      thinBasic_Class_AddProperty (pStringBuilder, "LastUsed"         , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_Property_LastUsed)    ) 
'      thinBasic_Class_AddProperty (pStringBuilder, "ArrayCount"       , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Array_Property_ArrayCount)  )

            
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

    If thinBasic_CheckOpenParens_Optional And thinBasic_ErrorFree Then
      Dim sName As String
      thinBasic_ParseString sName
  
      If thinBasic_ErrorFree Then
        oObject(&1).Name = sName
      End If

      thinBasic_CheckCloseParens_Mandatory
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

    If thinBasic_CheckOpenParens_optional Then
      thinBasic_ParseString sSeparator
      thinBasic_CheckCloseParens_Mandatory
    End If
    
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

'=============================================================================
' Hystoric for complete explanation during study on how to implement
'=============================================================================
'  Function StringBuilder_Array_Append(ByVal pObject As Dword) As Dword
'  '-----------------------------------------------------------------------------    
'    Local pp As Byte
'    Local pStringBuilder_Array_ToMerge As Dword
'    Local lIdx As Dword
'
'    pp = thinBasic_CheckOpenParens_Optional
'
'    While %TRUE
'      '---thinBasic will call GetClassObject method if present to get a pointer to the interface of the passed object
'      'pStringBuilder_Array_ToMerge = thinBasic_Parse1Number 
'      thinBasic_ParseDWord pStringBuilder_Array_ToMerge
'    
'      If thinBasic_ErrorFree Then
'        '---If we have the pointer
'        If pStringBuilder_Array_ToMerge > 0 Then 
'  
'          '---Create a new virtual object of iStringBuilder_Array using parsed pointer
'          '---Is Virtual because it is a shadow overlay of the original object and not a new copy
'          '---There are 2 ways to do this:
'  
'  '-------OPTION 1: use DIM ObjArray of 1 element AT the memory position of the passed pointer
'          '             Easy to do and PowerBasic will take care of all because ... DIM ... AT ... 
'          '             do not init variable at the beginning and do not release variable at the end
'          ReDim oMergeObject(1& To 1&) As iStringBuilder_Array At VarPtr(pStringBuilder_Array_ToMerge)
'  
'  '-------OPTION 2: create a new obect, point it to the memory location of the passed object
'  '        '             Hard way: programmer has to do all the stuff using iUnknown interface to be sure object
'  '        '             is not released at the end 
'  '        Dim oMergeObject(1& To 1&) As iStringBuilder_Array
'  '        oMergeObject(1&) = Class "cStringBuilder_Array"
'  '        
'  '        '---Now let the new object point to the memory location where the passed pStringBuilder_Array_ToMerge point
'  '        '---So it is like dim ... at double indirection @@
'  '        Poke Dword, VarPtr(oMergeObject(1&)), pStringBuilder_Array_ToMerge
'  '        '---At the end ... an object is a pointer that contains a pointer to a memory area.
'  '        '---Very similar to a Ole32 String BSTR
'  '
'  '        '---IMPORTANT ... Call IUnknown.AddRef otherwise when we will exit from
'  '        '---this function releae of oMergeObject(1&) will also release the original object
'  '        '---And we do not want this to happen !!!!
'  '        oMergeObject(1&).AddRef
'  '--------                    
'          
'          '---If all went fine ...'
'          If IsObject(oMergeObject(1&)) Then
'  
'            '---Point to the main object to which this function is connected        
'            ReDim oObject(1& To 1&) As iStringBuilder_Array At pObject 
'  
'            '---Now we are ready to use our 2 obejcts        
'            For lIdx = 1 To oMergeObject(1&).LastUsed         
'              oObject(1&).Add(oMergeObject(1&).DataByIndex(lIdx))
'            Next
'  
'          End If
'          
'        End If
'  
'      End If
'
'      '---If there is a comma, iterate merge else exit loop     
'      If thinBasic_CheckComma_Optional Then
'      Else  
'        Exit Loop
'      End If
'      
'    Wend
'    
'    If IsTrue(pp) Then thinBasic_CheckCloseParens_Mandatory
'    
'  End Function

  Function StringBuilder_Array_Append(ByVal pObject As Dword) As Dword
  '-----------------------------------------------------------------------------    
    Local pp As Byte
    Local pStringBuilder_Array_ToMerge As Dword
    Local lIdx As Dword

    pp = thinBasic_CheckOpenParens_Optional

    While %TRUE
      '---When thinBasic encounter a class object and parse ask to parse a DWORD ...
      '   thinBasic will call GetClassObject method (if present) to get a pointer to the interface of the passed object
      '   or a pointer to an internal objct as needed by the programmer.
      '   In ADODB (for example) this method will return a pointer to an ADODB Connection object

      thinBasic_ParseDWord pStringBuilder_Array_ToMerge
    
      If thinBasic_ErrorFree Then
      
        '---If we have the pointer
        If IsTrue(pStringBuilder_Array_ToMerge) Then 
  
          '---Create a new virtual object of iStringBuilder_Array using parsed pointer
          '---Is Virtual because it is a shadow overlay of the original object and not a new copy
  
          '---Use DIM ObjArray of 1 element AT the memory position of the passed pointer
          '   Easy to do and PowerBasic will take care of all because ... DIM ... AT ... 
          '   do not init variable at the beginning and do not release variable at the end
          ReDim oMergeObject(1& To 1&) As iStringBuilder_Array At VarPtr(pStringBuilder_Array_ToMerge)
  
          '---If all went fine ...'
          If IsObject(oMergeObject(1&)) Then
  
            '---Point to the main object to which this function is connected        
            ReDim oObject(1& To 1&) As iStringBuilder_Array At pObject 
  
            '---Now we are ready to use our 2 obejcts        
            For lIdx = 1 To oMergeObject(1&).LastUsed         
              oObject(1&).Add(oMergeObject(1&).DataByIndex(lIdx))
            Next
  
          End If
          
        End If
  
      End If

      '---If there is a comma, iterate merge else exit loop     
      If thinBasic_CheckComma_Optional Then
      Else  
        Exit Loop
      End If
      
    Wend
    
    If IsTrue(pp) Then thinBasic_CheckCloseParens_Mandatory
    
  End Function







'  '-----------------------------------------------------------------------------
'  Function StringBuilder_Array_Property_LastUsed(ByVal pObject As Dword, ByVal lAction As Long) As Dword
'  '-----------------------------------------------------------------------------  
'    ReDim oObject(1& To 1&) As iStringBuilder_Array At pObject
'  
'    Select Case Long lAction
'  
'      Case %Class_Action_Get
'        Function = oObject(1&).LastUsed
'  
'      Case %Class_Action_Set
'      '  thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Assignment to Length is not supported." )
'  
'    End Select
'  
'  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Array_Property_Size(ByVal pObject As Dword, ByVal lAction As Long) As Dword
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
  Function StringBuilder_Array_Property_Name(ByVal pObject As Dword, ByVal lAction As Long) As String
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iStringBuilder_Array At pObject
  
    Select Case Long lAction
  
      Case %Class_Action_Get
        Function = oObject(1&).Name
  
      Case %Class_Action_Set
        Local lEqualType    As Long
        lEqualType = thinBasic_CheckEqualType_Mandatory

        If thinBasic_ErrorFree Then
          If lEqualType = %Equal_EQ Then
            Dim sName As String
            thinBasic_ParseString sName

            If thinBasic_ErrorFree Then
              oObject(&1).Name = sName
            End If
          End If
        End If
  
    End Select
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Array_Property_Items(ByVal pObject As Dword, ByVal lAction As Long) As Dword
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iStringBuilder_Array At pObject
  
    Select Case Long lAction
  
      Case %Class_Action_Get
        Function = oObject(1&).DataCount
  
      Case %Class_Action_Set
      '  thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Assignment to Length is not supported." )
  
    End Select
  
  End Function

'  '-----------------------------------------------------------------------------
'  Function StringBuilder_Array_Property_ArrayCount(ByVal pObject As Dword, ByVal lAction As Long) As Dword
'  '-----------------------------------------------------------------------------  
'    ReDim oObject(1& To 1&) As iStringBuilder_Array At pObject
'  
'    Select Case Long lAction
'  
'      Case %Class_Action_Get
'        Function = oObject(1&).ArrayCount
'  
'      Case %Class_Action_Set
'      '  thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Assignment to Length is not supported." )
'  
'    End Select
'  
'  End Function
