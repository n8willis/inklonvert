// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PAPERINKRECOGNIZER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PAPERINKANALYZER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PAPERINKANALYZER_EXPORTS
#define PAPERINKANALYZER_API extern "C" __declspec(dllexport)
#else
#define PAPERINKANALYZER_API extern "C" __declspec(dllimport)
#endif

#define MANAGED_TRUE 1
#define MANAGED_FALSE 0


typedef int (CALLBACK* InkAnalyzerResultAvailable) ();
typedef int (CALLBACK* InkInkDrawingInfo) (int Left, int Top, int width, int height, char* pText);
typedef int (CALLBACK* InkLineBoxInfo) (int Left, int Top, int width, int height, char* pText);
typedef int (CALLBACK* InkBulletInfo) (int Left, int Top, int width, int height);
typedef int (CALLBACK* AnalyserHasFinished) (int Left, int Top, int width, int height);
typedef int (CALLBACK* MathPanelReply) ();

#define RECOGNIZER_FULLDLLNAME	TEXT("GeneralInkRecognizer.dll")

typedef struct AnalyserStrokeInfo {
	int Command;
	long id;
	ULONG packetDataCount;
	ULONG guidCount;
} ;

class CMyInkEvents : public InkCollectorEvents
{
public:

    // Event: Stroke
    virtual void Stroke(
        IInkCursor* Cursor,
        IInkStrokeDisp* Stroke,
        VARIANT_BOOL *Cancel);
    
    CMyInkEvents();

    HRESULT Init(
        HWND hWnd);   
    HWND m_hWnd;
};


#ifdef ExternAnalyzer
LPTHREAD_START_ROUTINE InkAnalyzerReceiver(LPVOID lpv);

class CMyInkAnalyserEvents
{
private:
	HANDLE hReadChild;
private:
// Pointer to callback Functions
			InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction;
            InkInkDrawingInfo InkInkDrawingInfoFunction;
            InkLineBoxInfo InkLineBoxInfoFunction;
            InkBulletInfo InkBulletInfoFunction;
            AnalyserHasFinished AnalyserHasFinishedFunction;
            MathPanelReply MathPanelReplyFunction;
// Variables for internal use
			ULONG LineBoxesCount;
			ULONG BulletBoxesCount;
			ULONG DrawinfBoxesCount;

public:
	void Init(HANDLE hReadChild_In)
	{
		hReadChild = hReadChild_In;
		HANDLE AddressChangeThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InkAnalyzerReceiver, (LPVOID)this, 0, NULL);
	};

	void SetCallbacks(
			InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction_In,
            InkInkDrawingInfo InkInkDrawingInfoFunction_In,
            InkLineBoxInfo InkLineBoxInfoFunction_In,
            InkBulletInfo InkBulletInfoFunction_In,
            AnalyserHasFinished AnalyserHasFinishedFunction_In
            //TODO Test Only  MathPanelReply MathPanelReplyFunction_In
		)
	{
			InkAnalyzerResultAvailableFunction = InkAnalyzerResultAvailableFunction_In;
            InkInkDrawingInfoFunction = InkInkDrawingInfoFunction_In;
            InkLineBoxInfoFunction = InkLineBoxInfoFunction_In;
            InkBulletInfoFunction = InkBulletInfoFunction_In;
            AnalyserHasFinishedFunction = AnalyserHasFinishedFunction_In;
            // TODO Test Only  MathPanelReply MathPanelReplyFunction,
		return;
	}

	void ReceiveInkAnalyzerResult() ;

};

LPTHREAD_START_ROUTINE InkAnalyzerReceiver(LPVOID lpv) 
{
	CMyInkAnalyserEvents* pCInkAnalyzerResults = (CMyInkAnalyserEvents*) lpv;
	pCInkAnalyzerResults->ReceiveInkAnalyzerResult();
	return (0);
}
#else
class CMyInkAnalyserEvents : public InkAnalyserEvents
{
public:

virtual HRESULT STDMETHODCALLTYPE ResultsUpdated(
    IInkAnalyzer* pInkAnalyzer,
    IAnalysisStatus* pAnalysisStatus); 

