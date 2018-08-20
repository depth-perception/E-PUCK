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


#define M_PI (3.141593 * 5 / 6)

int main() {

    e_init_port();
    e_init_uart1();
    e_init_randb(I2C);

    btcomWaitForCommand('s');

    e_randb_set_range(0);
    e_randb_set_calculation(ON_BOARD);
    e_randb_store_light_conditions();

    unsigned int data = 2971;
    
    char tmp2[40];
    sprintf(tmp2,"This robot number is %d\n",data);
    btcomSendString(tmp2);
    
    unsigned int rec_data = 0;
    double angle = 0.0;
    unsigned int range;
    
    int speedl = 0;
    int speedr = 0;
    int speed_max,speed_min;
    
   // unsigned int i = 0;
    
    while(1)
    {
        unsigned int _range[10] = {0,0,0,0,0,0,0,0,0,0};
        double _angle[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
        unsigned int _rec_data[10] = {0,0,0,0,0,0,0,0,0,0};
        
        int j = 0;int k = 0;
        for(j=0;j<10&&k<500;k++)
        {
            //e_randb_send_all_data(data);
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
        
        if(j<10) 
        {
            e_set_speed_left(speedl);
            e_set_speed_right(speedr);
            continue; 
        }
        
        m_sort3(_range,_angle,_rec_data,10);//sort the _range[5] and _rec_data[5]
        rec_data = _rec_data[8];angle = _angle[8];range = _range[8];

//      if(e_randb_get_if_received() != 0)
//        {
//            rec_data = e_randb_get_data();
//            range = e_randb_get_range();
//            angle = e_randb_get_bearing();
//        }
        
        if( rec_data!=data)
        {
            speed_max = 600 - range/16; // 6
            speed_min = speed_max;// 8
            
            if(abs(angle)>0.8)
            {
                speed_max += -(int)(angle*300);//300
                speed_min += (int)(angle*300);
            }
        }
        
        speedl = speed_max; // ;
        speedr = speed_min;// 160 -;   
        
        e_set_speed_left(speedl);
        e_set_speed_right(speedr);
      
    }
    
    return 0;
}

