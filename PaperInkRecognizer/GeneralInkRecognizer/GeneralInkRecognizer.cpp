/**********************************************************************************
 * copyright   : (C) 2012 by Herbert Ellebruch
 * email       : herbert@useful-tools.de

 **********************************************************************************
 *   This file is part of PaperInkRecognizer.                                     *
 *                                                                                *
 *   PaperInkRecognizer is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by         *
 *   the Free Software Foundation, either version 3 of the License, or            *
 *   (at your option) any later version.                                          *
 *                                                                                *
 *   Foobar is distributed in the hope that it will be useful,                    *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 *   GNU General Public License for more details.                                 *
 *                                                                                *
 *   You should have received a copy of the GNU General Public License            *
 *   along with PaperInkRecognizer.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                                *
 *********************************************************************************/

//  : Defines the exported functions for the DLL application.
#ifdef GENERALINKANALYZER_EXPORTS
#define PAPERINKANALYZER_API extern "C" __declspec(dllexport)
#else
#define PAPERINKANALYZER_API extern "C" __declspec(dllimport)
#endif

#include <Windows.h>
#include <comdef.h>
#include <assert.h>

// Headers for Tablet PC Automation interfaces
// Headers Included in Tablet PC SDK 1.7
#include <msinkaut.h>
#include <msinkaut_i.c>

#include <tpcerror.h>

// Headers for the Tablet PC Ink Analysis interfaces
#include <IACom.h>
#include <IACom_i.c>
#include <initguid.h>
#include <IAGuid.h>


#include <stdio.h>
#include <malloc.h>


// #define MathInput TODO
#ifdef MathInput
 #include "micaut.h"
 #include "micaut_i.c"
#endif

#include "PaperInkConverter.h"
#include "InkEvents.h"
#ifdef ExternAnalyzer
#include "AnalyzerCommunication.h"
#endif
#include "GeneralInkRecognizer.h"

///
///
// Managed Wrapper Functions
// for PaperInkConverter
PaperInkConverter* InitPaperInkReader(wchar_t* FileName, int RelativThreshold, int AbsoluteThreshold, int SlidingThreshold)
{
	return new PaperInkConverter(FileName,RelativThreshold,AbsoluteThreshold,SlidingThreshold);
}

int EOFfound(PaperInkConverter* pPaperInkConverter) {
	return(pPaperInkConverter->EOF_Found());
}
///
// Wrapper for Analyzer Class
PaperInkAnalyzer* NewPaperInkAnalyzer(
			InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction,
            InkInkDrawingInfo InkInkDrawingInfoFunction,
            InkLineBoxInfo InkLineBoxInfoFunction,
            InkBulletInfo InkBulletInfoFunction,
            AnalyserHasFinished AnalyserHasFinishedFunction,
            MathPanelReply MathPanelReplyFunction,
			HWND DrawingHandle) {
	return (new PaperInkAnalyzer(
			InkAnalyzerResultAvailableFunction,
            InkInkDrawingInfoFunction,
            InkLineBoxInfoFunction,
            InkBulletInfoFunction,
            AnalyserHasFinishedFunction,
            MathPanelReplyFunction,
			DrawingHandle));
};

int DeletePaperInkAnalyzer(PaperInkAnalyzer* aPaperInkAnalyzer) {
	delete aPaperInkAnalyzer;
	return(0);
};

int StrokesToMathInputPanel(PaperInkAnalyzer* aPaperInkAnalyzer) {
	return aPaperInkAnalyzer->SendAllStrokesToMathInputPanel();
};

int AnalyzeInk(PaperInkAnalyzer* aPaperInkAnalyzer) {
	return aPaperInkAnalyzer->AnalyzeInk();
};

int AddStrokesFromFile(PaperInkAnalyzer* aPaperInkAnalyzer,PaperInkConverter* pPaperInkConverter, long StrokeColorARGB) { 
	long StrokeColorRGB;
				// Convert Color from ARGB to RGB
	char* src = ((char*)&StrokeColorARGB)+2;
	char* dst = ((char*)&StrokeColorRGB);
	*dst++ = *src--;
	*dst++ = *src--;
	*dst++ = *src--;
	*dst++ = 0;
	return aPaperInkAnalyzer->AddStrokesFromFile(pPaperInkConverter, StrokeColorRGB);
};

int DeleteAllStrokes(PaperInkAnalyzer* aPaperInkAnalyzer) {
	return aPaperInkAnalyzer->DeleteAllStrokes();
};

int PositioningInkPanel(PaperInkAnalyzer* aPaperInkAnalyzer, int Basic_X_Offset, int Basic_Y_Offset, HWND HorizontalScrollHandle, HWND VerticalScrollHandle)
{
	return aPaperInkAnalyzer->PositioningInkPanel(Basic_X_Offset, Basic_Y_Offset, HorizontalScrollHandle, VerticalScrollHandle);
};

int CopyToClipboard(PaperInkAnalyzer* aPaperInkAnalyzer) {
	return aPaperInkAnalyzer->CopyToClipboard();
};
#ifdef NoCallback
int WrapFindUpperLeftLinePosition(PaperInkAnalyzer* aPaperInkAnalyzer, int* Min_X_Position_Left, int*  Min_X_Position_Right, int* Min_Y_Position_TOP, int* Min_Y_PositionBottom) {
	return aPaperInkAnalyzer->FindUpperLeftLinePosition(Min_X_Position_Left, Min_X_Position_Right,Min_Y_Position_TOP ,Min_Y_PositionBottom);
};

int GetInkBullets(PaperInkAnalyzer* aPaperInkAnalyzer) {
	return aPaperInkAnalyzer->GetInkBullets();
};

