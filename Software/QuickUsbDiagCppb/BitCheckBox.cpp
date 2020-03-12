//---------------------------------------------------------------------------

#include <vcl.h>
#include <string>
#pragma hdrstop

#include "BitCheckBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBitCheckBoxFrame *BitCheckBoxFrame;

//---------------------------------------------------------------------------


__fastcall TBitCheckBoxFrame::TBitCheckBoxFrame(TComponent* Owner) : TFrame(Owner), value(0){}

// Returns the integer value in the ByteBox
int TBitCheckBoxFrame::getValue() {
   return value;
}

// Returns the current value in the ByteBox wich the user can change
String TBitCheckBoxFrame::getText() {
   return ByteBox->Text;
}


//  Sets the value of the ByteBox
void TBitCheckBoxFrame::setBoxValue(int newValue){
    ByteBox->Text = toHex(newValue);
}

// Goes throw all 8 bits of the unsigned char portData and decides
// whether or not each check box should have a check
// When the program starts up and a check box is not checked it will set the correct hint string
// do not delete the hint strings from this method even though they exist in the method below this one
// they are needed for program initialization
void TBitCheckBoxFrame::setValue(unsigned char portData) {
   // Disable onClick actions
   settingValue = true;
   value = portData;

   if ( ( portData & 1 ) == 1) { CheckBox1->Checked = true; }
   else { CheckBox1->Checked = false;
          CheckBox1->Hint = "Bit 0 = Input";
   }

   if ( (portData & 2) == 2) { CheckBox2->Checked = true; }
   else { CheckBox2->Checked = false;
          CheckBox2->Hint = "Bit 1 = Input";
   }

   if( (portData & 4) == 4){ CheckBox3->Checked = true; }
   else { CheckBox3->Checked = false;
          CheckBox3->Hint = "Bit 2 = Input";
   }

   if( (portData & 8) == 8){ CheckBox4->Checked = true; }
   else { CheckBox4->Checked = false;
          CheckBox4->Hint = "Bit 3 = Input";
   }

   if( (portData & 16) == 16){ CheckBox5->Checked = true; }
   else { CheckBox5->Checked = false;
          CheckBox5->Hint = "Bit 4 = Input";
   }

   if( (portData & 32) == 32){ CheckBox6->Checked= true; }
   else { CheckBox6->Checked = false;
          CheckBox6->Hint = "Bit 5 = Input";
   }

   if( (portData & 64) == 64){ CheckBox7->Checked = true; }
   else { CheckBox7->Checked = false;
          CheckBox7->Hint = "Bit 6 = Input";
   }

   if( (portData & 128) == 128){ CheckBox8->Checked= true; }
   else { CheckBox8->Checked = false;
          CheckBox8->Hint = "Bit 7 = Input";
   }


   ByteBox->Text = toHex(value);
}


// Following methods adjust the member variable "value" and sets the correct
// hint string
//---------------------------------------------------------------------------

void __fastcall TBitCheckBoxFrame::CheckBox8Click() {
  // Check if click action is currently allowed
  if (settingValue) {
     return;
  }

  if (CheckBox8->Checked) {
    value += 128;
    CheckBox8->Hint = "Bit 7 = Output";
  }
  else {
    value -= 128;
    CheckBox8->Hint = "Bit 7 = Input";
  }

  ByteBox->Text = toHex(value);

  // If the OnClick handler is defined, call it
  if (OnClick) {
     OnClick(this);
  }

}
//---------------------------------------------------------------------------

void __fastcall TBitCheckBoxFrame::CheckBox7Click() {
  // Check if click action is currently allowed
  if (settingValue) {
     return;
  }

  if (CheckBox7->Checked) {
    value += 64;
    CheckBox7->Hint = "Bit 6 = Output";
  }
  else {
    value -= 64;
    CheckBox7->Hint = "Bit 6 = Input";
  }
  ByteBox->Text = toHex(value);

  // If the OnClick handler is defined, call it
  if (OnClick) {
     OnClick(this);
  }

}
//---------------------------------------------------------------------------

void __fastcall TBitCheckBoxFrame::CheckBox6Click() {
  // Check if click action is currently allowed
  if (settingValue) {
     return;
  }

  if (CheckBox6->Checked) {
    value += 32;
    CheckBox6->Hint = "Bit 5 = Output";
  }
  else {
    value -= 32;
    CheckBox6->Hint = "Bit 5 = Input";
  }
  ByteBox->Text = toHex(value);

  // If the OnClick handler is defined, call it
  if (OnClick) {
     OnClick(this);
  }

}
//---------------------------------------------------------------------------

void __fastcall TBitCheckBoxFrame::CheckBox5Click() {
  // Check if click action is currently allowed
  if (settingValue) {
     return;
  }

  if (CheckBox5->Checked) {
    value += 16;
    CheckBox5->Hint = "Bit 4 = Output";
  }
  else {
    value -= 16;
   CheckBox5->Hint = "Bit 4 = Input";
  }
  ByteBox->Text = toHex(value);

  // If the OnClick handler is defined, call it
  if (OnClick) {
     OnClick(this);
  }

}
//---------------------------------------------------------------------------

void __fastcall TBitCheckBoxFrame::CheckBox4Click() {
  // Check if click action is currently allowed
  if (settingValue) {
     return;
  }

  if (CheckBox4->Checked) {
    value += 8;
    CheckBox4->Hint = "Bit 3 = Output";
  }
  else {
    value -= 8;
    CheckBox4->Hint = "Bit 3 = Input";
  }
  ByteBox->Text = toHex(value);

  // If the OnClick handler is defined, call it
  if (OnClick) {
     OnClick(this);
  }

}
//---------------------------------------------------------------------------

void __fastcall TBitCheckBoxFrame::CheckBox3Click() {
  // Check if click action is currently allowed
  if (settingValue) {
     return;
  }

  if (CheckBox3->Checked) {
    value += 4;
    CheckBox3->Hint = "Bit 2 = Output";
  }
  else{
    value -= 4;
    CheckBox3->Hint = "Bit 2 = Input";
  }
  ByteBox->Text = toHex(value);

  // If the OnClick handler is defined, call it
  if (OnClick) {
     OnClick(this);
  }


}

//---------------------------------------------------------------------------

void __fastcall TBitCheckBoxFrame::CheckBox2Click() {
  // Check if click action is currently allowed
  if (settingValue) {
     return;
  }

  if (CheckBox2->Checked) {
    value += 2;
    CheckBox2->Hint = "Bit 1 = Output";
  }
  else {
    value -= 2;
    CheckBox2->Hint = "Bit 1 = Input";
  }
  ByteBox->Text = toHex(value);

  // If the OnClick handler is defined, call it
  if (OnClick) {
     OnClick(this);
  }


}
//---------------------------------------------------------------------------

void __fastcall TBitCheckBoxFrame::CheckBox1Click() {
  // Check if click action is currently allowed
  if (settingValue) {
     return;
  }

  if(CheckBox1->Checked) {
    value += 1;
    CheckBox1->Hint = "Bit 0 = Output";
  }
  else {
    value -= 1;
    CheckBox1->Hint = "Bit 0 = Input";
  }
  ByteBox->Text = toHex(value);

  // If the OnClick handler is defined, call it
  if (OnClick) {
     OnClick(this);
  }


}
//---------------------------------------------------------------------------

String TBitCheckBoxFrame::toHex(int decimal) {

   String hex = "";
   for (int i=1; i>=0; i--) {
      hex += "0123456789ABCDEF"[((decimal >> i*4) & 0xF)];
   }

   return ("0x" + hex);
}


//---------------------------------------------------------------------------


