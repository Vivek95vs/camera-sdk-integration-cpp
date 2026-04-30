// MathLibrary.h - Contains declarations of math functions
#pragma once
#include <iostream>
#include <list>

#include "itkLogImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "rtkThreeDCircularProjectionGeometry.h"
#include "rtkThreeDCircularProjectionGeometryXMLFile.h"
# include "rtkCudaDisplacedDetectorImageFilter.h"
#include "rtkCudaParkerShortScanImageFilter.h"
#include "rtkCudaFDKConeBeamReconstructionFilter.h"
#include "rtkFDKConeBeamReconstructionFilter.h"
#include "rtkConstantImageSource.h"
#include "rtkCudaDisplacedDetectorImageFilter.h"
#include "rtkI0EstimationProjectionFilter.h"
#include "rtkLUTbasedVariableI0RawToAttenuationImageFilter.h"
#include "rtkFieldOfViewImageFilter.h"
#include "rtkDisplacedDetectorForOffsetFieldOfViewImageFilter.h"
//
//
////ITK headers
#include"itkImageFileReader.h"
#include "itkRawImageIO.h"
#include "itkImageIOFactory.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkNumericSeriesFileNames.h"
#include "itkCastImageFilter.h"
#include "itkMedianImageFilter.h"
#include "itkOrientImageFilter.h"
#include "itkSpatialOrientation.h"
#include "itkChangeInformationImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "rtkCudaParkerShortScanImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkImportImageFilter.h"
#include "itkMedianImageFilter.h"
//
//
//
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkFlatStructuringElement.h"
#include "rtkBoellaardScatterCorrectionImageFilter.h"
#include "rtkScatterGlareCorrectionImageFilter.h"
#include "rtkLagCorrectionImageFilter.h"
#include "rtkLookupTableImageFilter.h"
//#include "rtkMedianImageFilter.h"
#include "rtkFFTRampImageFilter.h"
#include "rtkWaterPrecorrectionImageFilter.h"
#include "itkImage.h"
#include <itkImageFileReader.h>
#include <itkExtractImageFilter.h>
#include "rtkSARTConeBeamReconstructionFilter.h"
#include <rtkFFTConvolutionImageFilter.h>
#include <rtkFFTRampImageFilter.h>
#include <rtkDrawCylinderImageFilter.h>
#include <rtkVarianObiRawImageFilter.h>
#include <rtkConjugateGradientConeBeamReconstructionFilter.h>
#include <itkTIFFImageIOFactory.h>
#include <itkTIFFImageIO.h>

#ifdef RECONDLL_EXPORTS
#define RECONDLL_API __declspec(dllexport)
#else
#define RECONDLL_API __declspec(dllimport)
#endif

// The Fibonacci recurrence relation describes a sequence F
// where F(n) is { n = 0, a
//               { n = 1, b
//               { n > 1, F(n-2) + F(n-1)
// for some initial integral values a and b.
// If the sequence is initialized F(0) = 1, F(1) = 1,
// then this relation produces the well-known Fibonacci
// sequence: 1, 1, 2, 3, 5, 8, 13, 21, 34, ...

// Initialize a Fibonacci relation sequence
// such that F(0) = a, F(1) = b.
// This function must be called before any other function.
extern "C" RECONDLL_API short *getvolume1(
	unsigned short *ExtBuffer, float *angles, std::list<double> mDetailsD1, std::list<double> mDetailsD2, int angleLength, float kVP, int curDMode, float slop, float inter, float uw, float uai,int FanSelection,int *angleIndex1);

//// Produce the next value in the sequence.
//// Returns true on success and updates current value and index;
//// false on overflow, leaves current value and index unchanged.
//extern "C" RECONDLL_API bool fibonacci_next();
//
//// Get the current value in the sequence.
//extern "C" RECONDLL_API unsigned long long fibonacci_current();
//
//// Get the position of the current value in the sequence.
//extern "C" RECONDLL_API unsigned fibonacci_index();