int GetNextInkBulletsInfo(PaperInkAnalyzer* aPaperInkAnalyzer, int BoxIndex, int* Left, int* Top, int* width, int* height ) {
	return aPaperInkAnalyzer->GetNextInkBulletsInfo(BoxIndex, Left, Top, width, height );
};

int GetInkLineBoxes(PaperInkAnalyzer* aPaperInkAnalyzer) {
	return aPaperInkAnalyzer->GetInkLineBoxes();
};

int GetNextInkLineBoxInfo(PaperInkAnalyzer* aPaperInkAnalyzer, int BoxIndex, int* Left, int* Top, int* width, int* height, char** pText ) {
	return aPaperInkAnalyzer->GetNextInkLineBoxInfo(BoxIndex, Left, Top, width, height, pText );
};

int GetInkDrawingBoxes(PaperInkAnalyzer* aPaperInkAnalyzer) {
	return aPaperInkAnalyzer->GetInkDrawingBoxes();
};

int GetNextInkInkDrawingHotSpots(PaperInkAnalyzer* aPaperInkAnalyzer, int BoxIndex, int* Left, int* Top, int* width, int* height, char** pText ) {
	return aPaperInkAnalyzer->GetNextInkInkDrawingHotSpots(BoxIndex, Left, Top, width, height, pText );
}
#endif


#ifdef ExternAnalyzer
void PaperInkAnalyzer::AddStrokeToAnalyzer( IInkStrokeDisp* pIInkStroke) {
{
        VARIANT varPacketData;
        HRESULT hr = pIInkStroke->GetPacketData(
            ISC_FirstElement,
            ISC_AllElements,
            &varPacketData);							// Packet Data of the Stroke
        if( SUCCEEDED(hr) )
        {
            // Get the stroke id
            long id;
            hr = pIInkStroke->get_ID(&id);
            if( SUCCEEDED(hr) )
            {
                // Get the packet description
                VARIANT varPacketDesc;
                hr = pIInkStroke->get_PacketDescription(&varPacketDesc);
                if( SUCCEEDED(hr) )
                {
                    LONG * plPacketData = NULL;
                    BSTR * pbstrPacketDesc = NULL;

                    hr = ::SafeArrayAccessData(
                        varPacketData.parray,
                        (void **)&plPacketData);
														// plPacketData  pointer to the packet data
                    if( SUCCEEDED(hr) )
                    {
                        hr = ::SafeArrayAccessData(
                            varPacketDesc.parray,
                            (void **)&pbstrPacketDesc);
                    }
														// pbstrPacketDesc  pointer to the packet descriptor

                    if( SUCCEEDED(hr) )
                    {
                        ULONG guidCount = varPacketDesc.parray->rgsabound[0].cElements;
                        ULONG packetDataCount = varPacketData.parray->rgsabound[0].cElements;
														// 1* ULONG ID
														// 2* ULONG Counter
/*
                        GUID *pPacketDescGuids = new GUID[guidCount];
														// needed memory
														// 3 * ULONG
														// + guidCount* sizeof(GUID)
														// + packetDataCount
														// Memeory Layout
														// ULONG
														// ULONG
														// GUIDS
														// Packet Data
                        if( NULL == pPacketDescGuids )
                        {
                            hr = E_OUTOFMEMORY;
                        }
*/
						char* m_Pipe = (char*) malloc(sizeof(AnalyserStrokeInfo) + guidCount*sizeof(GUID)+ packetDataCount*sizeof(LONG));
						((AnalyserStrokeInfo*)m_Pipe)->Command = AddAnalyzerStroke;
						((AnalyserStrokeInfo*)m_Pipe)->id = id;
						((AnalyserStrokeInfo*)m_Pipe)->packetDataCount = packetDataCount;
						((AnalyserStrokeInfo*)m_Pipe)->guidCount = guidCount;

						GUID* ptempPacketDescGuids = (GUID*)(m_Pipe + sizeof(AnalyserStrokeInfo));
						memcpy(m_Pipe+sizeof(AnalyserStrokeInfo) + guidCount*sizeof(GUID),plPacketData, packetDataCount*sizeof(LONG));
                        if( SUCCEEDED(hr) )
                        {
                            for( ULONG ul = 0; ul < guidCount; ul++ )
                            {
								::CLSIDFromString(
									pbstrPacketDesc[ul],
									&ptempPacketDescGuids[ul]);			// pointer to the GUIDS memory
															}
 								DWORD ByteWritten;
																// Send Stroke to external Analyzer
								WriteFile(hWriteChild,m_Pipe, sizeof(AnalyserStrokeInfo) + guidCount*sizeof(GUID)+ packetDataCount*sizeof(LONG), &ByteWritten,NULL);
                        }
						free(m_Pipe);
                    }
                    ::SafeArrayUnaccessData(varPacketDesc.parray);
                    ::SafeArrayUnaccessData(varPacketData.parray);
                }
            }
        }
        VariantClear(&varPacketData);

    }
}
#else
void PaperInkAnalyzer::AddStrokeToAnalyzer( IInkStrokeDisp* pIInkStroke) {
{
        VARIANT varPacketData;
        HRESULT hr = pIInkStroke->GetPacketData(
            ISC_FirstElement,
            ISC_AllElements,
            &varPacketData);							// Packet Data of the Stroke
        if( SUCCEEDED(hr) )
        {
            // Get the stroke id
            long id;
            hr = pIInkStroke->get_ID(&id);
            if( SUCCEEDED(hr) )
            {
                // Get the packet description
                VARIANT varPacketDesc;
                hr = pIInkStroke->get_PacketDescription(&varPacketDesc);
                if( SUCCEEDED(hr) )
                {
                    LONG * plPacketData = NULL;
                    BSTR * pbstrPacketDesc = NULL;

                    hr = ::SafeArrayAccessData(
                        varPacketData.parray,
                        (void **)&plPacketData);
														// plPacketData  pointer to the packet data
                    if( SUCCEEDED(hr) )
                    {
                        hr = ::SafeArrayAccessData(
                            varPacketDesc.parray,
                            (void **)&pbstrPacketDesc);
                    }
														// pbstrPacketDesc  pointer to the packet descriptor

                    if( SUCCEEDED(hr) )
                    {
                        ULONG guidCount = varPacketDesc.parray->rgsabound[0].cElements;
                        ULONG packetDataCount = varPacketData.parray->rgsabound[0].cElements;
														// 1* ULONG ID
														// 2* ULONG Counter
                        GUID *pPacketDescGuids = new GUID[guidCount];
														// needed memory
														// 3 * ULONG
														// + guidCount* sizeof(GUID)
														// + packetDataCount
														// Memeory Layout
														// ULONG
														// ULONG
														// GUIDS
														// Packet Data
                        if( NULL == pPacketDescGuids )
                        {
                            hr = E_OUTOFMEMORY;
                        }
                        if( SUCCEEDED(hr) )
                        {
                            for( ULONG ul = 0; ul < guidCount; ul++ )
                            {
                                ::CLSIDFromString(
                                    pbstrPacketDesc[ul],
                                    &pPacketDescGuids[ul]);			// pointer to the GUIDS memory
                            }
                            IContextNode* spNode = NULL;
                            hr = m_spIInkAnalyzer->AddStroke(
                                id,
                                packetDataCount,
                                plPacketData,
                                guidCount,
                                pPacketDescGuids,
                                &spNode);

                        }
                        delete [] pPacketDescGuids;
                    }



                    ::SafeArrayUnaccessData(varPacketDesc.parray);
                    ::SafeArrayUnaccessData(varPacketData.parray);
                }
            }
        }
        VariantClear(&varPacketData);
    }
}
#endif

