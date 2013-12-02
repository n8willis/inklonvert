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
 *   PaperInkRecognizer is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 *   GNU General Public License for more details.                                 *
 *                                                                                *
 *   You should have received a copy of the GNU General Public License            *
 *   along with PaperInkRecognizer.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                                *
 *********************************************************************************/
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
namespace PaperInkRecognizer
{
   public delegate int InkAnalyzerResultAvailable();
   public delegate bool InkInkDrawingInfo(int Left, int Top, int width, int height, IntPtr pText);
   public delegate bool InkLineBoxInfo(int Left, int Top, int width, int height, IntPtr pText);
   public delegate bool InkBulletInfo(int Left, int Top, int width, int height);
   public delegate bool AnalyserHasFinished(int Left, int Top, int width, int height);
   public delegate bool MathPanelReply();

    public class PaperInkAnalysedPage
    {
        protected delegate void SetBoxTextCallback();

        // used to prevent that the delegates are removed by the Garbage Collector
        private InkAnalyzerResultAvailable InkAnalyzerResultAvailableFunctionPara;
        private InkInkDrawingInfo InkInkDrawingInfoPara;
        private InkLineBoxInfo InkLineBoxInfoPara;
        private InkBulletInfo InkBulletInfoPara;
        private AnalyserHasFinished AnalyserHasFinishedPara;
        private MathPanelReply MathPanelReplyPara;


        int Stroke_Y_Offset;                        // Offset of the first Stroke Line
        LineLayout[] AnalysedPage;


        RichTextBox richRecognizedTextBox;
        PaperInkAnalyzer aInkAnalyzerPage;


        public void PageToMathInputPanel()
        {
            aInkAnalyzerPage.TransferStrokesToMathInputPanel();
        }

        private void DisplayAnalysedText()
        {
            richRecognizedTextBox.Clear();

            String TextToWrite = "";
            richRecognizedTextBox.BulletIndent = 30;
            bool BulletOutputActive = false;
            int LineIndex = 0;
            foreach (LineLayout actualLine in AnalysedPage)
            {
                if (actualLine.Status == LineStatus.Empty)
                {
                    // richTextBox1.AppendText("\n");
                    if (LineIndex > 1)                  // dont add newlines to the start of the first 2 Lines
                    {
                        if (BulletOutputActive)
                        {
                            TextToWrite = TextToWrite + "\u2028";
                        }
                        else
                        {
                            TextToWrite = TextToWrite + "\n";
                        }
                    }
                }
                else
                {
                    if (actualLine.Status == LineStatus.BulletStart)
                    {
                        if (LineIndex > 1)                  // dont add newlines to the start of the first 2 Lines
                        {
                            TextToWrite = TextToWrite + "\n";
                        }
                        if (BulletOutputActive)
                        {
//                            TextToWrite = TextToWrite + "\n";
                        }
                        else
                        {
//                            TextToWrite = TextToWrite + "\n";
                            richRecognizedTextBox.SelectedText = TextToWrite;
                            TextToWrite = "";
                            BulletOutputActive = true;
                            richRecognizedTextBox.SelectionBullet = true;
                            richRecognizedTextBox.SelectionIndent = 30;
                        }
                    }
                    else
                    {
                        if (actualLine.Status == LineStatus.Normal)
                        {
                            if (LineIndex > 1)                  // dont add newlines to the start of the first 2 Lines
                            {
                                TextToWrite = TextToWrite + "\n";
                            }
                            if (BulletOutputActive)
                            {
                                //                               TextToWrite = TextToWrite + "\n";
                                richRecognizedTextBox.SelectedText = TextToWrite;
                                TextToWrite = "";
                                richRecognizedTextBox.SelectionBullet = false;
                                richRecognizedTextBox.SelectionIndent = 0;
                                BulletOutputActive = false;
                            }
                            else
                            {
                                //                                TextToWrite = TextToWrite + "\n";
                            }
                        }
                        else
                        {
                            if (LineIndex > 1)                  // dont add newlines to the start of the first 2 Lines
                            {
                                if (actualLine.Status == LineStatus.BulletContinue)
                                {
                                    TextToWrite = TextToWrite + "\u2028";
                                }
                                else
                                {
                                    TextToWrite = TextToWrite + "\n";
                                }
                            }
                        }
                    }

                    //TextLayout wir = (TextLayout)actualLine.LineContents[0];
                    ArrayList LineStrings = actualLine.LineContents;
                    bool first = true;
                    foreach (TextLayout RecognizedString in LineStrings)
                    {
                        if (!first)
                        {
                            // richTextBox1.AppendText(" ");
                            TextToWrite = TextToWrite + (" ");
                        }
                        first = false;
                        String actualText = RecognizedString.LineString;
                        // richTextBox1.AppendText(actualText);
                        TextToWrite = TextToWrite + actualText;
                    }
                }
                LineIndex = LineIndex + 1;
            }
            richRecognizedTextBox.SelectedText = TextToWrite;
            richRecognizedTextBox.Select(1, 0);

        }
        public class LineSorter : IComparer
        {
            int IComparer.Compare(Object x, Object y)
            {
                return (((TextLayout)x).TextStart.X - ((TextLayout)y).TextStart.X);
            }
        }

