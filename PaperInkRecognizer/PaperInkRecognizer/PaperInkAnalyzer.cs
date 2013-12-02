using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Drawing;

namespace PaperInkRecognizer
{
    class PaperInkAnalyzer
    {
#if DebugDLL
#if win32

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern IntPtr NewPaperInkAnalyzer(InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction,
                                InkInkDrawingInfo InkInkDrawingInfoFunction,
                                InkLineBoxInfo InkLineBoxInfoFunction,
                                InkBulletInfo InkBulletInfoFunction,
                                AnalyserHasFinished AnalyserHasFinishedFunction,
                                MathPanelReply MathPanelReplyFunction,
                                IntPtr DrawingWindowHandle);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool AddStrokesFromFile(IntPtr pInkRecognizerClass, IntPtr usedFile, int StrokeColor);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool DeleteAllStrokes(IntPtr pInkRecognizerClass);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool AnalyzeInk(IntPtr pInkRecognizerClass);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool PositioningInkPanel(IntPtr pInkRecognizerClass, int Basic_X_Offset, int Basic_Y_Offset, IntPtr HorizontalScrollHandle, IntPtr VerticalScrollHandle);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool CopyToClipboard(IntPtr pInkRecognizerClass);

#if NoCallback
        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool WrapFindUpperLeftLinePosition(IntPtr pInkRecognizerClass, out int Min_X_Position_Left, out  int Min_X_Position_Right, out  int Min_Y_Position_TOP, out int Min_Y_PositionBottom);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern int GetInkLineBoxes(IntPtr pInkRecognizerClass);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool GetNextInkLineBoxInfo(IntPtr pInkRecognizerClass, int BoxIndex, out int Left, out int Top, out int width, out int height, out IntPtr pText);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern int GetInkBullets(IntPtr pInkRecognizerClass);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool GetNextInkBulletsInfo(IntPtr pInkRecognizerClass, int BoxIndex, out int Left, out int Top, out int width, out int height);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern int GetInkDrawingBoxes(IntPtr pInkRecognizerClass);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool GetNextInkInkDrawingHotSpots(IntPtr pInkRecognizerClass, int BoxIndex, out int Left, out int Top, out int width, out int height, out IntPtr pText);

//        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
//        public static extern bool GetNextInkLineBoxInfo(IntPtr pInkRecognizerClass, int BoxIndex, out int Left, out int Top, out int width, out int height, out IntPtr pText);
#endif

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern int DeletePaperInkAnalyzer(IntPtr pInkRecognizerClass);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool SendAllStrokesToMathInputPanel(IntPtr pInkRecognizerClass);


#else

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern IntPtr NewPaperInkAnalyzer(InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction,
                                InkInkDrawingInfo InkInkDrawingInfoFunction,
                                InkLineBoxInfo InkLineBoxInfoFunction,
                                InkBulletInfo InkBulletInfoFunction,
                                AnalyserHasFinished AnalyserHasFinishedFunction,
                                MathPanelReply MathPanelReplyFunction,
                                IntPtr DrawingWindowHandle);

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool AddStrokesFromFile(IntPtr pInkRecognizerClass, IntPtr usedFile, int StrokeColor);

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool DeleteAllStrokes(IntPtr pInkRecognizerClass);

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool AnalyzeInk(IntPtr pInkRecognizerClass);

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool PositioningInkPanel(IntPtr pInkRecognizerClass, int Basic_X_Offset, int Basic_Y_Offset, IntPtr HorizontalScrollHandle, IntPtr VerticalScrollHandle);

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool CopyToClipboard(IntPtr pInkRecognizerClass);
#if NoCallback
        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool WrapFindUpperLeftLinePosition(IntPtr pInkRecognizerClass, out int Min_X_Position_Left, out  int Min_X_Position_Right, out  int Min_Y_Position_TOP, out int Min_Y_PositionBottom);

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern int GetInkLineBoxes(IntPtr pInkRecognizerClass);

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool GetNextInkLineBoxInfo(IntPtr pInkRecognizerClass, int BoxIndex, out int Left, out int Top, out int width, out int height, out IntPtr pText);

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern int GetInkBullets(IntPtr pInkRecognizerClass);

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool GetNextInkBulletsInfo(IntPtr pInkRecognizerClass, int BoxIndex, out int Left, out int Top, out int width, out int height);

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern int GetInkDrawingBoxes(IntPtr pInkRecognizerClass);

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool GetNextInkInkDrawingHotSpots(IntPtr pInkRecognizerClass, int BoxIndex, out int Left, out int Top, out int width, out int height, out IntPtr pText);

