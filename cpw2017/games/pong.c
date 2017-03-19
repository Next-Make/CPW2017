/*
 * pong.c
 *
 * You win if you can use your paddle to hit
 * the ball into your opponent's goal five times
 * before they are able to!
 */


#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>


//PA0 BUT_R_R
//PA1 BUT_R_U
//PA2 BUT_R_D
//PA3 BUT_R_L
//PA4 
//PA5 
//PA6 
//PA7 

//PB0 BUT_L_U
//PB1 BUT_L_R
//PB2 BUT_L_D
//PB3 
//PB4 
//PB5 
//PB6 BUT_L_L
//PB7 

//If the LED matrix is in the right way
//char correctLED = 1;
//PC0 Extra2
//PC1 Row1-
//PC2 Row2-
//PC3 Row3-
//PC4 Row4-
//PC5 Row5-
//PC6 Reset*
//PC7 Extra3

//PD0 Extra1
//PD1 Col1+
//PD2 Col2+
//PD3 Col3+
//PD4 Col4+
//PD5 Col5+
//PD6 Col6+
//PD7 Col7+


//If the LED matrix is in backwards
char correctLED = 0;
//PC0 Extra2
//PC1 Row4-//
//PC2 Col4+//
//PC3 Col5+//
//PC4 Row1-//
//PC5 Col3+//
//PC6 Reset*
//PC7 Extra3

//PD0 Extra1
//PD1 Col7+//
//PD2 Col6+//
//PD3 Row5-//
//PD4 Row2-//
//PD5 Row3-//
//PD6 Col2+//
//PD7 Col1+//




//current LED output
//0 is off
//1 is on dim
//2 in on bright
unsigned char outputLed[7][5];
char buttons[8];

