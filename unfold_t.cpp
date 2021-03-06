#include <fstream>
#include <TUnfold.h>
#include <TUnfoldDensity.h>
#include <TF1.h>
#include "detalpha.h"

void unfold_t(DetAlpha *detector, DetAlpha *detector_data){
	///////////////////////////////////////////////
	//UNFOLDING
	TUnfoldDensity *unfold=new TUnfoldDensity(detector->hist2d_t_t_alpha, TUnfold::kHistMapOutputVert);
	int input_err=unfold->SetInput(detector_data->hist_t_alpha);
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
	cnv->SaveAs("unfold_t.png");
		
	Mrefolded->Draw();
	cnv->SaveAs("Mrefolded.png");
	
	Munfold->SetLineColor(kOrange);
	Mrefolded->SetLineColor(kGreen);
	Munfold->Draw();
	Mrefolded->Draw("same");
	cnv->SaveAs("data_to_ref_to_unf.png");
	
	cnv->Clear();
	
	lCurve->Draw();
	bestLCurve->Draw("*");
	cnv->SaveAs("lCurve.png");
	
	cnv->Clear();
	bestLogTauLogChi2->Draw();
	cnv->SaveAs("logTauLogChi2.png");
	cnv->Close();



	TCanvas *cnv_log = new TCanvas();
	cnv_log->Clear();
	cnv_log->SetLogy();
	

	TF1 *fun_exp = new TF1("fun_exp", "[0] * exp(-[1] * x)", 0, detector->getTmax());
	fun_exp->SetParameters(10000, 0.05);
	Munfold->Fit("fun_exp");
	Munfold->Draw();
	fun_exp->Draw("same");
	cnv_log->SaveAs("unfold_t_log.png");
	cnv_log->Close();
	TF1 *fun_fit = Munfold->GetFunction("fun_exp");
	
	ofstream fout;
	fout.open((detector->getPrefix() + "fit_t_unf.txt").c_str());
	fout << "fun: A * exp(-B * x)\n";
	fout << "fitted:\n";
	fout << "A = " << fun_fit->GetParameter(0) << ", err = " << fun_fit->GetParError(0) << endl;
	fout << "B = " << fun_fit->GetParameter(1) << ", err = " << fun_fit->GetParError(1) << endl;
	fout.close();

	
	std::cout<<"tau="<<unfold->GetTau()<<"\n";
	std::cout<<"chi**2="<<unfold->GetChi2A()<<"+"<<unfold->GetChi2L()<<" / "<<unfold->GetNdf()<<"\n";
}