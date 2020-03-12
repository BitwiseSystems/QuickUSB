//---------------------------------------------------------------------------

#ifndef OutputTextBoxH
#define OutputTextBoxH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <String.h>
//---------------------------------------------------------------------------
class TFormOutputTextBox : public TForm
{
__published:	// IDE-managed Components
   TRichEdit *OutputTextBox;
private:	// User declarations
public:		// User declarations
   void ShowByteData(unsigned char * theData, unsigned long dataLength);
   String toHex(int decimal, int length);
   __fastcall TFormOutputTextBox(TComponent* Owner);
};
#endif
