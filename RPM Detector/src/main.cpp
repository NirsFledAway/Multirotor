#include <mbed.h>
#include <esc_pwm.h>
#include <algorithm>

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

// static float currentValue = 0;
// static float currentLog[10] = {0};
// static
// const int windowSize = 10;

class MeanValue {
public:
  MeanValue() {
    for(int i = 0; i < size_; ++i) {
      values_[i] = 0;
    }
  }

  void append(float val) {
    values_[last_idx] = val;
    last_idx = (last_idx + 1) % size_;
  }

  float get() {
    // std::sort(values_, values_+size_);
    // return values_[size_/2];
    float val = 0;
    for (float v : values_) {
      val += v;
    }
    return val / size_;
  }

  const static int size_ = 20;
  float values_[size_];
  int last_idx = 0;
};

MeanValue currentValue, voltageValue;


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
  // const float amperage_meter_scale = 445/3.3;
  // const float amperage_meter_offset = 0/3.3;
  // const float REF_VOLTAGE = 3.3;  // V

  // float val = currentSensor.read(); // [0.0; 1.1]
  // val *= 1000;  // mV
  
  // pc.printf("Amps: %f\n", (val * REF_VOLTAGE) / amperage_meter_scale * 10 + amperage_meter_offset / 1000);
  pc.printf("Amps: %.1f, v: %.2f\n", currentValue.get(), voltageValue.get());
  flipper.attach(&printCurrent, 0.5);
}

void calcCurrent() {
  const float amperage_meter_scale = 285;
  const float amperage_meter_offset = 500;
  const float REF_VOLTAGE = 3.3;  // V

  float val = currentSensor.read(); // [0.0; 1.1]
  val *= 1000;  // mV

  voltageValue.append(val);
  
  currentValue.append((val * REF_VOLTAGE) / amperage_meter_scale * 10 + amperage_meter_offset / 1000);
  flipper1.attach(&calcCurrent, 0.05);
}

// void handleSerial() {
//   char buff[100];
//   char i = 0, c = ' ';
//   int num = pc.read((uint8_t*)buff, 10, nullptr);
//   if (num > 0) {
//     pc.printf("Got %d\n", num);
//     bool got_whole_line = 0;
//     int pwm_val = 0;
//     for (int j = 0; j < num; ++j) {
//       c = buff[j];
//       if (c == '\n' || c == '\r') {
//           got_whole_line = 1;
//       }
//       if (c >= '0' && c <= '9') {
//         pwm_val *= 10 + (int)(c - '0');
//       }
//     }
//     pc.printf("Got1 %d\n", got_whole_line);
//     i += num;
//     if (got_whole_line) {
//       // pc.printf("Got\n");
//       // esc_write(pwm_val);
//       i = 0;
//     }
//   }
//   // flipper1.attach(&handleSerial, 1);
// }
void doCurrentCalc() {
  // pc.set_baudrate(115200);
  // pc.set_format(
  //     /* bits */ 8,
  //     /* parity */ BufferedSerial::None,
  //     /* stop bit */ 1
  // );

  // wait(3);
  // arm(&pc);
  // wait(5);
  esc_init();
  pc.printf("Connect the esc\n");
  wait(5);
  // setupLimits(&pc);
  // wait(2);
  arm(&pc);
  flipper1.attach(&calcCurrent, 0.05);
  flipper.attach(&printCurrent, 0.8);
  // flipper1.attach(&handleSerial, 1);
  while(1) {
    // handleSerial();
    int i = 1000;
    for(i = 1050; i <= 2000; i+=50) {
      esc_write(i);
      pc.printf("ESC: %d\n", i);
      wait(3);
    }
  }
}

int main() {
  doCurrentCalc();
}
