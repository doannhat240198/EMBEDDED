/*
* File: SPI.ino
* Author: THAI DOAN NHAT																																							
* Date: 18/06/2023
* Description: This is file slave for SPI 
*/

/***********slave**************/
/*cau hinh va gan vao chan tuong tu*/
#define SCK_PIN 4
#define MOSI_PIN 5
#define MISO_PIN 6
#define SS_PIN 7


#define SCK_INPUT DDRD &= ~(1 << DDD4)
#define MOSI_INPUT DDRD &= ~(1 << DDD5)
#define MISO_OUTPUT DDRD |= (1 << DDD6)
#define SS_INPUT DDRD &= ~(1 << DDD7)


#define read_SCK() ((PIND & (1 << PIND4)) ? HIGH:LOW)
#define read_MOSI() ((PIND & (1 << PIND5)) ? HIGH:LOW)
#define write_MISO(x) PORTD = ((x) ? (PORTD | 1 << PD6) : (PORTD & (~(1 << PD6))))
#define read_SS() ((PIND & (1 << PIND7)) ? HIGH:LOW)

void setup() {
  SPI_setup();
}

void loop() {
  uint8_t rev;
  rev = SPI_trensfer('A');
  /*chi co 1 ham truyen nhan vi ben master bat dau thi slave cung bat dau 
  va master ket thuc thi slave cung ket thuc*/
}

void SPI_setup(void) {
  /*ban dau chua thuc hien qua trinh doc ghi*/
  SCK_INPUT;
  MOSI_INPUT;
  MISO_OUTPUT;
  SS_INPUT;
}


/*qua trinh truyen nhan data*/
uint8_t SPI_trensfer(uint8_t byte_out) {
  uint8_t byte_in = 0;//bit doc vao cau hinh mac dinh=0
  uint8_t ibit, res;//bien chay de nhan tung bit

  while( read_SS() == HIGH );/*waiting until ss=0(low), 
  chan ss khi nao xuong thap moi bat dau chyen nhan duoc*/

  for (ibit = 0x80; ibit > 0; ibit = ibit >> 1) 
  {
    res = byte_out & ibit;
    write_MISO(res);

    while (read_SCK() == LOW);
    if (read_MOSI() == HIGH)
      byte_in = byte_in | ibit;

    while (read_SCK() == HIGH);
    //waiting  until sck=0, this is the"end of 1 clock cycle"
  }
}
