# Microsoft Developer Studio Generated NMAKE File, Based on QuickUsbVstudio.dsp
!IF "$(CFG)" == ""
CFG=QuickUsbVstudio - Win32 Debug
!MESSAGE No configuration specified. Defaulting to QuickUsbVstudio - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "QuickUsbVstudio - Win32 Release" && "$(CFG)" != "QuickUsbVstudio - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "QuickUsbVstudio.mak" CFG="QuickUsbVstudio - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "QuickUsbVstudio - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "QuickUsbVstudio - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QuickUsbVstudio - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\QuickUsbVstudio.exe" "$(OUTDIR)\QuickUsbVstudio.bsc"


CLEAN :
	-@erase "$(INTDIR)\QuickUsbVstudio.obj"
	-@erase "$(INTDIR)\QuickUsbVstudio.pch"
	-@erase "$(INTDIR)\QuickUsbVstudio.res"
	-@erase "$(INTDIR)\QuickUsbVstudio.sbr"
	-@erase "$(INTDIR)\QuickUsbVstudioDlg.obj"
	-@erase "$(INTDIR)\QuickUsbVstudioDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\QuickUsbVstudio.bsc"
	-@erase "$(OUTDIR)\QuickUsbVstudio.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\QuickUsbVstudio.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\QuickUsbVstudio.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\QuickUsbVstudio.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\QuickUsbVstudio.sbr" \
	"$(INTDIR)\QuickUsbVstudioDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\QuickUsbVstudio.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=Release/quickusb.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\QuickUsbVstudio.pdb" /machine:I386 /out:"$(OUTDIR)\QuickUsbVstudio.exe" 
LINK32_OBJS= \
	"$(INTDIR)\QuickUsbVstudio.obj" \
	"$(INTDIR)\QuickUsbVstudioDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\QuickUsbVstudio.res"

"$(OUTDIR)\QuickUsbVstudio.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "QuickUsbVstudio - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\QuickUsbVstudio.exe"


CLEAN :
	-@erase "$(INTDIR)\QuickUsbVstudio.obj"
	-@erase "$(INTDIR)\QuickUsbVstudio.pch"
	-@erase "$(INTDIR)\QuickUsbVstudio.res"
	-@erase "$(INTDIR)\QuickUsbVstudioDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\QuickUsbVstudio.exe"
	-@erase "$(OUTDIR)\QuickUsbVstudio.ilk"
	-@erase "$(OUTDIR)\QuickUsbVstudio.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\QuickUsbVstudio.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\QuickUsbVstudio.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\QuickUsbVstudio.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Debug/quickusb.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\QuickUsbVstudio.pdb" /debug /machine:I386 /out:"$(OUTDIR)\QuickUsbVstudio.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\QuickUsbVstudio.obj" \
	"$(INTDIR)\QuickUsbVstudioDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\QuickUsbVstudio.res"

"$(OUTDIR)\QuickUsbVstudio.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("QuickUsbVstudio.dep")
!INCLUDE "QuickUsbVstudio.dep"
!ELSE 
!MESSAGE Warning: cannot find "QuickUsbVstudio.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "QuickUsbVstudio - Win32 Release" || "$(CFG)" == "QuickUsbVstudio - Win32 Debug"
SOURCE=.\QuickUsbVstudio.cpp

!IF  "$(CFG)" == "QuickUsbVstudio - Win32 Release"


"$(INTDIR)\QuickUsbVstudio.obj"	"$(INTDIR)\QuickUsbVstudio.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\QuickUsbVstudio.pch"


!ELSEIF  "$(CFG)" == "QuickUsbVstudio - Win32 Debug"


"$(INTDIR)\QuickUsbVstudio.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\QuickUsbVstudio.pch"


!ENDIF 

SOURCE=.\QuickUsbVstudio.rc

"$(INTDIR)\QuickUsbVstudio.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\QuickUsbVstudioDlg.cpp

!IF  "$(CFG)" == "QuickUsbVstudio - Win32 Release"


"$(INTDIR)\QuickUsbVstudioDlg.obj"	"$(INTDIR)\QuickUsbVstudioDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\QuickUsbVstudio.pch"


!ELSEIF  "$(CFG)" == "QuickUsbVstudio - Win32 Debug"


"$(INTDIR)\QuickUsbVstudioDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\QuickUsbVstudio.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "QuickUsbVstudio - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\QuickUsbVstudio.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\QuickUsbVstudio.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "QuickUsbVstudio - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\QuickUsbVstudio.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\QuickUsbVstudio.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

