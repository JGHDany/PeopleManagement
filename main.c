#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>

#define FICH "Dados.dat" //ficheiro com os dados

#define OP_INSERIR    '1'
#define OP_ALTERAR    '2'
#define OP_APAGAR     '3'
#define OP_LISTAR     '4'
#define OP_PESQUISAR  '5'

#define OP_SAIR       '0'

#define OP_PESQ_IDADE '1'
#define OP_PESQ_NOME  '2'

char *MainMenu[] = {
	"1. Inserir Registo",
	"2. Alterar Registo",
	"3. Apagar Registo",
	"4. Listar Registos",
	"5. Pesquisas",
	"0. Sair",
	NULL /* Acabaram as opcoes */
};

char *PesqMenu[] = {
	"1. Pesquisar por Intervalo de Idades",
	"2. Pesquisar por Nomes",
	"0. Voltar",
	NULL /*Acabaram as opcoes*/
};


FILE *fp; /* variavel global pois vai ser util ao longo de todo o programa */

/* Estrutura que contem os campos de uma pessoa*/
typedef struct
{
	char Nome[30+1];
	int Idade;
	float Salario;
	char Status;    // '*' indica que o registo esta apagado
}PESSOA;

/* coloca uma mensagem no ecra */
void Mensagem(char *msg);
/* Le os dados introduzidos pelo utilizador */
void Ler_Pessoa(PESSOA *p);
/* Mostra no ecra, os dados existentes no registo */
void Mostrar_Pessoa(PESSOA p);
/* Adicciona uma pessoa ao ficheiro*/
void Adicionar_Pessoa(PESSOA p);
/* Verifica se o ficheiro ja existe, Se nao existir cria-o
   Se ja existir, abre-o em modo de leitura e escrita
*/
void Inic();
/* Faz um menu Simples com as opcoes do vector de strings.
   Selecciona a opcao, usando o 1o caracter de cada string'
   Devolve o primeiro caracter da opcao
*/
char Menu(char *Opcoes[]);
/* Permite inserir dados de uma pessoa no registo */
void Inserir_Pessoa();
/* Permite alterar dados de uma pessoa no registo */
void Alterar_Pessoa();
/* Remove as informacoes referentes a uma pessoa */
void Apagar_Pessoa();
/* Lista informacao de uma pessoa */
void Listar_Pessoa();
/* Pesquisa um conjunto de registos por um intervalo de idades [ini, fim] */
void Pesquisar_Idades(int ini, int fim);
/* Pesquisa um reisto dado o nome */
void Pesquisar_Nome(char *s);

/* Funcao Principal */
int main()
{
	char Opcao;
	Inic();
	while((Opcao = Menu(MainMenu))!= OP_SAIR){
		switch(Opcao){
			case OP_INSERIR: Inserir_Pessoa(); break;
			case OP_ALTERAR: Alterar_Pessoa(); break;
			case OP_APAGAR: Apagar_Pessoa(); break;
			case OP_LISTAR: Listar_Pessoa(); break;
			case OP_PESQUISAR:
				while((Opcao=Menu(PesqMenu))!=OP_SAIR)
					switch(Opcao){
						case OP_PESQ_IDADE:{
							int n1, n2;
							printf("Qual o intervalo de Idades: ");
							scanf("%d%d",&n1,&n2); fflush(stdin);
							Pesquisar_Idades(n1,n2);
							break;
						}
						case OP_PESQ_NOME:{
							char string[BUFSIZ+1];
							printf("Qual o Nome a procurar: ");
							gets(string); fflush(stdin);
							Pesquisar_Nome(string);
						}
					}
		}
	}
    return 0;
}

/* coloca uma mensagem no ecra */
void Mensagem(char *msg){
	printf(msg);
	getchar();
}
/* Le os dados introduzidos pelo utilizador */
void Ler_Pessoa(PESSOA *p){
	printf("Nome     : "); gets(p->Nome);
	printf("Idade    : "); scanf("%d",&p->Idade);
	printf("Salario  : "); scanf("%f",&p->Salario);
	p->Status=' ';
	fflush(stdin);
}
/* Mostra no ecra, os dados existentes no registo */
void Mostrar_Pessoa(PESSOA p){
	printf("%-30s  %3d  %10.2f\n",p.Nome, p.Idade, p.Salario);
}
/* Adicciona uma pessoa ao ficheiro*/
void Adicionar_Pessoa(PESSOA p){
	fseek(fp,0L,SEEK_END);
	//A função retorna o número de itens escritos. Este valor será igual a count a menos que ocorra algum erro.
	if(fwrite(&p, sizeof(p),1,fp)!=1)
		Mensagem("Adiccionar Pessoa: Falhou a escrita do Registo");
}

