#include <iostream>
#include "mapa.cpp"
#include <fstream>
#include <chrono>
#include "recorde.cpp"
Mapa *mapa;
Recordes *recordes = new Recordes;
const std::string ARQ_CONFIG = "config.cfg";
const std::string AJUDA = "ajuda.txt";


void ImprimirMapa(){
  std::cout << "  ";
  for(int i = 0; i < mapa->RetornaConfiguracao().qtdColunas ;i++ ){
    std::cout << (i > 9 ? "" : " ") << i << " ";
  }
  std::cout << "\n";
  int index = 0;
  for(std::vector < Celula > linha : mapa->retornaCampoImpressao()){
    std::cout << char(65 + index++) << " ";
    for(Celula coluna : linha){
      if(coluna.isBomba())
        std::cout << " N ";
      else if(coluna.getBandeira())
        std::cout << " |>";
      else if(coluna.isBomba() && coluna.isVisivel())
        std::cout << " B ";
      else if(coluna.isVisivel())
        std::cout << " " << coluna.getQtdBombas() << " ";       
      else
        std::cout << " X ";
    }
    std::cout << "\n";
  }
}
void exibirAjuda(){
  std::ifstream arquivo;
  arquivo.open ( AJUDA.c_str(), std::ifstream::in);
  if(arquivo.is_open()){
   while (! arquivo.eof() ) {
   std::string linha;
   getline(arquivo,linha);
   std::cout << linha << std::endl;
   }    
  } 
}
void gravarDificuldade(Dificuldade dif){
  std::ofstream arquivo;
  arquivo.open (ARQ_CONFIG.c_str(), std::ifstream::out);
  if(arquivo.is_open()){
    switch(dif){
      case Dificuldade::iniciante: arquivo << 'b'; break;
      case Dificuldade::intermediario: arquivo << 'i'; break;
      case Dificuldade::avancado: arquivo << 'a'; break;
    }
    arquivo.close();
  }
}
void lerDificuldade(){
    std::ifstream file;
  file.open (ARQ_CONFIG.c_str(), std::ifstream::in);  
  if(file.is_open()){
    char c;
    file >> c;
    switch(c){
      case 'b': mapa = new Mapa(*(new Configuracao(Dificuldade::iniciante))); break;
      case 'i': mapa = new Mapa(*(new Configuracao(Dificuldade::intermediario))); break;
      case 'a': mapa = new Mapa(*(new Configuracao(Dificuldade::avancado))); break;
      default:  mapa = new Mapa(*(new Configuracao(Dificuldade::iniciante)));
    }
  file.close();  
}
}
void inicializarMapa(){
  lerDificuldade();
  mapa->inicializaMapa(); 
}
void exibirRecordePorDificuldade (){
  lerDificuldade();
  std::cout << "Nome | Dif. | Tempo\n";
  char dif;
  for(Recorde r : recordes->ler_recordes()){
    switch(mapa->RetornaConfiguracao().dif){
      case Dificuldade::iniciante: dif ='b'; break;
      case Dificuldade::intermediario: dif = 'i'; break;
      case Dificuldade::avancado: dif = 'a'; break;
    }
    if(r.dificuldade == dif)
      std::cout << r.NomeJogador << " | "  << r.dificuldade << " | " << r.RecordeTempo << "\n";
  };
}

void exibirTodosOsRecordes(){
   std::cout << "Nome | Dif. | Tempo\n";
  for(Recorde r : recordes->ler_recordes()){
    std::cout << r.NomeJogador << " | "  << r.dificuldade << " | " << r.RecordeTempo << "\n";
  }; 
}
bool lerArgumentos(int argc, char ** argv){
  if(argc > 1){
    std::string argumento = argv[1];
    if(argumento == "-h" || argumento == "--help"){
      exibirAjuda();
      return false;
    }
    else if(argumento == "-r" || argumento == "--records"){
      exibirRecordePorDificuldade();
      return  false;
    }
    else if(argumento == "-ar" || argumento == "--allrecords"){
      exibirTodosOsRecordes();
      return false;
    }      
    else if(argumento == "-d" || argumento == "--difficulty"){
      if(argc > 2){
        std::string dif = argv[2];
        if(dif == "-b" || dif == "--beginner"){
          gravarDificuldade(Dificuldade::iniciante);
          return true;
        } else if(dif == "-i" || dif == "--intermediary"){
          gravarDificuldade(Dificuldade::intermediario);
          return true;
        } else if(dif == "-a" || dif == "--advanced"){
          gravarDificuldade(Dificuldade::avancado);
          return true;
        } else {
          std::cout << "Argumento desconhecido para dificuldade: " << dif << std::endl;
          exibirAjuda();
          return false;
        }
      } else {
        std::cout << "É esperado uma seleção de dificuldade: " << argv[1] << std::endl;
        exibirAjuda();
        return false;
        
      }
    } else {
      std::cout << "Argumento desconhecido: " << argv[1] << std::endl;
      exibirAjuda();
      return false;    
    }
  } else {
    return true;
  }
}
bool rodarJogo(){
  while(true){
    ImprimirMapa();
    char acao;
    int x = 0; 
    char y = 0;
    std::cout << "Informe ação, letra e número, separados por espaço: ";
    std::cin >> std::ws >> acao >> y >> x;
     
    if(acao != 'B' && acao != 'R'){
       std::cout << "Comando inválido! 'R' para revelar uma posição 'B' para adicionar/remover bandeira\n";
      continue;
    } 
    switch(acao){
      case 'B': mapa->alternarBandeira(x,y-65); break;
      case 'R': mapa->RevelarPosicao(x,y-65); break;
      default: std::cout << "Ação inválida! Tente ações válidas: 'B' para incluir bandeira e 'R' para revelar a posição\n";
    }
    std::string resultadoAcao = mapa->verificarEstadoJogo();
    if(resultadoAcao != ""){
      ImprimirMapa();
      std::cout << resultadoAcao;
      if(resultadoAcao.find("todas")!=std::string::npos)
        return true;      
      return false;
    }
  }  
}
int main(int argc, char ** argv){
  if(lerArgumentos(argc,argv)){
    inicializarMapa();
    std::chrono::time_point<std::chrono::system_clock> inicio, fim;
    inicio = std::chrono::system_clock::now();
    bool vitoria = rodarJogo();
    fim = std::chrono::system_clock::now();
    if(vitoria){
      std::string nome;
      std::cout << "Informe seu nome: ";
      std::cin >> nome;
      std::chrono::duration<double> tempoPassado = fim - inicio;
      char dificuldade;
      switch(mapa->RetornaConfiguracao().dif){
        case Dificuldade::iniciante: dificuldade = 'b';break;
        case Dificuldade::intermediario: dificuldade = 'i';break;
        case Dificuldade::avancado: dificuldade = 'a';break;    
      }
      recordes->ler_recordes();
      recordes->adicionaRecorde({nome,tempoPassado.count(),dificuldade});
      recordes->escreve_recordes_arquivos();
      
    }
  }
  return 0;
}
  
