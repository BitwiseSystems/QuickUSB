<ComClass(QuickUsbComCtl.ClassId, QuickUsbComCtl.InterfaceId, QuickUsbComCtl.EventsId)> _
Public Class QuickUsbComCtl

#Region "COM GUIDs"
    ' These  GUIDs provide the COM identity for this class 
    ' and its COM interfaces. If you change them, existing 
    ' clients will no longer be able to access the class.
    Public Const ClassId As String = "EB3A7DBD-F9FE-4F0E-B575-C253B6F8B9A6"
    Public Const InterfaceId As String = "6858F691-BC66-4151-A2BB-CEF5FD3AAEEA"
    Public Const EventsId As String = "93962C09-5BDA-4D40-BB00-D95082D21781"
#End Region

    ' A creatable COM class must have a Public Sub New() 
    ' with no parameters, otherwise, the class will not be 
    ' registered in the COM registry and cannot be created 
    ' via CreateObject.
    Public Sub New()
        MyBase.New()
    End Sub

End Class


