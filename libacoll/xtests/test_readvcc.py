from nose.tools import eq_
import sim


def almost_eq(a, b, percent=1, msg=None):
    x = abs(1 - b / a) / a
    assert x < percent / 100.0, msg or "%r != %r (%r%%)" % (a, b, percent)


def check_sim(snippet, mcu, value):
    almost_eq(int(sim.simulator(snippet, mcu).get_serial()), value)


def check(mcu):
    check_sim('Serial.print(readVcc());', mcu, 5001)
    check_sim('Serial.print(readVccError(5000));', mcu, 25)

for mcu in sim.targets():
    exec '''
def test_{mcu}():
    check("{mcu}")
'''.format(mcu=mcu)
