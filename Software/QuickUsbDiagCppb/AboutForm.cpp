//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AboutForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAbout *About;
//---------------------------------------------------------------------------
__fastcall TAbout::TAbout(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAbout::FormCreate()
{
   Title->Caption = Application->Title;
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
