/*
* File: SPI.ino
* Author: THAI DOAN NHAT																																							
* Date: 18/06/2023
* Description: This is file master for SPI 
*/

//SPI speed: Mhz(real), khz(simulation)mo phong
#define TFULL 50         //f=20khz,t=50us(full chu ky)
#define THALF TFULL / 2  // nua chu ky la no di chuyen
// master chu dong tao ra sck tu thap len cao

/***********Master**************/
/*cau hinh va gan vao chan tuong tu*/
#define SCK_PIN 4
#define MOSI_PIN 5
#define MISO_PIN 6
#define SS_PIN 7


/*tuong tu pinMode(SCK,4,OUT,PUT),pinMode thuong cau hinh chan 
cho arduino nhung toc do cham k tot cho qua trinh truyen nhan*/
//tiep xuc thang voi thanh ghi de qua trinh truyen nhan tot hon
#define SCK_OUTPUT DDRD |= (1 << DDD4)
#define MOSI_OUTPUT DDRD |= (1 << DDD5)
#define MISO_INPUT DDRD &= ~(1 << DDD6)
#define SS_OUTPUT DDRD |= (1 << DDD7)


#define write_SCK(x) PORTD = ((x) ? (PORTD | 1 << PD4) : (PORTD & (~(1 << PD4))))
/*
mosi_high port D|=(1<<pd5) like(nhu) digital write(mosi_pin,high)
mosi_low port D &= ~(1<<pd5) like(nhu) digital write(mosi_pin,low)
gop high va low a=(value)?x:y
*/
#define write_MOSI(x) PORTD = ((x) ? (PORTD | 1 << PD5) : (PORTD & (~(1 << PD5))))
// dua mosi len high(1) va xuong low(0)-> ghi du lieu vao port D
#define read_MISO() ((PIND & (1 << PIND6)) ? HIGH : LOW)  //digital read(MOSI_PIN) thu vien arduino
#define write_SS(x) PORTD = ((x) ? (PORTD | 1 << PD7) : (PORTD & (~(1 << PD7))))

void setup() {
  SPI_setup();
}

void loop() {
  uint8_t rev;
  SPI_begin();//bat dau qua trinh tryen nhan
  rev = SPI_trensfer('a');// dua data mon gui
  SPI_end();
  delay(1000);
}

void SPI_setup() {
  //mac dinh cau hinh ban dau 4 chan in hay out
  MOSI_OUTPUT;
  MISO_INPUT;
   write_SCK(LOW);  //mac dinh muc thap
  SCK_OUTPUT;
  write_SS(HIGH);  //mac dinh muc cao hoat dong moi cho xuong thap
  SS_OUTPUT;
  delay(1);  //maybe  don't need
}

void SPI_begin() {
  write_SS(LOW); /*thay doi khi su dung mode: 1 master - nhieu slsave
	(change when using mode: one master- many slave)*/
}

/*qua trinh truyen nhan data*/
uint8_t SPI_trensfer(uint8_t byte_out) {
  uint8_t byte_in = 0;
  uint8_t ibit, res;
  /*vong lap 8 chu ky sung dong thoi doc 1 bit va gui 1 bit*/
  for (ibit = 0x80; ibit > 0 ; ibit=ibit >> 1) {
    res = byte_out & ibit;
    write_MOSI(res);  // ghi data
    delayMicroseconds(THALF);
    write_SCK(HIGH);
    if (read_MISO() == HIGH)
      byte_in = byte_in | ibit;
    else byte_in = byte_in & (~ibit);  // don't need
    delayMicroseconds(THALF);
    write_SCK(LOW);  //end 1 clock cycle
  }
  return byte_in;
}

void SPI_end() {
  write_SCK(LOW);
  write_SS(HIGH);
}