int PaperInkAnalyzer::SendAllStrokesToMathInputPanel() {
#ifdef MathInput
	HRESULT hr;
	if (!MathInputPanelOpen) {
		MathInputPanelOpen = true;
		HRESULT hr = CoInitialize(NULL);

		hr = CoCreateInstance(CLSID_MathInputControl,
                      NULL, CLSCTX_INPROC_SERVER,
                      IID_IMathInputControl,
                      (void **) &g_spMIC);
		hr = g_spMIC->EnableExtendedButtons(VARIANT_TRUE);
		hr = g_spMIC->Show();
	}
	hr = g_spMIC->LoadInk(    g_pIInkDisp  );


    hr = m_MathInputEvents.Init(g_pMainWindowHandle);
    // Set up connection between Math Input Panel and our event sink        
	hr = m_MathInputEvents.AdviseMathInputPanel(g_spMIC);



	return(MANAGED_TRUE);
#else
	return(MANAGED_FALSE);
#endif

}
int PaperInkAnalyzer::AnalyzeInk() {
	IAnalysisStatus* status;

#ifdef ExternAnalyzer
	{
		int Command = ClearAllAnalyzerStrokes;
									// Unadvise and Advise is done automaticly
		DWORD ByteWritten;
		WriteFile(hWriteChild,&Command, sizeof(Command), &ByteWritten,NULL);
	}
	HRESULT hr;
#else
	HRESULT hr = m_InkAnalyserEvents.UnadviseInkAnalyzer();
	m_spIInkAnalyzer->Release();
            hr = CoCreateInstance(CLSID_InkAnalyzer,
                                  NULL, CLSCTX_INPROC_SERVER,
                                  IID_IInkAnalyzer, 
                                  (void **) &m_spIInkAnalyzer);
if (FAILED(hr)) 
	int kkkk = 0;//        return -1;
#endif



#ifdef ExternAnalyzer
				// not needed at this place may be later for more functionality
#else
					// Set up connection between Ink Analyser and our event sink
	hr = m_InkAnalyserEvents.AdviseInkAnalyser(m_spIInkAnalyzer);

					// Set the analysis mode to do autoreconciliation and personlization
	hr = m_spIInkAnalyzer->SetAnalysisModes((AnalysisModes) AnalysisModes_AutomaticReconciliation );
#endif



	IInkStrokes*      m_spIInkCustomStrokes;
	hr = g_pIInkDisp->get_Strokes(&m_spIInkCustomStrokes);

	long NumberOfStrokes;
	hr = m_spIInkCustomStrokes->get_Count(&NumberOfStrokes);

	for (int icount=0;icount<NumberOfStrokes;icount++) {
		IInkStrokeDisp * pIInkStroke;
		hr = m_spIInkCustomStrokes->Item(icount,&pIInkStroke);
		AddStrokeToAnalyzer( pIInkStroke);
		pIInkStroke->Release();
	}
	hr = m_spIInkCustomStrokes->Release();

#ifdef ExternAnalyzer
	int Command = (int) AnalyzeStrokes;
	DWORD ByteWritten;
#ifdef NoAnalyzerCallback
	WriteFile(hWriteChild,&Command, sizeof(int), &ByteWritten,NULL);
	ReadFile(hReadChild,&LayoutBoxesCounts,sizeof(structLayoutBoxesCounts),&NumberOfBytesIn,NULL);
#else
	WriteFile(hWriteChild,&Command, sizeof(int), &ByteWritten,NULL);		// start BackgroundAnalyze
#endif
#else
						// hr = m_spIInkAnalyzer->Analyze(&status);
	hr = m_spIInkAnalyzer->BackgroundAnalyze();
    if (FAILED(hr)) 
        int test = 1;//return -1;
#endif
	return(MANAGED_TRUE);
}

