enum class Dificuldade : int
{
  iniciante,intermediario,avancado 
};
class Configuracao{
public:
  int dificuldade;
  int qtdLinhas;
  int qtdColunas;
  int qtdBombas;
  Dificuldade dif;
  Configuracao (Dificuldade dif){
    this-> dif = dif;
    InicializaItens();
  }
  Configuracao(){
  
  }
  void InicializaItens(){
    switch(dif){
      case Dificuldade::iniciante:
        qtdLinhas = 10;
        qtdColunas = 10;
        qtdBombas = 10;
        break;
      case Dificuldade::intermediario:
        qtdLinhas = 15;
        qtdColunas = 15;
        qtdBombas = 40;
        break;
      case Dificuldade::avancado:
        qtdLinhas = 15;
        qtdColunas = 30;
        qtdBombas = 100;
        break;
    }
  }
};
