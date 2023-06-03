#include <iostream>
#include <fstream>
#include <vector>
#include <TFile.h>
#include <TH1F.h>
#include <sstream>

void ReadFile(const char* filename, TH1F* histograma) {
  std::ifstream ficheiro(filename);
  
  if (!ficheiro) std::cerr << "Erro ao abrir o ficheiro: " << filename << std::endl;
  
  std::string linha;
  int can, cnt;
  char virg;
  // Passar os dados para o histograma
  while (std::getline(ficheiro, linha)) {
    std::stringstream ss(linha);
    ss >> can >> virg >> cnt;
    histograma->SetBinContent(can+1, cnt);
  }
  ficheiro.close();
}