int PaperInkAnalyzer::AddStrokesFromFile(PaperInkConverter* m_pPaperInkConverter, long StrokeColor) {

	VARIANT   var, varPK;
	LONG*   plongArray=NULL;
	VariantInit( &var );
	VariantInit( &varPK );

    int StrokeFound = 0;				// Mark we have a stroke

	SECURITY_ATTRIBUTES lsa;

	int nSize = 0;

	lsa.nLength=sizeof(SECURITY_ATTRIBUTES);
	lsa.lpSecurityDescriptor=NULL;
	lsa.bInheritHandle=TRUE;

#ifdef ForTestOnlyTODO
// NewAttributes->put_Color(RGB(0xff,0,0));
int sehen1 = RGB(0xff,0,0);
int sehen2 = StrokeColor & RGB(0xff,0xFF,0xFF);

typedef struct sColorRGB{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char unused;
} ColorRGB;



struct sColorARGB{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char alpha;
} ColorARGB;

sColorRGB dest;
dest.r = ((ColorARGB)StrokeColor).r;
dest.g = ((ColorARGB)StrokeColor).g;
dest.b = ((ColorARGB)StrokeColor).b;
dest.unused = 0;

int ConvertedColor;
char* src = ((char*)&StrokeColor)+2;
char* dst = ((char*)&ConvertedColor);
*dst++ = *src--;
*dst++ = *src--;
*dst++ = *src--;
*dst++ = 0;
#endif


	IInkDrawingAttributes* NewAttributes;
	HRESULT hr = g_pIInkCollector->get_DefaultDrawingAttributes(&NewAttributes);// TODO Wrong Place

	//NewAttributes->put_IgnorePressure(VARIANT_FALSE);
	NewAttributes->put_Color(StrokeColor);// TODO Wrong Place
	NewAttributes->put_Height(20);
	NewAttributes->put_Width(20);
	hr = g_pIInkCollector->putref_DefaultDrawingAttributes(NewAttributes);

	while (StrokeFound == 0)
	{
		long   lSize=m_pPaperInkConverter->GetNumberOfPoints();;
		SAFEARRAY* psa = SafeArrayCreateVector( VT_I4, 0, lSize*2 );
		if( psa )
		{
			if( SUCCEEDED( hr = SafeArrayAccessData( psa, (void**)&plongArray) ))
			{
				short* actualStroke_X = m_pPaperInkConverter->GetPenPositionX();
				short* actualStroke_Y = m_pPaperInkConverter->GetPenPositionY();
				for( long i = 0; i < lSize; i++ )
				{

					plongArray[2*i]  = ((int)*actualStroke_X) + 12000;			// TODO Change constant
					plongArray[2*i+1] = (((int)*actualStroke_Y)) + 1900;		// TODO Change constant
					actualStroke_X++;
					actualStroke_Y++;
				}
				hr = SafeArrayUnaccessData( psa );

				if ( SUCCEEDED( hr ) )
				{
					var.vt     = VT_ARRAY | VT_I4;
					var.parray = psa;

					// varPK (packet description) is currently reserved, so it is
					// just empty variant for now.
					IInkStrokeDisp * pIInkStroke;
					hr = g_pIInkDisp->CreateStroke(var,varPK,&pIInkStroke);
					hr = pIInkStroke->putref_DrawingAttributes(NewAttributes);// TODO clipboardfly different colors will be added later

					AddStrokeToAnalyzer( pIInkStroke);
					hr = pIInkStroke->Release();
				}
			}
	   }
	   StrokeFound = m_pPaperInkConverter->ReadNextStroke();
  
	}
	VariantClear( &var );
	VariantClear( &varPK );
	NewAttributes->Release();

#ifdef ExternAnalyzer
	int Command = (int) AnalyzeStrokes;
	DWORD ByteWritten;
#ifdef NoAnalyzerCallback
	DWORD NumberOfBytesIn;
	WriteFile(hWriteChild,&Command, sizeof(int), &ByteWritten,NULL);
	ReadFile(hReadChild,&LayoutBoxesCounts,sizeof(structLayoutBoxesCounts),&NumberOfBytesIn,0);
#else
	WriteFile(hWriteChild,&Command, sizeof(int), &ByteWritten,NULL);
#endif
#else
    hr = m_spIInkAnalyzer->BackgroundAnalyze();
	if (FAILED(hr)) 
        return MANAGED_FALSE;
#endif
	return(MANAGED_TRUE);
}


