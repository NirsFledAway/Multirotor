#include <mbed.h>
#include <esc_pwm.h>

// Serial pc(PA_9, PA_10, 115200);
Serial pc(PA_9, PA_10, 115200);

// const PinName SENSOR = PinName::PB_13;
// const PinName LED = PinName::PB_12;
const PinName CURRENT_SENSOR = PinName::PA_0;

// DigitalOut led(LED);
// InterruptIn sensor(SENSOR);
AnalogIn currentSensor(CURRENT_SENSOR);
// DigitalIn sensor(SENSOR);
// volatile unsigned long long rpm = 0; 
// volatile unsigned long long last_time = 0;
// volatile bool led_status = false;
// volatile long long checks = 0;

Timeout flipper, flipper1;

// void printRPMSerial() {
  
//   pc.printf("%llu; %llu\n", rpm, checks);

//   // pc.printf("SystemCoreClock = %d", SystemCoreClock);
//   rpm = 0;
//   checks = 0;
//   // last_time =  Kernel::get_ms_count();
//   flipper.attach(&printRPMSerial, 1.0);
// }
// void print_val() {
//   pc.printf("%d\n", sensor.read());
// }

// void readRPM() {
//   ++rpm;
//   // led_status = !led_status;
//   // led.write(led_status);

//   wait_us(20);
// }

// void printADC() {
//   bool val = sensor.read();
//   led.write(sensor.read());
// }

// void doRPMCalc() {
//   // sensor.rise(&readRPM);
//   flipper.attach(&printRPMSerial, 1.0);
//   bool val;
//   bool hatch_beneath = false;
//   while(1) {
//     val = sensor.read();
//     ++checks;
//     if (val) {
//       if (!hatch_beneath) {
//         hatch_beneath = true;
//         ++rpm;
//         wait_us(10);
//       }
//     } else {
//       if (hatch_beneath) {
//         if (!val)
//           hatch_beneath = false;
//           wait_us(10);
//       }
//     }
//   }
// }

void printCurrent() {
  const float amperage_meter_scale = 445/3.3;
  const float amperage_meter_offset = 0/3.3;
  const float REF_VOLTAGE = 3.3;  // V

  float val = currentSensor.read(); // [0.0; 1.1]
  val *= 1000;  // mV
  
  pc.printf("Amps: %f\n", (val * REF_VOLTAGE) / amperage_meter_scale * 10 + amperage_meter_offset / 1000);
  flipper.attach(&printCurrent, 0.5);
}

void handleSerial() {
  char buff[100];
  char i = 0, c = ' ';
  int num = pc.read((uint8_t*)buff, 10, nullptr);
  if (num > 0) {
    pc.printf("Got %d\n", num);
    bool got_whole_line = 0;
    int pwm_val = 0;
    for (int j = 0; j < num; ++j) {
      c = buff[j];
      if (c == '\n' || c == '\r') {
          got_whole_line = 1;
      }
      if (c >= '0' && c <= '9') {
        pwm_val *= 10 + (int)(c - '0');
      }
    }
    pc.printf("Got1 %d\n", got_whole_line);
    i += num;
    if (got_whole_line) {
      // pc.printf("Got\n");
      // esc_write(pwm_val);
      i = 0;
    }
  }
  // flipper1.attach(&handleSerial, 1);
}
void doCurrentCalc() {
  // pc.set_baudrate(115200);
  // pc.set_format(
  //     /* bits */ 8,
  //     /* parity */ BufferedSerial::None,
  //     /* stop bit */ 1
  // );

  // wait(3);
  arm(&pc);
  flipper.attach(&printCurrent, 0.5);
  // flipper1.attach(&handleSerial, 1);
  while(1) {
    // handleSerial();
    int i = 1000;
    for(i = 1000; i < 1700; i+=50) {
      esc_write(i);
      pc.printf("ESC: %d\n", i);
      wait(10);
    }
    wait(5);
ampermetr currentSensor
1050 0.16 0
1100 0.36 0
1150 0.61 0.0
1200 0.96 1
1250 1.5 2
1300 2.14 3.4
1350 3.0 5.2
1400 4.0 7.3
1450 4.96 9.2
1500 6.07 11.8
1550 7.37 14.6
1600 9.03 17.7
1650 10.57 21
  
  }
}

int main() {
  doCurrentCalc();
//   Ток Скважность Вес
//   0.3 1050  0 57
//   0.82  1100  9.55
//   1.72  1150  21.5
//   3.24  120 49.0 197
//   5.04  1250  57  230
//   7.35  1300  75.6
//   10.36 1350  94

//   Ток знач
//   0.28  0.034
//   0.78  0.21
//   1.68  0.60 
//   3.21  0.124
// 4.89  0.21
// 7.22  0.30
// 10.22 0.44

}
