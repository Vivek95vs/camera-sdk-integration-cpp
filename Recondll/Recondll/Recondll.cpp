// Recondll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "Recondll.h"

short *getvolume1(unsigned short * ExtBuffer, float * angles, std::list<double> mDetailsD1, std::list<double> mDetailsD2, int angleLength, float kVP, int curDMode, float slop, float inter, float uw, float uai, int FanSelection, int *angleIndex1)
{

	////  auto start77 = high_resolution_clock::now();
	const int dimension = 3;
	//const unsigned int InputImageDimension = 2;
	/*!
	* \brief Geometry Three dimensional circular geometry used in the machine.
	*/
	typedef rtk::ThreeDCircularProjectionGeometry Geometry;
	typedef itk::Image<unsigned short, dimension> ProjectionsType;
	typedef itk::CudaImage<float, dimension>VolumeType;
	typedef rtk::ConstantImageSource< VolumeType> ConstantImageSourceType;
	/*!
	* \brief PSSFType parker short scan filter type.
	*/
	//  typedef rtk::CudaParkerShortScanImageFilter PSSFType;
	typedef rtk::ParkerShortScanImageFilter<VolumeType> PSSFType;
	/*!
	* \brief FDKType Cone Beam reconstruction using FDK.
	*/

	typedef rtk::CudaFDKConeBeamReconstructionFilter FDKType;

	typedef rtk::SARTConeBeamReconstructionFilter <VolumeType>SARTType;
	/*!
	* \brief FOVFilterType
	*/
	typedef rtk::FieldOfViewImageFilter<VolumeType, VolumeType> FOVFilterType;
	/*!
	* \brief ImageMulType
	*/



	typedef itk::MultiplyImageFilter<VolumeType, VolumeType, VolumeType>ImageMulType;
	/*!
	* \brief ProjMulType
	*/
	typedef itk::MultiplyImageFilter<ProjectionsType, ProjectionsType, ProjectionsType>ProjMulType;
	/*!
	* \brief GaussianFilterType
	*/
	typedef itk::DiscreteGaussianImageFilter<itk::Image<float, 3>, VolumeType>GaussianFilterType;

	typedef itk::MedianImageFilter<ProjectionsType, ProjectionsType > FilterType1;

	typedef itk::ImportImageFilter<unsigned short, dimension >   ImportFilterType;
	ImportFilterType::Pointer importFilter = ImportFilterType::New();
	ImportFilterType::SizeType  size;
	size[0] = 1536;  // size along X
	size[1] = 1536;  // size along Y
	size[2] = angleLength;  // size along Z

	ImportFilterType::IndexType start;
	start.Fill(0);



	ImportFilterType::RegionType region;
	region.SetIndex(start);
	region.SetSize(size);

	importFilter->SetRegion(region);

	double spacing[dimension];
	spacing[0] = 0.278;    // along X direction
	spacing[1] = 0.278;    // along Y direction
	spacing[2] = 1.0;
	double origin[dimension];
	origin[0] = -0.5*  spacing[0] * (size[0] - 1);// X coordinate
	origin[1] = -0.5*  spacing[1] * (size[1] - 1);    // Y coordinate
	origin[2] = -0.5*spacing[2] * size[2]; // Z coordinate

	importFilter->SetOrigin(origin);// along Z direction

	importFilter->SetSpacing(spacing);

	if (ExtBuffer == NULL)
	{
	}
	else
	{

	}

	const unsigned int numberOfPixels = size[0] * size[1] * size[2];

	const bool importImageFilterWillOwnTheBuffer = false; // use of imporrtImageFilterWillOwnTheBuffer = false is that once the reconstruction will complere the memory of 'proj' automatic deleted
														  // other wise it's need to delete the 'proj' memory from distructor of mainwindow after the use
	importFilter->SetImportPointer(ExtBuffer, numberOfPixels,
		importImageFilterWillOwnTheBuffer);
	try {
		importFilter->Update();
	}
	catch (itk::ExceptionObject e) {
		// std::cout<<e.GetDescription();
		//return 1;
		throw(e);

	}


	ProjectionsType::Pointer rval = importFilter->GetOutput();

	itk::OrientImageFilter<ProjectionsType, ProjectionsType>::Pointer orienter =
		itk::OrientImageFilter<ProjectionsType, ProjectionsType>::New();

	orienter->SetInput(rval);


	try
	{

		orienter->Update();
		//    qDebug()<<"FFF"<<orienter->GetO;


	}
	catch (itk::ExceptionObject e)
	{
		throw(e);

	}


	orienter->GenerateOutputInformation();
	rval = orienter->GetOutput();
	typedef itk::ChangeInformationImageFilter< ProjectionsType>  FilterType;
	FilterType::Pointer filter = FilterType::New();
	ProjectionsType::SpacingType pSpacing;
	pSpacing[0] = 0.278;
	pSpacing[1] = 0.278;
	pSpacing[2] = 1;
	ProjectionsType::PointType pOrigin;
	pOrigin[0] = -0.5* pSpacing[0] * (1536 - 1);
	pOrigin[1] = -0.5* pSpacing[0] * (1536 - 1);
	pOrigin[2] = -0.5* pSpacing[2] * angleLength;
	//inProj->Update();
	filter->SetOutputOrigin(pOrigin);
	ProjectionsType::DirectionType direction = importFilter->GetOutput()->GetDirection();
	filter->SetOutputDirection(direction);
	filter->SetOutputSpacing(pSpacing);
	filter->ChangeAll();
	filter->SetInput(rval);
	try
	{
		filter->Update();
	}
	catch (itk::ExceptionObject e)
	{
		throw(e);
	}


	std::vector<int> elements(mDetailsD1.begin(), mDetailsD1.end());

	int m1_valueAtIndexzero = elements[0];
	int m1_valueAtIndexone = elements[1];
	int m1_valueAtIndextwo = elements[2];
	int m1_valueAtIndexthree = elements[3];
	int m1_valueAtIndexfour = elements[4];
	int m1_valueAtIndexfive = elements[5];
	int m1_valueAtIndexsix = elements[6];
	int m1_valueAtIndexseven = elements[7];
	int m1_valueAtIndexEight = elements[8];


	std::vector<int> elements1(mDetailsD2.begin(), mDetailsD2.end());

	int m2_valueAtIndexzero = elements1[0];
	int m2_valueAtIndexone = elements1[1];
	int m2_valueAtIndextwo = elements1[2];
	int m2_valueAtIndexthree = elements1[3];
	int m2_valueAtIndexfour = elements1[4];
	int m2_valueAtIndexfive = elements1[5];
	int m2_valueAtIndexsix = elements1[6];
	int m2_valueAtIndexseven = elements1[7];
	int m2_valueAtIndexEight = elements1[8];


	typedef rtk::ThreeDCircularProjectionGeometry Geometry;
	Geometry::Pointer geo = Geometry::New();
	int angleCount = 0;
	if ((&mDetailsD1)->size() == 0)
		angleCount = m1_valueAtIndexEight;
	else
		angleCount = m1_valueAtIndexEight;




	float angleValue = angles[0];

	bool angleStatus = false;

	for (int i = 0; i<angleLength; ++i)
	{

		if (curDMode == 3)
		{


			if (angleIndex1[i]<m1_valueAtIndexEight)
			{

				geo->AddProjection(m1_valueAtIndexzero, m1_valueAtIndexone, angles[i], m1_valueAtIndextwo, m1_valueAtIndexthree, m1_valueAtIndexfour, m1_valueAtIndexfive, m1_valueAtIndexsix, m1_valueAtIndexseven);

			}
			else
			{

				geo->AddProjection(m2_valueAtIndexzero, m2_valueAtIndexone, angles[i], m2_valueAtIndextwo, m2_valueAtIndexthree, m2_valueAtIndexfour, m2_valueAtIndexfive, m2_valueAtIndexsix, m2_valueAtIndexseven);

			}


		}
		else if (curDMode == 2)
		{
			geo->AddProjection(m2_valueAtIndexzero, m2_valueAtIndexone, angles[i], m2_valueAtIndextwo, m2_valueAtIndexthree, m2_valueAtIndexfour, m2_valueAtIndexfive, m2_valueAtIndexsix, m2_valueAtIndexseven);

		}
		else
		{


			geo->AddProjection(m1_valueAtIndexzero, m1_valueAtIndexone, angles[i], m1_valueAtIndextwo, m1_valueAtIndexthree, m1_valueAtIndexfour, m1_valueAtIndexfive, m1_valueAtIndexsix, m1_valueAtIndexseven);

		}

		if (angles[i] != angleValue)
			angleStatus = true;



	}

	if (angleStatus == false)
		return 0;




	try
	{
		geo->Update();
	}
	catch (itk::ExceptionObject e)
	{
		throw(e);
	}



	typedef rtk::ThreeDCircularProjectionGeometryXMLFileWriter GeometryWriterType;
	GeometryWriterType::Pointer geometryWriter = GeometryWriterType::New();
	geometryWriter->SetFilename("D:\\geo_cirs.xml");
	geometryWriter->SetObject(geo);
	geometryWriter->WriteFile();
	try
	{
		geo->Update();
	}
	catch (itk::ExceptionObject e)
	{
		throw(e);
	}


	ConstantImageSourceType::Pointer outVol = ConstantImageSourceType::New();

	//generate a constant image for reconstruction.
	ConstantImageSourceType::PointType origin_out;
	ConstantImageSourceType::SizeType size_out;
	ConstantImageSourceType::SpacingType spacing_out;

	int m_ctDIM1 ;
	int m_ctDIM2 ;
	int m_ctDIM3 ;

	if (FanSelection == 1) {
		 m_ctDIM1 = 512;
		 m_ctDIM2 = 512;
		 m_ctDIM3 = 512;
	}
	else {
		 m_ctDIM1 = 454;//940
		 m_ctDIM2 = 454;
		 m_ctDIM3 = 454;
	}
	size_out[0] = m_ctDIM1;
	size_out[1] = m_ctDIM3;
	size_out[2] = m_ctDIM2;

	if (FanSelection == 1)
	{
		spacing_out[0] = 256.0 / (float)m_ctDIM1;
		spacing_out[1] = 256.0 / (float)m_ctDIM3;
		spacing_out[2] = 256.0 / (float)m_ctDIM2;
	}
	else
	{


		spacing_out[0] = /*256.0/(float)m_ctDIM1;*/1;
		spacing_out[1] = /*256.0/(float)m_ctDIM3;*/1;
		spacing_out[2] = /*256.0/(float)m_ctDIM2;*/1;
	}


	origin_out[0] = -0.5*(size_out[0] - 1)*(spacing_out[0]);
	origin_out[1] = -0.5*(size_out[1] - 1)*(spacing_out[1]);
	origin_out[2] = -0.5*(size_out[2] - 1)*(spacing_out[2]);
	outVol->SetOrigin(origin_out);
	outVol->SetSpacing(spacing_out);
	outVol->SetSize(size_out);
	outVol->SetConstant(0);

	typedef rtk::BoellaardScatterCorrectionImageFilter<ProjectionsType, ProjectionsType> LagCorrectionImageFilterType1;
	LagCorrectionImageFilterType1::Pointer LagCorrection = LagCorrectionImageFilterType1::New();
	LagCorrection->SetInput(filter->GetOutput()/*ioFilter->GetOutput()*/);
	LagCorrection->SetScatterToPrimaryRatio(1.2);//0.8
												 //    LagCorrection->SetNonNegativityConstraintThreshold(900.0);
	try
	{
		LagCorrection->Update();
	}
	catch (itk::ExceptionObject e)
	{
		throw(e);

	}



	typedef rtk::I0EstimationProjectionFilter<ProjectionsType, ProjectionsType, 3>IoFilterType;
	IoFilterType::Pointer ioFilter = IoFilterType::New();
	ioFilter->SetInput(LagCorrection->GetOutput());
	try
	{
		ioFilter->Update();
	}
	catch (itk::ExceptionObject e)
	{
		throw(e);

	}


	typedef rtk::LUTbasedVariableI0RawToAttenuationImageFilter<ProjectionsType, VolumeType> ConvertFilterType;
	ConvertFilterType::Pointer convert = ConvertFilterType::New();
	convert->SetInput(ioFilter->GetOutput());

	try
	{
		convert->Update();
	}
	catch (itk::ExceptionObject e)
	{
		throw(e);
	}



	using DDFOFFFOVType = rtk::DisplacedDetectorForOffsetFieldOfViewImageFilter<VolumeType>;
	DDFOFFFOVType::Pointer ddf;
	ddf = DDFOFFFOVType::New();
	ddf->SetInput(convert->GetOutput());
	ddf->SetGeometry(geo);


	PSSFType::Pointer pssf = PSSFType::New();

	pssf->SetInput(ddf->GetOutput());

	pssf->SetGeometry(geo);

	pssf->InPlaceOff();

	try
	{

		pssf->Update();

	}
	catch (itk::ExceptionObject e)
	{
		throw(e);
	}


	FDKType::Pointer feldkamp = FDKType::New();


	feldkamp->SetInput(0, outVol->GetOutput());


	feldkamp->SetInput(1, pssf->GetOutput());

	feldkamp->SetGeometry(geo);


	feldkamp->GetRampFilter()->SetTruncationCorrection(1.0);

	feldkamp->GetRampFilter()->SetHannCutFrequency(0.8);//high pass filter.
														// qDebug()<<"99991112222";

	feldkamp->SetGPUEnabled(1); // 1 to work into GPU; 0 to Work into CPU
								//qDebug()<<"wwwww";

								//      std::cout<<"ParkerWeight2";
	try {

		feldkamp->Update();
	}
	catch (itk::ExceptionObject e) {

		throw(e);

	}


	FOVFilterType::Pointer fov = FOVFilterType::New();
	fov->SetInput(0, feldkamp->GetOutput());
	fov->SetProjectionsStack(ddf->GetOutput());
	fov->SetGeometry(geo);
	try
	{
		fov->Update();
	}
	catch (itk::ExceptionObject e)
	{
		throw(e);
	}


	struct HuParam {
		float hslope, hintecept, muWater, muAir;
	};

	struct overDetails
	{
		std::string patientName, patientId, patientDob, kvp, ma, ms, aqDate, slThickness;

	};

	float slope = 0, intercept = 0, muWater = 0, muAir = 0;


	if (slop == 0 || inter == 0 || uw == 0 || uai == 0)
	{
		slope = 18.15;
		intercept = -0.165;
		muWater = 0.17070;
		muAir = 0.0001;
	}
	else
	{
		slope = slop;
		intercept = inter;
		muWater = uw;
		muAir = uai;

	}

	typedef itk::AddImageFilter<VolumeType, VolumeType, VolumeType>ImageAddType;
	typedef itk::SubtractImageFilter<VolumeType, VolumeType, VolumeType> ImageSubType;


	ImageMulType::Pointer slopeMul = ImageMulType::New();
	slopeMul->SetInput(fov->GetOutput());
	slopeMul->SetConstant(slope);
	ImageAddType::Pointer imageAdd = ImageAddType::New();
	imageAdd->SetInput(slopeMul->GetOutput());
	imageAdd->SetConstant(intercept);
	ImageSubType::Pointer imageSub = ImageSubType::New();
	imageSub->SetInput(imageAdd->GetOutput());
	imageSub->SetConstant(muWater);
	ImageMulType::Pointer imageMul = ImageMulType::New();
	imageMul = ImageMulType::New();
	imageMul->SetInput(imageSub->GetOutput());
	imageMul->SetConstant(1000.f / (muWater - muAir));
	try
	{
		imageMul->Update();
	}
	catch (itk::ExceptionObject e)
	{
		throw(e);
	}


	itk::OrientImageFilter<VolumeType, itk::Image<float, 3>>::Pointer orienter_out =
		itk::OrientImageFilter<VolumeType, itk::Image<float, 3>>::New();

	orienter_out->SetGivenCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RSP);
	//orienter_out->SetDesiredCoordinateOrientationToAxial();
	orienter_out->SetInput(imageMul->GetOutput());
	orienter_out->GenerateOutputInformation();
	try
	{
		orienter_out->Update();

	}
	catch (itk::ExceptionObject e)
	{
		throw(e);

	}

	//typedef itk::Image<float, 3> imageType;
	//itk::TIFFImageIOFactory::RegisterOneFactory();
	//// Writersddsds

	//std::cout << "Writing output image..." << std::endl;
	//using WriterType = itk::ImageFileWriter<imageType>;
	//WriterType::Pointer writer = WriterType::New();
	//writer->SetFileName("D:\\orient_output.tiff");
	//writer->SetInput(orienter_out->GetOutput());
	//writer->Update();
	//writer->Write();

	itk::Image<float, 3>::Pointer outVolume= orienter_out->GetOutput();
	outVolume->GetPixelContainer()->SetContainerManageMemory(false);
	float *volume= outVolume->GetPixelContainer()->GetImportPointer();
	volume = outVolume->GetBufferPointer();


	short *volShort;
	volShort = new  short[m_ctDIM1*m_ctDIM2*m_ctDIM3];


	for (int i = 0; i<m_ctDIM3; i++)//z
	{

		for (int j = 0; j<m_ctDIM2; j++)//y
		{

			for (int k = 0; k<m_ctDIM1; k++)//x
			{
				volShort[k + m_ctDIM1*j + (m_ctDIM1*m_ctDIM2*i)] = static_cast<float>(volume[k + m_ctDIM1*m_ctDIM3*(m_ctDIM2 - 1 - j) + (m_ctDIM2*i)]);

			}
		}
	}


	return volShort;

}