        //        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        //        public static extern bool GetNextInkLineBoxInfo(IntPtr pInkRecognizerClass, int BoxIndex, out int Left, out int Top, out int width, out int height, out IntPtr pText);


#endif
        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern int DeletePaperInkAnalyzer(IntPtr pInkRecognizerClass);


        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool SendAllStrokesToMathInputPanel(IntPtr pInkRecognizerClass);
#endif
#else
        [DllImport("GeneralInkRecognizer.dll")]
        public static extern IntPtr NewPaperInkAnalyzer(InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction,
                                InkInkDrawingInfo InkInkDrawingInfoFunction,
                                InkLineBoxInfo InkLineBoxInfoFunction,
                                InkBulletInfo InkBulletInfoFunction,
                                AnalyserHasFinished AnalyserHasFinishedFunction,
                                MathPanelReply MathPanelReplyFunction,
                                IntPtr DrawingWindowHandle);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern bool AddStrokesFromFile(IntPtr pInkRecognizerClass, IntPtr usedFile, int StrokeColor);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern bool DeleteAllStrokes(IntPtr pInkRecognizerClass);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern bool AnalyzeInk(IntPtr pInkRecognizerClass);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern bool PositioningInkPanel(IntPtr pInkRecognizerClass, int Basic_X_Offset, int Basic_Y_Offset, IntPtr HorizontalScrollHandle, IntPtr VerticalScrollHandle);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern bool CopyToClipboard(IntPtr pInkRecognizerClass);
#if NoCallback
        [DllImport("GeneralInkRecognizer.dll")]
        public static extern bool WrapFindUpperLeftLinePosition(IntPtr pInkRecognizerClass, out int Min_X_Position_Left, out  int Min_X_Position_Right, out  int Min_Y_Position_TOP, out int Min_Y_PositionBottom);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern int GetInkLineBoxes(IntPtr pInkRecognizerClass);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern bool GetNextInkLineBoxInfo(IntPtr pInkRecognizerClass, int BoxIndex, out int Left, out int Top, out int width, out int height, out IntPtr pText);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern int GetInkBullets(IntPtr pInkRecognizerClass);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern bool GetNextInkBulletsInfo(IntPtr pInkRecognizerClass, int BoxIndex, out int Left, out int Top, out int width, out int height);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern int GetInkDrawingBoxes(IntPtr pInkRecognizerClass);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern bool GetNextInkInkDrawingHotSpots(IntPtr pInkRecognizerClass, int BoxIndex, out int Left, out int Top, out int width, out int height, out IntPtr pText);

        //        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        //        public static extern bool GetNextInkLineBoxInfo(IntPtr pInkRecognizerClass, int BoxIndex, out int Left, out int Top, out int width, out int height, out IntPtr pText);

#endif

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern int DeletePaperInkAnalyzer(IntPtr pInkRecognizerClass);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern bool SendAllStrokesToMathInputPanel(IntPtr pInkRecognizerClass);
#endif

        public IntPtr CInkAnalyzerClass;

