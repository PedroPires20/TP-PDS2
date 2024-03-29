#ifndef BUSCA_H
#define BUSCA_H
#include <string>
#include <map>
#include <set>
#include <vector>
#include "Documento.h"

using std::string;
using std::map;
using std::set;
using std::vector;

class MBus{
    public:
        //Constroi uma base de dados com os documentos de docs.
        MBus(vector<Documento> docs);
        //Insere o documento doc na base de dados.
        void inserir_doc(Documento doc);
        //Remove o documento de nome "doc" da base de dados.
        void remover_doc(string doc);
        //Atualiza o índice invertido e as coordenadas.
        void att_doc();
        //Realiza uma busca pela palavra "Q" na base de dados (Q é inicializada como "Documento").
        vector<int> consulta(Documento &Q);
        /*Retorna o nome do documento de índice "id" na base de dados. 
        A função retorna ""  caso o id não seja encontrado.*/
        string nome_doc(int id);
    private:
        int N_docs_;
        //vector dos documetnos
        vector<Documento> docs_;
        //map de palavras para um set com o índice dos documentos.
        map<string,set<int>> id_inv_;
        //Calcula o índice invertido para a base de dados atual.
        void calcula_id_inv();
        //Calcula as coordenadas de um documento.
        void calcula_coord();
    friend class Teste_MBus;
};

#endif //BUSCA_H 