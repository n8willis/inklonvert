#pragma once

// includes for the 64 Bit / 32 Bit Analyzer communication
typedef enum InkAnalyzerResult {
	AnalyzingReady = 0,
	AnalyzerResultAvailable,
	LineBoxes,
	BulletBoxes,
	InkDrawingBoxes,
	InkAnalyzeFinished,
	InkAnalyzerUnadvise,
	FinischInkAnalyzerListener
};

typedef struct sCountInfo{
		ULONG LineBoxesCount;
		ULONG BulletBoxesCount;
		ULONG DrawinfBoxesCount;
};

typedef struct structLayoutBoxesCounts {
	long command;
	sCountInfo CountInfo;
};

enum AnalyzerCommand {
	AnalyzeStrokes = 1,
	AddAnalyzerStroke,
	SearchUpperLeftLinePosition,
	GetLineBoxInfo,
	GetBulletPosition,
	GetDrawingHotspots,
	ClearAllAnalyzerStrokes,
	AdviseInkAnylyzer,
	UnanadviseInkAnylyzer,
	FinishInkAnylyzer
};

struct sPointsInfo{
		long X_MinValue_Left;
		long X_MinValue_Right;
		long Y_MinValue_Top;
		long Y_MinValue_Bottom;
};
struct sLineBoxInfo{
		int Left;
		int Top;
		int width;
		int height;
		int RecognizedStringSize;
};

struct sBulletBoxInfo{
		int Left;
		int Top;
		int width;
		int height;
};

struct sDrawingBoxInfo{
		int Left;
		int Top;
		int width;
		int height;
		int RecognizedStringSize;
	};
