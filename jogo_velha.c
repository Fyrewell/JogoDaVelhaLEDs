#include <16F873A.h>
#device adc=16

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES XT                       //Crystal osc <= 4mhz for PCM/PCH , 3mhz to 10 mhz for PCD
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(clock=4000000)
//------------------------------//
//DEFINICAO DE ATALHOS UTILIZADOS
//-----------------------------//
#define rs pin_b0
#define en pin_b1
#define data1 pin_c0
#define clk1 pin_c1
#define stb1 pin_c2
#define data2 pin_c3
#define clk2 pin_c4
#define stb2 pin_c5
#define c1 pin_a0
#define c2 pin_a1
#define c3 pin_a2
#define lin1 pin_a3
#define lin2 pin_a4
#define lin3 pin_a5
#define L9A pin_c6
#define L9C pin_c7
//-----------------------------//
//CABECALHOS DE FUNCOES PARA SIMPLIFICACAO
//-----------------------------//
void lcd_comandos(unsigned int8 dado);
void lcd_dados(unsigned int8 dado);
void lcd_inicia(void);
void desloca_4094_1(unsigned int8 dado);
void desloca_4094_2(unsigned int8 dado);
unsigned int8 le_teclado(void);
void indica_jogada(void);
void verifica_combinacao(void);
void testa_combinacoes(unsigned int8 cor,unsigned int8 cor9, unsigned int8 *quem_ganhou,unsigned int8 qual);
void imprime_vencedor(unsigned int8 quem_ganhou);

//-----------------------------//
//VARIAVEIS
//-----------------------------//
unsigned int1 verde9=0,vermelho9=0;  //0 = vd, 1 =vm ; 0,0 = apagado 1,1 = apagado
unsigned int8 tecla=255;
unsigned int8 verdes=0;
unsigned int8 vermelhos=0;
unsigned int8 conta_press=0;
unsigned int8 jogo=0,player1=0,player2=0;
unsigned int1 vez=0;

void main()
{

   lcd_inicia();
   lcd_comandos(0x40);    //GRAU! (°)
   lcd_dados(0b00000110);
   lcd_dados(0b00001001);
   lcd_dados(0b00001001);
   lcd_dados(0b00000110);
   lcd_dados(0b00000000);
   lcd_dados(0b00000000);
   lcd_dados(0b00000000);
   lcd_dados(0b00000000); //linha cursor
   lcd_comandos(0x01);
   printf(lcd_dados,"~Jogo da Velha~");
   lcd_comandos(0xC0);
   printf(lcd_dados,"J:%2d P1:%2d P2:%2d",jogo,player1,player2);
   delay_ms(3000);
   
while(TRUE){
   lcd_comandos(0xC0);
   printf(lcd_dados,"J:%2d P1:%2d P2:%2d",jogo,player1,player2);
   
   if (vez==0){
      lcd_comandos(0x80);
      printf(lcd_dados,"Vez: Player 1   ");
   }else{
      lcd_comandos(0x80);
      printf(lcd_dados,"Vez: Player 2   ");
   }
   tecla=le_teclado();
   indica_jogada();
   desloca_4094_1(verdes);
   desloca_4094_2(vermelhos);
   verifica_combinacao();
   
   //printf(lcd_dados,"%c",0x00);
}
}

//------------------------------//
//FUNCOES 
//-----------------------------//

void lcd_comandos(unsigned int8 dado){     //HABILITA COMANDO
   output_low(rs);
   if (bit_test(dado,7)==0){
      output_low(pin_b5);
   }else
      output_high(pin_b5);
   if (bit_test(dado,6)==0){
      output_low(pin_b4);
   }else
      output_high(pin_b4);
   if (bit_test(dado,5)==0){
      output_low(pin_b3);
   }else
      output_high(pin_b3);
   if (bit_test(dado,4)==0){
      output_low(pin_b2);
   }else
      output_high(pin_b2);
   
   output_high(en);
   output_low(en);
   
   if (bit_test(dado,3)==0){
      output_low(pin_b5);
   }else
      output_high(pin_b5);
   if (bit_test(dado,2)==0){
      output_low(pin_b4);
   }else
      output_high(pin_b4);
   if (bit_test(dado,1)==0){
      output_low(pin_b3);
   }else
      output_high(pin_b3);
   if (bit_test(dado,0)==0){
      output_low(pin_b2);
   }else
      output_high(pin_b2);
      
   output_high(en);
   output_low(en);
   delay_ms(2);
}

