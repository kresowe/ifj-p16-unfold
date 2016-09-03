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

int main()
{
	gStyle->SetOptStat(111111);
	
	const int N_MC=100000000;
	const int N_DATA=10000000;
	
	TRandom3 *rnd=new TRandom3();
	
	///////////////////////////////////////////////
	//MC
	string prefix="MC_";
	DetAlpha *detector=new DetAlpha(rnd, prefix, 0.1);
	GenSignal *signal_generator=detector; //dla czytelności: by rozróżnić kiedy generowany jest
										  //event sygnału, a kiedy symulowany jest jego odczyt detektora
	signal_generator->enableCuts();
	
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
	signal_generator_data->enableCuts();
	
	for (int i = 0; i < N_DATA; i++)
	{
		signal_generator_data->generateGenEvent(0.11, 0.052);
		detector_data->calcOutput();
	}
	signal_generator_data->saveHistos();
	
	// do the unfolding
	unfold_t(detector, detector_data);
	unfold_theta_from_theta_y(detector, detector_data);

	
	return 0;
}
	


/*

	
	//Alternative MC for Systematic Err
	for (int i = 0; i < 970000; i++){
		genev = generateGenEvent(0.5, 3.6, 0.15, rnd);		//0.14, 3.6, 0.15, rnd);
		detev = generateDetEvent(abs(genev), rnd);
		MDetToGenSysMC->Fill(detev, genev);
	}
	

	double sysErrMstart=6, sysErrSize=0.1;
	
	TH2D * MGenToDetSys=new TH2D("MGenToDetSys","MGenToDetSys", nBinDet, xminDet, xmaxDet, nBinGen,xminGen, xmaxGen);
	for (int i=0; i<nBinDet+2; i++)
	{
		if (MDetData->GetBinCenter(i)>=sysErrMstart)
		{
			for (int j=0; j<nBinGen+2; j++)
			{
				MGenToDetSys->SetBinContent(i,j,sysErrSize);
			}
		}
	}
	
	unfold->AddSysError(MDetToGenSysMC, "MDetToGenSysMC", TUnfold::kHistMapOutputVert, TUnfoldSys::kSysErrModeMatrix);
	unfold->AddSysError(MGenToDetSys, "MGenToDetSys", TUnfold::kHistMapOutputVert, TUnfoldSys::kSysErrModeRelative);
	
	
	double t[1], x[1], y[1];
	logTauX->GetKnot(iBest, t[0], x[0]);
	logTauY->GetKnot(iBest, t[0], y[0]);
	
	
	TGraph *bestLCurve=new TGraph(1,x,y);
	TGraph *bestLogTauLogChi2=new TGraph(1,t,x);
	
	TH1 * Munfold=unfold->GetOutput("Unfolded");
	
	TH1 * Mrefolded=unfold->GetFoldedOutput("Folded back");
		
	//TH2 * MerrorData;
	
	mkdir("out_img", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	TCanvas *cnv=new TCanvas();
	
	MGenToDet->Draw("colz");
	cnv->SaveAs("out_img/MGenToDet.png");
	MGenMC->Draw();
	cnv->SaveAs("out_img/MGenMC.png");
	MDetMC->Draw();
	cnv->SaveAs("out_img/MDetMC.png");
	
	MDetData->Draw();
	cnv->SaveAs("out_img/MDetData.png");
	MGenData->Draw();
	cnv->SaveAs("out_img/MGenData.png");
	
	MGenMC->Draw();
	MGenData->SetLineColor(kRed);
	MGenData->Draw("same");
	cnv->SaveAs("out_img/cmp_GenMCvData.png");
	
	MDetMC->Draw();
	MDetData->SetLineColor(kRed);
	MDetData->Draw("same");
	cnv->SaveAs("out_img/cmp_DetMCvData.png");
	
	Munfold->Draw();
	cnv->SaveAs("out_img/Munfold.png");
	
	Mrefolded->Draw();
	cnv->SaveAs("out_img/Mrefolded.png");
	
	Munfold->SetLineColor(kOrange);
	Mrefolded->SetLineColor(kGreen);
	Munfold->Draw();
	Mrefolded->Draw("same");
	MGenData->Draw("same");
	MDetData->SetLineColor(kBlack);
	MDetData->Draw("same");
	cnv->SaveAs("out_img/data_to_ref_to_unf.png");
	
	cnv->Clear();
	
	lCurve->Draw();
	bestLCurve->Draw("*");
	cnv->SaveAs("out_img/lCurve.png");
	
	cnv->Clear();
	bestLogTauLogChi2->Draw();
	cnv->SaveAs("out_img/logTauLogChi2.png");
	
	cnv->Clear();
	MDetToGenSysMC->Draw("colz");
	cnv->SaveAs("out_img/MDetToGenSysMC.png");
	MGenToDetSys->Draw("colz");
	cnv->SaveAs("out_img/MGenToDetSys.png");
	
	std::cout<<"tau="<<unfold->GetTau()<<"\n";
	std::cout<<"chi**2="<<unfold->GetChi2A()<<"+"<<unfold->GetChi2L()<<" / "<<unfold->GetNdf()<<"\n";

	return 0;
}
*/