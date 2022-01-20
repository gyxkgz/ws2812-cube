#include <Adafruit_I2CDevice.h>


//mpu6050 addr
#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     MPU6050_ADDRESS_AD0_LOW
//mpu6050 reg
#define SELF_TEST_X      0x0d
#define SELF_TEST_Y      0x0e
#define SELF_TEST_Z      0x0f
#define SELF_TEST_A      0x10
#define SMPLRT_DIV       0x19  //Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
#define CONFIG           0x1A  //FSYNC Bit Location and DLPF Bandwidth 
#define GYRO_CONFIG      0x1B  //gyro self-test and  full scale range
#define ACCEL_CONFIG     0x1C   //accel self-test and  full scale range
#define FF_THR           0x1D
#define FF_DUR           0x1E
#define MOT_THR          0x1F
#define MOT_DUR          0x20
#define ZRMOT_THR        0x21
#define ZRMOT_DUR        0x22
#define FIFO_EN          0x23   //which sensor measurements loaded into FIFO buffer. 
#define I2C_MST_CTRL     0x24   //
#define I2C_SLV0_ADDR    0x25
#define I2C_SLV0_REG     0x26
#define I2C_SLV0_CTRL    0x27
#define I2C_SLV1_ADDR    0x28
#define I2C_SLV1_REG     0x29
#define I2C_SLV1_CTRL    0x2A
#define I2C_SLV2_ADDR    0x2B
#define I2C_SLV2_REG     0x2C
#define I2C_SLV2_CTRL    0x2D
#define I2C_SLV3_ADDR    0x2E
#define I2C_SLV3_REG     0x2F
#define I2C_SLV3_CTRL    0x30
#define I2C_SLV4_ADDR    0x31
#define I2C_SLV4_REG     0x32
#define I2C_SLV4_DO      0x33
#define I2C_SLV4_CTRL    0x34
#define I2C_SLV4_DI      0x35
#define I2C_MST_STATUS   0x36
#define INT_PIN_CFG      0x37   //config behavior of the int signals at the INT pins
#define INT_ENABLE       0x38   //enables interrupt generation by interrupt sources.
#define DMP_INT_STATUS   0x39
#define INT_STATUS       0x3A
#define ACCEL_XOUT_H     0x3B
#define ACCEL_XOUT_L     0x3C
#define ACCEL_YOUT_H     0x3D
#define ACCEL_YOUT_L     0x3E
#define ACCEL_ZOUT_H     0x3F
#define ACCEL_ZOUT_L     0x40
#define TEMP_OUT_H       0x41   //Temperature in degrees C = 
#define TEMP_OUT_L       0x42   //(TEMP_OUT Register Value as a signed quantity)/340 + 36.53
#define GYRO_XOUT_H      0x43
#define GYRO_XOUT_L      0x44
#define GYRO_YOUT_H      0x45
#define GYRO_YOUT_L      0x46
#define GYRO_ZOUT_H      0x47
#define GYRO_ZOUT_L      0x48
#define EXT_SENS_DATA_00 0x49
#define EXT_SENS_DATA_01 0x4A
#define EXT_SENS_DATA_02 0x4B
#define EXT_SENS_DATA_03 0x4C
#define EXT_SENS_DATA_04 0x4D
#define EXT_SENS_DATA_05 0x4E
#define EXT_SENS_DATA_06 0x4F
#define EXT_SENS_DATA_07 0x50
#define EXT_SENS_DATA_08 0x51
#define EXT_SENS_DATA_09 0x52
#define EXT_SENS_DATA_10 0x53
#define EXT_SENS_DATA_11 0x54
#define EXT_SENS_DATA_12 0x55
#define EXT_SENS_DATA_13 0x56
#define EXT_SENS_DATA_14 0x57
#define EXT_SENS_DATA_15 0x58
#define EXT_SENS_DATA_16 0x59
#define EXT_SENS_DATA_17 0x5A
#define EXT_SENS_DATA_18 0x5B
#define EXT_SENS_DATA_19 0x5C
#define EXT_SENS_DATA_20 0x5D
#define EXT_SENS_DATA_21 0x5E
#define EXT_SENS_DATA_22 0x5F
#define EXT_SENS_DATA_23 0x60
#define MOT_DETECT_STATUS    0x61
#define I2C_SLV0_DO      0x63
#define I2C_SLV1_DO      0x64
#define I2C_SLV2_DO      0x65
#define I2C_SLV3_DO      0x66
#define I2C_MST_DELAY_CTRL   0x67
#define SIGNAL_PATH_RESET    0x68
#define MOT_DETECT_CTRL      0x69
#define USER_CTRL        0x6A   //en/dis fifo/i2c_master and reset fifo /i2cmaster /sensor sig
#define PWR_MGMT_1       0x6B   //power mode and clock source,one bit reset entire device,one bit dis temp
#define PWR_MGMT_2       0x6C
#define BANK_SEL         0x6D
#define MEM_START_ADDR   0x6E
#define MEM_R_W          0x6F
#define DMP_CFG_1        0x70
#define DMP_CFG_2        0x71
#define FIFO_COUNTH      0x72
#define FIFO_COUNTL      0x73
#define FIFO_R_W         0x74
#define WHO_AM_I         0x75
//mpu6050 reg val
#define MPU6050_DEVICE_ID 0x68
#define DEVICE_RESET    (1<<7)
#define CYCLE_ENABLE    (1<<5)
#define TEMP_DIS        (1<<3)
#define STBY_XG         (1<<2)
#define STBY_YG         (1<<1)
#define STBY_ZG         (1)
#define STBY_XYZG       (STBY_XG|STBY_YG|STBY_ZG)
#define DATA_RDY_EN     (1)

