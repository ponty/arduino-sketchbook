/// source: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1273055713

#ifndef TESTDAC_H
#define TESTDAC_H

class TestDac
{
public:
  void testAll();

private:
  void triangleWave(int begin, int end, int step, int _delay=0);
  void rand();
//  void minMax();
//  void refresh();
  void test(int val, int _delay=0, bool refresh=0);
  void printMaxError();
  void scopeTrigger();
  
  int maxError, maxSettlingTime2;
};

#endif