PaperInkAnalyzer::PaperInkAnalyzer(
			InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction,
            InkInkDrawingInfo InkInkDrawingInfoFunction,
            InkLineBoxInfo InkLineBoxInfoFunction,
            InkBulletInfo InkBulletInfoFunction,
            AnalyserHasFinished AnalyserHasFinishedFunction,
            MathPanelReply MathPanelReplyFunction,
			HWND DrawingHandle) {
										// initialize the Class Variables

	PreviousVerticalShiftPos = 0;
	PreviousVerticalShiftPosFloat = 0;

	PreviousHorizontalShiftPos = 0;
	PreviousHorizontalShiftPosFloat = 0;

#ifdef MathInput
// TODO
	MathInputPanelOpen = false;
	g_pMainWindowHandle = (HWND) DrawingHandle;
#endif

    // Create the InkCollector object.
    HRESULT hr = CoCreateInstance(CLSID_InkCollector, 
                                  NULL, CLSCTX_INPROC_SERVER, 
                                  IID_IInkCollector, 
                                  (void **) &g_pIInkCollector);
    if (FAILED(hr)) 
		int test = 1;// return -1;
																InkCollectorEventInterest TODOeventtest = ICEI_NewPackets;
            // Get a pointer to the Ink object
    hr = g_pIInkCollector->get_Ink(&g_pIInkDisp);
																g_pIInkCollector->SetEventInterest(TODOeventtest,VARIANT_TRUE);
    if (FAILED(hr)) 
        int test = 1;// return -1;
	hr = g_pIInkCollector->get_Renderer(&g_pInkRenderer);
	g_pInkRenderer->ScaleTransform(1.08,1.08);// TODO Check Variables

#ifdef StrokeEvents
    hr = m_InkEvents.Init((HWND) DrawingHandle);
        // Set up connection between Ink Collector and our event sink        
	hr = m_InkEvents.AdviseInkCollector(g_pIInkCollector);
#endif

/* TODO Error Handling
        if (FAILED(hr))
        {
            return hr;
        }
*/

#ifdef ExternAnalyzer
// Example for ink layout 
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms704040(v=vs.85).aspx

	BOOL Success;
	SECURITY_ATTRIBUTES lsa;
	STARTUPINFOA         si;
	PROCESS_INFORMATION pi;

	lsa.nLength=sizeof(SECURITY_ATTRIBUTES);
	lsa.lpSecurityDescriptor=NULL;
	lsa.bInheritHandle=TRUE;


			//
			// Create Parent_Write to ChildStdIn Pipe.  Then
			// duplicate the parent copy to a noninheritable
			// handle and close the inheritable one so that
			// the child won't be holding open a handle to
			// the server end of its stdin pipe when we try
			// to nuke that pipe to close the child.
			//

	int nSize = 0;
	Success = CreatePipe(
			  &ChildIn,                     // read handle
			  &hWriteChild,                 // write handle
			  &lsa,							// security attributes
			  nSize                         // pipe size
			);

	Success = CreatePipe(
		  &hReadChild,                      // read handle
		  &ChildOut,						// write handle
		  &lsa,								// security attributes
		  nSize                             // pipe size
		);

	SetHandleInformation(hWriteChild,HANDLE_FLAG_INHERIT,0);

	ZeroMemory(&si, sizeof(si));
	si.cb            = sizeof(STARTUPINFOA);
	si.dwFlags       = STARTF_USESTDHANDLES;
	si.hStdInput     = ChildIn;					// ReadPipeHandle
	si.hStdOutput    = ChildOut;				// WritePipeHandle
	si.hStdError     = 0;
	si.wShowWindow   = SW_SHOW;

			//
			// Create Child Process
		 
	char buffer[512];
	DWORD DLL_FileLength = GetModuleFileNameA( GetModuleHandle(RECOGNIZER_FULLDLLNAME ), buffer, sizeof(buffer) ) - 1;
	char* pDLL_Name = &buffer[DLL_FileLength];
	while(DLL_FileLength > 0) {
		if (*pDLL_Name == L'\\') 
		{
			pDLL_Name++;
			break;
		}
		DLL_FileLength--;
		pDLL_Name--;
	}


#ifdef DebugDLL
// old Version	strcpy(pDLL_Name,"..\\..\\..\\..\\InkAnalyser32\\Debug\\InkAnalyser32.exe");
	strcpy(pDLL_Name,"..\\..\\..\\InkAnalyser32\\Debug\\InkAnalyser32.exe");
// \PaperInkRecognizer\InkAnalyser32\Debug
#else
	strcpy(pDLL_Name,"InkAnalyser32.exe");
#endif


	BOOL res = CreateProcessA(NULL,		// module name
					 buffer,			// Command Name
					 NULL,				// Process handle not inheritable.
					 NULL,				// Thread handle not inheritable.
					 TRUE, //FALSE,		// Set handle inheritance
					 CREATE_NO_WINDOW,                // .
					 NULL,				// Do not use parent's environment block.
					 NULL,				// Do not use parent's starting directory.
					 &si,				// Pointer to STARTUPINFO structure.
					 &pi );				// Pointer to PROCESS_INFORMATION structure.
	if ( !res) {
		MessageBoxA(NULL,"Sending Modul not available","Fatal Error",MB_ICONWARNING);
	} else {

		WaitForInputIdle( pi.hProcess,  8000);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		m_InkAnalyserEvents.Init (hReadChild);
		m_InkAnalyserEvents.SetCallbacks(
				InkAnalyzerResultAvailableFunction,
				InkInkDrawingInfoFunction,
				InkLineBoxInfoFunction,
				InkBulletInfoFunction,
				AnalyserHasFinishedFunction);
	}


#else
    // Create an ink analyzer object.
	hr = CoCreateInstance(CLSID_InkAnalyzer, 
                                  NULL, CLSCTX_INPROC_SERVER,
                                  IID_IInkAnalyzer, 
                                  (void **) &m_spIInkAnalyzer);


	if (FAILED(hr)) 
		int test = 1;//return -1;// todo  Insert Error Handling

	hr = m_InkAnalyserEvents.Init( DrawingHandle);
					// Set up connection between Ink Analyser and our event sink
	m_InkAnalyserEvents.SetCallbacks(
			InkAnalyzerResultAvailableFunction,
            InkInkDrawingInfoFunction,
            InkLineBoxInfoFunction,
            InkBulletInfoFunction,
            AnalyserHasFinishedFunction);
// TODO for Test only           MathPanelReply MathPanelReplyFunction


	hr = m_InkAnalyserEvents.AdviseInkAnalyser(m_spIInkAnalyzer);

					// Set the analysis mode to do autoreconciliation and personlization
								//    hr = m_spIInkAnalyzer->SetAnalysisModes((AnalysisModes) AnalysisModes_Personalization );
	hr = m_spIInkAnalyzer->SetAnalysisModes((AnalysisModes) AnalysisModes_AutomaticReconciliation );

	if (FAILED(hr))
	{
		int Test = 1;// int return hr;// todo  Insert Error Handling
	}

#endif

// Enable ink input in the m_wndInput window
	    hr = g_pIInkCollector->put_hWnd((long)DrawingHandle);
    if (FAILED(hr)) 
        int test = 1;//return -1;
    hr = g_pIInkCollector->put_Enabled(VARIANT_TRUE);
    if (FAILED(hr)) 
        int test = 1;//return -1;
}

