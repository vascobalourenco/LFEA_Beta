void hist()
{
    //Abertura o ficheiro ASC
    fstream file;
    file.open("2BTL.ASC", ios::in);

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

    //Criação do canvas
    TCanvas *c1 = new TCanvas();
    hist->SetStats(0);
    c1->SetTickx();
    c1->SetTicky();
    c1->SetGridx();
    c1->SetGridy();
    hist->Draw("");
    hist->Fit("fit","R");
}