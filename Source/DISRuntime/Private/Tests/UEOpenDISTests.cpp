#include "DIS_BPFL.h"
#include "DISEnumsAndStructs.h"
#include "HAL/Platform.h"

BEGIN_DEFINE_SPEC(FUnitConversionSpec, "DIS.UnitConversions", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)

	struct FTestCase
{
	double LatitudeDegrees;
	double LongitudeDegrees;
	double HeightMeters;
	double EcefX;
	double EcefY;
	double EcefZ;
	float HeadingDegrees;
	float PitchDegrees;
	float RollDegrees;
	float PsiDegrees;
	float ThetaDegrees;
	float PhiDegrees;
	float HeadingRadians;
	float PitchRadians;
	float RollRadians;
	float PsiRadians;
	float ThetaRadians;
	float PhiRadians;
};
int NumTestCases = 3;
FTestCase TestCases[3] =
{
	
	{
		-34.9, 138.5, 10000., -3928260.52, 3475431.327, -3634495.175,
	   135.f, 20.f, 30.f, -123.f, 47.8f, -29.7f,
	   2.35619f, 0.349066f, 0.523599f, -2.14675f, 0.8342674f, -0.5183628f
	},
	{
		37.616570, -115.023070, 7924.8, -2142319.25172204, -4589392.96481526, 3876650.34976432,
		-90.f, 2.f, 0.f, 157.6063655f, -1.46572917235f, 128.5665726f,
		-1.5708f, 0.0349066f, 0.f, 2.75075f, -0.0255818f, 2.24391f
	},
	{
		37.633298, -116.147799, 7315.2, -2231279.89541404, -4545007.68472335, 3877752.58430422,
		90.f, 0.f, 0.f, -25.99795995f, 0.126010607883f, -127.6332244f,
		1.5708f, 0.f, 0.f, -0.45375f, 0.0021993f, -2.22762f
	}
};
float FloatTolerance = 1.05f;
END_DEFINE_SPEC(FUnitConversionSpec)
void FUnitConversionSpec::Define()
{
	Describe("CalculateHeadingPitchRoll", [this]()
		{			
			for(int Index = 0; Index < NumTestCases; Index++)
			{
				FTestCase TestCase = TestCases[Index];

				FPsiThetaPhi psiThetaPhiDegrees;
				psiThetaPhiDegrees.Psi = TestCase.PsiDegrees;
				psiThetaPhiDegrees.Theta = TestCase.ThetaDegrees;
				psiThetaPhiDegrees.Phi = TestCase.PhiDegrees;

				FPsiThetaPhi psiThetaPhiRadians;
				psiThetaPhiRadians.Psi = TestCase.PsiRadians;
				psiThetaPhiRadians.Theta = TestCase.ThetaRadians;
				psiThetaPhiRadians.Phi = TestCase.PhiRadians;

				FHeadingPitchRoll headingPitchRollDegrees;

				It(FString::Printf(
					   TEXT(
						   "should calculate Heading->\"%7.4f°\", Pitch->\"%7.4f°\", and Roll->\"%7.4f°\" from Psi=\"%7.4f°\", Theta=\"%7.4f°\", and Phi=\"%7.4f°\" at Latitude=\"%7.4f°\" and Longitude=\"%7.4f°\""),
						TestCase.HeadingDegrees, TestCase.PitchDegrees, TestCase.RollDegrees, TestCase.PsiDegrees,
					   TestCase.ThetaDegrees, TestCase.PhiDegrees, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees).Replace(TEXT("."), TEXT(",")),
				   [this, TestCase, psiThetaPhiDegrees, psiThetaPhiRadians, &headingPitchRollDegrees]()
				   {
						UDIS_BPFL::CalculateHeadingPitchRollDegreesFromPsiThetaPhiDegreesAtLatLon(psiThetaPhiDegrees, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees, headingPitchRollDegrees);
						TestEqual(FString::Printf(TEXT("Calculated Heading %7.4f° = Desired Heading %7.4f°"), headingPitchRollDegrees.Heading, TestCase.HeadingDegrees), headingPitchRollDegrees.Heading, TestCase.HeadingDegrees, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Pitch %7.4f° = Desired Pitch %7.4f°"), headingPitchRollDegrees.Pitch, TestCase.PitchDegrees), headingPitchRollDegrees.Pitch, TestCase.PitchDegrees, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Roll %7.4f° = Desired Roll %7.4f°"), headingPitchRollDegrees.Roll, TestCase.RollDegrees), headingPitchRollDegrees.Roll, TestCase.RollDegrees, FloatTolerance);
					});
				It(FString::Printf(
					TEXT(
						"should calculate Heading->%7.4f°, Pitch->%7.4f°, and Roll->%7.4f° from Psi=%7.4fr, Theta=%7.4fr, and Phi=%7.4fr at Latitude=%7.4f° and Longitude=%7.4f°"),
					TestCase.HeadingDegrees, TestCase.PitchDegrees, TestCase.RollDegrees, TestCase.PsiRadians,
					TestCase.ThetaRadians, TestCase.PhiRadians, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees).Replace(TEXT("."), TEXT(",")),
					[this, TestCase, psiThetaPhiDegrees, psiThetaPhiRadians, &headingPitchRollDegrees]()
					{
						UDIS_BPFL::CalculateHeadingPitchRollDegreesFromPsiThetaPhiRadiansAtLatLon(psiThetaPhiRadians, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees, headingPitchRollDegrees);
						TestEqual(FString::Printf(TEXT("Calculated Heading %7.4f° = Desired Heading %7.4f°"), headingPitchRollDegrees.Heading, TestCase.HeadingDegrees), headingPitchRollDegrees.Heading, TestCase.HeadingDegrees, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Pitch %7.4f° = Desired Pitch %7.4f°"), headingPitchRollDegrees.Pitch, TestCase.PitchDegrees), headingPitchRollDegrees.Pitch, TestCase.PitchDegrees, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Roll %7.4f° = Desired Roll %7.4f°"), headingPitchRollDegrees.Roll, TestCase.RollDegrees), headingPitchRollDegrees.Roll, TestCase.RollDegrees, FloatTolerance);
				   });
				It(FString::Printf(
					   TEXT(
						   "should calculate Heading->%7.4fr, Pitch->%7.4fr, and Roll->%7.4fr from Psi=%7.4f°, Theta=%7.4f°, and Phi=%7.4f° at Latitude=%7.4f° and Longitude=%7.4f°"),
					   TestCase.HeadingRadians, TestCase.PitchRadians, TestCase.RollRadians, TestCase.PsiDegrees,
					   TestCase.ThetaDegrees, TestCase.PhiDegrees, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees).Replace(TEXT("."), TEXT(",")),
				   [this, TestCase, psiThetaPhiDegrees, psiThetaPhiRadians, &headingPitchRollDegrees]()
				   {
						UDIS_BPFL::CalculateHeadingPitchRollRadiansFromPsiThetaPhiDegreesAtLatLon(psiThetaPhiDegrees, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees, headingPitchRollDegrees);
						TestEqual(FString::Printf(TEXT("Calculated Heading %7.4fr = Desired Heading %7.4fr"), headingPitchRollDegrees.Heading, TestCase.HeadingRadians), headingPitchRollDegrees.Heading, TestCase.HeadingRadians, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Pitch %7.4fr = Desired Pitch %7.4fr"), headingPitchRollDegrees.Pitch, TestCase.PitchRadians), headingPitchRollDegrees.Pitch, TestCase.PitchRadians, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Roll %7.4fr = Desired Roll %7.4fr"), headingPitchRollDegrees.Roll, TestCase.RollRadians), headingPitchRollDegrees.Roll, TestCase.RollRadians, FloatTolerance);
					});
				It(FString::Printf(
					TEXT(
						"should calculate Heading->%7.4fr, Pitch->%7.4fr, and Roll->%7.4fr from Psi=%7.4fr, Theta=%7.4fr, and Phi=%7.4fr at Latitude=%7.4f° and Longitude=%7.4f°"),
					TestCase.HeadingRadians, TestCase.PitchRadians, TestCase.RollRadians, TestCase.PsiRadians,
					TestCase.ThetaRadians, TestCase.PhiRadians, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees).Replace(TEXT("."), TEXT(",")),
					[this, TestCase, psiThetaPhiDegrees, psiThetaPhiRadians, &headingPitchRollDegrees]()
					{
						UDIS_BPFL::CalculateHeadingPitchRollRadiansFromPsiThetaPhiRadiansAtLatLon(psiThetaPhiRadians, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees, headingPitchRollDegrees);
						TestEqual(FString::Printf(TEXT("Calculated Heading %7.4fr = Desired Heading %7.4fr"), headingPitchRollDegrees.Heading, TestCase.HeadingRadians), headingPitchRollDegrees.Heading, TestCase.HeadingRadians, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Pitch %7.4fr = Desired Pitch %7.4fr"), headingPitchRollDegrees.Pitch, TestCase.PitchRadians), headingPitchRollDegrees.Pitch, TestCase.PitchRadians, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Roll %7.4fr = Desired Roll %7.4fr"), headingPitchRollDegrees.Roll, TestCase.RollRadians), headingPitchRollDegrees.Roll, TestCase.RollRadians, FloatTolerance);
				   });
			}
		});

	Describe("CalculatePsiThetaPhi", [this]()
		{
			for(int Index = 0; Index < NumTestCases; Index++)
			{
				FTestCase TestCase = TestCases[Index];

				FHeadingPitchRoll headingPitchRollDegrees;
				headingPitchRollDegrees.Heading = TestCase.HeadingDegrees;
				headingPitchRollDegrees.Pitch = TestCase.PitchDegrees;
				headingPitchRollDegrees.Roll = TestCase.RollDegrees;

				FHeadingPitchRoll headingPitchRollRadians;
				headingPitchRollRadians.Heading = TestCase.HeadingRadians;
				headingPitchRollRadians.Pitch = TestCase.PitchRadians;
				headingPitchRollRadians.Roll = TestCase.RollRadians;

				FPsiThetaPhi psiThetaPhi;

				It(FString::Printf(
					   TEXT(
						   "should calculate Psi->%7.4f°, Theta->%7.4f°, and Phi->%7.4f° from Heading=%7.4f°, Pitch=%7.4f°, and Roll=%7.4f° at Latitude=%7.4f° and Longitude=%7.4f°"),
					   TestCase.PsiDegrees, TestCase.ThetaDegrees, TestCase.PhiDegrees, TestCase.HeadingDegrees,
					   TestCase.PitchDegrees, TestCase.RollDegrees, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees).Replace(TEXT("."), TEXT(",")),
				   [this, TestCase, headingPitchRollDegrees, &psiThetaPhi]()
				   {
						UDIS_BPFL::CalculatePsiThetaPhiDegreesFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees, psiThetaPhi);
						TestEqual(FString::Printf(TEXT("Calculated Psi %7.4f° = Desired Psi %7.4f°"), psiThetaPhi.Psi, TestCase.PsiDegrees), psiThetaPhi.Psi, TestCase.PsiDegrees, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Theta %7.4f° = Desired Theta %7.4f°"), psiThetaPhi.Theta, TestCase.ThetaDegrees), psiThetaPhi.Theta, TestCase.ThetaDegrees, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Phi %7.4f° = Desired Phi %7.4f°"), psiThetaPhi.Phi, TestCase.PhiDegrees), psiThetaPhi.Phi, TestCase.PhiDegrees, FloatTolerance);
					});
				It(FString::Printf(
					TEXT(
						"should calculate Psi->%7.4f°, Theta->%7.4f°, and Phi->%7.4f° from Heading=%7.4fr, Pitch=%7.4fr, and Roll=%7.4fr at Latitude=%7.4f° and Longitude=%7.4f°"),
					TestCase.PsiDegrees, TestCase.ThetaDegrees, TestCase.PhiDegrees, TestCase.HeadingRadians,
					TestCase.PitchRadians, TestCase.RollRadians, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees).Replace(TEXT("."), TEXT(",")),
					[this, TestCase, headingPitchRollRadians, &psiThetaPhi]()
					{
						UDIS_BPFL::CalculatePsiThetaPhiDegreesFromHeadingPitchRollRadiansAtLatLon(headingPitchRollRadians, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees, psiThetaPhi);
						TestEqual(FString::Printf(TEXT("Calculated Psi %7.4f° = Desired Psi %7.4f°"), psiThetaPhi.Psi, TestCase.PsiDegrees), psiThetaPhi.Psi, TestCase.PsiDegrees, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Theta %7.4f° = Desired Theta %7.4f°"), psiThetaPhi.Theta, TestCase.ThetaDegrees), psiThetaPhi.Theta, TestCase.ThetaDegrees, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Phi %7.4f° = Desired Phi %7.4f°"), psiThetaPhi.Phi, TestCase.PhiDegrees), psiThetaPhi.Phi, TestCase.PhiDegrees, FloatTolerance);
					});
				It(FString::Printf(
					TEXT(
						"should calculate Psi->%7.4fr, Theta->%7.4fr, and Phi->%7.4fr from Heading=%7.4f°, Pitch=%7.4f°, and Roll=%7.4f° at Latitude=%7.4f° and Longitude=%7.4f°"),
					TestCase.PsiRadians, TestCase.ThetaRadians, TestCase.PhiRadians, TestCase.HeadingDegrees,
					TestCase.PitchDegrees, TestCase.RollDegrees, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees).Replace(TEXT("."), TEXT(",")),
					[this, TestCase, headingPitchRollDegrees, &psiThetaPhi]()
					{
						UDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollDegreesAtLatLon(headingPitchRollDegrees, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees, psiThetaPhi);
						TestEqual(FString::Printf(TEXT("Calculated Psi %7.4fr = Desired Psi %7.4fr"), psiThetaPhi.Psi, TestCase.PsiRadians), psiThetaPhi.Psi, TestCase.PsiRadians, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Theta %7.4fr = Desired Theta %7.4fr"), psiThetaPhi.Theta, TestCase.ThetaRadians), psiThetaPhi.Theta, TestCase.ThetaRadians, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Phi %7.4fr = Desired Phi %7.4fr"), psiThetaPhi.Phi, TestCase.PhiRadians), psiThetaPhi.Phi, TestCase.PhiRadians, FloatTolerance);
					});
				It(FString::Printf(
					TEXT(
						"should calculate Psi->%7.4fr, Theta->%7.4fr, and Phi->%7.4fr from Heading=%7.4fr, Pitch=%7.4fr, and Roll=%7.4fr at Latitude=%7.4f° and Longitude=%7.4f°"),
					TestCase.PsiRadians, TestCase.ThetaRadians, TestCase.PhiRadians, TestCase.HeadingRadians,
					TestCase.PitchRadians, TestCase.RollRadians, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees).Replace(TEXT("."), TEXT(",")),
					[this, TestCase, headingPitchRollRadians, &psiThetaPhi]()
					{
						UDIS_BPFL::CalculatePsiThetaPhiRadiansFromHeadingPitchRollRadiansAtLatLon(headingPitchRollRadians, TestCase.LatitudeDegrees, TestCase.LongitudeDegrees, psiThetaPhi);
						TestEqual(FString::Printf(TEXT("Calculated Psi %7.4fr = Desired Psi %7.4fr"), psiThetaPhi.Psi, TestCase.PsiRadians), psiThetaPhi.Psi, TestCase.PsiRadians, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Theta %7.4fr = Desired Theta %7.4fr"), psiThetaPhi.Theta, TestCase.ThetaRadians), psiThetaPhi.Theta, TestCase.ThetaRadians, FloatTolerance);
						TestEqual(FString::Printf(TEXT("Calculated Phi %7.4fr = Desired Phi %7.4fr"), psiThetaPhi.Phi, TestCase.PhiRadians), psiThetaPhi.Phi, TestCase.PhiRadians, FloatTolerance);
				   });
			}
		});

	Describe("CalculateLatitudeLongitudeHeight", [this]()
		{
			for (int Index = 0; Index < NumTestCases; Index++)
			{
				FTestCase TestCase = TestCases[Index];

				FEarthCenteredEarthFixedDouble ECEF;
				ECEF.X = TestCase.EcefX;
				ECEF.Y = TestCase.EcefY;
				ECEF.Z = TestCase.EcefZ;

				FLatLonHeightDouble latLonHeight;

				It(FString::Printf(
					   TEXT(
						   "should calculate Latitude->%7.4f°, Longitude->%7.4f°, and Height->%7.4fm from ECEF X=%14.8f, Y=%14.8f, and Z=%14.8f"),
					   TestCase.LatitudeDegrees, TestCase.LongitudeDegrees, TestCase.HeightMeters, TestCase.EcefX,
					   TestCase.EcefY, TestCase.EcefZ).Replace(TEXT("."), TEXT(",")), [this, TestCase, ECEF, &latLonHeight]()
				   {
						UDIS_BPFL::CalculateLatLonHeightFromEcefXYZ(ECEF, latLonHeight);
						TestEqual(FString::Printf(TEXT("Calculated Latitude %7.4f° = Desired Latitude %7.4f°"), latLonHeight.Latitude, TestCase.LatitudeDegrees), latLonHeight.Latitude, TestCase.LatitudeDegrees, 1.05);
						TestEqual(FString::Printf(TEXT("Calculated Longitude %7.4f° = Desired Longitude %7.4f°"), latLonHeight.Longitude, TestCase.LongitudeDegrees), latLonHeight.Longitude, TestCase.LongitudeDegrees, 1.05);
						TestEqual(FString::Printf(TEXT("Calculated Height %7.4fm = Desired Height %7.4fm"), latLonHeight.Height, TestCase.HeightMeters), latLonHeight.Height, TestCase.HeightMeters, 23);
				   });
			}
		});

	Describe("CalculateEcefXYZ", [this]()
		{
			for (int Index = 0; Index < NumTestCases; Index++)
			{
				FTestCase TestCase = TestCases[Index];

				FLatLonHeightDouble latLonHeight;
				latLonHeight.Latitude = TestCase.LatitudeDegrees;
				latLonHeight.Longitude = TestCase.LongitudeDegrees;
				latLonHeight.Height = TestCase.HeightMeters;

				FEarthCenteredEarthFixedDouble ECEF;

				It(FString::Printf(
					   TEXT(
						   "should calculate EcefX->%14.8f, EcefY->%14.8f, and Height->%14.8f from Latitude=%7.4f°, Longitude=%7.4f°, and Height=%7.4fm"),
					   TestCase.EcefX, TestCase.EcefY, TestCase.EcefZ, TestCase.LatitudeDegrees,
					   TestCase.LongitudeDegrees, TestCase.HeightMeters).Replace(TEXT("."), TEXT(",")), [this, TestCase, latLonHeight, &ECEF]()
				   {
						UDIS_BPFL::CalculateEcefXYZFromLatLonHeight(latLonHeight, ECEF);
						TestEqual(FString::Printf(TEXT("Calculated EcefX %14.8f = Desired EcefX %14.8f"), ECEF.X, TestCase.EcefX), ECEF.X, TestCase.EcefX, 10);
						TestEqual(FString::Printf(TEXT("Calculated EcefY %14.8f = Desired EcefY %14.8f"), ECEF.Y, TestCase.EcefY), ECEF.Y, TestCase.EcefY, 10);
						TestEqual(FString::Printf(TEXT("Calculated EcefZ %14.8f = Desired EcefZ %14.8f"), ECEF.Z, TestCase.EcefZ), ECEF.Z, TestCase.EcefZ, 10);
				   });
			}
		});
}
