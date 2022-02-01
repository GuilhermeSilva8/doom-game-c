#include "PIG.h"

PIG_Evento evento;
PIG_Teclado meuTeclado;

//======================================================================================================================================
//---------- STRUCTS ----------
struct personagens{
    int id;
    int Vida;
};

//======================================================================================================================================
//---------- FUNCOES WOLFENSTEIN ----------

//---------- INICIALIZA ----------

void IniciaWolf(int Wolf, int ParTiro){
    CriaModoAnimacao(Wolf, 0, 0); // 0 == PARADO PARA DIREITA
    CriaFrameAnimacao(Wolf, 0, 64*7, 64*0, 64, 64);
    InsereFrameAnimacao(Wolf, 0, 0, 0.2);

    CriaModoAnimacao(Wolf, 1, 0); // 1 == PARADO PARA ESQUERDA
    CriaFrameAnimacao(Wolf, 1, 64*1, 64*0, 64, 64);
    InsereFrameAnimacao(Wolf, 1, 1, 0.2);

    CriaModoAnimacao(Wolf, 2, 0); // 2 == ANDANDO P/ DIREITA
    for(int i=0; i<5; i++){
        CriaFrameAnimacao(Wolf, 2+i, 64*6, 64*i, 64, 64);
        InsereFrameAnimacao(Wolf, 2, 2+i, 0.2);
    }

    CriaModoAnimacao(Wolf, 3, 0); // 3 == ANDANDO P/ ESQUERDA
    for(int i=0; i<5; i++){
        CriaFrameAnimacao(Wolf, 7+i, 64*2, 64*i, 64, 64);
        InsereFrameAnimacao(Wolf, 3, 7+i, 0.2);
    }

    MudaModoAnimacao(Wolf, 0, 1); // MODO INICIAL PARADO
    SetDimensoesAnimacao(Wolf, 64, 64);

    MudaDirecaoParticulas(ParTiro, 100, 0); // MODO INICIAL PARADO DAS PARTICULAS
    MudaEscalaParticulas(ParTiro, 0.015, 0.015);
    MoveGeradorParticulas(ParTiro, 50, 62);

}

//---------- MOVIMENTA ----------

void MovimentaWolf(int Wolf, int ParTiro, int *Xo, int *Yo, int TempoAnima, int TempoPulo, int *modo, int *wolfnoAr, int *wolfvelY, int *Cx, int PassosWolf, int TiroWolf){
        int Dx=4, Dy=4; // VETOR DIRECAO

        if(evento.tipoEvento == EVENTO_TECLADO && evento.teclado.acao == TECLA_PRESSIONADA){
            if(evento.teclado.tecla == TECLA_DIREITA){
                    if(*Xo < 640 - 64){
                        *Xo += Dx;
                        MoveAnimacao(Wolf, *Xo, *Yo);
                        MudaDirecaoParticulas(ParTiro, 100, 0);
                        MoveGeradorParticulas(ParTiro, *Xo+50, *Yo+27);
                    }else{
                        if(*Cx == 0)
                            *Cx += 576;
                        else
                            *Cx +=640;

                        *Xo = 0;
                    }

                }


            if(evento.teclado.tecla == TECLA_ESQUERDA && *Xo>0){
                    *Xo -= Dx;
                    MoveAnimacao(Wolf, *Xo, *Yo);
                    MudaDirecaoParticulas(ParTiro, -100, 0);
                    MoveGeradorParticulas(ParTiro, *Xo, *Yo+27);
            }

            if(evento.teclado.tecla == TECLA_BARRAESPACO && *Yo == 32){
                *wolfnoAr = 1;
                *wolfvelY = 10;
            }
        }

        if(TempoDecorrido(TempoPulo) > 0.030){
            if(*wolfnoAr == 1){
                    *Yo += *wolfvelY;
                    *wolfvelY -=1;
                    if(*Yo <=32){
                        *Yo=32;
                        *wolfnoAr = 0;
                    }
            }
            ReiniciaTimer(TempoPulo);
        }

        MoveAnimacao(Wolf, *Xo, *Yo);

        if(TempoDecorrido(TempoAnima)>1 && evento.tipoEvento == EVENTO_TECLADO && evento.teclado.acao == TECLA_PRESSIONADA){
            if(evento.teclado.tecla == TECLA_DIREITA){
                PlayAudio(PassosWolf);
                *modo = 2;
                MudaModoAnimacao(Wolf, *modo, 1);
                if(TempoDecorrido(TempoAnima) > 1)
                    ReiniciaTimer(TempoAnima);
            }

            if(evento.teclado.tecla == TECLA_ESQUERDA){
                PlayAudio(PassosWolf);
                *modo = 3;
                MudaModoAnimacao(Wolf, *modo, 1);
                if(TempoDecorrido(TempoAnima) > 1)
                    ReiniciaTimer(TempoAnima);
            }
        }

        if(TempoDecorrido(TempoAnima) > 1.2 && (*modo) == 3){
            *modo = 1;
            MudaModoAnimacao(Wolf, *modo, 1);
            MudaDirecaoParticulas(ParTiro, -100, 0);
            MoveGeradorParticulas(ParTiro, *Xo, *Yo+27);
        }
        if(TempoDecorrido(TempoAnima) > 1.2 && (*modo) == 2){
            *modo = 0;
            MudaModoAnimacao(Wolf, *modo, 1);
            MudaDirecaoParticulas(ParTiro, 100, 0);
            MoveGeradorParticulas(ParTiro, *Xo+50, *Yo+27);
        }

        if(evento.tipoEvento == EVENTO_TECLADO && evento.teclado.acao == TECLA_PRESSIONADA){
            if(evento.teclado.tecla == TECLA_s){
                PlayAudio(TiroWolf);
                CriaParticula(ParTiro);
            }
        }

}

