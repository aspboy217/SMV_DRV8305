#ifndef RAMPING_HPP
#define RAMPING_HPP

#define Kp -1
#define Kd -1 
#define MAX_SPD 255
#define MIN_SPD 0

// spd: reading from analogRead (0 ~ 1023)
// return: spd to use (0 ~ 255)
unsigned char direct(int spd);

class PD{
public:
  PD();
  unsigned char newSpd(int throttle, long timeMillis);
private:
  void setCurrentSpd(unsigned char spd);
  void setDesiredSpd(unsigned char spd);
  unsigned char getCurrentSpd() const;
  unsigned char getDesiredSpd() const;
  void spdUp(unsigned char amount);
  void spdDown(unsigned char amount);
  bool isTimeToChangeSpd(long timeMillis);

  unsigned char m_error, m_prevError, m_rateError;
  unsigned char m_currentSpd;
  unsigned char m_desiredSpd;
  long m_time = 0;
  const long m_maxWaitTime = 100;
};

#endif