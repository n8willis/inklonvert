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
    enum LineStatus
    {
        Empty,
        BulletStart,
        BulletContinue,
        Normal

    };

    struct LineLayout
    {
        public LineStatus Status;
        public int LineHeight;
        public Rectangle BulletPosition;
        public ArrayList LineContents;
    };

    struct TextLayout
    {
        public Point TextStart;
        public String LineString;
    }

    public partial class GeneralLayout : Form
    {

        // Declare Values for the Text Layout
        PaperInkAnalysedPage aCompletePaperInkPage;
        PaperInkFile OpenedFile;// todo remove this?
        bool InkPagExists;

        public GeneralLayout()
        {
            InitializeComponent();
        }

        private void GeneralLayout_Load(object sender, EventArgs e)
        {


#if FileAutoLoad
                        // for Testing only
           int x = 1;
            String FileOpened = "D:\\ink\\Multi\\SKETCH39.wpi";
            int RelativThreshold = 9;
            int AbsoluteThreshold = 10;
            int SlidingThreshold = 0;
            OpenedFile = new PaperInkFile(FileOpened, RelativThreshold, AbsoluteThreshold, SlidingThreshold);
            IntPtr DasInkHandel = LayoutArea.Panel1.Handle;
            aInkPage = new PaperInkPage(OpenedFile, DasInkHandel, ref richRecognizedTextBox);
            InkPagExists = true;
            aInkPage.AddStokesFromFileAndDisplay(OpenedFile);
#else
            InkPagExists = false;
#endif

        }

        private void GeneralLayout_FormClosed(object sender, FormClosedEventArgs e)
        {
        }

        private void richRecognizedTextBox_VScroll(object sender, EventArgs e)
        {
            // http://www.codeproject.com/Articles/7830/Scrolling-Around-with-the-RichTextBox-Control
            aCompletePaperInkPage.MoveInk(hInkHScrollBar.Handle, richRecognizedTextBox.Handle);
            LayoutArea.Panel1.Refresh();
       }

        private void FileMenueOpenFile_Click(object sender, EventArgs e)
        {
            openInkFile.ShowDialog();
        }

        private void openInkFile_FileOk(object sender, CancelEventArgs e)
        {
            String FileOpened = openInkFile.FileName;
            int StrokeThreshold = 9;
            int AllThreshold = 10;
            int SlidingThreshold = 0;
            OpenedFile = new PaperInkFile(FileOpened, StrokeThreshold, AllThreshold, SlidingThreshold);
            if (OpenedFile.EOFfound()) {
                MessageBox.Show("Cannot open File or no valid ink file");
            } else {
                IntPtr aInkHandel = LayoutArea.Panel1.Handle;
                if (InkPagExists)
                {
                    hInkHScrollBar.Value = 0;               // Set Scrollbar to the left side
                    aCompletePaperInkPage.ClearPage();
                }
                else
                {
                    aCompletePaperInkPage = new PaperInkAnalysedPage( aInkHandel, ref richRecognizedTextBox);
                    InkPagExists = true;
                }

                aCompletePaperInkPage.AddStokesFromInkFile(OpenedFile);
                LayoutArea.Panel1.Refresh();
            }

        }

        private void toolStripAnalyze_Click(object sender, EventArgs e)
        {
            aCompletePaperInkPage.AnalyzeInkPage();

            LayoutArea.Panel1.Refresh();
        }

        private void GeneralLayout_FormClosing(object sender, FormClosingEventArgs e)
        {
        }

        private void toolStripSendToMathPanel_Click(object sender, EventArgs e)
        {
            aCompletePaperInkPage.PageToMathInputPanel();
        }

        private void copyInkToolStripMenuItem_Click(object sender, EventArgs e)
        {
            aCompletePaperInkPage.CopyToClipboard();

        }

        private void hInkHScrollBar_Scroll(object sender, ScrollEventArgs e)
        {
                            // For test only            int position = hInkHScrollBar.Value;
                            // For test only            if (position > 10)
                            // For test only            {
                            // IntPtr ScrollHandle = hInkHScrollBar.Handle;
                aCompletePaperInkPage.MoveInk(hInkHScrollBar.Handle, richRecognizedTextBox.Handle);
                LayoutArea.Panel1.Refresh();

                            // For test only            }
        }

        private void loadNextLayerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (OpenedFile.EOFfound())
            {
                MessageBox.Show("NO more Layer");
            }
            else
            {
                hInkHScrollBar.Value = 0;                               // Set Scrollbar to the left side
                aCompletePaperInkPage.ClearPage();
                aCompletePaperInkPage.AddStokesFromInkFile(OpenedFile); // add Strokes from File also call AnalyzePage
                LayoutArea.Panel1.Refresh();
            }
        }

        private void addNextLayerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (OpenedFile.EOFfound())
            {
                MessageBox.Show("NO more Layer");
            }
            else
            {
                aCompletePaperInkPage.AddStokesFromInkFile(OpenedFile);           // add Strokes from File also call AnalyzePage
                LayoutArea.Panel1.Refresh();
            }
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AboutBox aAboutBox = new AboutBox();
            aAboutBox.Show();
        }
    }
}