//---------------------------------------------------------------------------

#ifndef AboutFormH
#define AboutFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TAbout : public TForm
{
__published:	// IDE-managed Components
   TImage *Image1;
   TLabel *Title;
   TLabel *Version;
   TLabel *Notice;
   void __fastcall FormCreate();
private:	// User declarations
public:		// User declarations
   __fastcall TAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAbout *About;
//---------------------------------------------------------------------------
#endif
