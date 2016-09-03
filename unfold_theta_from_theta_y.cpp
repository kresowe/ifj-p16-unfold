#include <TUnfold.h>
#include <TUnfoldDensity.h>
#include "detalpha.h"

void unfold_theta_from_theta_y(DetAlpha *detector, DetAlpha *detector_data){
	cout << "--Unfold theta from theta y--\n";
	///////////////////////////////////////////////
	//UNFOLDING
	TUnfoldDensity *unfold=new TUnfoldDensity(detector->hist2d_theta_theta_y_alpha, TUnfold::kHistMapOutputVert);
	int input_err = unfold->SetInput(detector_data->hist_theta_y_alpha);
	cout << "Input error(s): " << input_err << endl;

	if (input_err >= 10000)
	{
		cout << "May not be good" << endl;
	}

	double nScan=30;
	double taumin=0, taumax=0;	
	Int_t iBest;
	TSpline *logTauX, *logTauY;
	TGraph *lCurve;
	
	iBest=unfold->ScanLcurve(nScan,taumin,taumax,&lCurve,&logTauX,&logTauY);
	
	double t[1], x[1], y[1];
	logTauX->GetKnot(iBest, t[0], x[0]);
	logTauY->GetKnot(iBest, t[0], y[0]);
	
	
	TGraph *bestLCurve=new TGraph(1,x,y);
	TGraph *bestLogTauLogChi2=new TGraph(1,t,x);
	
	TH1 * Munfold=unfold->GetOutput("Unfolded");
	
	TH1 * Mrefolded=unfold->GetFoldedOutput("Folded back");
		
	//mkdir("out_img", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	TCanvas *cnv=new TCanvas();
	
	
	Munfold->Draw();
	cnv->SaveAs("Munfold_theta.png");
	
	Mrefolded->Draw();
	cnv->SaveAs("Mrefolded_theta.png");
	
	Munfold->SetLineColor(kOrange);
	Mrefolded->SetLineColor(kGreen);
	Munfold->Draw();
	Mrefolded->Draw("same");
	cnv->SaveAs("data_to_ref_to_unf_theta.png");
	
	cnv->Clear();
	
	lCurve->Draw();
	bestLCurve->Draw("*");
	cnv->SaveAs("lCurve_theta.png");
	
	cnv->Clear();
	bestLogTauLogChi2->Draw();
	cnv->SaveAs("logTauLogChi2.png");
	
	std::cout<<"tau="<<unfold->GetTau()<<"\n";
	std::cout<<"chi**2="<<unfold->GetChi2A()<<"+"<<unfold->GetChi2L()<<" / "<<unfold->GetNdf()<<"\n";
}