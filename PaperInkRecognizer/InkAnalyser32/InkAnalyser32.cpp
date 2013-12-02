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
#include <windows.h>
//#include <comdef.h>

#include <tchar.h>

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

#include "AnalyzerCommunication.h"

typedef struct AnalyserStrokeInfo {
	long id;
	ULONG packetDataCount;
	ULONG guidCount;
} ;

typedef struct structUpperPositionPoints {
	long command;
	sPointsInfo PointsInfo;
};

typedef struct structLineBoxesInfo {
	long Command;
	sLineBoxInfo BoxInfo;
};

typedef struct structBulletBoxesInfo {
	long Command;
	sBulletBoxInfo BoxInfo;
};

typedef struct structDrawingBoxHotspots {
	long Command;
	sDrawingBoxInfo BoxInfo;
};

#define BUF_SIZE 256
    IInkAnalyzer* m_spIInkAnalyzer;
	BYTE* pLineString = NULL;
	static char m_PipeMem[10000];// todo  Not static, create with alloc

int _tmain(int argc, _TCHAR* argv[])
{
	char* m_Pipe = &m_PipeMem[0];// todo  alloc Memory
	int receivedCommand;
	DWORD BytesRed;
    // Create an ink analyzer object.

    if (S_OK == ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))
    {
		int x = 3;// todo  Insert Error Handling
	}

	HANDLE hParentStdIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    HRESULT hr = CoCreateInstance(CLSID_InkAnalyzer,
                                  NULL, CLSCTX_INPROC_SERVER,
                                  IID_IInkAnalyzer, 
                                  (void **) &m_spIInkAnalyzer);

    if (FAILED(hr)) 
        int test = 1;//return -1;		// todo  Insert Error Handling

