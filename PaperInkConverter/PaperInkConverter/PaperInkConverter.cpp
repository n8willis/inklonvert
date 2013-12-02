/**********************************************************************************
 * Copyright   : (C) 2012 by Herbert Ellebruch
 * Email       : herbert@useful-tools.de

 **********************************************************************************
 *                                                                                *
 *   PaperInkConverter is free software: you can redistribute it and/or modify    *
 *   it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE as published by  *
 *   the Free Software Foundation, either version 3 of the License, or            *
 *   (at your option) any later version.                                          *
 *                                                                                *
 *   PaperInkConverter is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of               *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 *   GNU General Public License for more details.                                 *
 *                                                                                *
 *   You should have received a copy of the GNU LESSER GENERAL PUBLIC LICENSE     *
 *   along with PaperInkConverter.  If not, see <http://www.gnu.org/licenses/>.   *
 *                                                                                *
 *********************************************************************************/
 /* *******************************************************************************
	Modifications:
	17.Sep 2012 Ellebruch Herbert
	Modified Start of Stroke Data
 */
#include <stdio.h>
#include <malloc.h>

#include "PaperInkConverter.h"

PaperInkConverter::PaperInkConverter(wchar_t* Filename, int StrokeThreshold_In, int AllThreshold_In, int SlidingThreshold_In){
	f = _wfopen(Filename,L"rb");
	if (f == NULL) {
		SensitivityStrokeIndexList = NULL;
		PenPosition_X = NULL;
		PenPosition_Y = NULL;
		PenPressure = NULL;
		PenTilt_X = NULL;
		PenTilt_Y = NULL;

 		Number_of_Points_in_Substroke = -1;
 		FileClose = true;
	} else {
		FileClose = false;


		maxLayerPressure = 0;
		maxSlidingPressure = 0;

		AllThreshold = AllThreshold_In * 164;				// Threshold in % * 16384 / 100  (2**14);
		StrokeThreshold = StrokeThreshold_In * 164;			// Threshold in % * 16384 / 100  (2**14)

		char DummyMem[0x7f8];					// Dummy Buffer to receive unused Dummy information
		size_t FileSize = fread((void*)&DummyMem[0],1,sizeof(DummyMem),f);
		if (FileSize != sizeof(DummyMem)) {
			fclose(f);
 			Number_of_Points_in_Substroke = -1;
 			FileClose = true;
		} else {
			SensitivityStrokeIndexList = (short*)malloc(MaxStoreBuffer*sizeof(short));
			PenPosition_X = (short*)malloc(MaxStoreBuffer*sizeof(short));
			PenPosition_Y  = (short*)malloc(MaxStoreBuffer*sizeof(short));
			PenPressure = (short*)malloc(MaxStoreBuffer*sizeof(short));
			PenTilt_X  = (unsigned char*)malloc(MaxStoreBuffer);
			PenTilt_Y  = (unsigned char*)malloc(MaxStoreBuffer);
			*SensitivityStrokeIndexList = 0;					// Preset this 2 Values Only Relevant, if there ist
			*(SensitivityStrokeIndexList+1) = 0;				// no Stroke in the file

			int Status = ReadNextCompleteStroke();
			if (Status == 2) {
															// End of File found
				freeAllMemory();
									// Already done in ReadNextCompleteStroke
									// fclose(f);
 									// Number_of_Points_in_Substroke = -1;
 									// FileClose = true;
			}
		}
	}
}

	int PaperInkConverter::ReadGraphicBlock() {
		do {
			size_t BytesRed = fread((void*)&GraphicBlockMem[0],1,2,f);									// Read Block Descriptor and Size
			if (BytesRed >= 2) {
				Descriptor = GraphicBlockMem[0];
				BlockSize = GraphicBlockMem[1] - 2;

				size_t BlockBytesRed = fread((void*)&GraphicBlockMem[0],1,BlockSize,f);					// Read the complete Block
				if (BlockBytesRed != BlockSize) {
					Number_of_Points_in_Substroke = -1;
					FileClose = true;
					fclose(f);
																	// TODO Error Message
					return 2;										// End of file Found
				}
			} else {
				Number_of_Points_in_Substroke = -1;
				FileClose = true;
				fclose(f);
				return 2;											// End of file Found
			}
			BlockSize = BlockSize;									// Subtract Descriptor and Blocksize
		} while (BlockSize == 0);									// ignore empty Blocks
		return 0;													// Block is available
	}

	/// Search for Stroke Start
	// Descriptor 241 Marks Start Stroke Stroke End and New Layer
		// 241  03  01 Start of Stroke
		// 241  03  00 End of Stroke
		// 241  03 128 New Layer
	int PaperInkConverter::findStrokeStart() {
		int BlockAvailable = 2;								// Set block Availble to End of File
		int BlockStatus = 0;								// Set Blockstatus to Block found
		while (1) {
			BlockAvailable = ReadGraphicBlock();
			if (BlockAvailable == 0) {
													// 241  03  01 Start of Stroke
													// 241  03  00 End of Stroke
													// 241  03 128 New Layer
					if (Descriptor == 241) {		// Stroke Delimiter found
						int LayerDescriptor = GraphicBlockMem[0];
						if (LayerDescriptor == 01) {					// Stroke Start Found
							break;
						} else if (LayerDescriptor == 00) {				// Layer end found
							BlockStatus = 1;							// Set BlockStatus to Layer end
							break;
						} else if (LayerDescriptor == 128) {			// Layer Changed
							BlockStatus = 1;							// Set BlockStatus to Layer end
							Layer++;
						}
					}
			} else {
				BlockStatus = 2;								// Set BlockStatus to End of File
				break;
			}
		}
		return BlockStatus;
	}