/* Verifica se o ficheiro ja existe, Se nao existir cria-o
   Se ja existir, abre-o em modo de leitura e escrita
*/
void Inic(){
	fp = fopen(FICH, "r+b"); /*tentar abrir o ficheiro*/
	if(fp == NULL){
		fp = fopen(FICH,"w+b"); /*criar o ficheiro*/
		if(fp == NULL){
			fprintf(stderr,"ERRO FATAL: Impossivel Criar o Ficheiro de Dados");
			exit(1);
		}
	}
}
/* Faz um menu Simples com as opcoes do vector de strings.
   Selecciona a opcao, usando o 1o caracter de cada string'
   Devolve o primeiro caracter da opcao
*/
char Menu(char *Opcoes[]){
	int i, v;
	char ch;
	while(1){
		/*Cls*/
		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
		for(i=0; Opcoes[i] != NULL; i++)
			printf("\t\t%s\n\n",Opcoes[i]);

		printf("\n\n\t\tOpcao: ");
		ch = getchar(); fflush(stdin);

		for(i=0; Opcoes[i] != NULL; i++)
			if(Opcoes[i][0] == ch){
				return ch;
			}
	}
}
/* Permite inserir dados de uma pessoa no registo */
void Inserir_Pessoa(){
	PESSOA x;
	Ler_Pessoa(&x);
	Adicionar_Pessoa(x);
}
/* Permite alterar dados de uma pessoa no registo */
void Alterar_Pessoa(){
	PESSOA x;
	long int n_reg;
	printf("Qual o N. do Registo: ");
	scanf("%ld",&n_reg);fflush(stdin);

	if(fseek(fp, (n_reg - 1)*sizeof(PESSOA),SEEK_SET)!=0){
		Mensagem("Registo Inexistente ou problemas no posicionamento!!!");
		return;
	}
	if(fread(&x, sizeof(PESSOA),1,fp)!=1){
		Mensagem("Problemas na Leitura do Registo !!!");
		return;
	}
	if(x.Status =='*'){
		Mensagem("Um Registo Apagado nao pode ser Alterado !!!");
		return;
	}
	printf("\n\nDados Actuais\n\n");
	Mostrar_Pessoa(x);
	printf("\n\nNovos Dados\n\n");
	Ler_Pessoa(&x);
	//recuar um registor no ficheiro
	fseek(fp, -(long)sizeof(PESSOA),SEEK_CUR);
	//reescrever o registo;
	fwrite(&x, sizeof(PESSOA),1,fp);
	fflush(fp); /*Despejar os dados no disco rigido*/
}
/* Remove as informacoes referentes a uma pessoa */
void Apagar_Pessoa(){
	PESSOA x;
	long int n_reg;
	char resp;

	printf("Qual o N. do Registo: ");
	scanf("%ld", &n_reg); fflush(stdin);

	if(fseek(fp, (n_reg - 1)*sizeof(PESSOA),SEEK_SET)!=0){
		Mensagem("Registo Inexistente ou problemas no posicionamento!!!");
		return;
	}
	if(fread(&x, sizeof(PESSOA),1,fp)!=1){
		Mensagem("Problemas na Leitura do Registo !!!");
		return;
	}
	if(x.Status =='*'){
		Mensagem("Registo ja Apagado !!!");
		return;
	}
	printf("\n\nDados Actuais\n\n");
	Mostrar_Pessoa(x);
	printf("\n\nApagar o Registo (s/n)???: ");
	resp = getchar(); fflush(stdin);
	if(toupper(resp)!= 'S') return;

	x.Status='*';
	//recuar um registor no ficheiro
	fseek(fp, -(long)sizeof(PESSOA),SEEK_CUR);
	//reescrever o registo;
	fwrite(&x, sizeof(PESSOA),1,fp);
	fflush(fp); /*Despejar os dados no disco rigido*/
}
/* Lista informacao de uma pessoa */
void Listar_Pessoa(){
	long int N_Linhas = 0;
	PESSOA reg;
	rewind(fp);
	while(1){
		if(fread(&reg, sizeof(reg),1,fp)!=1)break; /*Sair do ciclo*/
		if(reg.Status=='*') continue; /* Passa ao proximo */
		Mostrar_Pessoa(reg);
		N_Linhas++;
		if(N_Linhas%20 == 0)
			Mensagem("PRIMA <ENTER> para continuar ...");
	}
	Mensagem("\n\nPRIMA <ENTER> para continuar ..."); /*No fim da listagem*/
}
/* Pesquisa um conjunto de registos por um intervalo de idades [ini, fim] */
void Pesquisar_Idades(int ini, int fim){
	PESSOA reg;
	rewind(fp);

	while(fread(&reg, sizeof(reg),1,fp))
		if(reg.Status != '*' && reg.Idade >= ini &&reg.Idade<=fim)
			Mostrar_Pessoa(reg);

	Mensagem("\n\nPRIMA <ENTER> para continuar ..."); /*No fim da listagem*/
}
/* Pesquisa um reisto dado o nome */
void Pesquisar_Nome(char *s){
	PESSOA reg;
	rewind(fp);

	while(fread(&reg, sizeof(reg),1,fp))
		if(reg.Status != '*' && strstr(reg.Nome,s))
			Mostrar_Pessoa(reg);

	Mensagem("\n\nPRIMA <ENTER> para continuar ..."); /*No fim da listagem*/
}