unsigned char row = 0;
void writeLeds()
{
    if(correctLED)
    {
        //turn off all rows
        DDRC |= (1<<PC1);
        DDRC |= (1<<PC2);
        DDRC |= (1<<PC3);
        DDRC |= (1<<PC4);
        DDRC |= (1<<PC5);

        PORTC |= (1<<PC1);
        PORTC |= (1<<PC2);
        PORTC |= (1<<PC3);
        PORTC |= (1<<PC4);
        PORTC |= (1<<PC5);

        if(row==1)
        {
            //turn on correct row
            PORTC &= ~(1<<PC1);
        }
        else if(row==2)
        {
            //turn on correct row
            PORTC &= ~(1<<PC2);
        }
        else if(row==3)
        {
            //turn on correct row
            PORTC &= ~(1<<PC3);
        }
        else if(row==4)
        {
            //turn on correct row
            PORTC &= ~(1<<PC4);
        }
        else
        {
            //turn on correct row
            PORTC &= ~(1<<PC5);
        }

        //turn on Leds in that row
        for(unsigned char i=0; i<7; i++)
        {
            if(outputLed[i][row]==0)
            {
                DDRD |= (1<<(i+1));
                PORTD &= ~(1<<(i+1));
            }
            else if(outputLed[i][row]==1)
            {
                DDRD &= ~(1<<(i+1));
                PORTD |= (1<<(i+1));
            }
            else
            {
                DDRD |= (1<<(i+1));
                PORTD |= (1<<(i+1));
            }
        }

    }
    else
    {
        //turn off all rows
        DDRC |= (1<<PC4);
        DDRD |= (1<<PD4);
        DDRD |= (1<<PD5);
        DDRC |= (1<<PC1);
        DDRD |= (1<<PD3);

        PORTC |= (1<<PC4);
        PORTD |= (1<<PD4);
        PORTD |= (1<<PD5);
        PORTC |= (1<<PC1);
        PORTD |= (1<<PD3);

        //and cols
        DDRD |= (1<<PD7);
        PORTD &= ~(1<<PD7);
        DDRD |= (1<<PD6);
        PORTD &= ~(1<<PD6);
        DDRC |= (1<<PC5);
        PORTC &= ~(1<<PC5);
        DDRC |= (1<<PC2);
        PORTC &= ~(1<<PC2);
        DDRC |= (1<<PC3);
        PORTC &= ~(1<<PC3);
        DDRD |= (1<<PD2);
        PORTD &= ~(1<<PD2);
        DDRD |= (1<<PD1);
        PORTD &= ~(1<<PD1);



        if(row==0)
        {
            //turn on correct row
            PORTC &= ~(1<<PC4);
        }
        else if(row==1)
        {
            //turn on correct row
            PORTD &= ~(1<<PD4);
        }
        else if(row==2)
        {
            //turn on correct row
            PORTD &= ~(1<<PD5);
        }
        else if(row==3)
        {
            //turn on correct row
            PORTC &= ~(1<<PC1);
        }
        else
        {
            //turn on correct row
            PORTD &= ~(1<<PD3);
        }


        //turn on Leds in that row
        //col1
        if(outputLed[6][4-row]==0)
        {

        }
        else if(outputLed[6][4-row]==1)
        {
            DDRD &= ~(1<<PD7);
            PORTD |= (1<<PD7);
        }
        else
        {
            DDRD |= (1<<PD7);
            PORTD |= (1<<PD7);
        }
        //col2
        if(outputLed[5][4-row]==0)
        {

        }
        else if(outputLed[5][4-row]==1)
        {
            DDRD &= ~(1<<PD6);
            PORTD |= (1<<PD6);
        }
        else
        {
            DDRD |= (1<<PD6);
            PORTD |= (1<<PD6);
        }
        //col3
        if(outputLed[4][4-row]==0)
        {

        }
        else if(outputLed[4][4-row]==1)
        {
            DDRC &= ~(1<<PC5);
            PORTC |= (1<<PC5);
        }
        else
        {
            DDRC |= (1<<PC5);
            PORTC |= (1<<PC5);
        }
        //col4
        if(outputLed[3][4-row]==0)
        {

        }
        else if(outputLed[3][4-row]==1)
        {
            DDRC &= ~(1<<PC2);
            PORTC |= (1<<PC2);
        }
        else
        {
            DDRC |= (1<<PC2);
            PORTC |= (1<<PC2);
        }
        //col5
        if(outputLed[2][4-row]==0)
        {

        }
        else if(outputLed[2][4-row]==1)
        {
            DDRC &= ~(1<<PC3);
            PORTC |= (1<<PC3);
        }
        else
        {
            DDRC |= (1<<PC3);
            PORTC |= (1<<PC3);
        }
        //col6
        if(outputLed[1][4-row]==0)
        {

        }
        else if(outputLed[1][4-row]==1)
        {
            DDRD &= ~(1<<PD2);
            PORTD |= (1<<PD2);
        }
        else
        {
            DDRD |= (1<<PD2);
            PORTD |= (1<<PD2);
        }
        //col7
        if(outputLed[0][4-row]==0)
        {

        }
        else if(outputLed[0][4-row]==1)
        {
            DDRD &= ~(1<<PD1);
            PORTD |= (1<<PD1);
        }
        else
        {
            DDRD |= (1<<PD1);
            PORTD |= (1<<PD1);
        }

    }

    row += 1;
    if(row >= 5)
    {
        row = 0;
    }
}

void clearLeds()
{
    for(unsigned char i=0; i<7; i++)
    {
        for(unsigned char j=0; j<5; j++)
        {
            outputLed[i][j] = 0;
        }
    }
}

void getButtons()
{
    //button 0
    if(PINB & (1<<PB0))
    {
        buttons[0] -= 1;
    }
    else
    {
        buttons[0] += 1;
    }
    //button 1
    if(PINA & (1<<PA1))
    {
        buttons[1] -= 1;
    }
    else
    {
        buttons[1] += 1;
    }
    //button 2
    if(PINB & (1<<PB6))
    {
        buttons[2] -= 1;
    }
    else
    {
        buttons[2] += 1;
    }
    //button 3
    if(PINB & (1<<PB1))
    {
        buttons[3] -= 1;
    }
    else
    {
        buttons[3] += 1;
    }
    //button 4
    if(PINA & (1<<PA3))
    {
        buttons[4] -= 1;
    }
    else
    {
        buttons[4] += 1;
    }
    //button 5
    if(PINA & (1<<PA0))
    {
        buttons[5] -= 1;
    }
    else
    {
        buttons[5] += 1;
    }
    //button 6
    if(PINB & (1<<PB2))
    {
        buttons[6] -= 1;
    }
    else
    {
        buttons[6] += 1;
    }
    //button 7
    if(PINA & (1<<PA2))
    {
        buttons[7] -= 1;
    }
    else
    {
        buttons[7] += 1;
    }

    for(unsigned char i=0; i<8; i++)
    {
        if(buttons[i]>20)
        {
            buttons[i] = 20;
        }
        else if(buttons[i]<0)
        {
            buttons[i] = 0;
        }
    }

}