bool PaperInkConverter::RecalcalateStrokesWithThreshold() {

/* Testpoints
PenPressure[0] = 0;
PenPressure[1] = 0;
PenPressure[2] = 10000;
PenPressure[3] = 10000;
PenPressure[4] = 0;
PenPressure[5] = 0;
PenPressure[6] = 10000;
PenPressure[7] = 10000;
Number_of_Points = 8;
*/
	short* pactualPressure = PenPressure;
	short* pactualStrokeIndex = SensitivityStrokeIndexList;
	SubStrokeIndex = 0;
	int actualThresholdStrokeIndex = 0;
	if ((AllThreshold == 0) && (StrokeThreshold == 0)) {				// no Threshold set
		numberOfSubStrokes = 1;
		Number_of_Points_in_Substroke = Number_of_Points;
		*SensitivityStrokeIndexList = 0;
	} else {
		int actualRelativeThreshold = (maxStrokePressure * StrokeThreshold) >> 14;
		if (LayerThreshold < actualRelativeThreshold) {
			if (AllThreshold > 0 ) { 
				actualRelativeThreshold = LayerThreshold;
			}
		} else {
			if (StrokeThreshold == 0) {
				actualRelativeThreshold = LayerThreshold;
			}
		}
		int PointCount = 0;
		numberOfSubStrokes = 0;
		int Number_Of_Points_To_Process = Number_of_Points - 1;
		// while (PointCount < Number_of_Points) {
		do {
			while (*pactualPressure++ < actualRelativeThreshold) {
				// pactualPressure++;
				PointCount++;
				if (PointCount > Number_Of_Points_To_Process) {
					break;
				}
			}
			if (PointCount > Number_Of_Points_To_Process) break;
			*pactualStrokeIndex++ = PointCount;
			numberOfSubStrokes++;
			pactualPressure--;
			while (*pactualPressure++ > actualRelativeThreshold) {
				// pactualPressure++;
				PointCount++;
				if (PointCount > Number_Of_Points_To_Process) {
					break;
				}
			}
			if ((PointCount - *(pactualStrokeIndex-1)) < 2) {
				numberOfSubStrokes--;
				pactualStrokeIndex--;
			} else {
				*pactualStrokeIndex++ = PointCount - 1;
			}
		} while ((PointCount <= Number_Of_Points_To_Process));
		Number_of_Points_in_Substroke = *(SensitivityStrokeIndexList+1) - *SensitivityStrokeIndexList + 1;
	}
	return (numberOfSubStrokes == 0);
}

void PaperInkConverter::freeAllMemory() {
		if (SensitivityStrokeIndexList != NULL) {
			free(SensitivityStrokeIndexList);
			SensitivityStrokeIndexList = NULL;
		}
		if (PenPosition_X != NULL) {
			free(PenPosition_X);
			PenPosition_X = NULL;
		}
		if (PenPosition_Y != NULL) {
			free(PenPosition_Y);
			PenPosition_Y = NULL;
		}
		if (PenPressure != NULL) {
			free(PenPressure);
			PenPressure = NULL;
		}
		if (PenTilt_X != NULL) {
			free(PenTilt_X);
			PenTilt_X = NULL;
		}
		if (PenTilt_Y != NULL) {
			free(PenTilt_Y);
			PenTilt_Y = NULL;
		}
}

