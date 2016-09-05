#include <string>
#include <fstream>
#include <TF1.h>
#include "gensignal.h"

GenSignal::GenSignal(TRandom3 *rand, string prefix, double luminosity_fraction){
	rnd=rand;
	global_prefix=prefix;
	luminosity_frac=luminosity_fraction;
	N0=2.9;
	E0=2.4;
	cuts=false;
	cutMin=0.009;
	cutMax=0.02;
	N_BIN = 100;
	t_max = 0.5;
	y_max = 0.04;
	x_max = 0.001;
	theta_max=0.0001;
	l_1=10;
	l_2=270;
	
	p = 7000;
	hist_t = new TH1D((global_prefix+"t").c_str(), (global_prefix+"t").c_str(), N_BIN, 0, t_max);
	hist_phi = new TH1D((global_prefix+"phi").c_str(), (global_prefix+"phi").c_str(), N_BIN, 0, 2*M_PI);
	hist_phi->SetMinimum(0);
	hist_x = new TH1D((global_prefix+"x").c_str(), (global_prefix+"x").c_str(), N_BIN, -x_max, x_max);
	hist_y = new TH1D((global_prefix+"y").c_str(), (global_prefix+"y").c_str(), N_BIN, -y_max, y_max);
	hist_theta = new TH1D((global_prefix+"theta").c_str(), (global_prefix+"theta").c_str(), N_BIN, 0,theta_max);
	hist_theta_sq = new TH1D((global_prefix+"theta_sq").c_str(), (global_prefix+"theta_sq").c_str(), 
		N_BIN, 0, theta_max * theta_max);
	hist_theta_y = new TH1D((global_prefix+"theta_y").c_str(), (global_prefix+"theta_y").c_str(), N_BIN, -theta_max, theta_max);
	}
	
void GenSignal::enableCuts()
{
	cuts=true;
}

void GenSignal::disableCuts()
{
	cuts=false;
}


void GenSignal::generateGenEvent(double bgr, double tau){
		double los=rnd->Rndm();
		if (true)//los>bgr) //czy sygnał czy tło
		{
			t =rnd->Exp(tau);
			los=rnd->Uniform(2*M_PI);
			phi =los;//+0.2*cos(los-M_PI/4.)-0.15;
			
			calcTheta();
			calcThetaX();
			calcThetaY();
			
			cut_status=false;
			calcY();
			if (!cuts || y>0)//(abs(y)<cutMax && abs(y)>cutMin ))
			{
				calcX();
				hist_y->Fill(y);
				hist_x->Fill(x);
				hist_t->Fill(t,luminosity_frac);
				hist_phi->Fill(phi);
				hist_theta->Fill(theta);
				hist_theta_sq->Fill(theta * theta);
				hist_theta_y->Fill(theta_y);
				cut_status=true;
			}
		}
		
		else
		{
			//event=-(pow(1.-event,1./(1.-N0))-1.0)*E0;
		}
	
}
	
double GenSignal::calcTheta(){
		theta = sqrt(t) / p;
		return theta;
	}
	
double GenSignal::calcThetaX() {
		theta_x = theta * cos(phi);
		return theta_x;
	}
	
double GenSignal::calcThetaY() {
		theta_y = theta * sin(phi);
		return theta_y;
	}
	
double GenSignal::calcX(){
		x = theta_x * l_1;
		return x;
	}
	
double GenSignal::calcY(){
		y = theta_y * l_2;
		return x;
	}

// double fit_fun_exp(double *x, double *par) {
// 	return par[0] * exp(-x[0] / par[1]);
// }
	
void GenSignal::saveHistos(){
		TCanvas *cnv=new TCanvas();
		cnv->cd();

		TF1 *fun_exp = new TF1("fun_exp", "[0] * exp(-[1] * x)", 0, t_max);
		fun_exp->SetParameters(10000, 0.05);
		hist_t->Fit("fun_exp");
		hist_t->Draw();
		fun_exp->Draw("same");
		cnv->SaveAs((global_prefix+"t.png").c_str());
		TF1 *fun_fit = hist_t->GetFunction("fun_exp");
		
		ofstream fout;
		fout.open((global_prefix + "fit_t.txt").c_str());
		fout << "fun: A * exp(-B * x)\n";
		fout << "fitted:\n";
		fout << "A = " << fun_fit->GetParameter(0) << ", err = " << fun_fit->GetParError(0) << endl;
		fout << "B = " << fun_fit->GetParameter(1) << ", err = " << fun_fit->GetParError(1) << endl;
		fout.close();

		hist_phi->Draw();
		cnv->SaveAs((global_prefix+"phi.png").c_str());
		hist_x->Draw();
		cnv->SaveAs((global_prefix+"x.png").c_str());
		hist_y->Draw();
		cnv->SaveAs((global_prefix+"y.png").c_str());
		hist_theta->Draw();
		cnv->SaveAs((global_prefix+"theta.png").c_str());
		hist_theta_y->Draw();
		cnv->SaveAs((global_prefix+"theta_y.png").c_str());

		//log scale histograms
		//cnv->Clear();
		cnv->SetLogy();
		hist_t->Draw();
		cnv->SaveAs((global_prefix + "t_log.png").c_str());

		hist_theta->Draw();
		cnv->SaveAs((global_prefix + "theta_log.png").c_str());
		
		hist_theta_sq->Draw();
		cnv->SaveAs((global_prefix + "theta_sq_log.png").c_str());
		cnv->Close();
	}

	double GenSignal::getTmax(){
		return t_max;
	}

	string GenSignal::getPrefix(){
		return global_prefix;
	}