        public int InkAnalyzerResultAvailable( )
        {
             ClearLayout();
            Stroke_Y_Offset = 0;

            return (0);
        }

        public bool InkInkDrawingInfo(int Left, int Top, int width, int height, IntPtr pText)
        {                           // for future use, not implemented at the moment
            return false;
        }

        public bool InkLineBoxInfo(int Left, int Top, int width, int height, IntPtr pText)
        {
            // long NumberOfInkLineBoxes = aLayer.GetInkLineBoxes();        // Counts Number of Lines (Line Boxes)
            string Text = Marshal.PtrToStringUni(pText);

                                // Rectangle ActualLineDimension = new Rectangle(Left, Bottom, width, height);
            int FoundLineNumber = ((Top + height) - Stroke_Y_Offset) / 450;   // Top is the upper position of the Line Top Position is less than Bottom Position

            TextLayout actualText = new TextLayout();
            actualText.TextStart.X = Left;              // TODO Change Point in Rectangle
            actualText.TextStart.Y = Top + height;
            actualText.LineString = Text;

            AnalysedPage[FoundLineNumber].LineContents.Add(actualText);
            AnalysedPage[FoundLineNumber].Status = LineStatus.Normal;
            return true;
        }

        public bool InkBulletInfo(int Left, int Top, int width, int height)
        {
            // long NumberOfBulletBoxes = aLayer.GetInkBullets();        // Counts Number of Bullets (Bullet Boxes)
            Rectangle ActualLineDimension = new Rectangle(Left, Top, width, height);

            int FoundLineNumber = ((Top + height) - Stroke_Y_Offset + 225) / 450;   // Top is the upper position of the Line Top Possition is less than Bottom Position
            if (AnalysedPage[FoundLineNumber].LineContents.Count == 0)
            {
                FoundLineNumber = FoundLineNumber + 1;
            }
            AnalysedPage[FoundLineNumber].Status = LineStatus.BulletStart;
            AnalysedPage[FoundLineNumber].BulletPosition = ActualLineDimension;
            return true;
        }

