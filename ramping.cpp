#include "ramping.hpp"

unsigned char direct(int spd){
  return spd / 4;
}

PD::PD(): m_error(0), m_prevError(0), m_rateError(0){}

void PD::setCurrentSpd(unsigned char spd)      {  m_currentSpd = spd; }
void PD::setDesiredSpd(unsigned char spd)      {  m_desiredSpd = spd; }
unsigned char PD::getCurrentSpd() const {  return m_currentSpd; }
unsigned char PD::getDesiredSpd() const {  return m_desiredSpd; }

void PD::spdUp(unsigned char amount){
  if(m_currentSpd + amount >= MAX_SPD)
    m_currentSpd = MAX_SPD;
  else
    m_currentSpd += amount;
}

void PD::spdDown(unsigned char amount){
  if(m_currentSpd - amount <= MIN_SPD)
    m_currentSpd = MIN_SPD;
  else
    m_currentSpd -= amount; 
}
bool PD::isTimeToChangeSpd(long timeMillis){
  if(timeMillis - m_time >= m_maxWaitTime){
    m_time = timeMillis;
    return true;
  }
  else{
    return false;
  }
}

unsigned char PD::newSpd(int throttle, long timeMillis){
  setDesiredSpd(throttle/4);
  
  if(isTimeToChangeSpd(timeMillis)){
    m_error = getDesiredSpd() - getCurrentSpd();
    m_rateError = m_error - m_prevError;
    m_prevError = m_error;

    unsigned char spdChange = Kp * m_error + Kd * m_rateError;
  
    if(spdChange >= 0)
      spdUp(spdChange);
    else
      spdDown(spdChange);
  }
  return getCurrentSpd();
}