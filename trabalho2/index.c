#include "index.h"

void CriarIndex(char *arquivoDados, char *arquivoIndex) {

    FILE *arquivoDadosBIN = fopen(arquivoDados, "rb");
    if (arquivoDadosBIN == NULL) {
        MensagemErro();
        return;
    }

    // Verificação da consistência. Se for inconsistente, aborta
    Header cabecalhoDados;
    LerCabecalhoBIN(arquivoDadosBIN, &cabecalhoDados);
    if (cabecalhoDados.status == '0') {
        MensagemErro();
        fclose(arquivoDadosBIN);
        return;
    }

    FILE *arquivoIndexBIN = fopen(arquivoIndex, "wb+"); 
    if (arquivoIndexBIN == NULL) {
        MensagemErro();
        fclose(arquivoDadosBIN);
        return;
    }

    // criação do cabeçalho do arquivo de índice
    IndexHeader cabecalhoIndex;
    cabecalhoIndex.status = '0';
    fwrite(&cabecalhoIndex.status, sizeof(char), 1, arquivoIndexBIN);

    int totalRegsIndex = 0;
    Registro regDados;

    // Busca dos registros no arquivo de dados para montar o índice
    // começa após cabeçalho do arquivo de dados
    fseek(arquivoDadosBIN, TAM_CABECALHO, SEEK_SET);

    for (int i = 0; i < cabecalhoDados.proxRRN; i++) {
        LerRegistroBIN(arquivoDadosBIN, &regDados);

        // Só indexamos registros não logicamente removidos
        if (regDados.removido != '1') {
            inserirRegistroIndex(arquivoIndexBIN, regDados.codEstacao, i, &totalRegsIndex);
        }

        LiberarStringRegistro(&regDados);
    }

    // Ao final do process, atualizar consistência do índice
    cabecalhoIndex.status = '1';
    fseek(arquivoIndexBIN, 0, SEEK_SET);
    fwrite(&cabecalhoIndex.status, sizeof(char), 1, arquivoIndexBIN);

    fclose(arquivoDadosBIN);
    fclose(arquivoIndexBIN);
}

void InserirRegistroIndex(FILE *arquivoIndex, int codEstacao, int RRN, int *totalRegs) {
    if (arquivoIndex == NULL) return;

    int i = *totalRegs - 1;
    IndexRegistro regAux;

    while (i >= 0) {
        fseek(arquivoIndex, TAM_INDEX_HEADER + (i * TAM_INDEX_REGISTRO), SEEK_SET);
        fread(&regAux, sizeof(IndexRegistro), 1, arquivoIndex);

        if (regAux.codEstacao > codEstacao) {
            // se o registro a ser colocado no índice é menos que o atual, coloca o atual para trás
            fseek(arquivoIndex, TAM_INDEX_HEADER + ((i + 1) * TAM_INDEX_REGISTRO), SEEK_SET);
            fwrite(&regAux, sizeof(IndexRegistro), 1, arquivoIndex);
            i--;
        } else {
            break;
        }
    }

    IndexRegistro novoReg;
    novoReg.codEstacao = codEstacao;
    novoReg.RRN = RRN;

    fseek(arquivoIndex, TAM_INDEX_HEADER + ((i + 1) * TAM_INDEX_REGISTRO), SEEK_SET);
    fwrite(&novoReg, sizeof(IndexRegistro), 1, arquivoIndex);

    // incrementa a contagem de registros no índice
    (*totalRegs)++;
}

int BuscarRegistroIndex(FILE *arquivoIndex, int codEstacao, int totalRegs) {

}

void removerRegistroIndex(FILE *arquivoIndex, int codEstacao, int *totalRegs) {
   
}