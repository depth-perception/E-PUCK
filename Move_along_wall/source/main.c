#include <p30f6014a.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "btcom.h"
#include "e_randb.h"
#include "e_prox.h"
#include "e_uart_char.h"
#include "e_init_port.h"
#include "e_motors.h"
#include "mlib.h"


#define M_PI (3.14159)
#define J 5

int main() {

    e_init_port();
    e_init_uart1();
    e_init_randb(I2C);

    btcomWaitForCommand('s');

    e_randb_set_range(0);
    e_randb_set_calculation(ON_BOARD);
    e_randb_store_light_conditions();

    unsigned int data = 2;
    
    char tmp2[40];
    sprintf(tmp2,"This robot number is %d\n",data);
    btcomSendString(tmp2);
    
    unsigned int rec_data = 0;
    double angle = 5;
    unsigned int range;
    
    int speedl = 0;
    int speedr = 0;
    int speed_outside,speed_inside;
    
    //variables of pid controler
    double kp_r = 20,kd_r = 0;//,ki_r = 0.2;
    double kp_a = 10,kd_a = 3;//,ki_a = 0.2;
    int output_r,input_r = 1000;
    double output_a,input_a = M_PI/2;
    int err_r,dErr_r,lastErr_r = 0;//,errSum_r = 0;
    double err_a,dErr_a,lastErr_a = 0.0;//,errSum_a = 0.0;
    int sampleTime  = 1;
    
    unsigned int i = 0;
    
    while(1)
    {
        e_set_speed_left(speedl);
        e_set_speed_right(speedr);
        
        unsigned int _range[J] = {0};
        double _angle[J] = {0.0};
        unsigned int _rec_data[J] = {0};
        
        int j = 0;int k = 0;
        for(j=0;j<J&&k<300;k++)
        {
            e_randb_send_all_data(data);
            for(i=0;i<10000;i++);
            if(e_randb_get_if_received() != 0)
            {
                _rec_data[j] = e_randb_get_data();
                _range[j] = e_randb_get_range();
                _angle[j] = e_randb_get_bearing();
                j++;
            }
        }
        
        speedl = 200;
        speedr = 200;
        
        if(j<J/2) 
        {
            e_set_speed_left(speedl);
            e_set_speed_right(speedr);
            continue; 
        }
        
        m_sort3(_range,_angle,_rec_data,j);//sort the _range[5] and _rec_data[5]
        rec_data = _rec_data[j/2+1];angle = _angle[j/2+1];range = _range[j/2+1];
        
        if(rec_data == data)
        {
//            if(angle>0) input_a = M_PI/2;
//            else input_a = -M_PI/2;
        
            err_r = range - input_r;err_a = angle - input_a;
            //errSum_r += (err_r * sampleTime);errSum_a += (err_a * sampleTime);
            dErr_r = (err_r - lastErr_r) / sampleTime;dErr_a = (err_a - lastErr_a) / sampleTime;
            output_r = kp_r * err_r + kd_r * dErr_r;// + ki_r * errSum_r;
            output_a = kp_a * err_a + kd_a * dErr_a;// + ki_a * errSum_a;
            lastErr_r = err_r;lastErr_a = err_a;
            
            speed_outside = 200 + output_a * 30 * sin(input_a) - output_r / 200;
            speed_inside = 200 - output_a * 30 * sin(input_a) + output_r / 200;
            
//            if(input_a > 0)
//            {
                speedl = speed_inside * (1 + sin(input_a))/2  + speed_outside * (1 - sin(input_a))/2;
                speedr = speed_inside * (1 - sin(input_a))/2  + speed_outside * (1 + sin(input_a))/2;
//            }
//            else
//            {
//                speedl = speed_outside;
//                speedr = speed_inside;
//            }
            
        }
        
//            if(angle<0 && angle>-M_PI/2 && rec_data==data)
//            {
//                speed_outside = -(int)(sin(angle+M_PI/2)*600) + 400 - range/30; // 20
//                speed_inside = (int)(sin(M_PI/2+angle)*600) + 80 + range/8;// 8
//                speedl = speed_outside; // ;
//                speedr = speed_inside;// 160 -;
//            }
//            if(angle<-M_PI/2 && rec_data==data)
//            {
//                speed_outside = -(int)(sin(angle+M_PI/2)*600) + 400 - range/30; // ;
//                speed_inside = (int)(sin(M_PI/2+angle)*600) + 80 + range/8;// 
//                speedl = speed_outside; // ;
//                speedr = speed_inside;// 160 -; 
//            }
//            if(angle>0 && angle<M_PI/2 && rec_data==data)
//            {
//                speed_outside = (int)(sin(angle-M_PI/2)*600) + 400 - range/30; // ;
//                speed_inside = (int)(sin(M_PI/2-angle)*600) + 80 + range/8;// 
//                speedr = speed_outside; // ;
//                speedl = speed_inside; //160 - ;
//            }
//            if(angle>M_PI/2 && rec_data==data)
//            {
//                speed_outside = (int)(sin(angle-M_PI/2)*600) + 400 - range/30; // 20
//                speed_inside = (int)(sin(M_PI/2-angle)*600) + 80 + range/8;// 8 
//                speedr = speed_outside; // ;
//                speedl = speed_inside; //160 - 
//            }
        

      
    }
    
    return 0;
}