typedef enum {
  MPU6050_RANGE_2_G = 0b00,  ///< +/- 2g (default value)
  MPU6050_RANGE_4_G = 0b01,  ///< +/- 4g
  MPU6050_RANGE_8_G = 0b10,  ///< +/- 8g
  MPU6050_RANGE_16_G = 0b11, ///< +/- 16g
} mpu6050_accel_range_t;

typedef enum {
  MPU6050_RANGE_250_DEG,  ///< +/- 250 deg/s (default value)
  MPU6050_RANGE_500_DEG,  ///< +/- 500 deg/s
  MPU6050_RANGE_1000_DEG, ///< +/- 1000 deg/s
  MPU6050_RANGE_2000_DEG, ///< +/- 2000 deg/s
} mpu6050_gyro_range_t;

typedef enum {
  MPU6050_BAND_260_HZ, ///< Docs imply this disables the filter
  MPU6050_BAND_184_HZ, ///< 184 Hz
  MPU6050_BAND_94_HZ,  ///< 94 Hz
  MPU6050_BAND_44_HZ,  ///< 44 Hz
  MPU6050_BAND_21_HZ,  ///< 21 Hz
  MPU6050_BAND_10_HZ,  ///< 10 Hz
  MPU6050_BAND_5_HZ,   ///< 5 Hz
} mpu6050_bandwidth_t;

//used for Accelerometer Only Low Power Mode
typedef enum {
  MPU6050_CYCLE_1_25_HZ, ///< 1.25 Hz
  MPU6050_CYCLE_5_HZ,    ///< 5 Hz
  MPU6050_CYCLE_20_HZ,   ///< 20 Hz
  MPU6050_CYCLE_40_HZ,   ///< 40 Hz
} mpu6050_cycle_rate_t;

/*
* 1.The device will come up in sleep mode upon power-up.

*/
class MPU6050{
    public:
        int16_t acc_raw_x,acc_raw_y,acc_raw_z;
        bool int_triggered;
    public:
        MPU6050(uint8_t addr = MPU6050_DEFAULT_ADDRESS){
           i2c_dev = new Adafruit_I2CDevice(addr);
        };
        bool begin();
        void cycle_mode(bool enable,mpu6050_cycle_rate_t freq = MPU6050_CYCLE_5_HZ);
        void get_acc_raw();
        void enable_int(bool en,uint8_t mode = DATA_RDY_EN,uint8_t config = 0);
        uint8_t rd_clr_int();
        ICACHE_RAM_ATTR void mpu6050_int_cb();
    protected:
        Adafruit_I2CDevice *i2c_dev = NULL;
        bool _init();
    private:
        void write_reg(uint8_t reg,uint8_t val);
        uint8_t read_reg(uint8_t reg);
        void read_reg_burst(uint8_t reg,uint8_t *buff,int n);
};