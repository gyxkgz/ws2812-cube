
#include "mpu6050.h"


void MPU6050::write_reg(uint8_t reg,uint8_t val)
{
    uint8_t buffer[2];
    buffer[0]=reg;
    buffer[1]=val;
    i2c_dev->write(buffer, 2,true);
}
uint8_t MPU6050::read_reg(uint8_t reg)
{
    uint8_t buffer = reg;
    i2c_dev->write_then_read(&buffer,1,&buffer,1,true);
    return buffer;
}

void MPU6050::read_reg_burst(uint8_t reg,uint8_t *buff,int n)
{
    uint8_t buffer = reg;
    i2c_dev->write_then_read(&buffer,1,buff,n);
}

 bool MPU6050::begin()
 {  
    bool ret;
    if(!i2c_dev->begin())
        return false;
    if(read_reg(WHO_AM_I)!= MPU6050_DEVICE_ID)
        return false;
    return _init();
 }

bool MPU6050::_init()
{
    write_reg(PWR_MGMT_1,DEVICE_RESET);//reset
    while(read_reg(PWR_MGMT_1)&DEVICE_RESET) //wait for reset done
    {
        delay(1);
    }
    delay(100);
    write_reg(SIGNAL_PATH_RESET,0x7);
    delay(100);
    write_reg(PWR_MGMT_1,0);    //wakeup
    if(read_reg(PWR_MGMT_1)&0x40) //
    {
        return false;
    }
    Serial.println(read_reg(PWR_MGMT_1),16);
    return true;
}

void MPU6050::cycle_mode(bool enable,mpu6050_cycle_rate_t freq)
{
    if(enable)
    { 
        write_reg(PWR_MGMT_1,read_reg(PWR_MGMT_1)|CYCLE_ENABLE|TEMP_DIS);
        write_reg(PWR_MGMT_2,(freq<<6)|STBY_XYZG);
    }       
    else
        write_reg(PWR_MGMT_1,read_reg(PWR_MGMT_1)&(~CYCLE_ENABLE));
}

void MPU6050::get_acc_raw()
{
    uint8_t buffer[6];
    read_reg_burst(ACCEL_XOUT_H,buffer,6);
    acc_raw_x = (int16_t)buffer[0]<<8|buffer[1];
    acc_raw_y = (int16_t)buffer[2]<<8|buffer[3];
    acc_raw_z = (int16_t)buffer[4]<<8|buffer[5];
}

void MPU6050::enable_int(bool en,uint8_t mode,uint8_t config)
{
    if(en)
    {
        write_reg(INT_PIN_CFG,read_reg(INT_PIN_CFG)|config);
        write_reg(INT_ENABLE,read_reg(INT_ENABLE)|mode);
    }
    else
        write_reg(INT_ENABLE,read_reg(INT_ENABLE)&~mode);
}

uint8_t MPU6050::rd_clr_int()
{
    int_triggered = false;
    return read_reg(INT_STATUS);
}
