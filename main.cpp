#include <iostream>
#include <cmath>
#include <sys/stat.h>
#include <string>

#include <TH1D.h>
//#include <TUnfold.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <TMath.h>
//#include <TUnfoldDensity.h>
#include <TSpline.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TAxis.h>

#include "gensignal.h"
#include "detalpha.h"

using namespace std;

void unfold_t(DetAlpha *detector, DetAlpha *detector_data);
void unfold_theta_from_theta_y(DetAlpha *detector, DetAlpha *detector_data);
void unfold_theta_sq_from_theta_y(DetAlpha *detector, DetAlpha *detector_data);

int main()
{
	gStyle->SetOptStat(111111);
	
	const int N_MC= 10000000;
	const int N_DATA=1000000;
	
	TRandom3 *rnd=new TRandom3();
	
	///////////////////////////////////////////////
	//MC
	string prefix="MC_";
	DetAlpha *detector=new DetAlpha(rnd, prefix, 0.1);
	GenSignal *signal_generator=detector; //dla czytelności: by rozróżnić kiedy generowany jest
										  //event sygnału, a kiedy symulowany jest jego odczyt detektora
	//signal_generator->enableCuts();
	
	for (int i=0; i<N_MC; i++)
	{
		signal_generator->generateGenEvent();

		detector->calcOutput();
	}
	
	detector->saveHistos();
	
	///////////////////////////////////////////////
	//DATA
	prefix="DATA_";
	DetAlpha *detector_data=new DetAlpha(rnd, prefix);
	GenSignal * signal_generator_data=detector_data;
	detector_data->setSigma(30*pow(10,-5.4));
	//signal_generator_data->enableCuts();
	
	for (int i = 0; i < N_DATA; i++)
	{
		signal_generator_data->generateGenEvent(0.11, 0.052);
		detector_data->calcOutput();
	}
	signal_generator_data->saveHistos();
	
	// do the unfolding
	unfold_t(detector, detector_data);
	unfold_theta_from_theta_y(detector, detector_data);
	unfold_theta_sq_from_theta_y(detector, detector_data);
	
	return 0;
}
	