void lcd_dados(unsigned int8 dado){     //HABILITA A ESCRITA
   output_high(rs);
   
   if (bit_test(dado,7)==0){
      output_low(pin_b5);
   }else
      output_high(pin_b5);
   if (bit_test(dado,6)==0){
      output_low(pin_b4);
   }else
      output_high(pin_b4);
   if (bit_test(dado,5)==0){
      output_low(pin_b3);
   }else
      output_high(pin_b3);
   if (bit_test(dado,4)==0){
      output_low(pin_b2);
   }else
      output_high(pin_b2);
   
   output_high(en);
   output_low(en);
   
   if (bit_test(dado,3)==0){
      output_low(pin_b5);
   }else
      output_high(pin_b5);
   if (bit_test(dado,2)==0){
      output_low(pin_b4);
   }else
      output_high(pin_b4);
   if (bit_test(dado,1)==0){
      output_low(pin_b3);
   }else
      output_high(pin_b3);
   if (bit_test(dado,0)==0){
      output_low(pin_b2);
   }else
      output_high(pin_b2);
      
   output_high(en);
   output_low(en);
}

void lcd_inicia(void){     //INICIA LCD
   output_low(RS);
   delay_ms(15);  //tempo de "power on"
   output_high(pin_b2);
   output_high(pin_b3);
   output_high(en);  //etapa1
   output_low(en);
   delay_ms(5);
   output_high(en);  //etapa2
   output_low(en);
   delay_us(100);
   
   lcd_comandos(0x2);      //INTERFACE 4BITS
   lcd_comandos(0x28);     //DEFINE TAMANHO DO DISPLAY
   lcd_comandos(0x06);     //DEFINE TIPO DE ESCRITA
   lcd_comandos(0x0c);     //DESATIVA CURSOR
   lcd_comandos(0x01);    //LIMPA TELA
   delay_ms(2);
}

void desloca_4094_1(unsigned int8 dado){
   unsigned int8 x=8;
      while(x>0){
         if(bit_test(dado,x-1)==0){
            output_low(data1);
         }else{
            output_high(data1);
         }
         output_high(clk1);
         delay_us(10);
         output_low(clk1);
         x--;
      }
      output_high(stb1);
      delay_us(10);
      output_low(stb1);
}
void desloca_4094_2(unsigned int8 dado){
   unsigned int8 x=8;
      while(x>0){
         if(bit_test(dado,x-1)==0){
            output_low(data2);
         }else{
            output_high(data2);
         }
         output_high(clk2);
         delay_us(10);
         output_low(clk2);
         x--;
      }
      output_high(stb2);
      delay_us(10);
      output_low(stb2);
}

unsigned int8 le_teclado(void){
   output_low(c1);
   output_high(c2);
   output_high(c3);
   delay_ms(10);
   if(input(lin1)==0){
      while(input(lin1)==0);
      return 1;
   }else if(input(lin2)==0){
      while(input(lin2)==0);
      return 4;
   }else if(Input(lin3)==0){
      while(input(lin3)==0);
      return 7;
   }
   output_high(c1);
   output_low(c2);
   output_high(c3);
   delay_ms(10);
   if(input(lin1)==0){
      while(input(lin1)==0);
      return 2;
   }else if(input(lin2)==0){
      while(input(lin2)==0);
      return 5;
   }else if(input(lin3)==0){
      while(input(lin3)==0);
      return 8;
   }
   output_high(c1);
   output_high(c2);
   output_low(c3);
   delay_ms(10);
   if(input(lin1)==0){
      while(input(lin1)==0);
      return 3;
   }else if(input(lin2)==0){
      while(input(lin2)==0);
      return 6;
   }else if(input(lin3)==0){
      while(input(lin3)==0);
      return 9;
   }
      return 255; //não foi press tecla
}

