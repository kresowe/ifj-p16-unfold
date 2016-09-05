#include "detalpha.h"


DetAlpha::DetAlpha(TRandom3 *rand, string prefix, double luminosity_fraction) : GenSignal(rand, prefix,luminosity_fraction)
{
    theta_max = 0.002;
    theta_real_max=0.0001;
    sigma=30*pow(10,-5.5);
    detNbinCoef=3;
    hist2d_x_y_alpha = new TH2D((global_prefix+"x_y").c_str(), (global_prefix+"x_y").c_str(), N_BIN, -y_max, y_max, N_BIN, -y_max, y_max);
    hist_theta_y_alpha= new TH1D((global_prefix+"theta_y_alpha").c_str(), (global_prefix+"theta_y_alpha").c_str(), 
								 N_BIN*detNbinCoef, -theta_max/10, theta_max/10);
    hist_t_alpha = new TH1D((global_prefix+"t_alpha").c_str(), (global_prefix+"t_alpha").c_str(), N_BIN*detNbinCoef, 0, t_max);
    hist_t_y_alpha = new TH1D((global_prefix+"t_y_alpha").c_str(), (global_prefix+"t_y_alpha").c_str(), N_BIN*detNbinCoef, 0, t_max);
    hist2d_t_t_alpha = new TH2D((global_prefix+"t_t_alpha").c_str(), (global_prefix+"t_t_alpha").c_str(), 
								N_BIN*detNbinCoef, 0, t_max, N_BIN, 0, t_max);
    hist2d_t_t_y_alpha = new TH2D((global_prefix+"t_t_y_alpha").c_str(), (global_prefix+"t_t_y_aplha").c_str(), 
								  N_BIN*detNbinCoef, 0, t_max, N_BIN, 0, t_max);
    hist_theta_alpha = new TH1D((global_prefix+"Theta_alpha").c_str(),(global_prefix+"Theta_alpha").c_str(),N_BIN,0 ,theta_real_max);
	hist2d_theta_theta_y_alpha = new TH2D((global_prefix+"theta_theta_y_alpha").c_str(), (global_prefix+"theta_theta_y_y_alpha").c_str(), 
									N_BIN*detNbinCoef, -theta_real_max, theta_real_max, N_BIN, -theta_real_max, theta_real_max);
    hist2d_theta_sq_theta_y_alpha = new TH2D((global_prefix+"theta_sq_theta_y_alpha").c_str(), 
                                    (global_prefix+"theta_sq_theta_y_y_alpha").c_str(), 
                                    N_BIN*detNbinCoef, -theta_real_max, theta_real_max, N_BIN, 
                                    -theta_real_max * theta_real_max, theta_real_max * theta_real_max);
}

bool DetAlpha::calcXAlpha() {
    if (cut_status) x_alpha = rnd->Gaus(x,sigma);
    return cut_status;
}

bool DetAlpha::calcYAlpha() {
    if (cut_status) y_alpha = rnd->Gaus(y, sigma);
    return cut_status;
}

bool DetAlpha::fillXYAlpha() {
    if (cut_status) hist2d_x_y_alpha->Fill(x_alpha, y_alpha);
    return cut_status;
}

bool DetAlpha::calcThetaXAlpha() {
    if (cut_status) theta_x_alpha = x_alpha / l_1;
    return cut_status;
}

bool DetAlpha::calcThetaYAlpha() {
    if (cut_status) {
        theta_y_alpha =  y_alpha / l_2;
        hist_theta_y_alpha->Fill(theta_y_alpha);
		hist2d_theta_theta_y_alpha->Fill(theta_y_alpha, theta);
        hist2d_theta_sq_theta_y_alpha->Fill(theta_y_alpha, theta * theta);
    }
    return cut_status;
}

bool DetAlpha::calcThetaAlpha() {
    if (cut_status) {
        theta_alpha = sqrt(theta_x_alpha * theta_x_alpha + theta_y_alpha * theta_y_alpha);
        hist_theta_alpha->Fill(theta_alpha);
    }
    return cut_status;
}

bool DetAlpha::calcTYAlpha() {
    if (cut_status) {
        t_y_alpha = pow(p * theta_y_alpha, 2);
        hist_t_y_alpha->Fill(t_y_alpha);
        hist2d_t_t_y_alpha->Fill(t_y_alpha, t);
    }
    return cut_status;
}

bool DetAlpha::calcTAlpha() {
    if (cut_status) {
        t_alpha = pow(p * theta_alpha, 2);
        hist_t_alpha->Fill(t_alpha,luminosity_frac);
        hist2d_t_t_alpha->Fill(t_alpha, t,luminosity_frac);
    }
    return cut_status;
}

void DetAlpha::saveHistos() {
    GenSignal::saveHistos();
    TCanvas *cnv = new TCanvas();
    cnv->cd();
    hist2d_x_y_alpha->Draw("colz");
    cnv->SaveAs((global_prefix+"x_y_alpha.png").c_str());
    hist_theta_y_alpha->Draw();
    cnv->SaveAs((global_prefix+"theta_y_alpha.png").c_str());
    hist_theta_alpha->Draw();
    cnv->SaveAs((global_prefix+"theta_alpha.png").c_str());
    hist_t_y_alpha->Draw();
    cnv->SaveAs((global_prefix+"t_y_alpha.png").c_str());
    hist_t_alpha->Draw();
    cnv->SaveAs((global_prefix+"t_alpha.png").c_str());
    hist2d_t_t_y_alpha->Draw("colz");
    cnv->SaveAs((global_prefix+"t_ty_alpha.png").c_str());
    hist2d_t_t_alpha->Draw("colz");
    cnv->SaveAs((global_prefix+"t_t_alpha.png").c_str());
	hist2d_theta_theta_y_alpha->Draw("colz");
    cnv->SaveAs((global_prefix+"theta_theta_y_alpha.png").c_str());
    cnv->Close();
}

void DetAlpha::calcOutput()
{
    calcXAlpha();
    calcYAlpha();
    fillXYAlpha();
    calcThetaXAlpha();
    calcThetaYAlpha();
    calcThetaAlpha();
    calcTYAlpha();
    calcTAlpha();
}

void DetAlpha::setSigma(double sgm)
{
    sigma=sgm;
}