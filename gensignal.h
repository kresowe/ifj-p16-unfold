#include <TH1D.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <cmath>
#include <iostream>
#include <string>

#ifndef GENSIGNAL_H
#define GENSIGNAL_H

using namespace std;

class GenSignal
{
protected:
	double N0;
	double E0;
	bool cuts;
	double t_max;
	double y_max;
	double x_max;
	double theta_max;
	int N_BIN;
	string global_prefix;
public:
	bool cut_status;
	double cutMin;
	double cutMax;
	double l_1;
	double l_2;
	double x;
	double y;
	double theta;
	double theta_x;
	double theta_y;
	double p; //ped
	double t; //czteroped
	double phi;
	TRandom3 *rnd;
	
	double luminosity_frac; // luminocity_data/luminocity_mc
	
	TH1D *hist_t;
	TH1D *hist_phi;
	TH1D *hist_y;
	TH1D *hist_x;
	TH1D *hist_theta_y;
	TH1D *hist_theta;
	TH1D *hist_theta_sq;
	
	GenSignal(TRandom3 *, string prefix="", double luminosity_fraction=1);
	void enableCuts();
	void disableCuts();
	void generateGenEvent(double bgr=0.1, double tau=0.05);
	double calcTheta();
	double calcThetaX();
	double calcThetaY();
	double calcX();
	double calcY();
	virtual void saveHistos();
	double getTmax();
	string getPrefix();
};

#endif // GENSIGNAL_H