unsigned char pong_posL = 2;
unsigned char pong_posR = 2;
unsigned int pong_delay = 0;

char pong_velY = 1;
char pong_velX = 1;
unsigned char pong_balX = 3;
unsigned char pong_balY = 2;

unsigned char pong_scoreL = 5;
unsigned char pong_scoreR = 5;
unsigned char pong_scoreTimer = 0;
unsigned char pong_score = 1;

void pong()
{
    pong_delay += 1;
    if(pong_delay>100)
    {
        pong_delay = 0;

        if(pong_score == 1)
        {

            if(pong_scoreTimer>5)
            {
                if((pong_scoreL == 5) | (pong_scoreR == 5))
                {
                    pong_scoreL = 0;
                    pong_scoreR = 0;
                }
                if( buttons[0] | buttons[1] | buttons[2] | buttons[3] | buttons[4] | buttons[5] | buttons[6] | buttons[7])
                {
                    pong_score = 0;
                    pong_scoreTimer = 0;
                    clearLeds();
                }
            }
            else
            {
                pong_scoreTimer += 1;
                clearLeds();
                //draw scores
                unsigned char l = 0;
                while(l<pong_scoreL)
                {
                    outputLed[0][l] = 2;
                    outputLed[1][l] = 2;
                    outputLed[2][l] = 2;
                    l += 1;
                }
                unsigned char r = 0;
                while(r<pong_scoreR)
                {
                    outputLed[4][r] = 2;
                    outputLed[5][r] = 2;
                    outputLed[6][r] = 2;
                    r += 1;
                }
            }
        }
        else
        {


            //left up
            if(buttons[0]>3)
            {
                pong_posL += 1;
                if(pong_posL>=3)
                {
                    pong_posL = 3;
                }
            }
            //left down
            if(buttons[6]>3)
            {
                if(pong_posL==0)
                {
                    pong_posL = 0;
                }
                else
                {
                    pong_posL -= 1;
                }
            }

            //draw paddles
            outputLed[0][0] = 0;
            outputLed[0][1] = 0;
            outputLed[0][2] = 0;
            outputLed[0][3] = 0;
            outputLed[0][4] = 0;

            outputLed[0][pong_posL] = 2;
            outputLed[0][pong_posL+1] = 2;


            //right up
            if(buttons[1]>3)
            {
                pong_posR += 1;
                if(pong_posR>=3)
                {
                    pong_posR = 3;
                }
            }
            //right down
            if(buttons[7]>3)
            {
                if(pong_posR==0)
                {
                    pong_posR = 0;
                }
                else
                {
                    pong_posR -= 1;
                }
            }

            //draw paddles
            outputLed[6][0] = 0;
            outputLed[6][1] = 0;
            outputLed[6][2] = 0;
            outputLed[6][3] = 0;
            outputLed[6][4] = 0;

            outputLed[6][pong_posR] = 2;
            outputLed[6][pong_posR+1] = 2;

            //update ball
            //erase old ball
            outputLed[pong_balX][pong_balY] = 0;
            //update velocities
            if(pong_balY == 4)
            {
                pong_velY = -pong_velY;
            }
            if(pong_balY == 0)
            {
                pong_velY = -pong_velY;
            }
            if(pong_balX == 6)
            {
                if(pong_balY == pong_posR+1)
                {
                    if(pong_velY != 1)
                    {
                        pong_velY += 1;
                    }
                }
                else if(pong_balY == pong_posR)
                {
                    if(pong_velY != -1)
                    {
                        pong_velY -= 1;
                    }

                }
                else
                {
                    pong_score = 1;
                    pong_scoreL += 1;
                }
                pong_velX = -pong_velX;
            }
            if(pong_balX == 0)
            {
                if(pong_balY == pong_posL+1)
                {
                    if(pong_velY != 1)
                    {
                        pong_velY += 1;
                    }

                }
                else if(pong_balY == pong_posL)
                {
                    if(pong_velY != -1)
                    {
                        pong_velY -= 1;
                    }

                }
                else
                {
                    pong_score = 1;
                    pong_scoreR += 1;
                }
                pong_velX = -pong_velX;
            }
            //get new ball coordinates
            pong_balX = pong_balX + pong_velX;
            pong_balY = pong_balY + pong_velY;
            if(pong_balY > 4)
            {
                pong_balY = 4;
            }
            else if(pong_balY == 0)
            {
                pong_balY = 0;
            }
            if(pong_balX > 6)
            {
                pong_balY = 6;
            }
            else if(pong_balY == 0)
            {
                pong_balY = 0;
            }

            //draw new ball
            outputLed[pong_balX][pong_balY] = 2;
        }

    }
}