void indica_jogada(void){
  unsigned int8 x;
  for (x=1;x<9;x++){
   if (tecla==x){
      if ((bit_test(verdes,x-1)==0)&&(bit_test(vermelhos,x-1)==0)){
         if (conta_press%2==0){
            bit_set(verdes,x-1);
            vez=1;
         }else{
            bit_set(vermelhos,x-1);
            vez=0;
         }
         conta_press++;
      }else{
         printf(lcd_dados,"Jogada Invalida!");
         delay_ms(1000);
         lcd_comandos(0x01);
      }
   }
  }
  if (tecla==9){
      if ((verde9==0)&&(vermelho9==0)){
         if (conta_press%2==0){
            verde9=1;
            output_high(L9A);
            output_low(L9C);
            vez=1;
         }else{
            vermelho9=1;
            output_high(L9C);
            output_low(L9A);
            vez=0;
         }
         conta_press++;
      }else{
         printf(lcd_dados,"Jogada Invalida!");
         delay_ms(1000);
         lcd_comandos(0x01);
      }
  }
}

void verifica_combinacao(void){
   unsigned int8 quem_ganhou=0;
   
   testa_combinacoes(verdes,verde9,&quem_ganhou,1);
   if (quem_ganhou!=0)
   imprime_vencedor(quem_ganhou);
   testa_combinacoes(vermelhos,vermelho9,&quem_ganhou,2);
   if (quem_ganhou!=0)
   imprime_vencedor(quem_ganhou);

   //23/11/2013 -- o teste abaixo é pra ver se não encontrou nenhum vencedor e acabaram as jogadas [empate]
   if (((verdes|vermelhos)==0b11111111)&&((verde9|vermelho9)==0b1)){  //faz ou nas duas mascaras para verificar se foi preenchimento total (todos acesos): 11111111 e o led9=1
      //empate: coloquei para reiniciar a jogada e ninguem pontua, poderia ser feito para os dois pontuarem
      jogo++;
      verdes=0;
      vermelhos=0;
      verde9=0;
      vermelho9=0;
      lcd_comandos(0x80);
      printf(lcd_dados,"Empatou!        ");
      delay_ms(1000);
      output_low(L9C);
      output_low(L9A);
      conta_press=0;
      vez=0;
   }
}

void testa_combinacoes(unsigned int8 cor,unsigned int8 cor9, unsigned int8 *quem_ganhou,unsigned int8 qual){
   unsigned int1 flag_ganhou=0;
   if ((cor&0b00000111)==0b00000111){
      flag_ganhou=1;
   }else if ((cor&0b00111000)==0b00111000){
      flag_ganhou=1;
   }else if (((cor&0b11000000)==0b11000000)&&(cor9==1)){
      flag_ganhou=1;
   }else if (((cor&0b00010001)==0b00010001)&&(cor9==1)){
      flag_ganhou=1;
   }else if ((cor&0b01010100)==0b01010100){
      flag_ganhou=1;
   }else if (((cor&0b00100100)==0b00100100)&&(cor9==1)){
      flag_ganhou=1;
   }else if ((cor&0b01001001)==0b01001001){
      flag_ganhou=1;
   }else if ((cor&0b10010010)==0b10010010){
      flag_ganhou=1;
   }
   if (flag_ganhou==1){
      if (qual==1){
         player1++;
         *quem_ganhou=1;
      }else{
         player2++;
         *quem_ganhou=2;
      }
   }else *quem_ganhou=0;
}

void imprime_vencedor(unsigned int8 quem_ganhou){
   if (quem_ganhou!=0){
      jogo++;
      verdes=0;
      vermelhos=0;
      verde9=0;
      vermelho9=0;
      lcd_comandos(0x80);
      printf(lcd_dados,"Jogador %d venceu!",quem_ganhou);
      delay_ms(1000);
      output_low(L9C);
      output_low(L9A);
      conta_press=0;
      vez=0;
   }
}
