#include "Busca.h"
#include <math.h>
#include <utility>
#include <algorithm>
#include <exception>

using std::unordered_multiset;

MBus::MBus(std::vector<Documento> doc){
    N_docs_ = doc.size();
    docs_ = doc;
    calcula_id_inv();
    calcula_coord();
} 

void MBus::calcula_id_inv(){
    id_inv_.clear();
    for (int i=0; i<docs_.size(); i++){
        for (auto &t : docs_[i].tokens()){
            id_inv_[t].insert(docs_[i].id());
        }
    }
}

void MBus::calcula_coord(){
    for (auto &d : docs_){
        vector <double> W;
        for (auto &t : id_inv_){
            W.push_back( (log(N_docs_) - log(t.second.size())) * d.freq(t.first)); 
        }
        d.Set_coord(W);
    }
}

bool sortinrev(const std::pair<double,int> &a, const std::pair<double,int> &b) { 
       return (a.first > b.first); 
} 

vector <int> MBus::consulta(Documento &Q){
    //unordered_multiset<string> Q_tokens = Q.tokens();
    vector <double> QW;
    for (auto &t : id_inv_){
        QW.push_back( (log(N_docs_ ) - log( t.second.size())) * Q.tokens().count(t.first));
    }

    vector <std::pair<double,int>> dist_Q_docs;
    double soma_q = 0;
    for (int i=0; i<QW.size(); i++){
        soma_q += QW[i] * QW[i];
    }
    for (int i=0; i<docs_.size(); i++){
        double soma_coord = 0, soma_doc = 0;
        vector <double> doc_coord = docs_[i].coord();
        for (int j=0; j<QW.size(); j++){
            soma_coord += QW[j] * doc_coord[j];
            soma_doc += doc_coord[j] * doc_coord[j];
        }
        if (soma_coord == 0.0 || soma_doc == 0.0 || soma_q == 0){
            dist_Q_docs.push_back( std::make_pair(0.0 , docs_[i].id()) );
        }
        else{
            dist_Q_docs.push_back( std::make_pair( soma_coord / (sqrt(soma_q) * sqrt(soma_doc)) , docs_[i].id()) );
        }
    }
    sort (dist_Q_docs.begin(), dist_Q_docs.end(), sortinrev);
   
   vector <int> docs_sorted;
   for (auto &i : dist_Q_docs){
       docs_sorted.push_back(i.second);
   }
   return docs_sorted;
}

void MBus::inserir_doc(Documento doc){
    for (auto &d : docs_){
        if (d.nome() == doc.nome()){
            throw std::invalid_argument("O documento informado já se encontra na base de dados");
        }
    }
    docs_.push_back(doc);
    N_docs_++;
    for (auto &t : doc.tokens()){
        id_inv_[t].insert(doc.id());
    }
    calcula_coord();
}

void MBus::remover_doc(string Doc){
    bool achou = false;
    for (int i=0; i<docs_.size(); i++){
        if (docs_[i].nome() == Doc){
            N_docs_--;
            docs_.erase(docs_.begin()+i);
            achou = true;
            calcula_id_inv();
            calcula_coord();
        }
    }
    if (achou == false){
        throw std::invalid_argument("Os termos informados não puderam ser encontrados na base de dados");
    }
    
}

void MBus::att_doc(){
    calcula_id_inv();
    calcula_coord();
}

string MBus::nome_doc(int id){
    if (docs_[id].id() == id){
        return docs_[id].nome();
    }
    for(int i = 0; i < docs_.size(); i++){
        if(docs_[i].id() == id){
            return docs_[i].nome();
        }
    }
    return "";
}