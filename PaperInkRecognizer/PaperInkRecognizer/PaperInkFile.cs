using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;


namespace PaperInkRecognizer
{
    public class PaperInkFile
    {
#if DebugDLL
#if win32
        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern IntPtr InitPaperInkReader([MarshalAs(UnmanagedType.LPWStr)]string FileName, int RelativThreshold, int AbsoluteThreshold, int SlidingThreshold);

        [DllImport("..\\..\\..\\GeneralInkRecognizer\\Win32\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool EOFfound(IntPtr pFile);

#else
        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern IntPtr InitPaperInkReader([MarshalAs(UnmanagedType.LPWStr)]string FileName, int RelativThreshold, int AbsoluteThreshold, int SlidingThreshold);

        [DllImport("..\\..\\..\\..\\GeneralInkRecognizer\\x64\\Debug\\GeneralInkRecognizer.dll")]
        public static extern bool EOFfound(IntPtr pFile);
#endif
#else
        [DllImport("GeneralInkRecognizer.dll")]
        public static extern IntPtr InitPaperInkReader([MarshalAs(UnmanagedType.LPWStr)]string FileName, int RelativThreshold, int AbsoluteThreshold, int SlidingThreshold);

        [DllImport("GeneralInkRecognizer.dll")]
        public static extern bool EOFfound(IntPtr pFile);
#endif
        
        public IntPtr cFileClass;
        public PaperInkFile(string FileName, int RelativThreshold, int AbsoluteThreshold, int SlidingThreshold)
        {
            cFileClass = InitPaperInkReader(FileName, RelativThreshold, AbsoluteThreshold, SlidingThreshold);
        }
        public bool EOFfound()
        {
            return EOFfound(cFileClass);
        }
    }
}
