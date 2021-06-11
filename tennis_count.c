 //////////////////////////Contagem_Tennis.c//////////////////////////////////
// AUTOR : Marcus Vinícius Marques Costa                                     //
// FUNÇÃO: Estabelecer a contagem de pontos no jogo de Tênnis                //
// OBS.  : Utiliza RB0 p/ incrementar pontuação do Jogador1;                 //
//         Utiliza RB1 p/ incrementar pontuação do Jogador2;                 //
//         Microcontolador utilizado PIC18F452.                              //
//         Contagem de sets ilimitada (limite = 99)                          //
 /////////////////////////////////////////////////////////////////////////////

/*
VISOR LCD 16x2 (exemplo):
+----------------+
|Jog1» · 15  2  1|
|Jog2»   40  1  0|
+----------------+
*/

////////////////////////////////////////////
//          VARIÁVEIS GLOBAIS             //
////////////////////////////////////////////
unsigned short int pontos_j1 = 0;
unsigned short int pontos_j2 = 0;
unsigned short int games_j1 = 0;
unsigned short int games_j2 = 0;
unsigned short int sets_j1 = 0;
unsigned short int sets_j2 = 0;
unsigned short int sacador1 = 0xFF;
unsigned short int tie_break = 0;
unsigned short int jogador1;
char ponto = 0xA5;
char seta = 0x7E;

////////////////////////////////////////////
//          CONTAGEM DE PONTOS            //
////////////////////////////////////////////
void soma_ponto(){
   if(jogador1){
      pontos_j1++;
      if((pontos_j1>=4 && pontos_j2<(pontos_j1-1) && tie_break==0) || (pontos_j1>=7 && pontos_j2<(pontos_j1-1) && tie_break)){
         pontos_j1=0;
         pontos_j2=0;
         sacador1 = ~sacador1;
         if(games_j1>=5 && games_j2<games_j1 || tie_break){
            tie_break = 0;
            sets_j1++;
            games_j1=0;
            games_j2=0;
            return;
         }
         games_j1++;
      }
   }else{
      pontos_j2++;
      if((pontos_j2>=4 && pontos_j1<(pontos_j2-1) && tie_break==0) || (pontos_j2>=7 && pontos_j1<(pontos_j2-1) && tie_break)){
         pontos_j1=0;
         pontos_j2=0;
         sacador1 = ~sacador1;
         if(games_j2>=5 && games_j1<games_j2 || tie_break){
            tie_break = 0;
            sets_j2++;
            games_j2=0;
            games_j1=0;
            return;
         }
         games_j2++;
      }
   }
   if(games_j1==6 && games_j2==6){
      tie_break = 1;
   }
   if(tie_break && (pontos_j1 + pontos_j2)%2) sacador1 = ~sacador1;
}

////////////////////////////////////////////
//          EXIBIÇÃO DO PLACAR            //
////////////////////////////////////////////
void mostra_placar(){
   unsigned long int temp;
   unsigned char lcd[4];
   if(sacador1){
      lcd8_chr(1,7,ponto);
      lcd8_out(2,7," ");
   }else{
      lcd8_out(1,7," ");
      lcd8_chr(2,7,ponto);
   }
   if(tie_break==0){
      if(pontos_j1 + pontos_j2 > 6 && pontos_j1 != pontos_j2){
         if(pontos_j1 > pontos_j2){
         lcd8_out(1,9,"Ad");
         lcd8_out(2,9,"--");
         }
         if(pontos_j1 < pontos_j2){
         lcd8_out(1,9,"--");
         lcd8_out(2,9,"Ad");
         }
      }else{
         temp = pontos_j1 * 15;
         if(temp>=45) temp = 40;
         bytetostr(temp,lcd);
         lcd8_out(1,8,lcd);
         temp = pontos_j2 * 15;
         if(temp>=45) temp = 40;
         bytetostr(temp,lcd);
         lcd8_out(2,8,lcd);
      }
   }else{
      bytetostr(pontos_j1,lcd);
      lcd8_out(1,8,lcd);
      bytetostr(pontos_j2,lcd);
      lcd8_out(2,8,lcd);
   }
   bytetostr(games_j1,lcd);
   lcd8_out(1,11,lcd);
   bytetostr(games_j2,lcd);
   lcd8_out(2,11,lcd);
   bytetostr(sets_j1,lcd);
   lcd8_out(1,14,lcd);
   bytetostr(sets_j2,lcd);
   lcd8_out(2,14,lcd);   
}

////////////////////////////////////////////
//          FUNÇÃO PRINCIPAL              //
////////////////////////////////////////////
void main(){
   trisD = 0;                         // configura portD como saída
   trisE = 0;                         // configura portE como saída
   trisB.f0 = 1;
   trisB.f1 = 1;
   portD = 0;                         // zera pinos do portD
   portE = 0;                         // zera pinos do portE
   ADCON1 = 0x06;                     // pinos A/D como I/O de uso geral
   lcd8_config(&porte,&portd,2,1,0,7,6,5,4,3,2,1,0);  // Inicializa LCD
   lcd8_cmd(lcd_clear);               // limpa LCD
   lcd8_cmd(lcd_cursor_off);          // desliga cursor do LCD
   lcd8_out(1,1,"Jog1");
   lcd8_out(2,1,"Jog2");
   lcd8_chr(1,5,seta);
   lcd8_chr(2,5,seta);
   mostra_placar();
   while(1){
      if(portB.f1==0){
         jogador1 = 1;
         soma_ponto();
         delay_ms(200);
         mostra_placar();
      }
      if(portB.f0==0){
         jogador1 = 0;
         soma_ponto();
         delay_ms(200);
         mostra_placar();
      }
   }
}