// DebugBreak();

	while (1) {
		ReadFile(hParentStdIn,&receivedCommand,sizeof(int),&BytesRed,NULL);

		if (BytesRed < sizeof(int)) break;				// End of file received 
		switch ( (AnalyzerCommand)receivedCommand) {
			case AdviseInkAnylyzer:
				{
				}
				break;
			case UnanadviseInkAnylyzer:
				{
				}
				break;
			case FinishInkAnylyzer:
				{
					int Command = FinischInkAnalyzerListener;
					DWORD CountBytesWritten;
					WriteFile(hParentStdOut,&Command,sizeof(Command),&CountBytesWritten,NULL);
				}
				break;
			case AddAnalyzerStroke:
				{
					ReadFile(hParentStdIn,m_Pipe,sizeof(AnalyserStrokeInfo) ,&BytesRed,NULL);			// Now Read the count Values
					if (BytesRed < sizeof(AnalyserStrokeInfo)) break;									// End of file received
					ReadFile(hParentStdIn,m_Pipe+sizeof(AnalyserStrokeInfo),((AnalyserStrokeInfo*)m_Pipe)->guidCount*sizeof(GUID)+ ((AnalyserStrokeInfo*)m_Pipe)->packetDataCount*sizeof(LONG),&BytesRed,NULL);			// Now Read the count Values
if (BytesRed > (sizeof(m_PipeMem) - sizeof(AnalyserStrokeInfo))) DebugBreak();// todo  realloc more Memory
					if (BytesRed < ((AnalyserStrokeInfo*)m_Pipe)->guidCount*sizeof(GUID)+ ((AnalyserStrokeInfo*)m_Pipe)->packetDataCount*sizeof(LONG)) break;				// End of file received 

					IContextNode* spNode = NULL;

					hr = m_spIInkAnalyzer->AddStroke(
						 ((AnalyserStrokeInfo*)m_Pipe)->id,													// id,
						 ((AnalyserStrokeInfo*)m_Pipe)->packetDataCount,									// packetDataCount,
						 (LONG*)(m_Pipe+(sizeof(AnalyserStrokeInfo) + ((AnalyserStrokeInfo*)m_Pipe)->guidCount*sizeof(GUID))),	// plPacketData,
						 ((AnalyserStrokeInfo*)m_Pipe)->guidCount,											// guidCount,
						 (GUID*)(m_Pipe+sizeof(AnalyserStrokeInfo)),										// pPacketDescGuids,
						 &spNode);
					}
				break;

			case AnalyzeStrokes:
			{
				IAnalysisStatus* status;
				hr = m_spIInkAnalyzer->Analyze(&status);
				status->Release();
				if (FAILED(hr)) 
					int test = 1;//return -1;	// todo  Insert Error Handling
				DWORD CountBytesWritten;

				structLayoutBoxesCounts LayoutBoxesCounts;

				IContextNodes *pLineContextNodesfound;					// received Line Nodes
				HRESULT hr = m_spIInkAnalyzer->FindNodesOfType(&GUID_CNT_LINE, &pLineContextNodesfound);
				LayoutBoxesCounts.command = AnalyzingReady;
				ULONG LineNodeCount;
				hr = pLineContextNodesfound->GetCount(&LineNodeCount);// Todo Error Message ???
				LayoutBoxesCounts.CountInfo.LineBoxesCount = LineNodeCount;
																		// Now Send the Lines

				IContextNodes *pBulletContextNodesfound;				// received Bullet Nodes
 				hr = m_spIInkAnalyzer->FindNodesOfType(&GUID_CNT_INKBULLET, &pBulletContextNodesfound);
				ULONG NumberOfBulletNodes;
				hr = pBulletContextNodesfound->GetCount(&NumberOfBulletNodes);// Todo Error Message ???
				LayoutBoxesCounts.CountInfo.BulletBoxesCount = NumberOfBulletNodes;
																		// Now Send the Bullets


				IContextNodes *pDrawingContextNodesfound;				// received Drawing Nodes
				hr = m_spIInkAnalyzer->FindNodesOfType(&GUID_CNT_INKDRAWING, &pDrawingContextNodesfound);
				ULONG NumberOfDrawingNodes;
				hr = pDrawingContextNodesfound->GetCount(&NumberOfDrawingNodes);// Todo Error Message ???
				LayoutBoxesCounts.CountInfo.DrawinfBoxesCount = NumberOfDrawingNodes;
																		// Now Send the Ink Drawings

																		// Now Send the Left Ink Corner
				int Command = AnalyzerResultAvailable;
				WriteFile(hParentStdOut,&Command,sizeof(Command),&CountBytesWritten,NULL);
																		// Now Send the Lines
				long temp_X_MinValue_Left = LONG_MAX;
				long temp_X_MinValue_Right = LONG_MAX;
				long temp_Y_MinValue_Top = LONG_MAX;
				long temp_Y_MinValue_Bottom = LONG_MAX;
				structUpperPositionPoints UpperPositionPoints;
				if (LayoutBoxesCounts.CountInfo.LineBoxesCount != 0) {
					for (int BoxIndex = 0;BoxIndex < LayoutBoxesCounts.CountInfo.LineBoxesCount;BoxIndex++) {
						IContextNode* pLineNode = NULL;
						HRESULT hr = pLineContextNodesfound->GetContextNode(BoxIndex,&pLineNode);
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


						structLineBoxesInfo LineBoxesInfo;
						LineBoxesInfo.Command = LineBoxes;
						LineBoxesInfo.BoxInfo.Left = bounds.left;
						LineBoxesInfo.BoxInfo.Top = bounds.top;
						LineBoxesInfo.BoxInfo.width = bounds.right - bounds.left;
						LineBoxesInfo.BoxInfo.height = bounds.bottom - bounds.top;
//						if (pLineString != NULL) {
//							::CoTaskMemFree(pLineString);			// free previous memory allocated by the GetPropertyData method
//						}
						spLocation->Release();
						ULONG NumberOfCharacterInLine;
						hr = pLineNode->GetPropertyData(&GUID_CNP_RECOGNIZEDSTRING,&NumberOfCharacterInLine,&pLineString);
//						*pText = (char*) pLineString;
						LineBoxesInfo.BoxInfo.RecognizedStringSize = NumberOfCharacterInLine;
						DWORD CountBytesWritten;
						WriteFile(hParentStdOut,&LineBoxesInfo,sizeof(LineBoxesInfo),&CountBytesWritten,NULL);
						WriteFile(hParentStdOut,pLineString,NumberOfCharacterInLine+1, &CountBytesWritten,NULL);
						::CoTaskMemFree(pLineString);			// free memory allocated by the GetPropertyData method
						pLineNode->Release();
					}
					UpperPositionPoints.PointsInfo.X_MinValue_Left = temp_X_MinValue_Left;
					UpperPositionPoints.PointsInfo.X_MinValue_Right = temp_X_MinValue_Right;
					UpperPositionPoints.PointsInfo.Y_MinValue_Top = temp_Y_MinValue_Top;
					UpperPositionPoints.PointsInfo.Y_MinValue_Bottom = temp_Y_MinValue_Bottom;
				} else {
					UpperPositionPoints.PointsInfo.X_MinValue_Left = 0;
					UpperPositionPoints.PointsInfo.X_MinValue_Right = 1;
					UpperPositionPoints.PointsInfo.Y_MinValue_Top = 0;
					UpperPositionPoints.PointsInfo.Y_MinValue_Bottom = 1;
				}
				pLineContextNodesfound->Release();					// release Line Nodes
				pBulletContextNodesfound->Release();				// release Bullet Nodes
				pDrawingContextNodesfound->Release();				// release Drawing Nodes
				for (int BoxIndex = 0;BoxIndex < LayoutBoxesCounts.CountInfo.BulletBoxesCount;BoxIndex++) {
					IContextNode* pBulletNode = NULL;
					HRESULT hr = pBulletContextNodesfound->GetContextNode(BoxIndex,&pBulletNode);
					RECT bounds;
					// Get the location's bounds
					IAnalysisRegion* spLocation;
					hr = pBulletNode->GetLocation(&spLocation);
					if( SUCCEEDED(hr) )
					{
						hr = spLocation->GetBounds(&bounds);
					}
					spLocation->Release();
					structBulletBoxesInfo BulletBoxesInfo;
					BulletBoxesInfo.Command = BulletBoxes;
					BulletBoxesInfo.BoxInfo.Left = bounds.left;
					BulletBoxesInfo.BoxInfo.Top = bounds.top;
					BulletBoxesInfo.BoxInfo.width = bounds.right - bounds.left;
					BulletBoxesInfo.BoxInfo.height = bounds.bottom - bounds.top;
//					if (pLineString != NULL) {
//						::CoTaskMemFree(pLineString);			// free previous memory allocated by the GetPropertyData method
//					}
					pBulletNode->Release();
					DWORD CountBytesWritten;
					WriteFile(hParentStdOut,&BulletBoxesInfo,sizeof(BulletBoxesInfo),&CountBytesWritten,NULL);
				}


																		// Now Send the Ink Drawings
				for (int BoxIndex=0; BoxIndex < LayoutBoxesCounts.CountInfo.DrawinfBoxesCount; BoxIndex++) {
					IContextNode* pDrawingNode = NULL;
					HRESULT hr = pDrawingContextNodesfound->GetContextNode(BoxIndex,&pDrawingNode);
					RECT bounds;
					// Get the location's bounds
					IAnalysisRegion* spLocation;
					hr = pDrawingNode->GetLocation(&spLocation);
					if( SUCCEEDED(hr) )
					{
						hr = spLocation->GetBounds(&bounds);
					}
					structDrawingBoxHotspots DrawingBoxHotspots;
					DrawingBoxHotspots.Command = InkDrawingBoxes;
					DrawingBoxHotspots.BoxInfo.Left = bounds.left;
					DrawingBoxHotspots.BoxInfo.Top = bounds.top;
					DrawingBoxHotspots.BoxInfo.width = bounds.right - bounds.left;
					DrawingBoxHotspots.BoxInfo.height = bounds.bottom - bounds.top;
					spLocation->Release();
//					if (pLineString != NULL) {
//						::CoTaskMemFree(pLineString);			// free previous memory allocated by the GetPropertyData method
//					}
					ULONG NumberOfCharacterInLine;
					hr = pDrawingNode->GetPropertyData(&GUID_CNP_SHAPENAME,&NumberOfCharacterInLine,&pLineString);
//					*pText = (char*) pLineString;
					DrawingBoxHotspots.BoxInfo.RecognizedStringSize = NumberOfCharacterInLine;
					pDrawingNode->Release();
					DWORD CountBytesWritten;
					WriteFile(hParentStdOut,&DrawingBoxHotspots,sizeof(DrawingBoxHotspots),&CountBytesWritten,NULL);
					WriteFile(hParentStdOut,pLineString,NumberOfCharacterInLine+1, &CountBytesWritten,NULL);
					::CoTaskMemFree(pLineString);			// free memory allocated by the GetPropertyData method
				}


																		// Now Send Analyse finished
				UpperPositionPoints.command = InkAnalyzeFinished;
				WriteFile(hParentStdOut,&UpperPositionPoints,sizeof(UpperPositionPoints),&CountBytesWritten,NULL);
			}
			break;

#ifdef NoMoreUsed
			case SearchUpperLeftLinePosition:
			{
				IContextNodes *pContextNodesfound = NULL;
 
				HRESULT hr = m_spIInkAnalyzer->FindNodesOfType(&GUID_CNT_LINE, &pContextNodesfound);
				ULONG LineNodeCount;
				structUpperPositionPoints UpperPositionPoints;
				long temp_X_MinValue_Left = LONG_MAX;
				long temp_X_MinValue_Right = LONG_MAX;
				long temp_Y_MinValue_Top = LONG_MAX;
				long temp_Y_MinValue_Bottom = LONG_MAX;

				hr = pContextNodesfound->GetCount(&LineNodeCount);

				if (LineNodeCount != 0) {
					for (int i = 0;i<LineNodeCount;i++) {
						IContextNode* pLineNode = NULL;// release???
						hr = pContextNodesfound->GetContextNode(i,&pLineNode);

						// Use the location's bounds
						RECT bounds;
						// Use the location's bounds
						IAnalysisRegion* spLocation // release???
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
					}
					UpperPositionPoints.PointsInfo.X_MinValue_Left = temp_X_MinValue_Left;
					UpperPositionPoints.PointsInfo.X_MinValue_Right = temp_X_MinValue_Right;
					UpperPositionPoints.PointsInfo.Y_MinValue_Top = temp_Y_MinValue_Top;
					UpperPositionPoints.PointsInfo.Y_MinValue_Bottom = temp_Y_MinValue_Bottom;
				} else {
					UpperPositionPoints.PointsInfo.X_MinValue_Left = 0;
					UpperPositionPoints.PointsInfo.X_MinValue_Right = 1;
					UpperPositionPoints.PointsInfo.Y_MinValue_Top = 0;
					UpperPositionPoints.PointsInfo.Y_MinValue_Bottom = 1;
				}
				DWORD CountBytesWritten;
				WriteFile(hParentStdOut,&UpperPositionPoints,sizeof(UpperPositionPoints),&CountBytesWritten,NULL);
			}
			break;
#endif

#ifdef NoMoreUsed
			case GetLineBoxInfo:
			{
				int BoxIndex;
				DWORD BytesRed;
				ReadFile(hParentStdIn,&BoxIndex,sizeof(int),&BytesRed,NULL);
				if (BytesRed < sizeof(int)) break;				// End of file received 
				IContextNode* pLineNode = NULL;// release???
				HRESULT hr = pLineContextNodesfound->GetContextNode(BoxIndex,&pLineNode);
				RECT bounds;
				// Get the location's bounds
				IAnalysisRegion* spLocation;// release???
				hr = pLineNode->GetLocation(&spLocation);
				if( SUCCEEDED(hr) )
				{
					hr = spLocation->GetBounds(&bounds);
				}
				structLineBoxesInfo LineBoxesInfo;
				LineBoxesInfo.Command = LineBoxes;
				LineBoxesInfo.BoxInfo.Left = bounds.left;
				LineBoxesInfo.BoxInfo.Top = bounds.top;
				LineBoxesInfo.BoxInfo.width = bounds.right - bounds.left;
				LineBoxesInfo.BoxInfo.height = bounds.bottom - bounds.top;
//				if (pLineString != NULL) {
//					::CoTaskMemFree(pLineString);			// free previous memory allocated by the GetPropertyData method
//				}
				//pLineNode->GetId(
				ULONG NumberOfCharacterInLine;
				hr = pLineNode->GetPropertyData(&GUID_CNP_RECOGNIZEDSTRING,&NumberOfCharacterInLine,&pLineString);
//				*pText = (char*) pLineString;
				LineBoxesInfo.BoxInfo.RecognizedStringSize = NumberOfCharacterInLine;
				DWORD CountBytesWritten;
				WriteFile(hParentStdOut,&LineBoxesInfo,sizeof(LineBoxesInfo),&CountBytesWritten,NULL);
				WriteFile(hParentStdOut,pLineString,NumberOfCharacterInLine+1, &CountBytesWritten,NULL);
				::CoTaskMemFree(pLineString);			// free memory allocated by the GetPropertyData method
			}
			break;
#endif

#ifdef NoMoreUsed
			case GetBulletPosition:
			{
				int BoxIndex;
				DWORD BytesRed;
				ReadFile(hParentStdIn,&BoxIndex,sizeof(int),&BytesRed,NULL);
				if (BytesRed < sizeof(int)) break;				// End of file received 
				IContextNode* pBulletNode = NULL;// release???
				HRESULT hr = pBulletContextNodesfound->GetContextNode(BoxIndex,&pBulletNode);
				RECT bounds;
				// Get the location's bounds
				IAnalysisRegion* spLocation;// release???
				hr = pBulletNode->GetLocation(&spLocation);
				if( SUCCEEDED(hr) )
				{
					hr = spLocation->GetBounds(&bounds);
				}
				structBulletBoxesInfo BulletBoxesInfo;
				BulletBoxesInfo.Command = BulletBoxes;
				BulletBoxesInfo.BoxInfo.Left = bounds.left;
				BulletBoxesInfo.BoxInfo.Top = bounds.top;
				BulletBoxesInfo.BoxInfo.width = bounds.right - bounds.left;
				BulletBoxesInfo.BoxInfo.height = bounds.bottom - bounds.top;
//				if (pLineString != NULL) {
//					::CoTaskMemFree(pLineString);			// free previous memory allocated by the GetPropertyData method
//				}
				DWORD CountBytesWritten;
				WriteFile(hParentStdOut,&BulletBoxesInfo,sizeof(BulletBoxesInfo),&CountBytesWritten,NULL);
			}
			break;
#endif

#ifdef NoMoreUsed
			case GetDrawingHotspots:
			{
				int BoxIndex;
				DWORD BytesRed;
				ReadFile(hParentStdIn,&BoxIndex,sizeof(int),&BytesRed,NULL);
				if (BytesRed < sizeof(int)) break;				// End of file received 
				IContextNode* pDrawingNode = NULL;// release???
				HRESULT hr = pDrawingContextNodesfound->GetContextNode(BoxIndex,&pDrawingNode);
				RECT bounds;
				// Get the location's bounds
				IAnalysisRegion* spLocation;// release???
				hr = pDrawingNode->GetLocation(&spLocation);
				if( SUCCEEDED(hr) )
				{
					hr = spLocation->GetBounds(&bounds);
				}
				structDrawingBoxHotspots DrawingBoxHotspots;
				DrawingBoxHotspots.Command = InkDrawingBoxes;
				DrawingBoxHotspots.BoxInfo.Left = bounds.left;
				DrawingBoxHotspots.BoxInfo.Top = bounds.top;
				DrawingBoxHotspots.BoxInfo.width = bounds.right - bounds.left;
				DrawingBoxHotspots.BoxInfo.height = bounds.bottom - bounds.top;
//				if (pLineString != NULL) {
//					::CoTaskMemFree(pLineString);			// free previous memory allocated by the GetPropertyData method
//				}
				//pLineNode->GetId(
				ULONG NumberOfCharacterInLine;
				hr = pDrawingNode->GetPropertyData(&GUID_CNP_SHAPENAME,&NumberOfCharacterInLine,&pLineString);
//				*pText = (char*) pLineString;
				DrawingBoxHotspots.BoxInfo.RecognizedStringSize = NumberOfCharacterInLine;
				DWORD CountBytesWritten;
				WriteFile(hParentStdOut,&DrawingBoxHotspots,sizeof(DrawingBoxHotspots.BoxInfo),&CountBytesWritten,NULL);
				WriteFile(hParentStdOut,pLineString,NumberOfCharacterInLine+1, &CountBytesWritten,NULL);
				::CoTaskMemFree(pLineString);			// free memory allocated by the GetPropertyData method
			}
			break;
#endif


			case ClearAllAnalyzerStrokes:
			{
				m_spIInkAnalyzer->Release();
				hr = CoCreateInstance(CLSID_InkAnalyzer,//CLSID_InkRecognizerContext, 
                                  NULL, CLSCTX_INPROC_SERVER,
                                  IID_IInkAnalyzer,//IID_IInkRecognizerContext, 
                                  (void **) &m_spIInkAnalyzer);
				if (FAILED(hr)) 
					int kkkk = 0;//        return -1;		// todo  Insert Error Handling
			}
			break;

		};
	}
	m_spIInkAnalyzer->Release();

	return 0;
}

