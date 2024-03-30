#include "sensors/RPI_ads1115.h"

/**************************************************************************/
/*!
    @brief  Instantiates a new ADS1115 class w/appropriate properties
*/
/**************************************************************************/
RPI_ADS1115::RPI_ADS1115() {
  m_bitShift = 0;
  m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
  m_dataRate = RATE_ADS1115_128SPS;
}


/**************************************************************************/
bool RPI_ADS1X15::begin(uint8_t i2c_addr) {
int status = wiringPiSetup();
    if (status != 0) {
        #ifdef DEBUG
        fprintf(stderr, "No se pudo inicializar WiringPi.\n");
        #endif
        return false;
    }

    #ifdef DEBUG
    fprintf(stdout, "WiringPi initialized.\n");
    #endif

    // Abrir la conexión I2C
    m_i2c_handle = wiringPiI2CSetup(i2c_addr);
    if (m_i2c_handle < 0) {
        #ifdef DEBUG
        fprintf(stderr, "Can't init I2C.\n");
        #endif
        return false;
    }

    #ifdef DEBUG
    fprintf(stdout, "Bus I2C Initialized.\n");
    #endif

    return true; 
}

/**************************************************************************/
/*!
    @brief  Sets the gain and input voltage range

    @param gain gain setting to use
*/
/**************************************************************************/
void RPI_ADS1X15::setGain(adsGain_t gain) { m_gain = gain; }

/**************************************************************************/
/*!
    @brief  Gets a gain and input voltage range

    @return the gain setting
*/
/**************************************************************************/
adsGain_t RPI_ADS1X15::getGain() { return m_gain; }

/**************************************************************************/
/*!
    @brief  Sets the data rate

    @param rate the data rate to use
*/
/**************************************************************************/
void RPI_ADS1X15::setDataRate(uint16_t rate) { m_dataRate = rate; }

/**************************************************************************/
/*!
    @brief  Gets the current data rate

    @return the data rate
*/
/**************************************************************************/
uint16_t RPI_ADS1X15::getDataRate() { return m_dataRate; }

/**************************************************************************/

/*!
    @brief  Gets a single-ended ADC reading from the specified channel

    @param channel ADC channel to read

    @return the ADC reading
*/
/**************************************************************************/
int16_t RPI_ADS1X15::readADC_SingleEnded(uint8_t channel) {
  if (channel > 3) {
    return 0;
  }

  startADCReading(MUX_BY_CHANNEL[channel], /*continuous=*/false);

  // Wait for the conversion to complete
  while (!conversionComplete())
    ;

  // Read the conversion results
  return getLastConversionResults();
}

/**************************************************************************/

/*!
    @brief  Sets up the comparator to operate in basic mode, causing the
            ALERT/RDY pin to assert (go from high to low) when the ADC
            value exceeds the specified threshold.

            This will also set the ADC in continuous conversion mode.

    @param channel ADC channel to use
    @param threshold comparator threshold
*/
/**************************************************************************/
void RPI_ADS1X15::startComparator_SingleEnded(uint8_t channel,
                                                   int16_t threshold) {
  // Start with default values
  uint16_t config =
      ADS1X15_REG_CONFIG_CQUE_1CONV |   // Comparator enabled and asserts on 1
                                        // match
      ADS1X15_REG_CONFIG_CLAT_LATCH |   // Latching mode
      ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
      ADS1X15_REG_CONFIG_CMODE_TRAD |   // Traditional comparator (default val)
      ADS1X15_REG_CONFIG_MODE_CONTIN |  // Continuous conversion mode
      ADS1X15_REG_CONFIG_MODE_CONTIN;   // Continuous conversion mode

  // Set PGA/voltage range
  config |= m_gain;

  // Set data rate
  config |= m_dataRate;

  config |= MUX_BY_CHANNEL[channel];

  // Set the high threshold register
  // Shift 12-bit results left 4 bits for the ADS1015
  writeRegister(ADS1X15_REG_POINTER_HITHRESH, threshold << m_bitShift);

  // Write config register to the ADC
  writeRegister(ADS1X15_REG_POINTER_CONFIG, config);
}
/**************************************************************************/
/*!
    @brief  In order to clear the comparator, we need to read the
            conversion results.  This function reads the last conversion
            results without changing the config value.

    @return the last ADC reading
*/
/**************************************************************************/
int16_t RPI_ADS1X15::getLastConversionResults() {
  // Read the conversion results
  uint16_t res = readRegister(ADS1X15_REG_POINTER_CONVERT) >> m_bitShift;
  if (m_bitShift == 0) {
    return (int16_t)res;
  } else {
    // Shift 12-bit results right 4 bits for the ADS1015,
    // making sure we keep the sign bit intact
    if (res > 0x07FF) {
      // negative number - extend the sign to 16th bit
      res |= 0xF000;
    }
    return (int16_t)res;
  }
}

