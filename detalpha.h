#include "gensignal.h"
#include <TH2D.h>
#include <iostream>
#include <TRandom.h>
#include <TRandom3.h>

#ifndef DETALPHA_H
#define DETALPHA_H

using namespace std;

class DetAlpha : public GenSignal
{
private:
	double theta_max;
	double theta_real_max;
	double detNbinCoef;
public:
	double sigma;
	double x_alpha;
	double y_alpha;
	double theta_y_alpha;
	double theta_x_alpha;
	double theta_alpha;
	double t_y_alpha;
	double t_alpha;
	
	GenSignal *input;
	
	TH2D *hist2d_x_y_alpha;
	TH1D *hist_theta_y_alpha;
	TH1D *hist_t_alpha;
	TH1D *hist_t_y_alpha;
	TH2D *hist2d_t_t_alpha;
	TH2D *hist2d_t_t_y_alpha;
	TH1D *hist_theta_alpha;
	TH2D *hist2d_theta_theta_y_alpha;
	
	//TRandom3 *rnd;
	
	DetAlpha(TRandom3 *, string prefix="", double luminosity_fraction=1);
	void setSigma(double);
	bool calcXAlpha();
	bool calcYAlpha();
	bool fillXYAlpha();
	bool calcThetaXAlpha();
	bool calcThetaYAlpha();
	bool calcThetaAlpha();
	bool calcTYAlpha();
	bool calcTAlpha();
	void saveHistos();
	void calcOutput();
};

#endif // DETALPHA_H
