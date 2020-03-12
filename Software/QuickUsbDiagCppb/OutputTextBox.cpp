//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "OutputTextBox.h"



//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

using namespace std;

#include "DisplayCode.h"


//---------------------------------------------------------------------------
__fastcall TFormOutputTextBox::TFormOutputTextBox(TComponent* Owner) : TForm(Owner) {
   if (Display->PageControl->ActivePageIndex == 1) {
	  this->Left = Display->Left + Display->Width;
	  this->Top = Display->Top;
	  this->Height = Display->Height;
	  this->Visible = true;
	  this->Enabled = true;

   }
   else {
	  this->Visible = false;
   }
}
//----------------------------------------------------------------------------

void TFormOutputTextBox::ShowByteData(unsigned char * theData, unsigned long dataLength) {
   unsigned long col, maxCol, start;
   String displayString;

   //start = FormOutputTextBox->OutputTextBox->SelText;
   this->OutputTextBox->Text = "";
   col = 0;
   maxCol = 15;
   for (unsigned long count = 0; count < dataLength; count++) {
      if (col == 0){
         displayString = displayString + toHex(count, 3) + ": ";
      }


      displayString = displayString + toHex(theData[count], 1);

      if (col == maxCol) {
         displayString = displayString + "\n";
      }
      if (col < maxCol) {
         col++;
      }
      else{
         col = 0;
      }

   }
   this->OutputTextBox->Text = displayString;
   this->OutputTextBox->SelStart = start;

}

//---------------------------------------------------------------------------

String TFormOutputTextBox::toHex(int decimal, int length) {
   String hex = "";
   for (int i=length; i>=0; i--) {
      hex += "0123456789ABCDEF"[((decimal >> i*4) & 0xF)];
   }

   return hex;
}

//---------------------------------------------------------------------------

