#ifndef RECORDEDAO_HPP
#define RECORDEDAO_HPP

struct Recorde{
    std::string NomeJogador;
    double RecordeTempo;
    char dificuldade;
} ;
int RetornaValorDificuldade(char dificuldade);
bool comparaRecorde(Recorde r1, Recorde r2);
#endif