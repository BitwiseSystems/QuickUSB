//---------------------------------------------------------------------------


#ifndef BitCheckBoxH
#define BitCheckBoxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TBitCheckBoxFrame : public TFrame
{
__published:	// IDE-managed Components
   TCheckBox *CheckBox8;
   TCheckBox *CheckBox7;
   TCheckBox *CheckBox6;
   TCheckBox *CheckBox5;
   TCheckBox *CheckBox4;
   TCheckBox *CheckBox3;
   TCheckBox *CheckBox2;
   TCheckBox *CheckBox1;
   TEdit *ByteBox;
   void __fastcall CheckBox8Click();
   void __fastcall CheckBox7Click();
   void __fastcall CheckBox6Click();
   void __fastcall CheckBox5Click();
   void __fastcall CheckBox4Click();
   void __fastcall CheckBox3Click();
   void __fastcall CheckBox2Click();
   void __fastcall CheckBox1Click();
private:	// User declarations
int value;
bool settingValue;
public:		// User declarations
    int  getValue();
    void  setValue(unsigned char portData);
    String  toHex(int decimal);
   __fastcall TBitCheckBoxFrame(TComponent* Owner);
   String getText();
   void setBoxValue(int newValue);
};
//---------------------------------------------------------------------------
extern PACKAGE TBitCheckBoxFrame *BitCheckBoxFrame;
//---------------------------------------------------------------------------
#endif