        public bool AnalyserHasFinished(int Left, int Top, int width, int height)
        {

            IComparer myLineSorter = new LineSorter();

            bool BulletLines = false;
            // int LineNumber = 1;
            int BulletLineNumber = 0;
            int Bullet_Y_Limit = 0;
            //foreach (LineLayout actualLine in AnalysedPage)
            for (int LineNumber = 0; LineNumber < AnalysedPage.Length; LineNumber++)
            {
                 // LineLayout actualLine = AnalysedPage[LineNumber];
                if (AnalysedPage[LineNumber].Status != LineStatus.Empty)
                {
                                                         /* Test for Sort Function 
                                                             TextLayout TestSort = (TextLayout)AnalysedPage[LineNumber].LineContents[0];
                                                             TestSort.TextStart.X = TestSort.TextStart.X + 2;
                                                             // TestSort.TextStart.Y = TestSort.TextStart.Y + 2;
                                                             AnalysedPage[LineNumber].LineContents.Add(TestSort);
                                                         */
                   AnalysedPage[LineNumber].LineContents.Sort(myLineSorter);
                   if (AnalysedPage[LineNumber].Status == LineStatus.BulletStart)
                   {
                        BulletLines = true;
                        Bullet_Y_Limit = (AnalysedPage[LineNumber].BulletPosition.Left + AnalysedPage[LineNumber].BulletPosition.Right) / 2;
                        BulletLineNumber = LineNumber;
                    }
                    else
                    {
                        if (BulletLines)
                        {
                            if ((LineNumber - BulletLineNumber) > 8)
                            {
                                BulletLines = false;
                            }
                            if (((TextLayout)AnalysedPage[LineNumber].LineContents[0]).TextStart.X < Bullet_Y_Limit)
                            {
                                BulletLines = false;
                            }
                            else
                            {
                                AnalysedPage[LineNumber].Status = LineStatus.BulletContinue;
                            }
                        }
                    }
                }
            }
            if (richRecognizedTextBox.InvokeRequired)
            {
                SetBoxTextCallback d = new SetBoxTextCallback(DisplayAnalysedText);
                richRecognizedTextBox.Invoke(d, new object[] { });

            }
            else
            {
                DisplayAnalysedText();
            }
            return false;
        }

        public bool MathPanelReply()
        {
            return false;
        }


        public void AnalyzeInkPage()
        {
            aInkAnalyzerPage.AnalyzeInk();
        }

        public void AddStokesFromInkFile(PaperInkFile OpenedFile) {
                                                           // red, green, blue
                                // Color x = Color.FromArgb(   255,   0,    0);
            aInkAnalyzerPage.AddStrokesFromFile(OpenedFile, Color.Black);
        }

        public PaperInkAnalysedPage( IntPtr InkDrawingHandle,  ref RichTextBox richRecognizedTextBox_in)
        {
            richRecognizedTextBox = richRecognizedTextBox_in;

                                                            // all delegates must be passed via a Variable witch exists during the
                                                            // livetime of the class
                                                            // if not it is deleted by the garbage Collector
            InkAnalyzerResultAvailableFunctionPara = this.InkAnalyzerResultAvailable;
            InkInkDrawingInfoPara = this.InkInkDrawingInfo;
            InkLineBoxInfoPara = this.InkLineBoxInfo;
            InkBulletInfoPara = this.InkBulletInfo;
            AnalyserHasFinishedPara = this.AnalyserHasFinished;
            MathPanelReplyPara = this.MathPanelReply;

            aInkAnalyzerPage = new PaperInkAnalyzer(InkAnalyzerResultAvailableFunctionPara,         // must be passed by a Class global Variable
                                InkInkDrawingInfoPara,                          // must be passed by a Class global Variable
                                InkLineBoxInfoPara,                             // must be passed by a Class global Variable
                                InkBulletInfoPara,                              // must be passed by a Class global Variable
                                AnalyserHasFinishedPara,                        // must be passed by a Class global Variable
                                MathPanelReplyPara,                             // must be passed by a Class global Variable
                                InkDrawingHandle);

            AnalysedPage = new LineLayout[80];
            ClearLayout();
        }

        public void ClearLayout()
        {
            for (int i = 0; i < AnalysedPage.Length; i++)
            {
                AnalysedPage[i].Status = LineStatus.Empty;
                AnalysedPage[i].LineContents = new ArrayList();

            }
            Stroke_Y_Offset = 0;
        }

        public void ClearPage()
        {
            aInkAnalyzerPage.DeleteAllStrokes();
            ClearLayout();
        }

        public void MoveInk(IntPtr HorinontalHandle, IntPtr VerticalHandle) {
            bool Success = aInkAnalyzerPage.PositioningInkPanel(0, Stroke_Y_Offset, HorinontalHandle, VerticalHandle);
        }

        public void CopyToClipboard()
        {
            bool Success = aInkAnalyzerPage.CopyToClipboard();
        }
    }

}