//======================================================================================================================================
//---------- FUNCOES INIMIGOS ----------

//---------- INICIALIZA O GOBLIN ----------

struct personagens* CriaGoblin(int modo, int frame){

    struct personagens *BaseGoblin;

    BaseGoblin = (struct personagens *)malloc(sizeof(struct personagens));

    BaseGoblin->id = CriaAnimacao("../imagens/Agora.png");

    CriaModoAnimacao(BaseGoblin->id, modo, 1);

    for(int i=0; i<4; i++){
        CriaFrameAnimacao(BaseGoblin->id, frame + i, 38*i,  0, 55, 38);
        InsereFrameAnimacao(BaseGoblin->id, modo, frame + i, 0.2);
    }

    MudaModoAnimacao(BaseGoblin->id, modo, 1);
    SetDimensoesAnimacao(BaseGoblin->id, 55, 38);

    BaseGoblin->Vida = 8;

    return BaseGoblin;
}

//---------- INICIALIZA O PINKY ----------

struct personagens* CriaPinky(int modo, int frame){

    struct personagens *BasePinky;

    BasePinky = (struct personagens *)malloc(sizeof(struct personagens));

    BasePinky->id = CriaAnimacao("../imagens/NovoPinky.png");

    CriaModoAnimacao(BasePinky->id, modo, 1);

    for(int i=0; i<4; i++){
        CriaFrameAnimacao(BasePinky->id, frame + i, 55*i,  0, 59, 55);
        InsereFrameAnimacao(BasePinky->id, modo, frame + i, 0.2);
    }

    MudaModoAnimacao(BasePinky->id, modo, 1);
    SetDimensoesAnimacao(BasePinky->id, 59, 55);

    BasePinky->Vida = 5;

    return BasePinky;
}

//---------- INICIALIZA O BARON ----------

struct personagens* CriaBaron(int modo, int frame){

    struct personagens *BaseBaron;

    BaseBaron = (struct personagens *)malloc(sizeof(struct personagens));

    BaseBaron->id = CriaAnimacao("../imagens/NovoBaron.png");

    CriaModoAnimacao(BaseBaron->id, modo, 1);

    for(int i=0; i<4; i++){
        CriaFrameAnimacao(BaseBaron->id, frame + i, 52*i,  0, 74, 52);
        InsereFrameAnimacao(BaseBaron->id, modo, frame + i, 0.2);
    }

    MudaModoAnimacao(BaseBaron->id, modo, 1);
    SetDimensoesAnimacao(BaseBaron->id, 74, 52);

    BaseBaron->Vida = 10;

    return BaseBaron;
}

//---------- INICIALIZA O REVENANT ----------

int CriaRevenant(int modo, int frame){

    int BaseRevenant = CriaAnimacao("../imagens/NovoRevenant.png");

    CriaModoAnimacao(BaseRevenant, modo, 1);

    for(int i=0; i<3; i++){
        CriaFrameAnimacao(BaseRevenant, frame + i, 70*i,  0, 85, 70);
        InsereFrameAnimacao(BaseRevenant, modo, frame + i, 0.2);
    }

    MudaModoAnimacao(BaseRevenant, modo, 1);
    SetDimensoesAnimacao(BaseRevenant, 85, 70);

    return BaseRevenant;
}

//======================================================================================================================================
//---------- MAIN ----------

