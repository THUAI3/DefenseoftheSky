#include "./player.hpp"

#define DEBUG 1

int main(){
	FILE *fp;
	Client* client = new Client();
	client->init();
	if(client->parameters->num == 0)fp = fopen("log0.txt", "w");
	else fp = fopen("log1.txt", "w");
#ifdef DEBUG
	client->parameters->Debug(fp);
#endif
	while(true){
		client->stateInfo();
#ifdef DEBUG
		client->state->Debug(fp);
#endif
		getOperations(client->parameters, client->state, client->opt);
#ifdef DEBUG
		client->opt->Debug(fp);
#endif
		client->sendOpt(fp);
	}
	fclose(fp);
	delete client;
}