PaperInkConverter::~PaperInkConverter()
{
	freeAllMemory();
}

int PaperInkConverter::EOF_Found()
{
	if (FileClose) {
		freeAllMemory();
		return(MANAGED_TRUE);
	} else {
		return(MANAGED_FALSE);
	}
}

short*  PaperInkConverter::GetPenPositionX() {
		short* temp = pPenPositionStart_X + *(SensitivityStrokeIndexList+SubStrokeIndex);
		return(temp);
	};
short*  PaperInkConverter::GetPenPositionY() {
		short* temp = pPenPositionStart_Y + *(SensitivityStrokeIndexList+SubStrokeIndex);
		return(temp);
	};
short*  PaperInkConverter::GetPenPressure() {
		short* temp = pPenPositionStart_X + *(SensitivityStrokeIndexList+SubStrokeIndex);
		return(temp);
	};

unsigned char*  PaperInkConverter::GetPenTiltX() {
		unsigned char* temp = pPenTiltStart_X + *(SensitivityStrokeIndexList+SubStrokeIndex);
		return(temp);
	};

unsigned char*  PaperInkConverter::GetPenTiltY() {
		unsigned char* temp = pPenTiltStart_Y + *(SensitivityStrokeIndexList+SubStrokeIndex);
		return(temp);
	};