    CMyInkAnalyserEvents();
    
    HRESULT Init( HWND hWnd);

private:
// Pointer to callback Functions
			InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction;
            InkInkDrawingInfo InkInkDrawingInfoFunction;
            InkLineBoxInfo InkLineBoxInfoFunction;
            InkBulletInfo InkBulletInfoFunction;
            AnalyserHasFinished AnalyserHasFinishedFunction;
// TODO            MathPanelReply MathPanelReplyFunction;

public:
	void SetCallbacks(
			InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction_In,
            InkInkDrawingInfo InkInkDrawingInfoFunction_In,
            InkLineBoxInfo InkLineBoxInfoFunction_In,
            InkBulletInfo InkBulletInfoFunction_In,
            AnalyserHasFinished AnalyserHasFinishedFunction_In
// TODO  MathPanelReply MathPanelReplyFunction_In
		);
   
    HWND m_hWnd;
};
#endif

class PaperInkAnalyzer {
private:
    // Automation API interface pointers
	IInkCollector *     g_pIInkCollector;
	IInkDisp *          g_pIInkDisp;
	IInkRenderer*		g_pInkRenderer;	

#ifdef ExternAnalyzer
#else
	IInkAnalyzer*           m_spIInkAnalyzer;
#endif

// Description of the stroke events
// http://msdn.microsoft.com/en-us/library/aa510904.aspx
/////////////////////////////////////////////////////////
class CMyMathPanelEvents : public MathPanelEvents
{
public:

    CMyMathPanelEvents()
    {
        m_hWnd = NULL;
    }
    
    HRESULT Init(
        HWND hWnd)
    {
        m_hWnd = hWnd;
        return MathPanelEvents::Init();
    }    
    
    HWND m_hWnd;
};

#ifdef StrokeEvents
    CMyInkEvents m_InkEvents;// TODO for future use 
#endif
    CMyInkAnalyserEvents m_InkAnalyserEvents;


#ifdef MathInput
    IMathInputControl* g_spMIC; // TODO Math Input Control
	bool MathInputPanelOpen;
	CMyMathPanelEvents m_MathInputEvents;
	HWND g_pMainWindowHandle;
#endif

	int PreviousVerticalShiftPos;
	float PreviousVerticalShiftPosFloat;
	int PreviousHorizontalShiftPos;
	float PreviousHorizontalShiftPosFloat;

#ifdef ExternAnalyzer
	HANDLE hWriteChild;
	HANDLE hReadChild;
	HANDLE ChildOut;
	HANDLE ChildIn;


structLayoutBoxesCounts LayoutBoxesCounts;
#endif

void AddStrokeToAnalyzer( IInkStrokeDisp* pIInkStroke);

public:
	PaperInkAnalyzer(
			InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction,
            InkInkDrawingInfo InkInkDrawingInfoFunction,
            InkLineBoxInfo InkLineBoxInfoFunction,
            InkBulletInfo InkBulletInfoFunction,
            AnalyserHasFinished AnalyserHasFinishedFunction,
            MathPanelReply MathPanelReplyFunction,
			HWND DrawingHandle);

	~PaperInkAnalyzer();
	int DeleteAllStrokes();

	int SendAllStrokesToMathInputPanel();

	int AnalyzeInk();
	int AddStrokesFromFile(PaperInkConverter* pPaperInkConverter,long StrokeColor);

	int PositioningInkPanel(int Basic_X_Offset, int Basic_Y_Offset, HWND HorizontalScrollHandle, HWND VerticalScrollHandle);

	int CopyToClipboard();

#ifdef NoCallback
	int FindUpperLeftLinePosition(int* Min_X_Position_Left, int*  Min_X_Position_Right, int* Min_Y_Position_TOP, int* Min_Y_PositionBottom);

	int GetInkBullets();
	int GetNextInkBulletsInfo(int BoxIndex, int* Left, int* Top, int* width, int* height );