int main( int argc, char* args[] ){

    CriaJogo("Meu Jogo", 0, 256, 640);

//======================================================================================================================================
//---------- SONS ----------

    //---------- FUNDO ----------

    int FundoFase = CriaAudio("../audios/Fase.mp3", 0);

    int TimerFundoFase = CriaTimer();

    int PrimeiroFundo = 0;

    //---------- PASSOS ----------

    int PassosWolf = CriaAudio("../audios/WolfPassos.wav", 0);

    int DanoWolf = CriaAudio("../audios/WolfDano.wav", 0);

    int TimerWolf = CriaTimer();

    //---------- TIRO ----------

    int TiroWolf = CriaAudio("../audios/Tiro.mp3", 0);

    //---------- GOBLIN ----------

    int GoblinVivo = CriaAudio("../audios/GoblinVivo.mp3", 0);

    int TimerGoblin = CriaTimer();

    int GoblinDano = CriaAudio("../audios/GoblinDano.mp3", 0);

    int GoblinMorto = CriaAudio("../audios/GoblinMorto.mp3", 0);

    //---------- PINKY ----------

    int PinkyVivo = CriaAudio("../audios/PinkyVivo.mp3", 0);

    int TimerPinky = CriaTimer();

    int PinkyDano = CriaAudio("../audios/PinkyDano.wav", 0);

    int PinkyMorto = CriaAudio("../audios/PinkyMorto.wav ", 0);

    //---------- BARON ----------

    int BaronVivo = CriaAudio("../audios/BaronVivo.wav", 0);

    int TimerBaron = CriaTimer();

    int BaronDano = CriaAudio("../audios/BaronDano.wav", 0);

    int BaronMorto = CriaAudio("../audios/BaronMorto.wav ", 0);

    //---------- REVENANT ----------

    int RevenantVivo = CriaAudio("../audios/RevenantVivo.wav", 0);

    int TimerRevenant = CriaTimer();

    int RevenantDano = CriaAudio("../audios/RevenantDano.wav", 0);

    int RevenantMorto = CriaAudio("../audios/RevenantMorto.wav ", 0);

//======================================================================================================================================
//---------- VARIAVEIS GERAIS ----------

    int modo=4, frame=12; // PARA CRIACAO DE NOVAS ANIMAÇÕES

    int Cx=0; // CAMERA HORIZONTAL

    char str[20];

    int AudioIntro = CriaAudio("../audios/Intro.mp3", 0);

    int TimerIntro = CriaTimer();

    int TimerIntro2 = CriaTimer();

    int ti=0;

    int TimerFim = CriaTimer();

    int op = 0;

    int Fundo = CriaObjeto("../imagens/Fundo.jpg");

    SetDimensoesObjeto(Fundo, 256, 640);

    MoveObjeto(Fundo, 0, 0);

    srand(time(NULL));

    int TimerRestart = CriaTimer();

    int dg=0, db=0, dpk=0, dr=0;

    //---------- VARIAVEIS DO CHAO ----------

    int Chao = CriaObjeto("../imagens/lava.png");

    SetDimensoesObjeto(Chao, 38, 1024);

    //---------- VARIAVEIS DAS PEDRAS ----------

    int *Pedra, *Xp, ip=1;

    //---------- VARIAVEIS DA PILHA DE OSSOS ----------

    int *BonePile, *Xbp, ibp=1;

    //---------- VARIAVEIS DA PILHA DE CORPOS ----------

    int *CorpsePile, *Xcp, icp=1;

    //---------- VARIAVEIS DO REVENANT EMPALHADO ----------

    int *RevenantPiled, *Xrp, irp=1;

    //---------- VARAIAVEIS DAS ROCHAS ----------

    int *Rocha, *Xr, *Yr, ir=1;


//----------VARIAVEIS DO WOLF ----------

    //---------- PARA INICIALIZAR ----------
    personagens Wolf;

    Wolf.id = CriaAnimacao("../imagens/Wolfenstein.png");

    int Tiro = CriaObjeto("../imagens/Tiro.png");

    SetDimensoesObjeto(Tiro, 22, 26);

    int ParTiro = CriaGeradorParticulasPorObjeto(100, Tiro);

    IniciaWolf(Wolf.id, ParTiro);

    Wolf.Vida = 1000;

    int WolfVivo = 1;

    //---------- PARA MOVIMENTAR ----------

    int Xo=0, Yo=32; // POSICAO WOLF

    int modowolf=0, wolfnoAr=0, wolfvelY=0; // VARIAVEL PARA MODO, SE ESTA NO AR E QUAL A VELOCIDADE NO AR

    int TempoAnima = CriaTimer(); // TIMER PARA CONTROLE DO MOVIMENTO AO ANDAR

    int TempoPulo = CriaTimer(); // TIMER PARA CONTROLE DO MOVIMENTO AO PULAR

    MoveAnimacao(Wolf.id, Xo , Yo); // ESTADO INICIAL

//======================================================================================================================================
//---------- VARIAVEIS DOS INIMIGOS ----------

    //---------- VARIAVEIS DO GOBLIN ----------
    personagens **Goblin = NULL;

    int *Xg, *Yg, ig=1;

    int *Movg, *TempoMovg;

    int *Jafoig;

    //----------VARIAVEIS DO PINKY ----------

    personagens **Pinky = NULL;

    int *Xpk, *Ypk, ipk=1;

    int *Movpk, *TempoMovpk;

    int *Jafoipk;

    //----------VARIAVEIS DO BARON ----------

    personagens **Baron = NULL;

    int *Xb, *Yb, ib=1;

    int *Movb, *TempoMovb;

    int *Jafoib;

    //----------VARIAVEIS DO REVENANT ----------

    personagens Revenant;

    int Xrv, Yrv, irv=1;

    int Movrv, TempoMovrv;

    int Jafoirv;

//======================================================================================================================================
//---------- LENDO A MATRIZ ----------
    int mat[8][90];

    FILE *arq = fopen("../textos/cenario.txt", "r");

    for(int i=0; i<8; i++){
        for(int j=0; j<90; j++){
            fscanf(arq, "%d", &mat[i][j]);
        }
    }

    fclose(arq);

//---------- DESENHANDO OS OBJETOS E ANIMACOES DA MATRIZ ----------
    for(int i=0; i<8; i++){
        for(int j=0; j<90; j++){

            //----------- CRIA O CHAO ----------
            if(mat[i][j]==1)
                MoveObjeto(Chao, 64*0, 0);

            //----------- CRIA O GOBLIN ----------
            if(mat[i][j]==2){

                //---------- CRIA UM GOBLIN E DA A POSICAO QUE ELE VAI TER ----------
                if(ig == 1){
                    Goblin = (struct personagens **)realloc(Goblin, sizeof(struct personagens*)*ig);
                    Goblin[ig-1] = CriaGoblin(modo, frame);
                    Xg = (int *)malloc(sizeof(int)*ig);
                    Yg = (int *)malloc(sizeof(int)*ig);
                    Movg = (int *)malloc(sizeof(int)*ig);
                    TempoMovg = (int *)malloc(sizeof(int)*ig);
                    TempoMovg[ig-1] = CriaTimer();
                    Jafoig = (int *)malloc(sizeof(int)*ig);
                    Jafoig[ig-1] = 0;
                    Xg[ig-1] = 64*j;
                    Yg[ig-1] = 24;
                }else{
                    Goblin = (struct personagens **)realloc(Goblin, sizeof(struct personagens*)*ig);
                    Goblin[ig-1] = CriaGoblin(modo, frame);
                    Xg = (int *)realloc(Xg, sizeof(int)*ig);
                    Yg = (int *)realloc(Yg, sizeof(int)*ig);
                    Movg = (int *)realloc(Movg, sizeof(int)*ig);
                    TempoMovg = (int *)realloc(TempoMovg, sizeof(int)*ig);
                    TempoMovg[ig-1] = CriaTimer();
                    Jafoig = (int *)realloc(Jafoig, sizeof(int)*ig);
                    Jafoig[ig-1] = 0;
                    Xg[ig-1] = 64*j;
                    Yg[ig-1] = 24;
                }

                //---------- ATUALIZA OS VALORES DO VETOR, MODO E FRAME ----------
                ig++;
                modo ++;
                frame += 4;
            }

            //---------- CRIA O PINKY ----------
            if(mat[i][j] == 3){
                    //---------- CRIA UM PINKY E DA A POSICAO QUE ELE VAI TER ----------
                if(ipk == 1){
                    Pinky = (struct personagens **)realloc(Pinky, sizeof(struct personagens*)*ipk);
                    Pinky[ipk-1] = CriaPinky(modo, frame);
                    Xpk = (int *)malloc(sizeof(int)*ipk);
                    Ypk = (int *)malloc(sizeof(int)*ipk);
                    Movpk = (int *)malloc(sizeof(int)*ipk);
                    TempoMovpk = (int *)malloc(sizeof(int)*ipk);
                    TempoMovpk[ipk-1] = CriaTimer();
                    Jafoipk = (int *)malloc(sizeof(int)*ipk);
                    Jafoipk[ipk-1] = 0;
                    Xpk[ipk-1] = 64*j;
                    Ypk[ipk-1] = 24;
                }else{
                    Pinky = (struct personagens **)realloc(Pinky, sizeof(struct personagens*)*ipk);
                    Pinky[ipk-1] = CriaPinky(modo, frame);
                    Xpk = (int *)realloc(Xpk, sizeof(int)*ipk);
                    Ypk = (int *)realloc(Ypk, sizeof(int)*ipk);
                    Movpk = (int *)realloc(Movpk, sizeof(int)*ipk);
                    TempoMovpk = (int *)realloc(TempoMovpk, sizeof(int)*ipk);
                    TempoMovpk[ipk-1] = CriaTimer();
                    Jafoipk = (int *)realloc(Jafoipk, sizeof(int)*ipk);
                    Jafoipk[ipk-1] = 0;
                    Xpk[ipk-1] = 64*j;
                    Ypk[ipk-1] = 24;
                }

                //---------- ATUALIZA OS VALORES DO VETOR, MODO E FRAME ----------
                ipk++;
                modo ++;
                frame += 4;
            }

            //---------- CRIA O BARON ----------
            if(mat[i][j] == 10){
                    //---------- CRIA UM BARON E DA A POSICAO QUE ELE VAI TER ----------
                if(ib == 1){
                    Baron = (struct personagens **)realloc(Baron, sizeof(struct personagens*)*ib);
                    Baron[ib-1] = CriaBaron(modo, frame);
                    Xb = (int *)malloc(sizeof(int)*ib);
                    Yb = (int *)malloc(sizeof(int)*ib);
                    Movb = (int *)malloc(sizeof(int)*ib);
                    TempoMovb = (int *)malloc(sizeof(int)*ib);
                    TempoMovb[ib-1] = CriaTimer();
                    Jafoib = (int *)malloc(sizeof(int)*ib);
                    Jafoib[ib-1] = 0;
                    Xb[ib-1] = 64*j;
                    Yb[ib-1] = 24;
                }else{
                    Baron = (struct personagens **)realloc(Baron, sizeof(struct personagens*)*ib);
                    Baron[ib-1] = CriaBaron(modo, frame);
                    Xb = (int *)realloc(Xb, sizeof(int)*ib);
                    Yb = (int *)realloc(Yb, sizeof(int)*ib);
                    Movb = (int *)realloc(Movb, sizeof(int)*ib);
                    TempoMovb = (int *)realloc(TempoMovb, sizeof(int)*ib);
                    TempoMovb[ib-1] = CriaTimer();
                    Jafoib = (int *)realloc(Jafoib, sizeof(int)*ib);
                    Jafoib[ib-1] = 0;
                    Xb[ib-1] = 64*j;
                    Yb[ib-1] = 24;
                }

                //---------- ATUALIZA OS VALORES DO VETOR, MODO E FRAME ----------
                ib++;
                modo ++;
                frame += 4;
            }

            //---------- CRIA O REVENANT ----------
            if(mat[i][j] == 11){
                    //---------- CRIA UM REVENANT E DA A POSICAO QUE ELE VAI TER ----------
                    Revenant.id = CriaRevenant(modo, frame);
                    Revenant.Vida = 20;
                    TempoMovrv = CriaTimer();
                    Jafoirv = 0;
                    Xrv = 64*j;
                    Yrv = 24;

                //---------- ATUALIZA OS VALORES DO VETOR, MODO E FRAME ----------
                irv++;
                modo ++;
                frame += 4;
            }


            //---------- CRIA O FUNDO ----------
            if(mat[i][j] == 4){
                if(ir == 1){
                    Rocha = (int *)malloc(sizeof(int)*ir);
                    Rocha[ir-1] = CriaObjeto("../imagens/download.jpg");
                    SetDimensoesObjeto(Rocha[ir-1], 32, 64);
                    Xr = (int *)malloc(sizeof(int)*ir);
                    Yr = (int *)malloc(sizeof(int)*ir);
                    Xr[ir-1] = 64*j;
                    Yr[ir-1] = 32*(7-i);
                    MoveObjeto(Rocha[ir-1], Xr[ir-1], Yr[ir-1]);
                    ir++;
                }else{
                    Rocha = (int *)realloc(Rocha, sizeof(int)*ir);
                    Rocha[ir-1] = CriaObjeto("../imagens/download.jpg");
                    SetDimensoesObjeto(Rocha[ir-1], 32, 64);
                    Xr = (int *)realloc(Xr, sizeof(int)*ir);
                    Yr = (int *)realloc(Yr, sizeof(int)*ir);
                    Xr[ir-1] = 64*j;
                    Yr[ir-1] = 32*(7-i);
                    MoveObjeto(Rocha[ir-1], Xr[ir-1], Yr[ir-1]);
                    ir++;
                }
            }

            //---------- CRIA PILHA DE OSSOS ----------
            if(mat[i][j] == 5){
                if(ibp == 1){
                    BonePile = (int *)malloc(sizeof(int)*ibp);
                    BonePile[ibp-1] = CriaObjeto("../imagens/BonePile.png");
                    SetDimensoesObjeto(BonePile[ibp-1], 32, 102);
                    Xbp = (int *)malloc(sizeof(int)*ibp);
                    Xbp[ibp-1] = 64*j;
                    MoveObjeto(BonePile[ibp-1], Xbp[ibp-1], 28);
                    ibp++;
                }else{
                    BonePile = (int *)realloc(BonePile, sizeof(int)*ibp);
                    BonePile[ibp-1] = CriaObjeto("../imagens/BonePile.png");
                    SetDimensoesObjeto(BonePile[ibp-1], 32, 102);
                    Xbp = (int *)realloc(Xbp, sizeof(int)*ibp);
                    Xbp[ibp-1] = 64*j;
                    MoveObjeto(BonePile[ibp-1], Xbp[ibp-1], 28);
                    ibp++;
                }
            }


            //---------- CRIA AS PEDRAS ----------
            if(mat[i][j]==6){
                if(ip == 1){
                    Pedra = (int *)malloc(sizeof(int)*ip);
                    Pedra[ip-1] = CriaObjeto("../imagens/Pedra.png");
                    SetDimensoesObjeto(Pedra[ip-1], 32, 64);
                    Xp = (int *)malloc(sizeof(int)*ip);
                    Xp[ip-1] = 64*j;
                    MoveObjeto(Pedra[ip-1], Xp[ip-1], 24);
                    ip++;
                }else{
                    Pedra = (int *)realloc(Pedra, sizeof(int)*ip);
                    Pedra[ip-1] = CriaObjeto("../imagens/Pedra.png");
                    SetDimensoesObjeto(Pedra[ip-1], 32, 64);
                    Xp = (int *)realloc(Xp, sizeof(int)*ip);
                    Xp[ip-1] = 64*j;
                    MoveObjeto(Pedra[ip-1], Xp[ip-1], 24);
                    ip ++;
                }
            }

            //----------CRIA AS PILHAS DE CORPOS ----------
            if(mat[i][j]==8){
                if(icp == 1){
                    CorpsePile = (int *)malloc(sizeof(int)*icp);
                    CorpsePile[icp-1] = CriaObjeto("../imagens/CorpsePile.png");
                    SetDimensoesObjeto(CorpsePile[icp-1], 74, 105);
                    Xcp = (int *)malloc(sizeof(int)*icp);
                    Xcp[icp-1] = 64*j;
                    MoveObjeto(CorpsePile[icp-1], Xcp[icp-1], 28);
                    icp++;
                }else{
                    CorpsePile = (int *)realloc(CorpsePile, sizeof(int)*icp);
                    CorpsePile[icp-1] = CriaObjeto("../imagens/CorpsePile.png");
                    SetDimensoesObjeto(CorpsePile[icp-1], 74, 105);
                    Xcp = (int *)realloc(Xcp, sizeof(int)*icp);
                    Xcp[icp-1] = 64*j;
                    MoveObjeto(CorpsePile[icp-1], Xcp[icp-1], 28);
                    icp++;
                }
            }

            //----------CRIA OS REVENANTS EMPALHADOS ----------
            if(mat[i][j]==9){
                if(irp == 1){
                    RevenantPiled = (int *)malloc(sizeof(int)*irp);
                    RevenantPiled[irp-1] = CriaObjeto("../imagens/RevenantPiled.png");
                    SetDimensoesObjeto(RevenantPiled[irp-1], 96, 64);
                    Xrp = (int *)malloc(sizeof(int)*irp);
                    Xrp[irp-1] = 64*j;
                    MoveObjeto(RevenantPiled[irp-1], Xrp[irp-1], 28);
                    irp++;
                }else{
                    RevenantPiled = (int *)realloc(RevenantPiled, sizeof(int)*irp);
                    RevenantPiled[irp-1] = CriaObjeto("../imagens/RevenantPiled.png");
                    SetDimensoesObjeto(RevenantPiled[irp-1], 96, 64);
                    Xrp = (int *)realloc(Xrp, sizeof(int)*irp);
                    Xrp[irp-1] = 64*j;
                    MoveObjeto(RevenantPiled[irp-1], Xrp[irp-1], 28);
                    irp++;
                }
            }
        }
    }

//======================================================================================================================================
//---------- EVENTOS ----------

    meuTeclado = GetTeclado();

    while(JogoRodando()){

        evento = GetEvento();

        if(evento.tipoEvento == EVENTO_TECLADO && evento.teclado.acao == TECLA_PRESSIONADA){
            op = 1;
        }

        if(op == 0){
            if(TempoDecorrido(TimerIntro)>45 || ti == 0){
                if(AudioIntro != -1)
                    PlayAudio(AudioIntro);
                ti = 1;
            }

        }else{
            if(AudioIntro != -1){
                DestroiAudio(AudioIntro);
                AudioIntro = -1;
            }
//---------- EVENTOS GERAIS ----------

        //---------- MOVE AS PEDRAS ----------

        for(int i=0; i<(ip-1); i++){
            MoveObjeto(Pedra[i], Xp[i]-Cx, 24);
        }

        //---------- MOVE PILHA DE OSSOS ----------

        for(int i=0; i<(ibp-1); i++){
            MoveObjeto(BonePile[i], Xbp[i]-Cx, 28);
        }

        //---------- MOVE PILHA DE CORPOS ----------

        for(int i=0; i<(icp-1); i++){
            MoveObjeto(CorpsePile[i], Xcp[i]-Cx, 28);
        }

        //---------- MOVE REVENANT EMPALHADO ----------

        for(int i=0; i<(irp-1); i++){
            MoveObjeto(RevenantPiled[i], Xrp[i]-Cx, 28);
        }

        //----------MOVE AS ROCHAS ----------

        for(int i=0; i<(ir-1); i++){
            MoveObjeto(Rocha[i], Xr[i]-Cx, Yr[i]);
        }

//---------- EVENTOS COM O WOLF ----------

        MovimentaWolf(Wolf.id, ParTiro, &Xo , &Yo, TempoAnima, TempoPulo, &modowolf, &wolfnoAr, &wolfvelY, &Cx, PassosWolf, TiroWolf);

        //---------- COLISAO COM O GOBLIN ----------
        for(int i=0; i<(ig-1); i++){
            if(Goblin[i]->id != -1){
                if(ColisaoAnimacoes(Wolf.id, Goblin[i]->id)){
                    if(TempoDecorrido(TimerWolf)>0.6){
                        PlayAudio(DanoWolf);
                        ReiniciaTimer(TimerWolf);
                    }
                    dg++;
                    if(dg > ig)
                        Wolf.Vida-=2;
                }
            }
        }

        //---------- COLISAO COM O PINK ----------
        for(int i=0; i<(ipk-1); i++){
            if(Pinky[i]->id != -1){
                if(ColisaoAnimacoes(Wolf.id, Pinky[i]->id)){
                    if(TempoDecorrido(TimerWolf)>0.6){
                        PlayAudio(DanoWolf);
                        ReiniciaTimer(TimerWolf);
                    }
                    dpk++;
                    if(dpk > ipk)
                        Wolf.Vida--;
                }
            }
        }

        //---------- COLISAO COM O BARON ----------
        for(int i=0; i<(ib-1); i++){
            if(Baron[i]->id != -1){
                if(ColisaoAnimacoes(Wolf.id, Baron[i]->id)){
                    if(TempoDecorrido(TimerWolf)>0.6){
                        PlayAudio(DanoWolf);
                        ReiniciaTimer(TimerWolf);
                    }
                    db++;
                    if(db > ib)
                        Wolf.Vida-=3;
                }
            }
        }

        //---------- COLISAO COM O REVENANT ----------
            if(Revenant.id != -1){
                if(ColisaoAnimacoes(Wolf.id, Revenant.id)){
                    if(TempoDecorrido(TimerWolf)>0.6){
                        PlayAudio(DanoWolf);
                        ReiniciaTimer(TimerWolf);
                    }
                    dr++;
                    if(dr>ir)
                        Wolf.Vida-=8;
                }
            }

        if(Wolf.Vida <= 0 && WolfVivo == 1){
            PausaTudo();
            ReiniciaTimer(TimerRestart);
            WolfVivo = 0;
        }

        if(WolfVivo == 0 && TempoDecorrido(TimerRestart)>2){
            Wolf.Vida = 1000;
            WolfVivo =1;
            op = 0;
            Xo = (Cx-640) + 64;
            DespausaTudo();
        }


        //---------- ATUALIZA VALOR DA VIDA ----------
        sprintf(str, "%d", Wolf.Vida);

//---------- EVENTOS COM O GOBLIN ----------
        //---------- ATUALIZA A POSICAO DO GOBLIN A CADA MOVIMENTO DO WOLF ----------

        //---------- ATUALIZA ANTES DE APARECER NA TELA ----------
        for(int i=0; i<(ig-1); i++){
            if(Jafoig[i]==0){
                Movg[i] = Xg[i] - Cx;
                MoveAnimacao(Goblin[i]->id, Movg[i], Yg[i]);
                if(Movg[i]<640)
                    Jafoig[i] = 1;
            }
        //---------- ATUALIZA DEPOIS DE APARECER NA TELA ----------
            else{
                if(TempoDecorrido(TempoMovg[i])>0.2 && Goblin[i]->id != -1){
                    if(TempoDecorrido(TimerGoblin)>5){
                        PlayAudio(GoblinVivo);
                        ReiniciaTimer(TimerGoblin);
                    }
                    Movg[i] -= 8;
                    MoveAnimacao(Goblin[i]->id, Movg[i], Yg[i]);
                    ReiniciaTimer(TempoMovg[i]);
                }

                if((Goblin[i]->id != -1 && ColisaoParticulasAnimacao(ParTiro, Goblin[i]->id))){
                    PlayAudio(GoblinDano);
                    Goblin[i]->Vida--;
                    if(Goblin[i]->Vida==0){
                        PlayAudio(GoblinMorto);
                        DestroiAnimacao(Goblin[i]->id);
                        Goblin[i]->id = -1;
                        if(Wolf.Vida <= 992)
                            Wolf.Vida += (rand()%8);
                    }
                }

                if(Xo==576 && Goblin[i]->id != -1){
                    DestroiAnimacao(Goblin[i]->id);
                    Goblin[i]->id = -1;
                }
            }
        }

//---------- EVENTOS COM O PINKY ----------
        //---------- ATUALIZA A POSICAO DO PINKY A CADA MOVIMENTO DO WOLF ----------

        //---------- ATUALIZA ANTES DE APARECER NA TELA ----------
        for(int i=0; i<(ipk-1); i++){
            if(Jafoipk[i]==0){
                Movpk[i] = Xpk[i] - Cx;
                MoveAnimacao(Pinky[i]->id, Movpk[i], Ypk[i]);
                if(Movpk[i]<640)
                    Jafoipk[i] = 1;
            }
        //---------- ATUALIZA DEPOIS DE APARECER NA TELA ----------
            else{
                if(TempoDecorrido(TempoMovpk[i])>0.2 && Pinky[i]->id != -1){
                    if(TempoDecorrido(TimerPinky)>3){
                        PlayAudio(PinkyVivo);
                        ReiniciaTimer(TimerPinky);
                    }
                    Movpk[i] -= 4;
                    MoveAnimacao(Pinky[i]->id, Movpk[i], Ypk[i]);
                    ReiniciaTimer(TempoMovpk[i]);
                }

                if((Pinky[i]->id != -1 && ColisaoParticulasAnimacao(ParTiro, Pinky[i]->id))){
                    PlayAudio(PinkyDano);
                    Pinky[i]->Vida--;
                    if(Pinky[i]->Vida==0){
                        PlayAudio(PinkyMorto);
                        DestroiAnimacao(Pinky[i]->id);
                        Pinky[i]->id = -1;
                        if(Wolf.Vida <= 995)
                            Wolf.Vida += (rand()%5);
                    }
                }

                if(Xo==576 && Pinky[i]->id != -1){
                    DestroiAnimacao(Pinky[i]->id);
                    Pinky[i]->id = -1;
                }
            }
        }

//---------- EVENTOS COM O BARON ----------
        //---------- ATUALIZA A POSICAO DO BARON A CADA MOVIMENTO DO WOLF ----------

        //---------- ATUALIZA ANTES DE APARECER NA TELA ----------
        for(int i=0; i<(ib-1); i++){
            if(Jafoib[i]==0){
                Movb[i] = Xb[i] - Cx;
                MoveAnimacao(Baron[i]->id, Movb[i], Yb[i]);
                if(Movb[i]<640)
                    Jafoib[i] = 1;
            }
        //---------- ATUALIZA DEPOIS DE APARECER NA TELA ----------
            else{
                if(TempoDecorrido(TempoMovb[i])>0.2 && Baron[i]->id != -1){
                    if(TempoDecorrido(TimerBaron)>4){
                        PlayAudio(BaronVivo);
                        ReiniciaTimer(TimerBaron);
                    }
                    Movb[i] -= 6;
                    MoveAnimacao(Baron[i]->id, Movb[i], Yb[i]);
                    ReiniciaTimer(TempoMovb[i]);
                }

                if((Baron[i]->id != -1 && ColisaoParticulasAnimacao(ParTiro, Baron[i]->id))){
                    PlayAudio(BaronDano);
                    Baron[i]->Vida--;
                    if(Baron[i]->Vida==0){
                        PlayAudio(BaronMorto);
                        DestroiAnimacao(Baron[i]->id);
                        Baron[i]->id = -1;
                        if(Wolf.Vida <= 990)
                            Wolf.Vida += (rand()%10);
                    }
                }

                if(Xo==576 && Baron[i]->id != -1){
                    DestroiAnimacao(Baron[i]->id);
                    Baron[i]->id = -1;
                }
            }
        }

        //---------- EVENTOS COM O REVENANT ----------
        //---------- ATUALIZA A POSICAO DO REVENANT A CADA MOVIMENTO DO WOLF ----------

        //---------- ATUALIZA ANTES DE APARECER NA TELA ----------
            if(Jafoirv==0){
                Movrv = Xrv - Cx;
                MoveAnimacao(Revenant.id, Movrv, Yrv);
                if(Movrv<640)
                    Jafoirv = 1;
            }
        //---------- ATUALIZA DEPOIS DE APARECER NA TELA ----------
            else{
                if(TempoDecorrido(TempoMovrv)>0.2 && Revenant.id != -1){
                    if(TempoDecorrido(TimerRevenant)>4){
                        PlayAudio(RevenantVivo);
                        ReiniciaTimer(TimerRevenant);
                    }
                    Movrv -= 8;
                    MoveAnimacao(Revenant.id, Movrv, Yrv);
                    ReiniciaTimer(TempoMovrv);
                }

                if((Revenant.id != -1 && ColisaoParticulasAnimacao(ParTiro, Revenant.id))){
                    PlayAudio(RevenantDano);
                    Revenant.Vida--;
                    if(Revenant.Vida==0){
                        PlayAudio(RevenantMorto);
                        DestroiAnimacao(Revenant.id);
                        Revenant.id = -1;
                        ReiniciaTimer(TimerFim);
                    }
                }

                if(Xo==576 && Revenant.id != -1){
                    DestroiAnimacao(Revenant.id);
                    Revenant.id = -1;
                }

                if(Revenant.id == -1){
                    if(TempoDecorrido(TimerFim)>3){
                        if(evento.tipoEvento == EVENTO_TECLADO && evento.teclado.acao == TECLA_PRESSIONADA)
                        exit(0);
                    }
                }
            }



//======================================================================================================================================
//---------- TOCA OS SONS ----------
    //---------- TOCA FUNDO ----------
        if(TempoDecorrido(TimerFundoFase) >= 69 || PrimeiroFundo == 0){
            PlayAudio(FundoFase);
            ReiniciaTimer(TimerFundoFase);
            PrimeiroFundo = 1;
        }

    }

//======================================================================================================================================
//---------- DESENHOS ----------

        IniciaDesenho();

//---------- DESENHOS GERAIS ----------

        if(op != 0){

        DesenhaObjeto(Chao); // DESENHA O CHAO

        for(int i=0; i<(ip-1); i++)
            DesenhaObjeto(Pedra[i]); // DESENHA AS PEDRAS

        for(int i=0; i<(ibp-1); i++)
            DesenhaObjeto(BonePile[i]); // DESENHA AS PILHAS DE OSSOS

        for(int i=0; i<(icp-1); i++)
            DesenhaObjeto(CorpsePile[i]); // DESENHA AS PILHAS DE OSSOS

        for(int i=0; i<(irp-1); i++)
            DesenhaObjeto(RevenantPiled[i]); // DESENHA O REVENANT EMPALHADO

        for(int i=0; i<(ir-1); i++)
            DesenhaObjeto(Rocha[i]); //DESENHA AS ROCHAS

//---------- DESENHOS DO WOLF ----------

        DesenhaAnimacao(Wolf.id);

        MoveParticulas(ParTiro);

        DesenhaParticulas(ParTiro);

//---------- DESENHOS DOS INIMIGOS ----------

        //---------- GOBLIN ----------
        for(int i=0; i<(ig-1); i++){
            if(Goblin[i]->id != -1)
                DesenhaAnimacao(Goblin[i]->id);
        }

        //---------- PINKY ----------
        for(int i=0; i<(ipk-1); i++){
            if(Pinky[i]->id != -1)
                DesenhaAnimacao(Pinky[i]->id);
        }

        //---------- BARON ----------
        for(int i=0; i<(ib-1); i++){
            if(Baron[i]->id != -1)
                DesenhaAnimacao(Baron[i]->id);
        }

        //---------- REVENANT ----------
        if(Revenant.id != -1)
            DesenhaAnimacao(Revenant.id);

        //---------- ESCRITAS ----------
        EscreverCentralizada("HP ", 32, 32*7);
        EscreverCentralizada(str, 100, 32*7); // ESCREVE VIDA WOLF

        }else {
            DesenhaObjeto(Fundo);
            if(TempoDecorrido(TimerIntro2) < 2)
                EscreverCentralizada("PRESSIONE QUALQUER TECLA", 320, 0);
            if(TempoDecorrido(TimerIntro2) > 4)
                ReiniciaTimer(TimerIntro2);
        }

        EncerraDesenho();

    }

    FinalizaJogo();

    free(Pedra);
    free(Xp);
    free(BonePile);
    free(Xbp);
    free(CorpsePile);
    free(Xcp);
    free(RevenantPiled);
    free(Xrp);
    free(Rocha);
    free(Xr);
    free(Yr);

    for(int i=0; i<(ig-1); i++){
        free(Goblin[i]);
    }
    free(Goblin);
    free(Xg);
    free(Yg);
    free(Movg);
    free(TempoMovg);
    free(Jafoig);

    for(int i=0; i<(ipk-1); i++){
        free(Pinky[i]);
    }
    free(Pinky);
    free(Xpk);
    free(Ypk);
    free(Movpk);
    free(TempoMovpk);
    free(Jafoipk);

    for(int i=0; i<(ib-1); i++){
        free(Baron[i]);
    }
    free(Baron);
    free(Xb);
    free(Yb);
    free(Movb);
    free(TempoMovb);
    free(Jafoib);

    return 0;
}