int PaperInkConverter::ReadNextStroke() {
/*
		StrokeMaximum.x = 0x7FFFFFF;
		StrokeMaximum.y = 0x7FFFFFF;

		StrokeMinimum.x = -0x7FFFFFF;
		StrokeMinimum.y = -0x7FFFFFF;

		StrokeStart.x = 0;
		StrokeStart.y = 0;
		pPenPositionStart_X = pPenPosition_X = &PenPosition_X[0];;
		pPenPositionStart_Y = pPenPosition_Y = &PenPosition_Y[0];;
		pPenPressureStart = pPenPressure = &PenPressure[0];
								
		pPenTiltStart_X = pPenTilt_X = &PenTilt_X[0];
		pPenTiltStart_Y = pPenTilt_Y = &PenTilt_Y[0];
*/

		bool BlockAvailable = true;
		int Blockstatus = 0;				// Set Block Status to Strokes available
		numberOfSubStrokes--;
		if (numberOfSubStrokes <= 0) {
			Blockstatus = ReadNextCompleteStroke();
		} else {
			SubStrokeIndex = SubStrokeIndex + 2;
			Number_of_Points_in_Substroke = *(SensitivityStrokeIndexList+SubStrokeIndex+1) - *(SensitivityStrokeIndexList+SubStrokeIndex) + 1;

		}

		return Blockstatus;
}
int PaperInkConverter::ReadNextCompleteStroke() {
/* testpoints 
		pactualStroke->Max.x = -0x7FFFFFF;
		pactualStroke->Max.y = -0x7FFFFFF;

		pactualStroke->Min.x = 0x7FFFFFF;
		pactualStroke->Min.y = 0x7FFFFFF;

		pactualStroke->Start.x = 0;
		pactualStroke->Start.y = 0;
*/

		FullStrokeMaximum.x = 0x7FFFFFF;
		FullStrokeMaximum.y = 0x7FFFFFF;

		FullStrokeMinimum.x = -0x7FFFFFF;
		FullStrokeMinimum.y = -0x7FFFFFF;

		FullStrokeStart.x = 0;
		FullStrokeStart.y = 0;

		pPenPositionStart_X = pPenPosition_X = PenPosition_X;
		pPenPositionStart_Y = pPenPosition_Y = PenPosition_Y;
		pPenPressureStart = pPenPressure = PenPressure;
								
		pPenTiltStart_X = pPenTilt_X = PenTilt_X;
		pPenTiltStart_Y = pPenTilt_Y = PenTilt_Y;

		maxStrokePressure = 0;

		Descriptor = 0;
		Number_of_Points = 0;
		bool NoPointsInStroke = true;
		int BlockAvailable;
		int StrokeStatus;
		do {
			StrokeStatus = findStrokeStart();
			BlockAvailable = StrokeStatus;
			while (BlockAvailable != 2) {							// while More Strokes Available
				BlockAvailable = ReadGraphicBlock();				// it only returns 0 and 2;
				if (BlockAvailable == 0) {
					if (         (Descriptor == 97)  ) {		// Descriptor 97 Contains Position x and Position y 
																// Position x in 1 mm Increments
																// Position y in 2 mm Increments
																// Format :
																//          96  06  Position x  Position Y
																//           1   1       2          2        Byte
																//
																//
															// int Posx = (((int)(char) (*pConvertStart++)) << 8);
															// Posx = Posx + (int)(*pConvertStart++) + 5 ;				// Get Pos x
						int Posx = (((int)(char) GraphicBlockMem[0]) << 8);
						Posx = Posx + (int)(GraphicBlockMem[1]) + 5 ;				// Get Pos x
															// int Posy = (((int)(char) (*pConvertStart++)) << 8);
															// Posy =  ((Posy + (int)(*pConvertStart++)) << 1)  + 5 ;	// Get Pos y
						int Posy = (((int)(char) GraphicBlockMem[2]) << 8);
						Posy =  ((Posy + (int)GraphicBlockMem[3]) << 1)  + 5 ;	// Get Pos y
															// pactualPoint->MySinglePoint.x = Posx;
															// pactualPoint->MySinglePoint.y = Posy;
						*pPenPosition_X = Posx;
						*pPenPosition_Y = Posy;
					} else if (  (Descriptor == 100) ){			// Descriptor 100 contains Pen Pressur
																// Format :
																//          100  06  unknown unknown Pressur
																//           1    1     1       1      2       Byte
																//
																//
					int Pressure = (((int)GraphicBlockMem[2]) << 8);
					Pressure = Pressure + (int)(GraphicBlockMem[3]);			// Get Pressure

						if (Pressure > maxStrokePressure) {
							maxStrokePressure = Pressure;
						}
						*pPenPressure = Pressure;
																	// move Pressure to one Position set first position 
					} else if (  (Descriptor == 101)  ){		// Descriptor 101 Contains Pen Tilt x and Pen Tilt y of the pen
														// Format :
														//         101  06  Tilt x  Tilt Y  u    u
														//           1   1     1       1    1    1   Byte
														//
														//

					int Tilt_x = GraphicBlockMem[0];							// Get Tilt x
					int Tilt_y =  GraphicBlockMem[1];							// Get Tilt y

						*pPenTilt_X = Tilt_x;
						*pPenTilt_Y = Tilt_y;
										// Recalculate Mininum and Maximum
						Number_of_Points++;											// Increment Number of Points

						if (*pPenPosition_X < FullStrokeMaximum.x) {
							FullStrokeMaximum.x = *pPenPosition_X;
						}
						if (*pPenPosition_Y < FullStrokeMinimum.y) {
							FullStrokeMinimum.y = *pPenPosition_Y;
						}
						if (*pPenPosition_X > FullStrokeMaximum.x) {
							FullStrokeMaximum.x = *pPenPosition_X;
						}
						if (*pPenPosition_Y > FullStrokeMaximum.y) {
							FullStrokeMaximum.y = *pPenPosition_Y;
						}

						if (Number_of_Points < MaxStoreBuffer) {
							pPenPosition_X++;
							pPenPosition_Y++;
							pPenPressure++;
							pPenTilt_X++;
							pPenTilt_Y++;
						} else {
												// ignore this points
												// to do insert Error Message
						}
					} else if (  (Descriptor == 241)  ) {
						int LayerDescriptor = GraphicBlockMem[0];
						if (LayerDescriptor == 0) {					// Stroke End Found
							break;
						}
					} else {
																// Unknown Identifier
																// ignore this block
					}
				}
			}
			if (Number_of_Points > 0) {
				if (maxStrokePressure > maxLayerPressure) {
					maxLayerPressure = maxStrokePressure;
					LayerThreshold = (maxLayerPressure * AllThreshold) >> 14;
				}
				NoPointsInStroke = RecalcalateStrokesWithThreshold();
			}
		} while (NoPointsInStroke && (BlockAvailable == 0));
		return StrokeStatus;
}
