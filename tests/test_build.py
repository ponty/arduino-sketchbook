from nose.tools import eq_, ok_
from path import Path
from pyavrutils.arduino import Arduino, ArduinoCompileError
from pyavrutils.support import simple_targets
import logging
from nose_ittr import IttrMultiplier, ittr
import tempfile

ROOT = Path(__file__).parent.parent

'''
sudo rm -f /usr/share/arduino/libraries/Robot_Control/Wire.h
'''

EXTERNAL_LIBS = [
    'USE_LiquidCrystal_I2C',
    'USE_OneWire',
    'USE_DallasTemperature',
    'USE_CapacitiveSensor',
    'USE_DHT',
    'USE_TLC5947'
]

MCUs = [
    #      'atmega8',
    #      'atmega48',
    #'atmega168',
    'atmega328p',
    #      'atmega640',
    #'atmega1280',
    #'atmega2560'
]


def tmpdir(dir=None, suffix=''):
    x = tempfile.mkdtemp(suffix=suffix, prefix='arduino-sketchbook_', dir=dir)
    return Path(x)

'''
AcAlarm
HelloLcd_i2c
FrequencyMeterSimple
FreqMeasureSerial
FreqCounterTest
'''

PROJECTS = '''
dds
hello
HelloLCD
'''.strip().splitlines()


def test_config():
    known_mcus = [t.mcu for t in simple_targets()]
    for mcu in MCUs:
        assert mcu in known_mcus, 'MCU %s was not found in config %s' % (
            mcu, known_mcus)

def first(ls):
    if len(ls):
        return ls[0]
    
class TestFoo(object):
    __metaclass__ = IttrMultiplier

    @ittr(mcu=MCUs, project=PROJECTS)
    def test_build(self):
        tdir = tmpdir()
        try:

            p = self.project


            (ROOT / p).copytree(tdir / p)
            ino = first((tdir / p).files('*.ino'))
            if not ino:
                ino = first((tdir / p).files('*.pde'))
            assert ino
                
            cc = Arduino(mcu=self.mcu)
            print cc.mcu, ino
            cc.build(ino)
            ok_(cc.size().ok)
        finally:
            tdir.rmtree()
