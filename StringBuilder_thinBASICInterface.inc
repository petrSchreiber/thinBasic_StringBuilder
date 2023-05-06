'
' StringBuilder thinBASIC interface
'

#Include Once "StringBuilder_Implementation.inc"

  '-----------------------------------------------------------------------------
  Function StringBuilder_LoadSymbols() As Long
  '-----------------------------------------------------------------------------  
    Local RetCode         As Long
    Local pStringBuilder  As Dword
  
    pStringBuilder = thinBasic_Class_Add("StringBuilder", 0)
  
    If pStringBuilder Then
  
      thinBasic_Class_AddMethod   (pStringBuilder, "_Create"      , %thinBasic_ReturnNone       , CodePtr(StringBuilder_Create)               )
      thinBasic_Class_AddMethod   (pStringBuilder, "_Destroy"     , %thinBasic_ReturnNone       , CodePtr(StringBuilder_Destroy)              )
                                                                                                                                              
      thinBasic_Class_AddMethod   (pStringBuilder, "Add"          , %thinBasic_ReturnCodeLong   , CodePtr(StringBuilder_Add)                  )
      thinBasic_Class_AddMethod   (pStringBuilder, "AddLine"      , %thinBasic_ReturnCodeLong   , CodePtr(StringBuilder_AddLine)              )
                                                                                                                                              
      thinBasic_Class_AddMethod   (pStringBuilder, "Clear"        , %thinBasic_ReturnCodeLong   , CodePtr(StringBuilder_Clear)                )
      thinBasic_Class_AddMethod   (pStringBuilder, "ToString"     , %thinBasic_ReturnString     , CodePtr(StringBuilder_ToString)             )
                                                                                                                                              
      thinBasic_Class_AddProperty (pStringBuilder, "Char"         , %thinBasic_ReturnString     , CodePtr(StringBuilder_Property_Char)        )
      thinBasic_Class_AddProperty (pStringBuilder, "CharCode"     , %thinBasic_ReturnCodeByte   , CodePtr(StringBuilder_Property_CharCode)    )
                                                                                                                                              
      thinBasic_Class_AddProperty (pStringBuilder, "Length"       , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Property_Length)      )
      thinBasic_Class_AddProperty (pStringBuilder, "Capacity"     , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Property_Capacity)    )
                                                                  
      thinBasic_Class_AddProperty (pStringBuilder, "StorageType"  , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Property_StorageType) )

      thinBasic_Class_AddProperty (pStringBuilder, "DataPTR"      , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Property_DataPtr)     )
      thinBasic_Class_AddProperty (pStringBuilder, "VarPTR"       , %thinBasic_ReturnCodeDWord  , CodePtr(StringBuilder_Property_VarPtr)      )
  
      Function = 0
    Else
      Function = 1
    End If
  
  End Function

  '-----------------------------------------------------------------------------
  Sub StringBuilder_Create(ByVal pObject As Dword)
  '-----------------------------------------------------------------------------
  
    Dim oObject(1& To 1&) As iThinStringBuilder At pObject
  
    oObject(1&) = Class "thinStringBuilder"
  
    If IsNothing(oObject(1&)) Then
      thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Failed to instantiate StringBuilder" )
    End If
  
    Dim initialCapacity As Ext
    Dim initialText As String 
    
    If thinBasic_CheckOpenParens_Optional() And thinBasic_ErrorFree() Then
  
      If thinBasic_CheckCloseParens_Optional() And thinBasic_ErrorFree() Then
        oObject(1&).Initialize1()
      Else
  
        Select Case thinBasic_DetermineType
          Case %thinBasic_ReturnNumber
            thinBasic_ParseNumber initialCapacity
  
            If thinBasic_CheckCloseParens_Mandatory() And thinBasic_ErrorFree() Then
              oObject(1&).Initialize2(ByCopy initialCapacity)
            End If
  
          Case %thinBasic_ReturnString
            thinBasic_ParseString initialText
  
            If thinBasic_CheckCloseParens_Mandatory() And thinBasic_ErrorFree() Then
              oObject(1&).Initialize3(initialText)
            End If
        End Select
  
      End If
    Else
      oObject(1&).Initialize1()
    End If
  
  End Sub

  '-----------------------------------------------------------------------------
  Function StringBuilder_Destroy(ByVal pObject As Dword) As Long
  '-----------------------------------------------------------------------------  
    Dim oObject(1& To 1&) As iThinStringBuilder At pObject 
    
    oObject(1&) = Nothing
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Add(ByVal pObject As Dword) As Long
  '-----------------------------------------------------------------------------  
    Dim oObject(1& To 1&) As iThinStringBuilder At pObject
  
    Dim addedText As String
    If thinBasic_CheckOpenParens_Mandatory() And thinBasic_ErrorFree() Then
      thinBasic_ParseString addedText
  
      If thinBasic_ErrorFree() Then
        oObject(1&).Add(addedText)
      End If
  
      While thinBasic_CheckComma_Optional() And thinBasic_ErrorFree()
        thinBasic_ParseString addedText
        oObject(1&).Add(addedText)
      Wend
  
      thinBasic_CheckCloseParens_Mandatory()
  
    End If
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_AddLine(ByVal pObject As Dword) As Long
  '-----------------------------------------------------------------------------  
    Dim oObject(1& To 1&) As iThinStringBuilder At pObject
  
    Dim addedText As String
    If thinBasic_CheckOpenParens_Mandatory() And thinBasic_ErrorFree() Then
      thinBasic_ParseString addedText
  
      If thinBasic_CheckCloseParens_Mandatory() And thinBasic_ErrorFree() Then
        oObject(1&).AddLine(addedText)
      End If
    End If
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Clear(ByVal pObject As Dword) As Long
  '-----------------------------------------------------------------------------    
    If thinBasic_CheckOpenParens_optional Then
      thinBasic_CheckCloseParens_Mandatory
    End If
    
    If thinBasic_ErrorFree Then
      Dim oObject(1& To 1&) As iThinStringBuilder At pObject
      oObject(1&).Clear()
    End If
    
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_ToString(ByVal pObject As Dword) As String
  '-----------------------------------------------------------------------------    
    If thinBasic_CheckOpenParens_optional Then
      thinBasic_CheckCloseParens_Mandatory
    End If
    
    If thinBasic_ErrorFree Then                                   
      Dim oObject(1& To 1&) As iThinStringBuilder At pObject  
      Function = oObject(1&).ToString()
    End If
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Property_Char(ByVal pObject As Dword, ByVal lAction As Long) As String
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iThinStringBuilder At pObject
  
    Dim position As Ext
    If thinBasic_CheckOpenParens_Mandatory() And thinBasic_ErrorFree() Then
      thinBasic_ParseNumber position
  
      If thinBasic_CheckCloseParens_Mandatory() And thinBasic_ErrorFree() Then
  
        Select Case Long lAction
  
          Case %Class_Action_Get
            Function = oObject(1&).Char(ByCopy position)
  
          Case %Class_Action_Set
            Local lEqualType    As Long
            lEqualType = thinBasic_CheckEqualType_Mandatory
  
            If thinBasic_ErrorFree Then
              If lEqualType = %Equal_EQ Then
                Dim character As String
                thinBasic_ParseString character
  
                If thinBasic_ErrorFree Then
                  oObject(&1).Char(ByCopy position) = character
                End If
              End If
            End If
  
        End Select
  
      End If
    End If
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Property_CharCode(ByVal pObject As Dword, ByVal lAction As Long) As Byte
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iThinStringBuilder At pObject
  
    Dim position As Ext
    If thinBasic_CheckOpenParens_Mandatory() And thinBasic_ErrorFree() Then
      thinBasic_ParseNumber position
  
      If thinBasic_CheckCloseParens_Mandatory() And thinBasic_ErrorFree() Then
  
        Select Case Long lAction
  
          Case %Class_Action_Get
            Function = oObject(1&).CharCode(ByCopy position)
  
          Case %Class_Action_Set
            Local lEqualType    As Long
            lEqualType = thinBasic_CheckEqualType_Mandatory
  
            If thinBasic_ErrorFree Then
              If lEqualType = %Equal_EQ Then
                Dim characterCode As Ext
                thinBasic_ParseNumber characterCode
  
                If thinBasic_ErrorFree Then
                  oObject(&1).CharCode(ByCopy position) = characterCode
                End If
              End If
            End If
  
        End Select
  
      End If
    End If
  
  End Function


  '-----------------------------------------------------------------------------
  Function StringBuilder_Property_Length(ByVal pObject As Dword, ByVal lAction As Long) As Dword
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iThinStringBuilder At pObject
  
    Select Case Long lAction
  
      Case %Class_Action_Get
        Function = oObject(1&).Length
  
      Case %Class_Action_Set
        thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Assignment to Length is not supported." )
  
    End Select
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Property_Capacity(ByVal pObject As Dword, ByVal lAction As Long) As Dword
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iThinStringBuilder At pObject
  
    Select Case Long lAction
  
      Case %Class_Action_Get
        Function = oObject(1&).Capacity
  
      Case %Class_Action_Set
        Local lEqualType    As Long
        lEqualType = thinBasic_CheckEqualType_Mandatory
  
        If thinBasic_ErrorFree Then
          If lEqualType = %Equal_EQ Then
            Dim capacity As Ext
            thinBasic_ParseNumber capacity
  
            If thinBasic_ErrorFree Then
              oObject(&1).Capacity = capacity
            End If
          End If
        End If
  
  
    End Select
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Property_StorageType(ByVal pObject As Dword, ByVal lAction As Long) As Dword
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iThinStringBuilder At pObject
  
    Select Case Long lAction
  
      Case %Class_Action_Get
        Function = oObject(1&).StorageType
  
      Case %Class_Action_Set
        Local lEqualType    As Long
        lEqualType = thinBasic_CheckEqualType_Mandatory
  
        If thinBasic_ErrorFree Then
          If lEqualType = %Equal_EQ Then
            Dim StorageType As Ext
            thinBasic_ParseNumber StorageType
  
            If thinBasic_ErrorFree Then
              oObject(&1).StorageType = StorageType
            End If
          End If
        End If
  
  
    End Select
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Property_DataPtr(ByVal pObject As Dword, ByVal lAction As Long) As Dword
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iThinStringBuilder At pObject
  
    Select Case Long lAction
  
      Case %Class_Action_Get
        Function = oObject(1&).DataPtr
  
      Case %Class_Action_Set
        thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Assignment to DataPtr is not supported." )
  
    End Select
  
  End Function

  '-----------------------------------------------------------------------------
  Function StringBuilder_Property_VarPtr(ByVal pObject As Dword, ByVal lAction As Long) As Dword
  '-----------------------------------------------------------------------------  
    ReDim oObject(1& To 1&) As iThinStringBuilder At pObject
  
    Select Case Long lAction
  
      Case %Class_Action_Get
        Function = oObject(1&).VarPtr
  
      Case %Class_Action_Set
        thinbasic_RunTimeError ( %ERR__MODULE_SPECIFIC, "Assignment to DataPtr is not supported." )
  
    End Select
  
  End Function
