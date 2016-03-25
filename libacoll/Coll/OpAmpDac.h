#ifndef DAC_H
#define DAC_H

#include <WProgram.h>

/// source: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1273055713
class OpAmpDac
{
public:
	OpAmpDac(const byte pinDigDacIn, const byte pinAnDacOut,
			const byte _overshoot = 5);
	void begin();
	bool write(int val);
	bool refresh();
	int read() const;

	int targetVal;
	byte settlingTime1, settlingTime2;

	inline int readMin()
	{
		write(0);
		return read();
	}
	inline int readMax()
	{
		write(1023);
		return read();
	}

private:
//	inline int fastRead() const;
	inline bool setDac();
	inline bool fineTune();
	inline void dacUp() const;
	inline void dacDown() const;
	inline void dacHold() const;

	const byte overshoot;
	const byte dacUpdownPin; // Digital
	const byte UDacPin; // Analog in
};

#endif

