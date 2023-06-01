void hist()
{
    //Abertura o ficheiro ASC
    fstream file;
    file.open("DADOS10M.ASC", ios::in);

    int chn, counts, roi;
    char comma;
    int n = 0;

    //Criação do histograma
    TH1F* hist = new TH1F("hist", "", 1024, 0, 1024);

    //Ignora o cabeçalho do histograma
    string line;
    for (int i = 0; i < 5; ++i) getline(file, line);
    for (int i=1030; i < 10; ++i) getline(file, line);

    //Leitura do ficheiro de dados
    while(1)
    {
        file >> chn >> comma >> counts >> comma >> roi;
        for(int n = 0; n < counts; ++n) hist->Fill(chn);
        if(file.eof()) break;
    }

    //Personalização do histograma
    hist->SetFillColor(kGray+1);
    hist->SetFillStyle(3004);
    hist->SetLineColor(kGray+1);
    hist->SetLineWidth(1);


    //Nomeação dos eixos
    hist->GetXaxis()->SetTitle("Canal");
    hist->GetYaxis()->SetTitle("# Contagens");

    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetYaxis()->SetTitleSize(0.05);
    hist->GetXaxis()->SetLabelSize(0.05);
    hist->GetYaxis()->SetLabelSize(0.05);
    
    TF1 *fit = new TF1("fit", "[0]*TMath::Exp(-0.5*((x-[1])/[2])*((x-[1])/[2]))+[3]*exp(-0.5*((x-[4])/[5])*((x-[4])/[5]))+[6]*exp(-0.5*((x-[7])/[8])*((x-[7])/[8]))",0,1024);

    //Para ajudar o fit podemos introduzir alguns parametros de entrada
    //No caso desta curva de ajuste temos três Gaussianas
    //Cada Guassiana é definida pela sua amplitude, valor médio e sigma
    //Definição dos parâmetros da função de ajuste

    fit->SetParameters(1150,590.32,19.4,117.72,585.7,19.4,22.04,579.90,19.4);

    //Nomeação dos parâmetros
    fit->SetParNames("C1","M1","S1","C2","M2","S2","C3","M3","S3");

    //Personalização da curva de ajuste
    fit->SetLineColor(kRed);
    fit->SetLineStyle(1);
    fit->SetLineWidth(2);

    //Criação do canvas
    TCanvas *c1 = new TCanvas();
    hist->SetStats(0);
    c1->SetTickx();
    c1->SetTicky();
    c1->SetGridx();
    c1->SetGridy();
    hist->Draw("");
    hist->Fit("fit","R");

    //O programa pode devolver os parâmetros após realizar o ajuste
    //Neste caso guardamos os parâmetros num array

    array<double, 9> par;
    for (int x = 0; x < par.size(); ++x) par[x] = fit->GetParameter(x);

    //Vamos agora desenhar os gráficos das gaussianas cujo
    //somatário dá a função de ajuste

    TF1 *f1 = new TF1("f1","gaus",0,1024);
    f1->SetParameters(par[0],par[1],par[2]);

    TF1 *f2 = new TF1("f2","gaus",0,1024);
    f2->SetParameters(par[3],par[4],par[5]);

    TF1 *f3 = new TF1("f3","gaus",0,1024);
    f3->SetParameters(par[6],par[7],par[8]);

    f1->SetLineColor(kMagenta);
    f2->SetLineColor(kGreen);
    f3->SetLineColor(kOrange+5);

    f1->Draw("same");
    f2->Draw("same");
    f3->Draw("same");

    //Legenda
    TLegend *leg = new TLegend(0.6,0.6,0.87,0.87);
    leg->SetBorderSize(0);
    leg->AddEntry(hist,"Dados adquiridos", "f");
    leg->AddEntry(fit, "Ajuste aos dados", "l");
    leg->AddEntry(f1, "Pico 1", "l");
    leg->AddEntry(f2, "Pico 2", "l");
    leg->AddEntry(f3, "Pico 3", "l");
    leg->Draw();

    //Criar Ficheiro em LaTeX
    c1->Print("hist10m.tex");
}