/**************************************************************************/
/*!
    @brief  Compute volts for the given raw counts.

    @param counts the ADC reading in raw counts

    @return the ADC reading in volts
*/
/**************************************************************************/
float RPI_ADS1X15::computeVolts(int16_t counts) {
  // see data sheet Table 3
  float fsRange;
  switch (m_gain) {
  case GAIN_TWOTHIRDS:
    fsRange = 6.144f;
    break;
  case GAIN_ONE:
    fsRange = 4.096f;
    break;
  case GAIN_TWO:
    fsRange = 2.048f;
    break;
  case GAIN_FOUR:
    fsRange = 1.024f;
    break;
  case GAIN_EIGHT:
    fsRange = 0.512f;
    break;
  case GAIN_SIXTEEN:
    fsRange = 0.256f;
    break;
  default:
    fsRange = 0.0f;
  }
  return counts * (fsRange / (32768 >> m_bitShift));
}

/**************************************************************************/
/*!
    @brief  Non-blocking start conversion function

    Call getLastConversionResults() once conversionComplete() returns true.
    In continuous mode, getLastConversionResults() will always return the
    latest result.
    ALERT/RDY pin is set to RDY mode, and a 8us pulse is generated every
    time new data is ready.

    @param mux mux field value
    @param continuous continuous if set, otherwise single shot
*/
/**************************************************************************/
void RPI_ADS1X15::startADCReading(uint16_t mux, bool continuous) {
  // Start with default values
  uint16_t config =
      ADS1X15_REG_CONFIG_CQUE_1CONV |   // Set CQUE to any value other than
                                        // None so we can use it in RDY mode
      ADS1X15_REG_CONFIG_CLAT_NONLAT |  // Non-latching (default val)
      ADS1X15_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
      ADS1X15_REG_CONFIG_CMODE_TRAD;    // Traditional comparator (default val)

  if (continuous) {
    config |= ADS1X15_REG_CONFIG_MODE_CONTIN;
  } else {
    config |= ADS1X15_REG_CONFIG_MODE_SINGLE;
  }

  // Set PGA/voltage range
  config |= m_gain;

  // Set data rate
  config |= m_dataRate;

  // Set channels
  config |= mux;

  // Set 'start single-conversion' bit
  config |= ADS1X15_REG_CONFIG_OS_SINGLE;

  // Write config register to the ADC
  writeRegister(ADS1X15_REG_POINTER_CONFIG, config);

  // Set ALERT/RDY to RDY mode.
  writeRegister(ADS1X15_REG_POINTER_HITHRESH, 0x8000);
  writeRegister(ADS1X15_REG_POINTER_LOWTHRESH, 0x0000);
}

/**************************************************************************/
/*!
    @brief  Returns true if conversion is complete, false otherwise.

    @return True if conversion is complete, false otherwise.
*/
/**************************************************************************/
bool RPI_ADS1X15::conversionComplete() {
  return (readRegister(ADS1X15_REG_POINTER_CONFIG) & 0x8000) != 0;
}

/**************************************************************************/
/*!
    @brief  Writes 16-bits to the specified destination register

    @param reg register address to write to
    @param value value to write to register
*/
/**************************************************************************/
void RPI_ADS1X15::writeRegister(uint8_t reg, uint16_t value) {
  wiringPiI2CWriteReg16(m_i2c_handle, reg,(value>>8) | (value<<8));
}

/**************************************************************************/
/*!
    @brief  Read 16-bits from the specified destination register

    @param reg register address to read from

    @return 16 bit register value read
*/
/**************************************************************************/
uint16_t RPI_ADS1X15::readRegister(uint8_t reg) {
  wiringPiI2CWrite(m_i2c_handle, ADS1X15_REG_POINTER_CONVERT);
  uint16_t reading = wiringPiI2CReadReg16(m_i2c_handle, reg);
  reading = (reading>>8) | (reading<<8); // yes, wiringPi did not assemble the bytes as we want
  return reading;

}