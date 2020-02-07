//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("..\Src\UnitFrameState0.cpp", FrameState0); /* TFrame: File Type */
USEFORM("..\Src\UnitFrameState1.cpp", FrameState1); /* TFrame: File Type */
USEFORM("..\Src\UnitLibraryTest.cpp", FormGlobalLibraryTest);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TFormGlobalLibraryTest), &FormGlobalLibraryTest);
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
