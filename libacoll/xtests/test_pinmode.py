from nose.tools import eq_, ok_
import sim


def check_sim(snippet, mcu, value):
    eq_(sim.simulator(snippet, mcu).get_serial(), value)


def check_invalid(mcu):
    prog = 'Serial.print(readPinMode(255), DEC);'
    check_sim(prog, mcu, '255')


def check(mcu):
    pins = [str(x) for x in range(14)] + ['A' + str(x) for x in range(6)]
    result1 = '0' * len(pins)
    result2 = '1' * len(pins)

    result = result1 + result2

    log = ['Serial.print(readPinMode(%s), DEC);' % x for x in pins]
    set = ['pinMode(%s, OUTPUT);' % x for x in pins]

    prog = '\n'.join(log + set + log)

    print prog

    check_sim(prog, mcu, result)
    check_invalid(mcu)

for mcu in sim.targets():
    exec '''
def test_{mcu}():
    check("{mcu}")
'''.format(mcu=mcu)