PaperInkAnalyzer::~PaperInkAnalyzer() {
    {
#ifdef ExternAnalyzer
		int Command = FinishInkAnylyzer;
		DWORD ByteWritten;
		WriteFile(hWriteChild,&Command, sizeof(Command), &ByteWritten,NULL);
					// hReadChild is closed in the thread
		BOOL temp = CloseHandle(hWriteChild);
		temp = CloseHandle(ChildOut);
		temp = CloseHandle(ChildIn);
#else
		m_InkAnalyserEvents.UnadviseInkAnalyzer();
		m_spIInkAnalyzer->Release();
#endif
		g_pInkRenderer->Release();
		g_pIInkDisp->Release();
		g_pIInkCollector->Release();
    }

};

int PaperInkAnalyzer::DeleteAllStrokes() {
	HRESULT hr = g_pIInkDisp->DeleteStrokes(0);
#ifdef ExternAnalyzer
									// Unadvise and Advise is done automaticly
	int Command = ClearAllAnalyzerStrokes;
	DWORD ByteWritten;
	DWORD NumberOfBytesIn;
	WriteFile(hWriteChild,&Command, sizeof(int), &ByteWritten,NULL);
#else
	hr = m_InkAnalyserEvents.UnadviseInkAnalyzer();
	m_spIInkAnalyzer->Release();
           hr = CoCreateInstance(CLSID_InkAnalyzer,
                                  NULL, CLSCTX_INPROC_SERVER,
                                  IID_IInkAnalyzer,
                                  (void **) &m_spIInkAnalyzer);
    if (FAILED(hr)) 
		int kk = 0;// todo  Insert Error Handling
#endif


#ifdef ExternAnalyzer
				// not needed at this place may be later for more functionality
#else
	hr = m_InkAnalyserEvents.AdviseInkAnalyser(m_spIInkAnalyzer);

					// Set the analysis mode to do autoreconciliation and personlization
								//    hr = m_spIInkAnalyzer->SetAnalysisModes((AnalysisModes) AnalysisModes_Personalization );
	hr = m_spIInkAnalyzer->SetAnalysisModes((AnalysisModes) AnalysisModes_AutomaticReconciliation );
#endif
	return(MANAGED_TRUE);
}

int PaperInkAnalyzer::CopyToClipboard() {

	// IInkStrokes* theStrokes;
	// InkClipboardFormats tempFormats = (InkClipboardFormats) (ICF_InkSerializedFormat | ICF_SketchInk | ICF_TextInk);
	// InkClipboardModes c;
	IDataObject* tempDataObject;
	// g_pIInkDisp->ClipboardCopy(a,b,c,&d);
	HRESULT hrxy = g_pIInkDisp->ClipboardCopy(NULL,ICF_Default, ICB_Copy,&tempDataObject);
	tempDataObject->Release();
	return(MANAGED_TRUE);
}

int PaperInkAnalyzer::PositioningInkPanel(int Basic_X_Offset, int Basic_Y_Offset, HWND HorizontalScrollHandle, HWND VerticalScrollHandle)
{
	    // Get the renderer from the ink collector's ink object
	IInkRenderer* spInkRenderer;
	HRESULT hr = g_pIInkCollector->get_Renderer(&spInkRenderer);
	int ActualVerticalShiftPos =  - (Basic_Y_Offset + GetScrollPos(VerticalScrollHandle,SB_VERT) * 27 );
	int ActualHorizontalShiftPos =  - (Basic_X_Offset + GetScrollPos(HorizontalScrollHandle,SB_CTL) * 27 );

	if ((ActualVerticalShiftPos != PreviousVerticalShiftPos) || (ActualHorizontalShiftPos != PreviousHorizontalShiftPos)) {

		float temp1 = Basic_Y_Offset * 1.055;
		float temp2 = GetScrollPos(VerticalScrollHandle,SB_VERT) * 27 ;// todo
		float ActualVerticalShiftPosFloat = -(temp1 + temp2);

		temp1 = Basic_X_Offset * 1.055;
		temp2 = GetScrollPos(HorizontalScrollHandle,SB_CTL) * 27 * 8 ;// todo
//		spInkRenderer->Move(0.,(float)(ActualVerticalShiftPos  - PreviousShiftPos) );
// 		hr = spInkRenderer->Move(0.,(ActualVerticalShiftPosFloat - PreviousVerticalShiftPosFloat ));
		float ActualHorizontalShiftPosFloat = -(temp1 + temp2);
		hr = spInkRenderer->Move((ActualHorizontalShiftPosFloat - PreviousHorizontalShiftPosFloat),(ActualVerticalShiftPosFloat - PreviousVerticalShiftPosFloat ));

		PreviousVerticalShiftPos = ActualVerticalShiftPos;
		PreviousVerticalShiftPosFloat = ActualVerticalShiftPosFloat;

		PreviousHorizontalShiftPos = ActualHorizontalShiftPos;
		PreviousHorizontalShiftPosFloat = ActualHorizontalShiftPosFloat;
	}
	return MANAGED_TRUE;
};


// Application Spezific Ink Events
// Event: Stroke
void CMyInkEvents::Stroke(
    IInkCursor* Cursor,
    IInkStrokeDisp* Stroke,
    VARIANT_BOOL *Cancel)
{
    // Demonstrate that we received the event notification.
    MessageBox(m_hWnd, TEXT("Stroke Event"), TEXT("Event Received"), MB_OK);
#ifdef TODO_Test
	VARIANT Punkte;
	Stroke->GetPoints(ISC_FirstElement,ISC_AllElements,&Punkte);
#endif
	int gucken = 9;
}

