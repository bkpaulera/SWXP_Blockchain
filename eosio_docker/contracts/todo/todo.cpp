#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>

class sky_contract : public eosio::contract {
  public:
    sky_contract(account_name self)
      :eosio::contract(self),
      votacoes(_self, _self)
      {}
	
	
	
	 // um metodo no qual  vou passar os dados para incrementar ATIVIDADE e ID.
     // @abi action
    void createvota( const std::string& materia, uint64_t id) {
		votacoes.emplace(_self, [&](auto& new_votacao) {
        new_votacao.id  = id;
        new_votacao.materia = materia;
		new_votacao.concordo = 0;
		new_votacao.discordo = 0;
		new_votacao.nulo = 0;
        new_votacao.completed = 0;
      });

      eosio::print("votacao#", " criada com sucesso");
    }
	
	
	 // exclui a tabela gerada pela votacao
     // @abi action
    void destroy(account_name author, const uint64_t id) {
      auto votacao_lookup = votacoes.find(id);
      votacoes.erase(votacao_lookup);

      eosio::print("votacaos#", " tabela de votacao destruida destroyed");
    }

     // @abi action
    void complete(account_name author, const uint64_t id) {
      auto votacao_lookup = votacoes.find(id);
      eosio_assert(votacao_lookup != votacoes.end(), "Todo does not exist");

      votacoes.modify(votacao_lookup, author, [&](auto& modifiable_votacao) {
      modifiable_votacao.completed = 1;
      });
	
      eosio::print("votacaos#", " marked as complete");
    }
	
     // metodo para realizar a votacao NOME DE USUARIO,  ID DO VOTA, A, B, C
     // @abi action
    void realizavota(account_name author,uint64_t id_votacao ,uint64_t a,uint64_t b,uint64_t c) {
     	
	auto votacao_lookup = votacoes.find(id_votacao);
	
      	eosio_assert(votacao_lookup != votacoes.end(), " does not exist");
	

		
      	votacoes.modify(votacao_lookup, author,[&](auto& modifiable_votacao) {
			modifiable_votacao.concordo = modifiable_votacao.concordo + a;
			modifiable_votacao.discordo = modifiable_votacao.discordo + b;
			modifiable_votacao.nulo = modifiable_votacao.nulo + c;
	});
	
	if(votacao_lookup->concordo+votacao_lookup->discordo+votacao_lookup->nulo==4){
		if((votacao_lookup->concordo)>(votacao_lookup->discordo)){
			votacoes.modify(votacao_lookup, author,[&](auto& modifiable_votacao) {
			modifiable_votacao.completed = 1;
			modifiable_votacao.result = 1; });	
		}
	
		else{	
			votacoes.modify(votacao_lookup, author,[&](auto& modifiable_votacao) {
			modifiable_votacao.completed = 0;
			modifiable_votacao.result = 0;});
		}
	}
	   	  eosio::print("votacaos#", "Votacao finalizada com sucesso");
	}

	
    
  private:
    // tabela feita
    // @abi table votacoes i64
    struct  votacao {
      uint64_t id;
      std::string materia;
      uint64_t completed;
      uint64_t concordo;
      uint64_t discordo;
      uint64_t nulo;
	uint64_t result;

      uint64_t primary_key() const { return id; }
      EOSLIB_SERIALIZE(votacao, (id)(materia)(completed)(concordo)(discordo)(nulo)(result))
    };

    typedef eosio::multi_index<N(votacoes), votacao> votacao_table;
    votacao_table votacoes;
};

EOSIO_ABI(sky_contract, (createvota)(complete)(destroy)(realizavota))
