Public Class Form1
    Inherits System.Windows.Forms.Form

#Region " Windows Form Designer generated code "

    Public Sub New()
        MyBase.New()

        'This call is required by the Windows Form Designer.
        InitializeComponent()

        'Add any initialization after the InitializeComponent() call

    End Sub

    'Form overrides dispose to clean up the component list.
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
   Friend WithEvents Button1 As System.Windows.Forms.Button
   Friend WithEvents QuickUsbCtl1 As QuickUsbDiagVbDotNet.QuickUsbCtl
   Friend WithEvents NotifyIcon1 As System.Windows.Forms.NotifyIcon
   Friend WithEvents QuickUsbCtl2 As QuickUsbDiagVbDotNet.QuickUsbCtl
   <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
      Me.components = New System.ComponentModel.Container
      Me.Button1 = New System.Windows.Forms.Button
      Me.NotifyIcon1 = New System.Windows.Forms.NotifyIcon(Me.components)
      Me.QuickUsbCtl2 = New QuickUsbDiagVbDotNet.QuickUsbCtl
      Me.SuspendLayout()
      '
      'Button1
      '
      Me.Button1.Location = New System.Drawing.Point(200, 76)
      Me.Button1.Name = "Button1"
      Me.Button1.Size = New System.Drawing.Size(72, 20)
      Me.Button1.TabIndex = 1
      Me.Button1.Text = "Button1"
      '
      'NotifyIcon1
      '
      Me.NotifyIcon1.Text = "NotifyIcon1"
      Me.NotifyIcon1.Visible = True
      '
      'QuickUsbCtl2
      '
      Me.QuickUsbCtl2.Connected = True
      Me.QuickUsbCtl2.devName = "QUSB-0"
      Me.QuickUsbCtl2.Location = New System.Drawing.Point(8, 8)
      Me.QuickUsbCtl2.Name = "QuickUsbCtl2"
      Me.QuickUsbCtl2.Size = New System.Drawing.Size(284, 52)
      Me.QuickUsbCtl2.TabIndex = 2
      '
      'Form1
      '
      Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
      Me.ClientSize = New System.Drawing.Size(308, 114)
      Me.Controls.Add(Me.QuickUsbCtl2)
      Me.Controls.Add(Me.Button1)
      Me.Name = "Form1"
      Me.Text = "Form1"
      Me.ResumeLayout(False)

   End Sub

#End Region

   Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click

   End Sub
End Class