CMyInkEvents::CMyInkEvents()
{
    m_hWnd = NULL;
}

HRESULT CMyInkEvents::Init(
    HWND hWnd)
{
    m_hWnd = hWnd;
    return InkCollectorEvents::Init();
}

// Application Spezific Ink Analyzer Events
#ifdef ExternAnalyzer
	void CMyInkAnalyserEvents::ReceiveInkAnalyzerResult() {// todo rename in ResultsUpdated
		bool AnalyzerThreadRunning = true;
		while (AnalyzerThreadRunning) {
			int x = 1;
			DWORD NumberOfBytesIn;
			int Command;
			ReadFile(hReadChild,&Command,sizeof(Command),&NumberOfBytesIn,NULL);
			if (NumberOfBytesIn != sizeof(Command)) {
				break;
			}

			switch (Command) {
			case AnalyzingReady:

				{
					DWORD NumberOfBytesIn;
					sCountInfo LayoutBoxesCounts;
					ReadFile(hReadChild,&LayoutBoxesCounts,sizeof(sCountInfo),&NumberOfBytesIn,NULL);
					if (NumberOfBytesIn != sizeof(sCountInfo)) {
						AnalyzerThreadRunning = false;
						break;
					}
					LineBoxesCount = 0;
					BulletBoxesCount = 0;
					DrawinfBoxesCount = 0;
				}
				break;

			case AnalyzerResultAvailable:
				{
												// TODO rename it in InkanalysesStarted 
					int Success =  (*InkAnalyzerResultAvailableFunction)    ( );
				}
				break;

			case LineBoxes:
				{
					DWORD NumberOfBytesIn;
					sLineBoxInfo LineBoxesInfo;
					ReadFile(hReadChild, &LineBoxesInfo, sizeof(LineBoxesInfo) ,&NumberOfBytesIn,NULL);
					if (NumberOfBytesIn != sizeof(LineBoxesInfo)) {
						AnalyzerThreadRunning = false;
						break;
					}
					BYTE* pLineString = (BYTE*) malloc(LineBoxesInfo.RecognizedStringSize+1);
					ReadFile(hReadChild, pLineString,LineBoxesInfo.RecognizedStringSize+1,&NumberOfBytesIn,NULL);
					if (NumberOfBytesIn != (LineBoxesInfo.RecognizedStringSize+1)) {
						AnalyzerThreadRunning = false;
						break;
					}
		
					int Success =  (*InkLineBoxInfoFunction)  ( LineBoxesInfo.Left ,  LineBoxesInfo.Top  ,  LineBoxesInfo.width  , LineBoxesInfo.height, (char*) pLineString);
					free(pLineString);

				}
				break;

			case BulletBoxes:
				{
					DWORD NumberOfBytesIn;
					sBulletBoxInfo BulletBoxesInfo;
					ReadFile(hReadChild, &BulletBoxesInfo, sizeof(BulletBoxesInfo) ,&NumberOfBytesIn,NULL);
					int Success =  (*InkBulletInfoFunction)  ( BulletBoxesInfo.Left ,  BulletBoxesInfo.Top ,  BulletBoxesInfo.width  , BulletBoxesInfo.height);
				}
				break;

			case InkDrawingBoxes:
				{
					DWORD NumberOfBytesIn;
					sDrawingBoxInfo DrawingBoxHotspots;
					ReadFile(hReadChild, &DrawingBoxHotspots, sizeof(DrawingBoxHotspots) ,&NumberOfBytesIn,NULL);
					if (NumberOfBytesIn != sizeof(DrawingBoxHotspots)) {
						AnalyzerThreadRunning = false;
						break;
					}
					BYTE* pLineString = (BYTE*) malloc(DrawingBoxHotspots.RecognizedStringSize+1);
					ReadFile(hReadChild, pLineString,DrawingBoxHotspots.RecognizedStringSize+1,&NumberOfBytesIn,NULL);
					if (NumberOfBytesIn != (DrawingBoxHotspots.RecognizedStringSize+1)) {
						AnalyzerThreadRunning = false;
						break;
					}
		
					int Success =  (*InkInkDrawingInfoFunction)  ( DrawingBoxHotspots.Left ,  DrawingBoxHotspots.Top ,  DrawingBoxHotspots.width  , DrawingBoxHotspots.height, (char*) pLineString);
					free(pLineString);
				}
				break;

			case InkAnalyzeFinished:
				{
					sPointsInfo UpperPositionPoints;
					DWORD NumberOfBytesIn;
					ReadFile(hReadChild,&UpperPositionPoints,sizeof(UpperPositionPoints),&NumberOfBytesIn,NULL);
					if (NumberOfBytesIn != sizeof(UpperPositionPoints)) {
						AnalyzerThreadRunning = false;
						break;
					}
					int Success =  (*AnalyserHasFinishedFunction)     ( UpperPositionPoints.X_MinValue_Left ,  UpperPositionPoints.X_MinValue_Right  ,  UpperPositionPoints.Y_MinValue_Top  , UpperPositionPoints.Y_MinValue_Bottom);
				}
				break;
			case FinischInkAnalyzerListener:
				{
					BOOL temp = CloseHandle(hReadChild);
					hReadChild = NULL;
					AnalyzerThreadRunning = false;
				}
				break;
			}
		}
		if (hReadChild != NULL) {
			BOOL temp = CloseHandle(hReadChild);		// close the read handle if not yet done
		}
	}
