//---------------------------------------------------------------------------
#ifndef circleHoughH
#define circleHoughH

#include "kfc.h"


using namespace std;

struct KCIRCLEHOUGH_INFO
{
	int		nLeft;
	int		nRight;
	int		nTop;
	int		nBottom;
	int		nMinR;
	int		nMaxR;
	int		nNumGridCx;
	int		nNumGridCy;
	int		nNumGridR;
    int		nDirEdge; 	    //defined in "kfc.h"
	int		nThreshVote;

};

class KCircleHough : public KObject
{
	KCIRCLEHOUGH_INFO	_oInfo;
    vector<vector<vector<double>>> _lllVotes;
    double				_dSx;
    double				_dSy;
    double				_dSr;

    //출력값
    KCircle             _oDetected;

public:
    KCircleHough(){}
    virtual ~KCircleHough(){}


    virtual int    Init(void* pInfo);
    virtual void*  Info(){ return &_oInfo; }
    virtual void*  Run(void* pEdge,void* pFrom=0,void* pTo=0,void* p4=0,void* p5=0);
};

//---------------------------------------------------------------------------
#endif

class GHT {
public:
    GeneralizedHoughTransform(const vector<KPoint>& points) : points_(points) {}

    void detectCircle(const std::vector<KPoint>& edgePoints, int radiusThreshold) {
        // Circle Hough Transform
        std::vector<std::vector<int>> accumulator(points_.size(), std::vector<int>(points_.size(), 0));

        for (const auto& edgePoint : edgePoints) {
            for (int i = 0; i < points_.size(); ++i) {
                int dx = edgePoint._dX - points_[i]._dX;
                int dy = edgePoint._dY - points_[i]._dY;
                int radius = std::sqrt(dx * dx + dy * dy);

                if (radius < points_.size()) {
                    accumulator[i][radius]++;
                }
            }
        }

        // Find maximum in accumulator
        int maxVotes = 0;
        int centerX = 0;
        int centerY = 0;

        for (int i = 0; i < points_.size(); ++i) {
            for (int radius = 0; radius < points_.size(); ++radius) {
                if (accumulator[i][radius] > maxVotes) {
                    maxVotes = accumulator[i][radius];
                    centerX = points_[i]._dX;
                    centerY = points_[i]._dY;
                }
            }
        }

        // Output the detected circle
        std::cout << "Detected Circle:" << std::endl;
        std::cout << "Center: (" << centerX << ", " << centerY << ")" << std::endl;
        std::cout << "Radius: " << maxVotes << std::endl;

        if (maxVotes >= radiusThreshold) {
            std::cout << "Circle detected with high confidence." << std::endl;
        } else {
            std::cout << "Circle detected with low confidence." << std::endl;
        }
    }
};
