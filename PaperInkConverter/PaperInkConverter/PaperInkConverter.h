
#define MANAGED_TRUE 1
#define MANAGED_FALSE 0

typedef struct MySinglePoint {
	int x;
	int y;
} MySinglePoint;

class PaperInkConverter {
private:
	FILE* f;
	bool FileClose;
short* pPenPosition_X;
short* pPenPosition_Y;
short* pPenPressure;

unsigned char* pPenTilt_X;
unsigned char* pPenTilt_Y;

short* pPenPositionStart_X;
short* pPenPositionStart_Y;
short* pPenPressureStart;

unsigned char* pPenTiltStart_X;
unsigned char* pPenTiltStart_Y;

short* SensitivityStrokeIndexList;
#define MaxStoreBuffer 20000
short* PenPosition_X;
short* PenPosition_Y;
short* PenPressure;
unsigned char* PenTilt_X;
unsigned char* PenTilt_Y;



int Number_of_Points;
int numberOfSubStrokes;
int Number_of_Points_in_Substroke;
// int SensitivityStrokeIndex;
int SubStrokeIndex;

MySinglePoint FullStrokeMaximum;
MySinglePoint FullStrokeMinimum;
MySinglePoint FullStrokeStart;

int AllThreshold;				// Contains Threshold in % * 16384 / 100  (2**14)
int StrokeThreshold;			// Contains Threshold in % * 16384 / 100  (2**14

int maxStrokePressure;

int maxLayerPressure;
int LayerThreshold;

int maxSlidingPressure;


	int Layer;
	int BlockSize;
	int Descriptor;
	unsigned char GraphicBlockMem[256];

	int ReadGraphicBlock();
	bool RecalcalateStrokesWithThreshold();
	int ReadNextCompleteStroke();
	int findStrokeStart();
	void PaperInkConverter::freeAllMemory();

public:
	PaperInkConverter(wchar_t* filename, int RelativThreshold, int AbsoluteThreshold, int SlidingThreshold);
	~PaperInkConverter();
	int EOF_Found();
	void SetAllSensitivity(int Sensitivity) {	AllThreshold = Sensitivity * 164; };					// Threshold in % * 16384 / 100  (2**14);
	void SetStrokeSensitivity(int Sensitivity) {	StrokeThreshold = Sensitivity * 164; };				// Threshold in % * 16384 / 100  (2**14)
	int ReadNextStroke();
	int GetLayer() {return(Layer);};
	int GetNumberOfPoints() {return(Number_of_Points_in_Substroke);};

	short* GetPenPositionX();
	short* GetPenPositionY();
	short* GetPenPressure();
	unsigned char* GetPenTiltX();
	unsigned char* GetPenTiltY();
};