#else
HRESULT STDMETHODCALLTYPE CMyInkAnalyserEvents::ResultsUpdated(
	IInkAnalyzer* pInkAnalyzer,
	IAnalysisStatus* pAnalysisStatus) {
#ifdef onlyForTesting
					VARIANT_BOOL bResult = VARIANT_FALSE;
					HRESULT     hrT = pAnalysisStatus->IsSuccessful(&bResult);

					if( SUCCEEDED(hrT) && (VARIANT_TRUE == bResult) )
					{
						// Get the top result string
						int x = 8;
						BSTR bstrTest = NULL;
						hr = pInkAnalyzer->GetRecognizedString(&bstrTest);
						int xui = 5;
					}
#endif
		int Success =  (*InkAnalyzerResultAvailableFunction)    ( );


		IContextNodes *pLineContextNodesfound;					// received Line Nodes
 		HRESULT hr = pInkAnalyzer->FindNodesOfType(&GUID_CNT_LINE, &pLineContextNodesfound);
		ULONG NumberOfLineNodes;
		pLineContextNodesfound->GetCount(&NumberOfLineNodes);
		if (NumberOfLineNodes != 0) {
			long temp_X_MinValue_Left = LONG_MAX;
			long temp_X_MinValue_Right = LONG_MAX;
			long temp_Y_MinValue_Top = LONG_MAX;
			long temp_Y_MinValue_Bottom = LONG_MAX;
			for (int i = 0;i<NumberOfLineNodes;i++) {
				IContextNode* pLineNode = NULL;
				HRESULT hr = pLineContextNodesfound->GetContextNode(i,&pLineNode);
				RECT bounds;
				// Get the location's bounds
				IAnalysisRegion* spLocation;
				hr = pLineNode->GetLocation(&spLocation);
				if( SUCCEEDED(hr) )
				{
					hr = spLocation->GetBounds(&bounds);
				}

				if ( bounds.left < temp_X_MinValue_Left) {
					temp_X_MinValue_Left = bounds.left;
					temp_X_MinValue_Right = bounds.right;
				}
				if ( bounds.top < temp_Y_MinValue_Top) {
					temp_Y_MinValue_Top = bounds.top;
					temp_Y_MinValue_Bottom = bounds.bottom;
				}
				spLocation->Release();

				BYTE* pLineString;
				ULONG NumberOfCharacterInLine;
				hr = pLineNode->GetPropertyData(&GUID_CNP_RECOGNIZEDSTRING,&NumberOfCharacterInLine,&pLineString);
				int Success =  (*InkLineBoxInfoFunction)  ( bounds.left ,  bounds.top  ,  bounds.right - bounds.left  , bounds.bottom - bounds.top, (char*) pLineString);
				::CoTaskMemFree(pLineString);			// free previous memory allocated by the GetPropertyData method

			}

			IContextNodes *pBulletContextNodesfound;				// received Bullet Nodes
 			hr = pInkAnalyzer->FindNodesOfType(&GUID_CNT_INKBULLET, &pBulletContextNodesfound);
			ULONG NumberOfBulletNodes;
			hr = pBulletContextNodesfound->GetCount(&NumberOfBulletNodes);// TODO generate Error messsge
			for (int i = 0;i<NumberOfBulletNodes;i++) {
				IContextNode* pBulletNode = NULL;
				HRESULT hr = pBulletContextNodesfound->GetContextNode(i,&pBulletNode);
				RECT bounds;
				// Get the location's bounds
				IAnalysisRegion* spLocation;
				hr = pBulletNode->GetLocation(&spLocation);
				if( SUCCEEDED(hr) )
				{
					hr = spLocation->GetBounds(&bounds);
				}
				spLocation->Release();
				pBulletNode->Release();
				int Success =  (*InkBulletInfoFunction)  ( bounds.left ,  bounds.top  ,  bounds.right - bounds.left  , bounds.bottom - bounds.top);
			}
			pBulletContextNodesfound->Release();
			int Success =  (*AnalyserHasFinishedFunction)    ( temp_X_MinValue_Left, temp_X_MinValue_Right, temp_Y_MinValue_Top, temp_Y_MinValue_Bottom);
		} else {
			//											   ( int Min_X_Position_Left, int Min_X_Position_Right, int Min_Y_Position_TOP, int Min_Y_PositionBottom)
			int Success =  (*AnalyserHasFinishedFunction)    ( 0                    ,                      1  ,  0                    ,           1);
		}
		pLineContextNodesfound->Release();
		return S_OK;
	}

    CMyInkAnalyserEvents::CMyInkAnalyserEvents()
    {
        m_hWnd = NULL;
    }
    
    HRESULT CMyInkAnalyserEvents::Init(
        HWND hWnd)
    {
        m_hWnd = hWnd;
		return S_OK;
    }

	void CMyInkAnalyserEvents::SetCallbacks(
			InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction_In,
            InkInkDrawingInfo InkInkDrawingInfoFunction_In,
            InkLineBoxInfo InkLineBoxInfoFunction_In,
            InkBulletInfo InkBulletInfoFunction_In,
            AnalyserHasFinished AnalyserHasFinishedFunction_In
            // TODO for test only  MathPanelReply MathPanelReplyFunction_In
		)
	{
			InkAnalyzerResultAvailableFunction = InkAnalyzerResultAvailableFunction_In;
            InkInkDrawingInfoFunction = InkInkDrawingInfoFunction_In;
            InkLineBoxInfoFunction = InkLineBoxInfoFunction_In;
            InkBulletInfoFunction = InkBulletInfoFunction_In;
            AnalyserHasFinishedFunction = AnalyserHasFinishedFunction_In;
            // TODO for test only  MathPanelReply MathPanelReplyFunction,
		return;
	}
    
#endif