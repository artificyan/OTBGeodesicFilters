#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryCrossStructuringElement.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

#include "itkOpeningByReconstructionImageFilter.h"
#include "itkClosingByReconstructionImageFilter.h"

namespace otb
{
  namespace Wrapper
  {

    class GrayScaleGeodesicMorphologicalOperation : public Application
    {
    public:
      /** Standard class typedefs. */
      typedef GrayScaleGeodesicMorphologicalOperation          Self;
      typedef Application                   Superclass;
      typedef itk::SmartPointer<Self>       Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;

      typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType,FloatVectorImageType::InternalPixelType>     ExtractorFilterType;

      typedef itk::BinaryBallStructuringElement<FloatImageType::PixelType, 2>        BallStructuringType;
      typedef BallStructuringType::RadiusType                                        RadiusType;
      typedef BallStructuringType::Superclass                                        StructuringType;
      typedef itk::BinaryCrossStructuringElement<FloatImageType::PixelType, 2>       CrossStructuringType;

      typedef itk::OpeningByReconstructionImageFilter<FloatImageType,FloatImageType,StructuringType> OpeningFilterType;
      typedef itk::ClosingByReconstructionImageFilter<FloatImageType,FloatImageType,StructuringType> ClosingFilterType;
      

      typedef ImageList<FloatImageType>                                              ImageListType;
      typedef ImageListToVectorImageFilter<ImageListType, FloatVectorImageType>      ImageListToVectorImageFilterType;

      /** Standard macro */
      itkNewMacro(Self);
      itkTypeMacro(GrayScaleGeodesicMorphologicalOperation, otb::Application);

    private:
      void DoInit()
      {
	SetName("GeodesicsFiltersApp");
	SetDescription("Performs Geodesic Filtering");

	// Input Output Images
	AddParameter(ParameterType_InputImage, "in",  "Input Image");
	SetParameterDescription( "in", "The input image to be filtered.");
    
	AddParameter(ParameterType_OutputImage, "out",  "Output Image");
	SetParameterDescription( "out", "Output image containing the filtered output image. ");
	SetDefaultOutputPixelType( "out", ImagePixelType_float);

	// Selected band for the filtering
	AddParameter(ParameterType_Int,  "channel",  "Selected Channel");
	SetParameterDescription("channel", "The selected channel index");
	SetDefaultParameterInt("channel", 1);
	SetMinimumParameterIntValue("channel", 1);

	// Paramaters of the filtering
	AddParameter(ParameterType_Choice, "structype", "Structuring Element Type");
	SetParameterDescription("structype", "Choice of the structuring element type");

	//Ball
	AddChoice("structype.ball", "Ball");
	AddParameter(ParameterType_Int, "structype.ball.xradius", "The Structuring Element X Radius");
	SetParameterDescription("structype.ball.xradius", "The Structuring Element X Radius");
	SetDefaultParameterInt("structype.ball.xradius", 5);
	AddParameter(ParameterType_Int, "structype.ball.yradius", "The Structuring Element Y Radius");
	SetParameterDescription("structype.ball.yradius", "The Structuring Element Y Radius");
	SetDefaultParameterInt("structype.ball.yradius", 5);

	// Type of the filtering
	AddParameter(ParameterType_Choice, "filter", "Morphological Operation");
	SetParameterDescription("filter", "Choice of the morphological operation");
    
	// Geodesic Opening
	AddChoice("filter.gopening", "Geodesic Opening");

	//Geodesic Closing
	AddChoice("filter.gclosing", "Geodesic Closing");
    
	// Ram parameter
	AddRAMParameter();
      }

      void DoUpdateParameters()
      {
      }

      void DoExecute()
      {
	 FloatVectorImageType::Pointer inImage = GetParameterImage("in");
	 inImage->UpdateOutputInformation();
	 int nBComp = inImage->GetNumberOfComponentsPerPixel();

	 if( GetParameterInt("channel") > nBComp )
	   {
	     itkExceptionMacro(<< "The specified channel index is invalid.");
	   }

	 ExtractorFilterType::Pointer  m_ExtractorFilter = ExtractorFilterType::New();
	 m_ExtractorFilter->SetInput(inImage);
	 m_ExtractorFilter->SetStartX(inImage->GetLargestPossibleRegion().GetIndex(0));
	 m_ExtractorFilter->SetStartY(inImage->GetLargestPossibleRegion().GetIndex(1));
	 m_ExtractorFilter->SetSizeX(inImage->GetLargestPossibleRegion().GetSize(0));
	 m_ExtractorFilter->SetSizeY(inImage->GetLargestPossibleRegion().GetSize(1));
	 m_ExtractorFilter->SetChannel(GetParameterInt("channel"));
	 m_ExtractorFilter->UpdateOutputInformation();

	 if (GetParameterString("structype")== "ball")
	   {
	     BallStructuringType se;
	     RadiusType rad;
	     rad[0] = this->GetParameterInt("structype.ball.xradius");
	     rad[1] = this->GetParameterInt("structype.ball.yradius");
	     se.SetRadius(rad);
	     se.CreateStructuringElement();

	     if (GetParameterString("filter") == "gopening")
	       {
		 OpeningFilterType::Pointer GeodesicOpening = OpeningFilterType::New();
		 GeodesicOpeningFilter->SetKernel(se);
		 GeodesicOpeningFilter->SetInput(m_ExtractorFilter->GetOutput());
		 SetParameterOutputImage("out", GeodesicOpeningFilter->GetOutput());
	       }

	     else if (GetParameterString("filter") == "gclosing")
	       {
		 ClosingFilterType::Pointer GeodesicClosingFilter = ClosingFilterType::New();
		 GeodesicClosingFilter->SetKernel(se);
		 GeodesicClosingFilter->SetInput(m_ExtractorFilter->GetOutput());
		 SetParameterOutputImage("out", GeodesicClosingFilter->GetOutput());
	       } 
	   }
      }
    };
  }
}
OTB_APPLICATION_EXPORT(otb::Wrapper::GrayScaleGeodesicMorphologicalOperation)
