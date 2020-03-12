program QuickUsbDiag;

uses
  Forms,
  DisplayUnit in 'DisplayUnit.pas' {Display},
  QuickUSB in 'QuickUSB.pas';

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TDisplay, Display);
  Application.Run;
end.
