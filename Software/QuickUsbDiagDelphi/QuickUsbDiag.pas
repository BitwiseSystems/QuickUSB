unit QuickUsbDiag;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls;

type
  TDisplay = class(TForm)
    Button1: TButton;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Display: TDisplay;

implementation

{$R *.DFM}

procedure TDisplay.Button1Click(Sender: TObject);
begin
   MessageBox
end;

end.
