#include <iostream>
#include <vector>
#include "TGraph.h"
#include "TSpline.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TMath.h"
#include "fstream"
#include "TF1.h"
#include "TGraphErrors.h"

int main(int argc, char** argv) {
    TApplication app("app", &argc, argv);

    // Valores de P e G para realizar a interpolação
    std::vector<double> P_val = {0.00, 0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90, 1.00, 1.20, 1.40, 1.60, 1.80, 2.00, 2.20, 2.40};
    std::vector<double> G_val = {28.26, 28.19, 27.99, 27.67, 27.25, 26.76, 26.23, 25.66, 25.09, 24.53, 23.98, 22.95, 22.01, 21.17, 20.41, 19.72, 19.10, 18.54};

    // Criação dos objetos necssários à interpolação
    std::ofstream outputFile("graphs/ValoresG.tex");
    TGraph* g1 = new TGraph(P_val.size(), P_val.data(), G_val.data());
    TSpline3* spline = new TSpline3("spline", g1);
    TCanvas* c1 = new TCanvas("c1", "", 800, 600);
    c1->SetMargin(0.15, 0.1, 0.15, 0.1);

    // Definição das características do gráfico
    g1->SetTitle("");
    g1->GetXaxis()->SetTitle("P");
    g1->GetYaxis()->SetTitle("G(Z, W)");
    g1->SetMarkerStyle(20);
    g1->SetMarkerSize(1.0);
    g1->SetLineColor(kBlue);

    c1->SetTickx();
    c1->SetTicky();
    c1->SetGridx();
    c1->SetGridy();

    spline->SetLineColor(kRed);
    g1->Draw("AP");
    spline->Draw("same");

    // Resolução de G(W, Z) para os valores de P medidos, através dos valores dos canais
    std::vector<int> Contagens = {518, 437, 402, 340, 297, 193, 158, 119, 72, 77};
    std::vector<int> Canais = {70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120};
    std::vector<double> Energia =  {};
    std::vector<double> ResVec = {};
    double E, W, P, G, Res;
    outputFile << "\\begin{tabular}{|c|c|c|c|c|c|}" << std::endl;
    outputFile << "\\hline" << std::endl;
    outputFile << "Canal & Energia [MeV] & Contagens & W & P & G(Z, W) & $\\frac{1}{W}\\sqrt{\\frac{N(E)}{G(Z,W)}}$ \\\\" << std::endl;
    outputFile << "\\hline" << std::endl;
    for(int i=0; i<10; i++)
    {
        E = 0.0155669 + 0.00534222* Canais[i]; // Conversão de Canal para Energia
        Energia.push_back(E);
        W = E/0.511 + 1; // Conversão de Energia para W
        P = sqrt(pow(W,2) - 1); // Conversão de W para P
        G = spline->Eval(P); // Interpolação para obter G
        Res = 1/W * sqrt(Contagens[i]/G);
        Energia.push_back(E); // Energias para Kurie Plot
        ResVec.push_back(Res); // Eixo do Y Kurie Plot
        outputFile << Canais[i] << " & " << E << " & " << Contagens[i] << " & " << W << " & " << P << " & " << G << " & " << Res << " \\\\" << std::endl;
    }
    outputFile << "\\hline" << std::endl;
    outputFile << "\\end{tabular}" << std::endl;

    // Definir os limites do gráfico
    double xMin = 0;
    double xMax = 2.4;
    double yMin = 16;
    double yMax = 30;
    g1->GetXaxis()->SetRangeUser(xMin, xMax);
    g1->GetYaxis()->SetRangeUser(yMin, yMax);

    // Desenhar o canvas
    c1->Update();
    c1->Draw();
    c1->Print("graphs/interpolacao.tex");

    outputFile.close();

    // Kurie Plot
    TGraph* g2 = new TGraph(Energia.size(), Energia.data(), ResVec.data());
    TCanvas* c2 = new TCanvas("c2", "", 800, 600);
    c2->SetMargin(0.15, 0.1, 0.15, 0.1);

    // Criação do Ajuste para o Kurie plot
    TF1 *fit = new TF1("fit", "pol1", 0, 1);

    // Personalização da curva de ajuste
    fit->SetLineColor(kRed);
    fit->SetLineStyle(1);
    fit->SetLineWidth(2);

    c2->SetTickx();
    c2->SetTicky();
    c2->SetGridx();
    c2->SetGridy();

    g2->Fit("fit","R");

    // Create new TGraphErrors to store data points with errors
    TGraphErrors* graf2Errors = new TGraphErrors(graf2->GetN());
    graf2Errors->SetName("graf2Errors");
    graf2Errors->SetTitle("Kurie Plot");
    graf2Errors->GetXaxis()->SetTitle("Energia [MeV]");
    graf2Errors->GetYaxis()->SetTitle("#frac{1}{W}#sqrt{#frac{N(E)}{G(Z,W)}}");

    // Copy data points and errors to the new TGraphErrors
    for (int i = 0; i < graf2->GetN(); i++) {
        double x, y;
        graf2->GetPoint(i, x, y);
        double ex = graf2->GetErrorX(i);
        double ey = graf2->GetErrorY(i);
        graf2Errors->SetPoint(i, x, y);
        graf2Errors->SetPointError(i, ex, ey);
    }

    graf2Errors->Draw("AP");  // Draw the new TGraphErrors with data points and fit

    c2->Print("graphs/kurie.tex");

    app.Run();

    delete spline;
    delete grafico;
    delete c1;
    delete c2;
    delete fit;
    delete graf2Errors;

    return 0;
}