        public PaperInkAnalyzer(InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunction,
                                InkInkDrawingInfo InkInkDrawingInfoFunction,
                                InkLineBoxInfo InkLineBoxInfoFunction,
                                InkBulletInfo InkBulletInfoFunction,
                                AnalyserHasFinished AnalyserHasFinishedFunction,
                                MathPanelReply MathPanelReplyFunction,
                                IntPtr DrawingWindowHandle)
        {
            CInkAnalyzerClass = NewPaperInkAnalyzer(InkAnalyzerResultAvailableFunction,
                                InkInkDrawingInfoFunction,
                                InkLineBoxInfoFunction,
                                InkBulletInfoFunction,
                                AnalyserHasFinishedFunction,
                                MathPanelReplyFunction,
                                DrawingWindowHandle);
        }

        ~PaperInkAnalyzer()
        {
            DeletePaperInkAnalyzer(CInkAnalyzerClass);
        }

        public bool AnalyzeInk()
        {
            return AnalyzeInk(CInkAnalyzerClass);
        }

        public bool TransferStrokesToMathInputPanel()
        {
            return SendAllStrokesToMathInputPanel(CInkAnalyzerClass);
        }

        public bool AddStrokesFromFile(PaperInkFile usedFile,Color StrokeColor)
        {
            int xyz = StrokeColor.ToArgb();
            return AddStrokesFromFile(CInkAnalyzerClass, usedFile.cFileClass, StrokeColor.ToArgb());
        }

        public bool DeleteAllStrokes()
        {
            return (DeleteAllStrokes(CInkAnalyzerClass));
        }

        public bool PositioningInkPanel(int Basic_X_Offset, int Basic_Y_Offset, IntPtr HorizontalScrollHandle, IntPtr VerticalScrollHandle)
        {
            return (PositioningInkPanel(CInkAnalyzerClass, Basic_X_Offset, Basic_Y_Offset, HorizontalScrollHandle, VerticalScrollHandle));
        }

        public bool CopyToClipboard()
        {
            return (CopyToClipboard(CInkAnalyzerClass));
        }

#if NoCallback
        public bool FindUpperLeftLinePosition(out int Min_X_Position_Left, out  int Min_X_Position_Right, out  int Min_Y_Position_TOP, out int Min_Y_PositionBottom)
        {
            return (WrapFindUpperLeftLinePosition(CInkAnalyzerClass, out Min_X_Position_Left, out Min_X_Position_Right, out Min_Y_Position_TOP, out Min_Y_PositionBottom));
        }

        public int GetInkLineBoxes()
        {
            return (GetInkLineBoxes(CInkAnalyzerClass));
        }

        public bool GetNextInkLineBoxInfo(int BoxIndex, out int Left, out int Top, out int width, out int height, out IntPtr pText)
        {
           return (GetNextInkLineBoxInfo(CInkAnalyzerClass, BoxIndex, out Left, out Top, out width, out height, out pText));
        }

        public int GetInkBullets()
        {
            return (GetInkBullets(CInkAnalyzerClass));
        }

        public  bool GetNextInkBulletsInfo(int BoxIndex, out int Left, out int Top, out int width, out int height)
        {
            return (GetNextInkBulletsInfo(CInkAnalyzerClass, BoxIndex, out Left, out Top, out width, out height));
        }


        public int GetInkDrawingBoxes()
        {
            return (GetInkDrawingBoxes(CInkAnalyzerClass));
        }

        public bool GetNextInkInkDrawingHotSpots(int BoxIndex, out int Left, out int Top, out int width, out int height, out IntPtr pText)
        {
           return (GetNextInkInkDrawingHotSpots(CInkAnalyzerClass, BoxIndex, out Left, out Top, out width, out height, out pText));
        }

//        public bool GetNextInkLineBoxInfo(int BoxIndex, out int Left, out int Top, out int width, out int height, out IntPtr pText)
//        {
//            return (GetNextInkLineBoxInfo(CInkAnalyzerClass, BoxIndex, out Left, out Top, out width, out height, out pText));
//        }
#endif

    }
}