	int GetInkLineBoxes();
	int GetNextInkLineBoxInfo(int BoxIndex, int* Left, int* Top, int* width, int* height, char** pText );		// Get Next Ink Line Information

	int GetInkDrawingBoxes();
//	int GetNextInkBulletsInfo(int BoxIndex, int* Left, int* Top, int* width, int* height );

	int GetNextInkInkDrawingHotSpots(int BoxIndex, int* Left, int* Top, int* width, int* height, char** pText );
#endif

};

// Managed Wrapper for Paper Ink Analyzer
PAPERINKANALYZER_API PaperInkAnalyzer* NewPaperInkAnalyzer(
								InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction,
                                InkInkDrawingInfo InkInkDrawingInfoFunction,
                                InkLineBoxInfo InkLineBoxInfoFunction,
                                InkBulletInfo InkBulletInfoFunction,
                                AnalyserHasFinished AnalyserHasFinishedFunction,
                                MathPanelReply MathPanelReplyFunction,
								HWND DrawingHandle);
PAPERINKANALYZER_API int DeletePaperInkAnalyzer(PaperInkAnalyzer* pPaperInkAnalyzer);// TODO Check this if we realy need it

PAPERINKANALYZER_API int SendAllStrokesToMathInputPanel(PaperInkAnalyzer* pPaperInkAnalyzer);

PAPERINKANALYZER_API int AnalyzeInk(PaperInkAnalyzer* pPaperInkAnalyzer);
PAPERINKANALYZER_API int AddStrokesFromFile(PaperInkAnalyzer* pPaperInkAnalyzer, PaperInkConverter* pPaperInkConverter,long StrokeColor);

PAPERINKANALYZER_API int DeleteAllStrokes(PaperInkAnalyzer* pPaperInkAnalyzer);
PAPERINKANALYZER_API int PositioningInkPanel(PaperInkAnalyzer* pPaperInkAnalyzer, int Basic_X_Offset, int Basic_Y_Offset, HWND HorizontalScrollHandle, HWND VerticalScrollHandle);

PAPERINKANALYZER_API int CopyToClipboard(PaperInkAnalyzer* aPaperInkAnalyzer);

#ifdef NoCallback
PAPERINKANALYZER_API int WrapFindUpperLeftLinePosition(PaperInkAnalyzer* pPaperInkAnalyzer, int* Min_X_Position_Left, int*  Min_X_Position_Right, int* Min_Y_Position_TOP, int* Min_Y_PositionBottom);
PAPERINKANALYZER_API int GetInkBullets(PaperInkAnalyzer* pPaperInkAnalyzer);				// Read Analysed Bullets and Return the Count
PAPERINKANALYZER_API int GetNextInkBulletsInfo(PaperInkAnalyzer* pPaperInkAnalyzer, int BoxIndex, int* Left, int* Top, int* width, int* height );
PAPERINKANALYZER_API int GetInkLineBoxes(PaperInkAnalyzer* pPaperInkAnalyzer);			// Read Analysed Line Boxes and Return the Count
PAPERINKANALYZER_API int GetNextInkLineBoxInfo(PaperInkAnalyzer* pPaperInkAnalyzer, int BoxIndex, int* Left, int* Top, int* width, int* height, char** pText );		// Get Next Ink Line Information

PAPERINKANALYZER_API int GetInkDrawingBoxes(PaperInkAnalyzer* pPaperInkAnalyzer);				// Read Analysed Bullets and Return the Count
PAPERINKANALYZER_API int GetNextInkInkDrawingHotSpots(PaperInkAnalyzer* pPaperInkAnalyzer, int BoxIndex, int* Left, int* Top, int* width, int* height, char** pText );
#endif

// Managed Wrapper for Paper Ink Converter Class

PAPERINKANALYZER_API PaperInkConverter* InitPaperInkReader(wchar_t* FileName, int RelativThreshold, int AbsoluteThreshold, int SlidingThreshold);
PAPERINKANALYZER_API int EOFfound(PaperInkConverter* pPaperInkConverter);