void buttonTest()
{
    clearLeds();

    #define BUT 5

    if(buttons[0]>BUT)
    {
        outputLed[1][3] = 2;
    }
    else
    {
        outputLed[1][3] = 0;
    }
    if(buttons[1]>BUT)
    {
        outputLed[5][3] = 2;
    }
    else
    {
        outputLed[5][3] = 0;
    }
    if(buttons[2]>BUT)
    {
        outputLed[0][2] = 2;
    }
    else
    {
        outputLed[0][2] = 0;
    }
    if(buttons[3]>BUT)
    {
        outputLed[2][2] = 2;
    }
    else
    {
        outputLed[2][2] = 0;
    }
    if(buttons[4]>BUT)
    {
        outputLed[4][2] = 2;
    }
    else
    {
        outputLed[4][2] = 0;
    }
    if(buttons[5]>BUT)
    {
        outputLed[6][2] = 2;
    }
    else
    {
        outputLed[6][2] = 0;
    }
    if(buttons[6]>BUT)
    {
        outputLed[1][1] = 2;
    }
    else
    {
        outputLed[1][1] = 0;
    }
    if(buttons[7]>BUT)
    {
        outputLed[5][1] = 2;
    }
    else
    {
        outputLed[5][1] = 0;
    }

}

//unsigned char posL = 0;
//unsigned char posR = 0;
unsigned char posx = 0;
unsigned char posy = 0;
unsigned int delay = 0;
void animation()
{

    delay += 1;
    if(delay>100)
    {
        delay = 0;
        /*
        //left up
        if(buttons[0]>5)
        {
            posL += 1;
            if(posL>=5)
            {
                posL = 4;
            }
        }
        //left down
        if(buttons[2]>5)
        {
            if(posL==0)
            {
                posL = 0;
            }
            else
            {
                posL -= 1;
            }
        }
        */
        outputLed[posx][posy] = 1;
        posx += 1;
        if(posx >= 7)
        {
            posx = 0;
            posy += 1;
            if(posy >= 5)
            {
                posy = 0;
                clearLeds();
            }
        }
        outputLed[posx][posy] = 2;
    }



    //clearLeds();

    //left side
    //outputLed[0][posL] = 2;
    //outputLed[0][posL+1] = 2;
    //right side
    //outputLed[7][posR] = 2;
    //outputLed[7][posR+1] = 2;
}

int main(void)
{

    DDRC = 0b10111111;
    DDRD = 0b11111111;
    PORTC = 0b00000000;
    PORTD = 0b00000000;

    //set up things for button inputs
    DDRA &= ~(1<<PA0);//BUT_R_R
    PORTA |= (1<<PA0);
    DDRA &= ~(1<<PA1);//BUT_R_U
    PORTA |= (1<<PA1);
    DDRA &= ~(1<<PA2);//BUT_R_D
    PORTA |= (1<<PA2);
    DDRA &= ~(1<<PA3);//BUT_R_L
    PORTA |= (1<<PA3);

    DDRB &= ~(1<<PB0);//PB0 BUT_L_U
    PORTB |= (1<<PB0);
    DDRB &= ~(1<<PB1);//PB1 BUT_L_R
    PORTB |= (1<<PB1);
    DDRB &= ~(1<<PB2);//PB2 BUT_L_D
    PORTB |= (1<<PB2);
    DDRB &= ~(1<<PB6);//PB6 BUT_L_L
    PORTB |= (1<<PB6);

    while(1) {
        //animation();
        //buttonTest();
        pong();
        //clearLeds();

        //left side
        //outputLed[0][0] = buttons[0];
        //outputLed[0][1] = buttons[2];

        writeLeds();
        getButtons();

        _delay_ms(1);
    }
}
