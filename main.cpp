#include "mbed.h"
#include "bbcar.h"

DigitalOut led1(LED1);
Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmIn servo0_f(D9), servo1_f(D10);
PwmOut servo0_c(D11), servo1_c(D12);

DigitalInOut pin8(D8);

BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);

int main() {
   float angle;
   parallax_ping  ping1(pin8);
   float min1 = 200, min2 = 200;
   int angular1, angular2;
   int i = 0;
   float dist[80];
   int ang[80];
   int former;
   int flag1 = 0;
   int flag2 = 0;
   car.rotate(40);
   dist[0] = 199;
   while(i<40) {
       former = i - 1;
       dist[i] = (float)ping1;
       printf("min1 = %f \n", dist[i]);
       ang[i] = car.servo1.angle;
       if(i > 0){
        if(((dist[former] + 4) < dist[i]) && (flag1 == 0)) {
          min1 = dist[(i - 1)]+2;
          angular1 = ang[former];
          flag1++;
        }
        else if((dist[(former)] - 2) > dist[i]) {
          flag2 = 1;
          min2 = dist[i]+2;
          angular2 = ang[i];
        }
        if ((flag2 == 1) && (dist[i] + 3 < dist[former])){
            min2 = dist[i]+2;
          angular2 = ang[i];
        }
       }
      i++;
      ThisThread::sleep_for(60ms);
   }
   angle = (angular2 - angular1)*0.47/180*3.14;
      float distance = sqrt(min1*min1 + min2*min2 - 2*min1*min2*cos(angle));
      printf("min1 = %f \n", min1);
      printf("angular1 = %d \n", angular1);
      printf("min2 = %f \n", min2);
      printf("angular2 = %d \n", angular2);
      printf("distance = %f \n", distance);
    car.stop();
}