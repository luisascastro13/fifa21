
struct s_ratings{
	int user_id;
	int sofifa_id;
	float rating;
};

vector<s_ratings> getUserRatings(vector<s_ratings> v_ratings, int user_id){
	vector<s_ratings> res;
	for (int x=0;x<v_ratings.size();x++){
		if (v_ratings[x].user_id == user_id){
			res.push_back(v_ratings[x]);
			
		}
	}
	return res;
}

void ordenaRatings(vector<s_ratings>& vet){ //shell sort
	int h=1;
	while (h< vet.size()/3){
		h = 3*h+1;//knuth
	} 
    int pos, auxpos;
	s_ratings num;
	while (h>0){
		for (int pos=h;pos<vet.size();pos++){//começa comparando numero na posição h
			num = vet[pos]; //numero na posição pos

			auxpos = pos; //salva posição atual

			//enquanto posição>= ao tamanho do "pulo" h &&
			//o número na posição pos for maior que o número na posição auxpos-h
			while (auxpos>=h && num.rating >vet[auxpos-h].rating){

				vet[auxpos] = vet[auxpos-h]; //troca de posição
				auxpos = auxpos - h;
			}
			vet[auxpos] = num;
		}
		h= (h-1)/3; //knuth
    }		
}
