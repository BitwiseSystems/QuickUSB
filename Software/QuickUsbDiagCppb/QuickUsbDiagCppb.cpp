//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("DisplayCode.cpp", Display);
USEFORM("AboutForm.cpp", About);
USEFORM("BitCheckBox.cpp", BitCheckBoxFrame); /* TFrame: File Type */
USEFORM("OutputTextBox.cpp", FormOutputTextBox);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		//Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TDisplay), &Display);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
