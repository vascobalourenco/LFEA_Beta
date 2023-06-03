#include <iostream>
#include <TH1F.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TApplication.h>
#include <TLine.h>
#include <TLegend.h>

// Declarar a Função ReadFile
void ReadFile(const char* filename, TH1F* histogram);

int main(int argc, char** argv) {
    // Criar uma TApp e um histograma vazio
    TApplication app("app", &argc, argv);
    TH1F* hist = new TH1F("histogram", "", 150, 0, 150);

    // Chamar a função ReadFile
    ReadFile("data/2BCS.ASC", hist);

    // Personalização do histograma
    hist->SetFillColor(kGray+1);
    hist->SetFillStyle(3004);
    hist->SetLineColor(kGray+1);
    hist->SetLineWidth(1);

    // Nomeação dos eixos
    hist->GetXaxis()->SetTitle("Canal");
    hist->GetYaxis()->SetTitle("# Contagens");

    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetYaxis()->SetTitleSize(0.05);
    hist->GetXaxis()->SetLabelSize(0.05);
    hist->GetYaxis()->SetLabelSize(0.05);

    // Definir o primeiro canal como zero
    hist->GetXaxis()->SetNdivisions(10);

    // Ajuste gaussiano ao pico do Césio
    TF1 *fit = new TF1("fit", "gaus",110,121);

    // Dar parâmetros ao ajuste
    fit->SetParameters(270,115.10,4.28);

    // Nomeação dos parâmetros
    fit->SetParNames("Contante","Centroide","Sigma");

    // Personalização da curva de ajuste
    fit->SetLineColor(kRed);
    fit->SetLineStyle(1);
    fit->SetLineWidth(2);

    // Inserir uma linha no gráfico - 620 KeV (PCA)
    TLine *linha = new TLine(115.10, 0, 115.10, 350);
    linha->SetLineWidth(2);
    linha->SetLineColor(kBlue);
    linha->SetLineStyle(2);

    TCanvas *c1 = new TCanvas();
    hist->SetStats(0);
    c1->SetTickx();
    c1->SetTicky();
    c1->SetGridx();
    c1->SetGridy();
    hist->Draw();
    linha->Draw("Same");
    hist->Fit("fit","R");

    // Obter os valores de interesse no ajuste
    double chiQuad = fit->GetChisquare();
    int ndf = fit->GetNDF();

    // Imprimir os resultados
    std::cout << "Chi-Quadrado / ndf = " << chiQuad / ndf << std::endl;

    //Legenda
    TLegend *leg = new TLegend(0.145,0.11,0.50,0.37);
    leg->SetBorderSize(0);
    leg->AddEntry(hist,"Dados adquiridos", "f");
    leg->AddEntry(fit, "Ajuste ao pico de 620 keV", "l");
    leg->AddEntry(linha, "Pico de 620 keV (MCA)", "l");
    leg->Draw();

    c1->Print("graphs/Cesio.tex");

    // Run the event loop
    app.Run();
    return 0;
}