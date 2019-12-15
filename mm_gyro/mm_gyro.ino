#include<Wire.h>
//#include<MPU6050.h>   // NOTE:: May use to change sensitivity

const int MPU_addr = 0x69;    // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;      // MPU data
int16_t cal_AcX, cal_AcY, cal_AcZ, cal_GyX, cal_GyY, cal_GyZ;    //calibration constants
int cal_counter = 0;    // calibration counter
//MPU6050 MPU;    // NOTE:: May use to change sensitivity

#define MPU_FS_GYRO_LSB 131
#define MPU_FS_ACCL_LSB 16384.0
#define MAX_CALIBRATION_ROUNDS 10

/* flashLED
 * @param total milliseconds for an LED flash. Default is set to
 * half a second. Optional afterDelay spaces consecutive flashLED calls.
 */
void flashLED(int duration = 500, int afterDelay = 0) {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(duration);
  digitalWrite(13, LOW);
  delay(afterDelay);
}

/* doMath
 * @breif converts raw data to total gs (9.81 m/s/s) and degree / sec (dps)
 */
double doMath(int16_t value, bool gyro) {
  
  if (gyro) {
    return value / MPU_FS_GYRO_LSB;
  } else {
    return value / MPU_FS_ACCL_LSB;
  }
  
}

/* arithmetic calibration
 *  @brief averages all gyro and accel data over a total of MAX_CALIBRATION_ROUNDS 
 *  computing a final numerical value to be added to new, use-intent data.
 */
void arithmetic_calibration() {
  if (cal_counter < MAX_CALIBRATION_ROUNDS) {
    
    flashLED(125, 125);
    flashLED(125);
    
    cal_AcX += AcX;
    cal_AcY += AcY;
    cal_AcZ += AcZ;
    cal_GyX += GyX;
    cal_GyY += GyY;
    cal_GyZ += GyZ;
    
    cal_counter++;
  }

  else if (cal_counter == MAX_CALIBRATION_ROUNDS) {
    cal_AcX /= -cal_counter;
    cal_AcY /= -cal_counter;
    cal_AcZ /= -cal_counter;
    cal_GyX /= -cal_counter;
    cal_GyY /= -cal_counter;
    cal_GyZ /= -cal_counter;

    cal_counter++;
  }

  return;
}

void setup(){
  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

//  MPU.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);   // NOTE:: May use to change sensitivity
  
  Serial.begin(9600);
}

void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  
  AcX = (Wire.read() << 8) | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY = (Wire.read() << 8) | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = (Wire.read() << 8) | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = (Wire.read() << 8) | Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = (Wire.read() << 8) | Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = (Wire.read() << 8) | Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = (Wire.read() << 8) | Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  arithmetic_calibration();
  
  Serial.print("AcX = "); Serial.print(doMath(AcX + cal_AcX, false));
  Serial.print(" | AcY = "); Serial.print(doMath(AcY + cal_AcY, false));
  Serial.print(" | AcZ = "); Serial.print(doMath(AcZ + cal_AcZ, false));
  
  Serial.print(" | GyX = "); Serial.print(doMath(GyX + cal_GyX, true));
  Serial.print(" | GyY = "); Serial.print(doMath(GyY + cal_GyY, true));
  Serial.print(" | GyZ = "); Serial.println(doMath(GyZ + cal_GyZ, true));
  delay(333);
}
